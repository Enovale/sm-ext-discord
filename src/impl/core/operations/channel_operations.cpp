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

#include "core/operations/channel_operations.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"
#include "entities/discord_channel.h"

void ChannelOperations::Get(dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->channel_get(channel_id, callback);
}

void ChannelOperations::Create(dpp::snowflake guild_id, const char* name, dpp::channel_type type, const char* topic, dpp::snowflake parent_id, IPluginFunction* callback, cell_t data) {
	if (!IsValid() || !name) return;
	dpp::channel ch; ch.guild_id = guild_id; ch.name = name; ch.set_type(type);
	if (topic) ch.topic = topic;
	if (parent_id > 0) ch.parent_id = parent_id;

	Handle_t client_handle = m_client->GetHandle();
	m_cluster->channel_create(ch, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
		if (callback) {
			PushResult<DiscordChannel>(client_handle, client, callback, data, cb);
		} else {
			Log.DppError(cb, "Failed to create channel");
		}
	});
}

void ChannelOperations::CreateDM(dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->create_dm_channel(user_id, callback);
}

void ChannelOperations::Modify(dpp::snowflake channel_id, const std::string& name, const std::string& topic, uint16_t position, bool nsfw, uint16_t rate_limit, uint16_t bitrate, uint8_t user_limit, dpp::snowflake parent_id, Callback callback) {
	if (!IsValid()) return;
	dpp::channel ch; ch.id = channel_id;
	if (!name.empty()) ch.name = name;
	if (!topic.empty()) ch.topic = topic;
	if (position > 0) ch.position = position;
	ch.set_nsfw(nsfw);
	if (rate_limit > 0) ch.rate_limit_per_user = rate_limit;
	if (bitrate > 0) ch.bitrate = bitrate;
	if (user_limit > 0) ch.user_limit = user_limit;
	if (parent_id > 0) ch.parent_id = parent_id;
	m_cluster->channel_edit(ch, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify channel"); });
}

void ChannelOperations::Delete(dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->channel_delete(channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete channel"); });
}

void ChannelOperations::Typing(dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->channel_typing(channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to send typing indicator"); });
}
