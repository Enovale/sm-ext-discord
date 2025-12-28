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

#include "entities/discord_role.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include <fstream>


bool DiscordRole::HasPermission(const char* permission) const {
	uint64_t target_perm;
	if (!ParseUInt64(permission, target_perm)) return false;
	return (m_role.permissions & target_perm) == target_perm;
}

void DiscordRole::Modify() {
	if (!m_client || !m_client->GetCluster()) return;
	m_role.guild_id = m_guild_id;
	m_client->GetCluster()->role_edit(m_role, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to modify role");
	});
}

void DiscordRole::Delete() {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->GetCluster()->role_delete(m_guild_id, m_role.id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to delete role");
	});
}

void DiscordRole::AddToUser(dpp::snowflake user_id) {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->GetCluster()->guild_member_add_role(m_guild_id, user_id, m_role.id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to add role to user");
	});
}

void DiscordRole::RemoveFromUser(dpp::snowflake user_id) {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->GetCluster()->guild_member_remove_role(m_guild_id, user_id, m_role.id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to remove role from user");
	});
}

bool DiscordRole::SetIcon(const char* filepath, dpp::image_type type) {
	if (!filepath) return false;

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", filepath);

	std::ifstream file(fullpath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		Log.Error("SetIcon: Failed to open file: %s", fullpath);
		return false;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) {
		Log.Error("SetIcon: Failed to read file: %s", fullpath);
		return false;
	}

	m_role.load_image(buffer, type);
	return true;
}
