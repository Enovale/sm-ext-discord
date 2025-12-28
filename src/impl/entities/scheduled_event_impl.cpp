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

#include "entities/discord_scheduled_event.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"

void DiscordScheduledEvent::SetChannelId(const char* channel_id) {
	if (!channel_id) return;
	dpp::snowflake id;
	if (ParseUInt64(channel_id, id)) {
		m_event.channel_id = id;
	}
}

void DiscordScheduledEvent::Delete() {
	if (!m_client) return;
	m_client->GetCluster()->guild_event_delete(m_event.guild_id, m_event.id,
		[id = m_event.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to delete scheduled event %" PRIu64 "", id);
		});
}

void DiscordScheduledEvent::Edit() {
	if (!m_client) return;
	m_client->GetCluster()->guild_event_edit(m_event,
		[id = m_event.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to edit scheduled event %" PRIu64 "", id);
		});
}

void DiscordScheduledEvent::Start() {
	if (!m_client) return;
	dpp::scheduled_event edited = m_event;
	edited.status = dpp::es_active;
	m_client->GetCluster()->guild_event_edit(edited,
		[id = m_event.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to start scheduled event %" PRIu64 "", id);
		});
}

void DiscordScheduledEvent::End() {
	if (!m_client) return;
	dpp::scheduled_event edited = m_event;
	edited.status = dpp::es_completed;
	m_client->GetCluster()->guild_event_edit(edited,
		[id = m_event.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to end scheduled event %" PRIu64 "", id);
		});
}

void DiscordScheduledEvent::Cancel() {
	if (!m_client) return;
	dpp::scheduled_event edited = m_event;
	edited.status = dpp::es_cancelled;
	m_client->GetCluster()->guild_event_edit(edited,
		[id = m_event.id](const dpp::confirmation_callback_t& cb) {
			Log.DppError(cb, "Failed to cancel scheduled event %" PRIu64 "", id);
		});
}
