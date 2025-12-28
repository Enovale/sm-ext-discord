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

#include "core/operations/guild_operations.h"
#include "utils/discord_common.h"

void GuildOperations::Get(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get(guild_id, callback);
}

void GuildOperations::GetMembers(dpp::snowflake guild_id, uint16_t limit, dpp::snowflake after, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_members(guild_id, limit, after, callback);
}

void GuildOperations::SearchMembers(dpp::snowflake guild_id, const char* query, uint16_t limit, Callback callback) {
	if (!IsValid() || !query) return;
	m_cluster->guild_search_members(guild_id, query, limit, callback);
}

void GuildOperations::GetBans(dpp::snowflake guild_id, dpp::snowflake before, dpp::snowflake after, uint16_t limit, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_bans(guild_id, before, after, limit, callback);
}

void GuildOperations::Leave(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_delete(guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to leave guild"); });
}
