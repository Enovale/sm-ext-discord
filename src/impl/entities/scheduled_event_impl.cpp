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

#include "entities/discord_scheduled_event.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/async_callback.h"

void DiscordScheduledEvent::SetChannelId(const char* channel_id) {
	if (!channel_id) return;
	dpp::snowflake id;
	if (ParseUInt64(channel_id, id)) {
		m_event.channel_id = id;
	}
}

void DiscordScheduledEvent::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().Delete(m_event.guild_id, m_event.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
		});
	} else {
		m_client->ScheduledEvents().Delete(m_event.guild_id, m_event.id);
	}
}

void DiscordScheduledEvent::Edit(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().ModifyFromObject(this, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().ModifyFromObject(this);
	}
}

void DiscordScheduledEvent::Start(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().Start(m_event.guild_id, m_event.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().Start(m_event.guild_id, m_event.id);
	}
}

void DiscordScheduledEvent::End(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().End(m_event.guild_id, m_event.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().End(m_event.guild_id, m_event.id);
	}
}

void DiscordScheduledEvent::Cancel(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->ScheduledEvents().Cancel(m_event.guild_id, m_event.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordScheduledEvent>(cb);
		});
	} else {
		m_client->ScheduledEvents().Cancel(m_event.guild_id, m_event.id);
	}
}
