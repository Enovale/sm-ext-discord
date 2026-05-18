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

#include "core/operations/scheduled_event_operations.h"
#include "entities/discord_scheduled_event.h"
#include "utils/discord_common.h"

void ScheduledEventOperations::GetAll(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_events_get(guild_id, callback);
}

void ScheduledEventOperations::Get(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_event_get(guild_id, event_id, callback);
}

void ScheduledEventOperations::GetUsers(dpp::snowflake guild_id, dpp::snowflake event_id, uint16_t limit, dpp::snowflake before, dpp::snowflake after, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_event_users_get(guild_id, event_id, callback, static_cast<uint8_t>(limit), before, after);
}

void ScheduledEventOperations::Create(dpp::snowflake guild_id, const char* name, const char* description, dpp::snowflake channel_id, time_t start_time, time_t end_time, dpp::event_entity_type type, Callback callback) {
	if (!IsValid() || !name) return;
	dpp::scheduled_event ev;
	ev.guild_id = guild_id;
	ev.name = name;
	if (description) ev.description = description;
	ev.channel_id = channel_id;
	ev.scheduled_start_time = start_time;
	if (end_time > 0) ev.scheduled_end_time = end_time;
	ev.entity_type = type;
	m_cluster->guild_event_create(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create scheduled event"); });
}

void ScheduledEventOperations::CreateExternal(dpp::snowflake guild_id, const char* name, const char* description, const char* location, time_t start_time, time_t end_time, Callback callback) {
	if (!IsValid() || !name || !location) return;
	dpp::scheduled_event ev;
	ev.guild_id = guild_id;
	ev.name = name;
	if (description) ev.description = description;
	ev.entity_metadata.location = location;
	ev.scheduled_start_time = start_time;
	ev.scheduled_end_time = end_time;
	ev.entity_type = dpp::eet_external;
	m_cluster->guild_event_create(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create external scheduled event"); });
}

void ScheduledEventOperations::Modify(dpp::snowflake guild_id, dpp::snowflake event_id, const char* name, const char* description, time_t start_time, time_t end_time, Callback callback) {
	if (!IsValid()) return;
	dpp::scheduled_event ev;
	ev.id = event_id;
	ev.guild_id = guild_id;
	if (name) ev.name = name;
	if (description) ev.description = description;
	if (start_time > 0) ev.scheduled_start_time = start_time;
	if (end_time > 0) ev.scheduled_end_time = end_time;
	m_cluster->guild_event_edit(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify scheduled event"); });
}

void ScheduledEventOperations::ModifyFromObject(const DiscordScheduledEvent* event_obj, Callback callback) {
	if (!IsValid() || !event_obj) return;
	m_cluster->guild_event_edit(event_obj->GetDPPEvent(), callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify scheduled event"); });
}

void ScheduledEventOperations::Delete(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_event_delete(event_id, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete scheduled event"); });
}

void ScheduledEventOperations::Start(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback) {
	if (!IsValid()) return;
	dpp::scheduled_event ev;
	ev.id = event_id;
	ev.guild_id = guild_id;
	ev.status = dpp::es_active;
	m_cluster->guild_event_edit(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to start scheduled event"); });
}

void ScheduledEventOperations::End(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback) {
	if (!IsValid()) return;
	dpp::scheduled_event ev;
	ev.id = event_id;
	ev.guild_id = guild_id;
	ev.status = dpp::es_completed;
	m_cluster->guild_event_edit(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to end scheduled event"); });
}

void ScheduledEventOperations::Cancel(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback) {
	if (!IsValid()) return;
	dpp::scheduled_event ev;
	ev.id = event_id;
	ev.guild_id = guild_id;
	ev.status = dpp::es_cancelled;
	m_cluster->guild_event_edit(ev, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to cancel scheduled event"); });
}
