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

#include "entities/discord_emoji.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/async_callback.h"

void DiscordEmoji::Delete(dpp::snowflake guild_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || guild_id == 0) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Emojis().Delete(guild_id, m_emoji.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Emojis().Delete(guild_id, m_emoji.id);
	}
}

void DiscordEmoji::Edit(dpp::snowflake guild_id, const char* name, IPluginFunction* callback, cell_t data) {
	if (!m_client || guild_id == 0 || !name) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Emojis().Modify(guild_id, m_emoji.id, name, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordEmoji>(cb);
		});
	} else {
		m_client->Emojis().Modify(guild_id, m_emoji.id, name);
	}
}
