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

#include "core/operations/stage_operations.h"
#include "utils/discord_common.h"

void StageOperations::Create(dpp::snowflake channel_id, const char* topic, dpp::stage_privacy_level privacy, Callback callback) {
	if (!IsValid() || !topic) return;
	dpp::stage_instance si;
	si.channel_id = channel_id;
	si.topic = topic;
	si.privacy_level = privacy;
	m_cluster->stage_instance_create(si, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create stage instance"); });
}

void StageOperations::Modify(dpp::snowflake channel_id, const char* topic, dpp::stage_privacy_level privacy, Callback callback) {
	if (!IsValid() || !topic) return;
	dpp::stage_instance si;
	si.channel_id = channel_id;
	si.topic = topic;
	si.privacy_level = privacy;
	m_cluster->stage_instance_edit(si, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify stage instance"); });
}

void StageOperations::Delete(dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->stage_instance_delete(channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete stage instance"); });
}
