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

#include "entities/discord_role.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/async_callback.h"

static constexpr std::streamoff kMaxRoleIconBytes = 10 * 1024 * 1024;

bool DiscordRole::HasPermission(const char* permission) const {
	uint64_t target_perm;
	if (!ParseUInt64(permission, target_perm)) return false;
	return (m_role.permissions & target_perm) == target_perm;
}

void DiscordRole::Modify(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	m_role.guild_id = m_guild_id;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Roles().ModifyFromObject(this, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordRole>(cb);
		});
	} else {
		m_client->Roles().ModifyFromObject(this);
	}
}

void DiscordRole::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Roles().Delete(m_guild_id, m_role.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Roles().Delete(m_guild_id, m_role.id);
	}
}

void DiscordRole::AddToUser(dpp::snowflake user_id) {
	if (!m_client) return;
	m_client->Members().AddRole(m_guild_id, user_id, m_role.id);
}

void DiscordRole::RemoveFromUser(dpp::snowflake user_id) {
	if (!m_client) return;
	m_client->Members().RemoveRole(m_guild_id, user_id, m_role.id);
}

bool DiscordRole::SetIcon(const char* filepath, dpp::image_type type) {
	if (!filepath) return false;

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", filepath);

	std::string buffer;
	std::string error;
	if (!ReadBinaryFile(fullpath, kMaxRoleIconBytes, "role icon file", buffer, error)) {
		Log.Error("SetIcon: %s", error.c_str());
		return false;
	}

	m_role.load_image(buffer, type);
	return true;
}
