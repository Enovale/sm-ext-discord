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

#include "entities/discord_emoji.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"

void DiscordEmoji::Delete() {
	if (!m_client || m_guild_id == 0) return;
	m_client->GetCluster()->guild_emoji_delete(m_guild_id, m_emoji.id, [id = m_emoji.id](const dpp::confirmation_callback_t& cb) {
		if (cb.is_error()) { Log.Error("Failed to delete emoji %" PRIu64 ": %s", id, cb.get_error().human_readable.c_str()); }
	});
}

void DiscordEmoji::Edit(const char* name) {
	if (!m_client || m_guild_id == 0 || !name) return;
	dpp::emoji edited = m_emoji;
	edited.name = name;
	m_client->GetCluster()->guild_emoji_edit(m_guild_id, edited, [id = m_emoji.id](const dpp::confirmation_callback_t& cb) {
		if (cb.is_error()) { Log.Error("Failed to edit emoji %" PRIu64 ": %s", id, cb.get_error().human_readable.c_str()); }
	});
}
