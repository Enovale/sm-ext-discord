/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2025 ProjectSky
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

#include "entities/discord_channel.h"
#include "entities/discord_message.h"
#include "entities/discord_thread_member.h"
#include "features/discord_invite.h"
#include "features/discord_webhook.h"
#include "utils/discord_common.h"
#include "utils/discord_embed.h"
#include "utils/discord_forum.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"

// Helper to parse emoji (snowflake ID or unicode string)
static void ParseEmojiForTag(dpp::forum_tag& tag, const char* emoji) {
	if (!emoji || emoji[0] == '\0') return;
	size_t len = strlen(emoji);
	if (std::all_of(emoji, emoji + len, ::isdigit)) {
		tag.emoji = dpp::snowflake(emoji);
	} else {
		tag.emoji = std::string(emoji);
	}
}

bool DiscordChannel::HasUserPermission(const dpp::user& user, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;

	dpp::guild* g = dpp::find_guild(m_channel.guild_id);
	if (!g) return false;

	uint64_t base_perms = g->base_permissions(&user);
	dpp::permission perms = g->permission_overwrites(base_perms, &user, &m_channel);
	return perms.has(perm);
}

bool DiscordChannel::HasMemberPermission(const dpp::guild_member& member, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;

	dpp::guild* g = dpp::find_guild(m_channel.guild_id);
	if (!g) return false;

	dpp::permission perms = g->permission_overwrites(member, m_channel);
	return perms.has(perm);
}

void DiscordChannel::SetName(const char* name) {
	if (!name) return;
	m_channel.set_name(name);
}

void DiscordChannel::SetTopic(const char* topic) {
	m_channel.set_topic(topic ? topic : "");
}

void DiscordChannel::SetPosition(uint16_t position) {
	m_channel.set_position(position);
}

void DiscordChannel::SetNSFW(bool nsfw) {
	m_channel.set_nsfw(nsfw);
}

void DiscordChannel::SetRateLimitPerUser(uint16_t seconds) {
	m_channel.set_rate_limit_per_user(seconds);
}

void DiscordChannel::SetBitrate(uint16_t bitrate) {
	m_channel.set_bitrate(bitrate);
}

void DiscordChannel::SetUserLimit(uint8_t limit) {
	m_channel.set_user_limit(limit);
}

void DiscordChannel::SetParent(dpp::snowflake parent_id) {
	m_channel.set_parent_id(parent_id);
}

void DiscordChannel::Modify(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().ModifyFromObject(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Channels().ModifyFromObject(this);
	}
}

void DiscordChannel::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().Delete(m_channel.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Channels().Delete(m_channel.id);
	}
}

void DiscordChannel::AddPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;

	dpp::permission_overwrite overwrite;
	overwrite.id = target_id;
	overwrite.type = static_cast<dpp::overwrite_type>(type);
	overwrite.allow = allowed;
	overwrite.deny = denied;

	m_channel.permission_overwrites.push_back(overwrite);

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().ModifyFromObject(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Channels().ModifyFromObject(this);
	}
}

void DiscordChannel::SetPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;

	dpp::permission_overwrite overwrite;
	overwrite.id = target_id;
	overwrite.type = static_cast<dpp::overwrite_type>(type);
	overwrite.allow = allowed;
	overwrite.deny = denied;

	auto& overwrites = m_channel.permission_overwrites;
	bool found = false;
	for (auto& existing : overwrites) {
		if (existing.id == target_id && static_cast<uint8_t>(existing.type) == type) {
			existing.allow = allowed;
			existing.deny = denied;
			found = true;
			break;
		}
	}

	if (!found) {
		overwrites.push_back(overwrite);
	}

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().ModifyFromObject(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Channels().ModifyFromObject(this);
	}
}

void DiscordChannel::RemovePermissionOverwrite(dpp::snowflake target_id, uint8_t type, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;

	auto& overwrites = m_channel.permission_overwrites;
	for (auto it = overwrites.begin(); it != overwrites.end(); ++it) {
		if (it->id == target_id && static_cast<uint8_t>(it->type) == type) {
			overwrites.erase(it);
			break;
		}
	}

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().ModifyFromObject(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Channels().ModifyFromObject(this);
	}
}

std::string DiscordChannel::GetUserPermissions(dpp::snowflake user_id) const {
	dpp::guild* g = dpp::find_guild(m_channel.guild_id);
	dpp::user* u = dpp::find_user(user_id);
	if (!g || !u) return "";

	uint64_t base_perms = g->base_permissions(u);
	dpp::permission perms = g->permission_overwrites(base_perms, u, &m_channel);
	return std::to_string(static_cast<uint64_t>(perms));
}

void DiscordChannel::CreateInvite(int max_age, int max_uses, bool temporary, bool unique, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	dpp::invite invite;
	invite.max_age = max_age;
	invite.max_uses = max_uses;
	invite.temporary = temporary;
	invite.unique = unique;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Invites().CreateWithChannel(m_channel, invite, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordInvite>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Invites().CreateWithChannel(m_channel, invite);
	}
}

void DiscordChannel::CreateInviteFromObject(const DiscordInvite* invite_obj, IPluginFunction* callback, cell_t data) {
	if (!m_client || !invite_obj) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Invites().CreateWithChannel(m_channel, invite_obj->GetDPPInvite(), [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordInvite>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Invites().CreateWithChannel(m_channel, invite_obj->GetDPPInvite());
	}
}

bool DiscordChannel::GetInvites(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Channels().GetInvites(m_channel, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordInvite, dpp::invite_map>(client_handle, client, callback, data, cb, DiscordResultType::Invites);
	});
	return true;
}

bool DiscordChannel::GetWebhooks(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Channels().GetWebhooks(m_channel.id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordWebhook, dpp::webhook_map>(client_handle, client, callback, data, cb, DiscordResultType::Webhooks);
	});
	return true;
}

void DiscordChannel::SendMessage(const char* content, IPluginFunction* callback, cell_t data) {
	if (!m_client || !content) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().Send(m_channel.id, content, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().Send(m_channel.id, content);
	}
}

void DiscordChannel::SendMessageEmbed(const char* content, const DiscordEmbed* embed, IPluginFunction* callback, cell_t data) {
	if (!m_client || !embed) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendEmbed(m_channel.id, content ? content : "", embed, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendEmbed(m_channel.id, content ? content : "", embed);
	}
}

void DiscordChannel::SendDiscordMessage(const DiscordMessage* message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !message) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendObjectToChannel(m_channel.id, message, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendObjectToChannel(m_channel.id, message);
	}
}

void DiscordChannel::SetRTCRegion(const char* region) {
	m_channel.rtc_region = region ? region : "";
}

void DiscordChannel::CreateForumTag(const char* name, const char* emoji, bool moderated) {
	if (!m_client || !name) return;

	dpp::forum_tag tag;
	tag.set_name(name);
	tag.moderated = moderated;
	ParseEmojiForTag(tag, emoji);

	m_channel.available_tags.push_back(tag);

	m_client->Channels().ModifyFromObject(this);
}

void DiscordChannel::EditForumTag(dpp::snowflake tag_id, const char* name, const char* emoji, bool moderated) {
	if (!m_client || !name) return;

	auto it = std::find_if(m_channel.available_tags.begin(), m_channel.available_tags.end(),
		[tag_id](const auto& tag) { return tag.id == tag_id; });

	if (it == m_channel.available_tags.end()) {
		Log.Error("Forum tag with ID %" PRIu64 " not found", tag_id);
		return;
	}

	it->set_name(name);
	it->moderated = moderated;

	if (emoji && emoji[0] != '\0') {
		ParseEmojiForTag(*it, emoji);
	} else {
		it->emoji = std::monostate{};
	}

	m_client->Channels().ModifyFromObject(this, [tag_id](const dpp::confirmation_callback_t& cb) {
		if (cb.is_error()) { Log.Error("Failed to edit forum tag %" PRIu64 ": %s", tag_id, cb.get_error().human_readable.c_str()); }
	});
}

void DiscordChannel::DeleteForumTag(dpp::snowflake tag_id) {
	if (!m_client) return;

	auto& tags = m_channel.available_tags;
	auto it = std::remove_if(tags.begin(), tags.end(), [tag_id](const auto& tag) { return tag.id == tag_id; });
	if (it == tags.end()) {
		Log.Error("Forum tag with ID %" PRIu64 " not found", tag_id);
		return;
	}
	tags.erase(it, tags.end());

	m_client->Channels().ModifyFromObject(this, [tag_id](const dpp::confirmation_callback_t& cb) {
		if (cb.is_error()) { Log.Error("Failed to delete forum tag %" PRIu64 ": %s", tag_id, cb.get_error().human_readable.c_str()); }
	});
}

void DiscordChannel::ApplyForumTag(const DiscordForumTag* tag) {
	if (!m_client || !tag) return;
	m_channel.available_tags.push_back(tag->GetTag());
	m_client->Channels().ModifyFromObject(this);
}

void DiscordChannel::CreateForumThread(const char* name, const char* message, const std::vector<dpp::snowflake>& tag_ids, int auto_archive, int rate_limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !name || !message) return;

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().CreateInForum(m_channel.id, name, message, tag_ids, auto_archive, rate_limit,
			[client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
				PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
			});
	} else {
		m_client->Threads().CreateInForum(m_channel.id, name, message, tag_ids, auto_archive, rate_limit);
	}
}

void DiscordChannel::CreateForumThreadEmbed(const char* name, const char* message, const DiscordEmbed* embed, const std::vector<dpp::snowflake>& tag_ids, int auto_archive, int rate_limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !name || !message || !embed) return;

	dpp::message starter_message(m_channel.id, message);
	starter_message.add_embed(embed->GetEmbed());

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().CreateInForumWithMessage(m_channel.id, name, starter_message, tag_ids, auto_archive, rate_limit,
			[client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
				PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
			});
	} else {
		m_client->Threads().CreateInForumWithMessage(m_channel.id, name, starter_message, tag_ids, auto_archive, rate_limit);
	}
}

void DiscordChannel::CreateThread(const char* name, uint8_t type, int auto_archive, bool invitable, int rate_limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !name) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Create(m_channel.id, name, static_cast<dpp::channel_type>(type), auto_archive, invitable, rate_limit,
			[client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
				PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
			});
	} else {
		m_client->Threads().Create(m_channel.id, name, static_cast<dpp::channel_type>(type), auto_archive, invitable, rate_limit);
	}
}

void DiscordChannel::CreateThreadWithMessage(dpp::snowflake message_id, const char* name, int auto_archive, int rate_limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !name) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().CreateWithMessage(m_channel.id, message_id, name, auto_archive, rate_limit, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().CreateWithMessage(m_channel.id, message_id, name, auto_archive, rate_limit);
	}
}

void DiscordChannel::JoinThread(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Join(m_channel.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().Join(m_channel.id);
	}
}

void DiscordChannel::LeaveThread(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Leave(m_channel.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().Leave(m_channel.id);
	}
}

void DiscordChannel::ModifyThread(const char* name, int auto_archive, int rate_limit, bool archived, bool locked, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Modify(m_channel.id, name ? name : "", auto_archive, rate_limit, archived, locked, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().Modify(m_channel.id, name ? name : "", auto_archive, rate_limit, archived, locked);
	}
}

void DiscordChannel::ThreadMemberAdd(dpp::snowflake user_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().MemberAdd(m_channel.id, user_id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::ThreadMemberAdd);
		});
	} else {
		m_client->Threads().MemberAdd(m_channel.id, user_id);
	}
}

void DiscordChannel::ThreadMemberRemove(dpp::snowflake user_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().MemberRemove(m_channel.id, user_id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::ThreadMemberRemove);
		});
	} else {
		m_client->Threads().MemberRemove(m_channel.id, user_id);
	}
}

void DiscordChannel::SetArchived(bool archived, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Archive(m_channel.id, archived, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().Archive(m_channel.id, archived);
	}
}

void DiscordChannel::SetLocked(bool locked, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().Lock(m_channel.id, locked, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().Lock(m_channel.id, locked);
	}
}

bool DiscordChannel::GetArchivedThreads(bool is_private, time_t before, uint16_t limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;

	Handle_t client_handle = m_client->GetHandle();
	dpp::snowflake channel_id = m_channel.id;

	auto handler = [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordChannel, dpp::thread_map>(client_handle, client, callback, data, cb, DiscordResultType::Threads);
	};

	if (is_private) {
		m_client->Threads().GetPrivateArchived(channel_id, before, limit, handler);
	} else {
		m_client->Threads().GetPublicArchived(channel_id, before, limit, handler);
	}
	return true;
}

bool DiscordChannel::GetThreadMembers(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;

	Handle_t client_handle = m_client->GetHandle();
	dpp::snowflake thread_id = m_channel.id;

	m_client->Threads().GetMembers(thread_id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordThreadMember, dpp::thread_member_map>(client_handle, client, callback, data, cb, DiscordResultType::ThreadMembers);
	});
	return true;
}

void DiscordChannel::EditPermissions(dpp::snowflake overwrite_id, uint64_t allow, uint64_t deny, bool is_member, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning()) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Channels().EditPermissions(m_channel.id, overwrite_id, allow, deny, is_member, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Channels().EditPermissions(m_channel.id, overwrite_id, allow, deny, is_member);
	}
}
