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

#include "core/operations/sticker_operations.h"
#include "utils/discord_common.h"

void StickerOperations::Create(dpp::snowflake guild_id, const char* name, const char* description, const char* tags, const char* file_path, dpp::sticker_format format, Callback callback) {
	if (!IsValid() || !name || !tags || !file_path) return;
	dpp::sticker st;
	st.guild_id = guild_id;
	st.name = name;
	if (description) st.description = description;
	st.tags = tags;
	st.format_type = format;
	st.set_filename(file_path);
	m_cluster->guild_sticker_create(st, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create sticker"); });
}

void StickerOperations::Modify(dpp::snowflake guild_id, dpp::snowflake sticker_id, const char* name, const char* description, const char* tags, Callback callback) {
	if (!IsValid()) return;
	dpp::sticker st;
	st.guild_id = guild_id;
	st.id = sticker_id;
	if (name) st.name = name;
	if (description) st.description = description;
	if (tags) st.tags = tags;
	m_cluster->guild_sticker_modify(st, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify sticker"); });
}

void StickerOperations::Delete(dpp::snowflake guild_id, dpp::snowflake sticker_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_sticker_delete(sticker_id, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete sticker"); });
}
