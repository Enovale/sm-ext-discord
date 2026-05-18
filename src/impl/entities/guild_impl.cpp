/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2026 ProjectSky
 * =============================================================================
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "entities/discord_guild.h"
#include "entities/discord_guild_member.h"
#include "entities/discord_channel.h"
#include "entities/discord_role.h"
#include "entities/discord_ban.h"
#include "entities/discord_emoji.h"
#include "entities/discord_sticker.h"
#include "entities/discord_scheduled_event.h"
#include "features/discord_invite.h"
#include "features/discord_webhook.h"
#include "features/discord_slashcommand.h"
#include "utils/discord_common.h"
#include "core/discord_handle_array.h"
#include "core/discord_client.h"
#include "core/handle_manager.h"
#include "core/async_callback.h"
#include "core/logger.h"
#include "core/task_manager.h"
#include <string>

static constexpr std::streamoff kMaxEmojiImageBytes = 256 * 1024;

bool DiscordGuild::HasPermission(dpp::snowflake user_id, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;
	uint64_t base_perms = GetBasePermissions(user_id);
	return (base_perms & perm) != 0;
}

bool DiscordGuild::HasPermissionInChannel(dpp::snowflake user_id, dpp::snowflake channel_id, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;
	uint64_t channel_perms = GetPermissionsInChannel(user_id, channel_id);
	return (channel_perms & perm) != 0;
}

void DiscordGuild::Modify(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Guilds().ModifyFromObject(this, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordGuild>(cb);
		});
	} else {
		m_client->Guilds().ModifyFromObject(this);
	}
}

bool DiscordGuild::GetInvites(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetInvites(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordInvite, dpp::invite_map>(cb, DiscordResultType::Invites);
	});
	return true;
}

bool DiscordGuild::GetWebhooks(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetWebhooks(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordWebhook, dpp::webhook_map>(cb, DiscordResultType::Webhooks);
	});
	return true;
}

bool DiscordGuild::GetRoles(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	dpp::snowflake guild_id = m_guild.id;
	m_client->Roles().GetAll(m_guild.id, [callback = AsyncCallback(client_handle, callback, data), guild_id](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordRole, dpp::role_map>(cb, DiscordResultType::Roles, [guild_id](const dpp::role& r) { return std::make_pair(r, guild_id); });
	});
	return true;
}

uint64_t DiscordGuild::GetBasePermissions(dpp::snowflake user_id) const {
	if (!m_client || !m_client->GetCluster()) return 0;

	dpp::user* user = dpp::find_user(user_id);
	if (!user) return 0;
	return m_guild.base_permissions(user);
}

uint64_t DiscordGuild::GetPermissionsInChannel(dpp::snowflake user_id, dpp::snowflake channel_id) const {
	dpp::channel* ch = dpp::find_channel(channel_id);
	if (!ch) return 0;

	dpp::user* user = dpp::find_user(user_id);
	if (!user) return 0;

	uint64_t base_perms = m_guild.base_permissions(user);
	return static_cast<uint64_t>(m_guild.permission_overwrites(base_perms, user, ch));
}

bool DiscordGuild::GetMember(dpp::snowflake user_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Members().Get(m_guild.id, user_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordGuildMember>(cb);
	});
	return true;
}

bool DiscordGuild::GetMembers(uint16_t limit, dpp::snowflake after, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetMembers(m_guild.id, limit, after, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordGuildMember, dpp::guild_member_map>(cb, DiscordResultType::Members);
	});
	return true;
}

bool DiscordGuild::SearchMembers(const char* query, uint16_t limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback || !query) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().SearchMembers(m_guild.id, query, limit, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordGuildMember, dpp::guild_member_map>(cb, DiscordResultType::Members);
	});
	return true;
}

bool DiscordGuild::GetActiveThreads(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Threads().GetActive(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordChannel, dpp::active_threads>(cb, DiscordResultType::Threads,
			[](const auto& item) { return item.active_thread; });
	});
	return true;
}

bool DiscordGuild::GetBans(dpp::snowflake before, dpp::snowflake after, uint16_t limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetBans(m_guild.id, before, after, limit, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordBan, dpp::ban_map>(cb, DiscordResultType::Bans);
	});
	return true;
}

void DiscordGuild::CreateEmoji(const char* name, const char* image_path, dpp::image_type type, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !name || !image_path) return;

	std::string imageData;
	std::string error;
	if (!ReadBinaryFile(image_path, kMaxEmojiImageBytes, "emoji image file", imageData, error)) {
		if (callback) {
			AsyncCallback(m_client->GetHandle(), callback, data).Error(error, DiscordResultType::Emoji);
		} else {
			Log.Error("CreateEmoji: %s", error.c_str());
		}
		return;
	}

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Emojis().Create(m_guild.id, name, imageData, type, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordEmoji>(cb);
		});
	} else {
		m_client->Emojis().Create(m_guild.id, name, imageData, type);
	}
}

void DiscordGuild::GetEmoji(dpp::snowflake emoji_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Emojis().Get(m_guild.id, emoji_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordEmoji>(cb);
	});
}

void DiscordGuild::GetAllEmojis(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Emojis().GetAll(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordEmoji, dpp::emoji_map>(cb, DiscordResultType::Emojis);
	});
}

void DiscordGuild::CreateSticker(const char* name, const char* description, const char* tags, const char* file_path, dpp::sticker_format format, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !name || !tags || !file_path) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Stickers().Create(m_guild.id, name, description, tags, file_path, format, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordSticker>(cb);
		});
	} else {
		m_client->Stickers().Create(m_guild.id, name, description, tags, file_path, format);
	}
}

void DiscordGuild::CreateScheduledEvent(const char* name, const char* description, dpp::snowflake channel_id, time_t start_time, time_t end_time, dpp::event_entity_type type, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !name) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().Create(m_guild.id, name, description, channel_id, start_time, end_time, type, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().Create(m_guild.id, name, description, channel_id, start_time, end_time, type);
	}
}

void DiscordGuild::CreateExternalScheduledEvent(const char* name, const char* description, const char* location, time_t start_time, time_t end_time, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !name || !location) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().CreateExternal(m_guild.id, name, description, location, start_time, end_time, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().CreateExternal(m_guild.id, name, description, location, start_time, end_time);
	}
}

void DiscordGuild::GetScheduledEvents(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->ScheduledEvents().GetAll(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordScheduledEvent, dpp::scheduled_event_map>(cb, DiscordResultType::ScheduledEvents);
	});
}

void DiscordGuild::GetScheduledEvent(dpp::snowflake event_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->ScheduledEvents().Get(m_guild.id, event_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordScheduledEvent>(cb);
	});
}

void DiscordGuild::GetScheduledEventUsers(dpp::snowflake event_id, uint16_t limit, dpp::snowflake before, dpp::snowflake after, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->ScheduledEvents().GetUsers(m_guild.id, event_id, limit, before, after, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordUser, dpp::event_member_map>(cb, DiscordResultType::Users, [](const dpp::event_member& em) { return em.user; });
	});
}

void DiscordGuild::BanUser(dpp::snowflake user_id, uint32_t delete_message_seconds, const char* reason, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Ban(m_guild.id, user_id, reason ? reason : "", delete_message_seconds, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Ban);
		});
	} else {
		m_client->Members().Ban(m_guild.id, user_id, reason ? reason : "", delete_message_seconds);
	}
}

void DiscordGuild::UnbanUser(dpp::snowflake user_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Unban(m_guild.id, user_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Unban);
		});
	} else {
		m_client->Members().Unban(m_guild.id, user_id);
	}
}

void DiscordGuild::CreateRole(const char* name, uint32_t color, bool hoist, bool mentionable, uint64_t permissions, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !name || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Roles().Create(m_guild.id, name, color, hoist, mentionable, permissions, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordRole>(cb);
	});
}

void DiscordGuild::CreateRoleFromObject(DiscordRole* role_obj, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !role_obj || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Roles().CreateFromObject(m_guild.id, role_obj, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordRole>(cb);
	});
}

void DiscordGuild::BulkDeleteCommands(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Guilds().BulkDeleteCommands(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Guilds().BulkDeleteCommands(m_guild.id);
	}
}

void DiscordGuild::GetCommands(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Commands().GetGuildCommands(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordSlashCommand, dpp::slashcommand_map>(cb, DiscordResultType::SlashCommands);
	});
}

void DiscordGuild::GetCommand(dpp::snowflake command_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Commands().GetGuildCommand(m_guild.id, command_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordSlashCommand>(cb);
	});
}

bool DiscordGuild::GetChannels(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Channels().GetChannels(m_guild.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordChannel, dpp::channel_map>(cb, DiscordResultType::Channels);
	});
	return true;
}

void DiscordGuild::EditChannelPositions(const std::vector<dpp::channel>& channels, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().EditPositions(channels, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb);
		});
	} else {
		m_client->Channels().EditPositions(channels);
	}
}

void DiscordGuild::EditRolePositions(const std::vector<dpp::role>& roles, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Roles().EditPositions(m_guild.id, roles, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb);
		});
	} else {
		m_client->Roles().EditPositions(m_guild.id, roles);
	}
}

void DiscordGuild::ModifyCurrentMember(const char* nickname, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Guilds().ModifyCurrentMember(m_guild.id, nickname, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb);
		});
	} else {
		m_client->Guilds().ModifyCurrentMember(m_guild.id, nickname);
	}
}

void DiscordGuild::GetPruneCount(uint16_t days, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster() || !callback) return;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetPruneCount(m_guild.id, days, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Confirm(cb);
	});
}

void DiscordGuild::BeginPrune(uint16_t days, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->GetCluster()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Guilds().BeginPrune(m_guild.id, days, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb);
		});
	} else {
		m_client->Guilds().BeginPrune(m_guild.id, days);
	}
}
