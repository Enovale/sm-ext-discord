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

#include "entities/discord_sticker.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"

void DiscordSticker::Delete() {
	if (!m_client || m_sticker.guild_id == 0) return;
	m_client->GetCluster()->guild_sticker_delete(m_sticker.guild_id, m_sticker.id,
		[id = m_sticker.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to delete sticker %" PRIu64 "", id);
		});
}

void DiscordSticker::Edit(const char* name, const char* description, const char* tags) {
	if (!m_client || m_sticker.guild_id == 0) return;
	dpp::sticker edited = m_sticker;
	if (name) edited.name = name;
	if (description) edited.description = description;
	if (tags) edited.tags = tags;
	m_client->GetCluster()->guild_sticker_modify(edited,
		[id = m_sticker.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to edit sticker %" PRIu64 "", id);
		});
}
