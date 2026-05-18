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

#include "entities/discord_guild_member.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/async_callback.h"

std::string DiscordGuildMember::GetPermissions() const {
	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	if (!g) return "";

	dpp::permission perms = g->permission_overwrites(m_member, dpp::channel());
	return std::to_string(static_cast<uint64_t>(perms));
}

bool DiscordGuildMember::HasPermission(const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	if (!g) return false;

	dpp::permission perms = g->permission_overwrites(m_member, dpp::channel());
	return perms.has(perm);
}

void DiscordGuildMember::AddRole(dpp::snowflake role_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().AddRole(m_member.guild_id, m_member.user_id, role_id,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::AddRole);
			});
	} else {
		m_client->Members().AddRole(m_member.guild_id, m_member.user_id, role_id);
	}
}

void DiscordGuildMember::RemoveRole(dpp::snowflake role_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().RemoveRole(m_member.guild_id, m_member.user_id, role_id,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::RemoveRole);
			});
	} else {
		m_client->Members().RemoveRole(m_member.guild_id, m_member.user_id, role_id);
	}
}

void DiscordGuildMember::SetNickName(const char* nickname, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Modify(m_member.guild_id, m_member.user_id, nickname ? nickname : "",
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Result<DiscordGuildMember>(cb);
			});
	} else {
		m_client->Members().Modify(m_member.guild_id, m_member.user_id, nickname ? nickname : "");
	}
}

void DiscordGuildMember::Kick(const char* reason, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Kick(m_member.guild_id, m_member.user_id, reason,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::Kick);
			});
	} else {
		m_client->Members().Kick(m_member.guild_id, m_member.user_id, reason);
	}
}

void DiscordGuildMember::Ban(uint32_t delete_message_seconds, const char* reason, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Ban(m_member.guild_id, m_member.user_id, reason, delete_message_seconds,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::BanOp);
			});
	} else {
		m_client->Members().Ban(m_member.guild_id, m_member.user_id, reason, delete_message_seconds);
	}
}

void DiscordGuildMember::Timeout(time_t until, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Members().Timeout(m_member.guild_id, m_member.user_id, until,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Result<DiscordGuildMember>(cb);
			});
	} else {
		m_client->Members().Timeout(m_member.guild_id, m_member.user_id, until);
	}
}

void DiscordGuildMember::RemoveTimeout(IPluginFunction* callback, cell_t data) {
	Timeout(0, callback, data);
}

void DiscordGuildMember::MoveToVoiceChannel(dpp::snowflake channel_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Voice().MoveToChannel(m_member.guild_id, m_member.user_id, channel_id,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::VoiceMove);
			});
	} else {
		m_client->Voice().MoveToChannel(m_member.guild_id, m_member.user_id, channel_id);
	}
}

void DiscordGuildMember::DisconnectFromVoice(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Voice().Disconnect(m_member.guild_id, m_member.user_id,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::VoiceDisconnect);
			});
	} else {
		m_client->Voice().Disconnect(m_member.guild_id, m_member.user_id);
	}
}

void DiscordGuildMember::SetMute(bool mute, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Voice().SetMute(m_member.guild_id, m_member.user_id, mute,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::VoiceMute);
			});
	} else {
		m_client->Voice().SetMute(m_member.guild_id, m_member.user_id, mute);
	}
}

void DiscordGuildMember::SetDeaf(bool deaf, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Voice().SetDeaf(m_member.guild_id, m_member.user_id, deaf,
			[callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
				callback.Confirm(cb, DiscordResultType::VoiceDeaf);
			});
	} else {
		m_client->Voice().SetDeaf(m_member.guild_id, m_member.user_id, deaf);
	}
}
