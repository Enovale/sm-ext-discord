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

#include "core/operations/role_operations.h"
#include "utils/discord_common.h"
#include "core/callback_helpers.h"
#include "core/discord_client.h"
#include "entities/discord_role.h"

void RoleOperations::GetAll(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->roles_get(guild_id, callback);
}

void RoleOperations::Create(dpp::snowflake guild_id, const char* name, uint32_t color, bool hoist, bool mentionable, uint64_t permissions, Callback callback) {
	if (!IsValid() || !name) return;
	dpp::role role; role.guild_id = guild_id; role.name = name; role.colour = color;
	role.flags = (hoist ? dpp::r_hoist : 0) | (mentionable ? dpp::r_mentionable : 0);
	role.permissions = permissions;
	m_cluster->role_create(role, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create role"); });
}

void RoleOperations::CreateFromObject(dpp::snowflake guild_id, const DiscordRole* role_obj, Callback callback) {
	if (!IsValid() || !role_obj) return;
	dpp::role role = role_obj->GetDPPRole();
	role.guild_id = guild_id; role.id = 0;
	m_cluster->role_create(role, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create role"); });
}

void RoleOperations::Modify(dpp::snowflake guild_id, dpp::snowflake role_id, const std::string& name, uint32_t color, bool hoist, bool mentionable, uint64_t permissions, Callback callback) {
	if (!IsValid()) return;
	dpp::role role; role.id = role_id; role.guild_id = guild_id;
	if (!name.empty()) role.name = name;
	if (color > 0) role.colour = color;
	role.flags = (hoist ? dpp::r_hoist : 0) | (mentionable ? dpp::r_mentionable : 0);
	if (permissions > 0) role.permissions = permissions;
	m_cluster->role_edit(role, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify role"); });
}

void RoleOperations::ModifyFromObject(const DiscordRole* role_obj, Callback callback) {
	if (!IsValid() || !role_obj) return;
	dpp::role role = role_obj->GetDPPRole();
	role.guild_id = role_obj->GetGuildId();
	m_cluster->role_edit(role, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify role"); });
}

void RoleOperations::Delete(dpp::snowflake guild_id, dpp::snowflake role_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->role_delete(guild_id, role_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete role"); });
}

void RoleOperations::EditPositions(dpp::snowflake guild_id, const std::vector<dpp::role>& roles, Callback callback) {
	if (!IsValid()) return;
	m_cluster->roles_edit_position(guild_id, roles, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to edit role positions"); });
}
