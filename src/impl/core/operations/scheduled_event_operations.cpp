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

#include "core/operations/scheduled_event_operations.h"
#include "utils/discord_common.h"

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
