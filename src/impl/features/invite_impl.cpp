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

#include "features/discord_invite.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"

void DiscordInvite::Create(dpp::snowflake channel_id) {
	if (!m_client) return;

	dpp::channel* channel = dpp::find_channel(channel_id);
	if (!channel) {
		Log.Error("Failed to create invite: channel not found in cache");
		return;
	}

	m_client->Invites().CreateWithChannel(*channel, m_invite);
}

void DiscordInvite::Delete() {
	if (!m_client) return;
	if (m_invite.code.empty()) {
		Log.Error("Cannot delete invite: invite code is empty");
		return;
	}
	m_client->Invites().Delete(m_invite.code.c_str());
}
