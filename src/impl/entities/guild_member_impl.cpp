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

#include "entities/discord_guild_member.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"

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
		m_client->GetCluster()->guild_member_add_role(m_member.guild_id, m_member.user_id, role_id,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::AddRole);
			});
	} else {
		m_client->GetCluster()->guild_member_add_role(m_member.guild_id, m_member.user_id, role_id,
			[user_id = m_member.user_id, role_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to add role %" PRIu64 " to member %" PRIu64 "", role_id, user_id);
			});
	}
}

void DiscordGuildMember::RemoveRole(dpp::snowflake role_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->GetCluster()->guild_member_remove_role(m_member.guild_id, m_member.user_id, role_id,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::RemoveRole);
			});
	} else {
		m_client->GetCluster()->guild_member_remove_role(m_member.guild_id, m_member.user_id, role_id,
			[user_id = m_member.user_id, role_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to remove role %" PRIu64 " from member %" PRIu64 "", role_id, user_id);
			});
	}
}

void DiscordGuildMember::SetNickname(const char* nickname, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	dpp::guild_member edited = m_member;
	edited.set_nickname(nickname ? nickname : "");
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->GetCluster()->guild_edit_member(edited,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushResult<DiscordGuildMember>(client_handle, client, callback, data, cb);
			});
	} else {
		m_client->GetCluster()->guild_edit_member(edited,
			[user_id = m_member.user_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to set nickname for member %" PRIu64 "", user_id);
			});
	}
}

void DiscordGuildMember::Kick(const char* reason, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (reason && reason[0] != '\0') {
		m_client->GetCluster()->set_audit_reason(reason);
	}
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->GetCluster()->guild_member_kick(m_member.guild_id, m_member.user_id,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Kick);
			});
	} else {
		m_client->GetCluster()->guild_member_kick(m_member.guild_id, m_member.user_id,
			[user_id = m_member.user_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to kick member %" PRIu64 "", user_id);
			});
	}
}

void DiscordGuildMember::Ban(uint32_t delete_message_seconds, const char* reason, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (reason && reason[0] != '\0') {
		m_client->GetCluster()->set_audit_reason(reason);
	}
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->GetCluster()->guild_ban_add(m_member.guild_id, m_member.user_id, delete_message_seconds,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::BanOp);
			});
	} else {
		m_client->GetCluster()->guild_ban_add(m_member.guild_id, m_member.user_id, delete_message_seconds,
			[user_id = m_member.user_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to ban member %" PRIu64 "", user_id);
			});
	}
}

void DiscordGuildMember::Timeout(time_t until, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	dpp::guild_member edited = m_member;
	edited.communication_disabled_until = until;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->GetCluster()->guild_edit_member(edited,
			[client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushResult<DiscordGuildMember>(client_handle, client, callback, data, cb);
			});
	} else {
		m_client->GetCluster()->guild_edit_member(edited,
			[user_id = m_member.user_id](const dpp::confirmation_callback_t& cb) {
				Log.DppError(cb, "Failed to timeout member %" PRIu64 "", user_id);
			});
	}
}

void DiscordGuildMember::RemoveTimeout(IPluginFunction* callback, cell_t data) {
	Timeout(0, callback, data);
}
