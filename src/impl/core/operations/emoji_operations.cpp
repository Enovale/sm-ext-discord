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

#include "core/operations/emoji_operations.h"
#include "utils/discord_common.h"

void EmojiOperations::Modify(dpp::snowflake guild_id, dpp::snowflake emoji_id, const char* name, Callback callback) {
	if (!IsValid() || !name) return;
	dpp::emoji em;
	em.id = emoji_id;
	em.name = name;
	m_cluster->guild_emoji_edit(guild_id, em, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify emoji"); });
}

void EmojiOperations::Delete(dpp::snowflake guild_id, dpp::snowflake emoji_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_emoji_delete(guild_id, emoji_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete emoji"); });
}
