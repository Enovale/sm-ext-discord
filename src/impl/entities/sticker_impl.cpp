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
#include "core/callback_helpers.h"

void DiscordSticker::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client || m_sticker.guild_id == 0) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Stickers().Delete(m_sticker.guild_id, m_sticker.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Stickers().Delete(m_sticker.guild_id, m_sticker.id);
	}
}

void DiscordSticker::Edit(const char* name, const char* description, const char* tags, IPluginFunction* callback, cell_t data) {
	if (!m_client || m_sticker.guild_id == 0) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Stickers().Modify(m_sticker.guild_id, m_sticker.id, name, description, tags, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordSticker>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Stickers().Modify(m_sticker.guild_id, m_sticker.id, name, description, tags);
	}
}
