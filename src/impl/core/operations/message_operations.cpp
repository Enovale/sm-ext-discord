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

#include "core/operations/message_operations.h"
#include "utils/discord_common.h"
#include "utils/discord_embed.h"
#include "entities/discord_message.h"

void MessageOperations::Get(dpp::snowflake message_id, dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_get(message_id, channel_id, callback);
}

void MessageOperations::GetMultiple(dpp::snowflake channel_id, dpp::snowflake around, dpp::snowflake before, dpp::snowflake after, uint32_t limit, Callback callback) {
	if (!IsValid()) return;
	m_cluster->messages_get(channel_id, around, before, after, limit, callback);
}

void MessageOperations::Send(dpp::snowflake channel_id, const char* message, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg(channel_id, message);
	m_cluster->message_create(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to send message"); });
}

void MessageOperations::SendEmbed(dpp::snowflake channel_id, const char* message, const DiscordEmbed* embed, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg(channel_id, message);
	msg.add_embed(embed->GetEmbed());
	m_cluster->message_create(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to send message with embed"); });
}

void MessageOperations::SendObject(const DiscordMessage* message, Callback callback) {
	if (!IsValid() || !message) return;
	const dpp::message& msg = message->GetDPPMessage();
	if (msg.channel_id == 0) { Log.Error("Cannot send DiscordMessage: channel_id not set"); return; }
	m_cluster->message_create(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to send DiscordMessage"); });
}

void MessageOperations::SendObjectToChannel(dpp::snowflake channel_id, const DiscordMessage* message, Callback callback) {
	if (!IsValid() || !message) return;
	dpp::message msg = message->GetDPPMessage();
	msg.channel_id = channel_id;
	m_cluster->message_create(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to send DiscordMessage to channel"); });
}

void MessageOperations::Edit(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg; msg.id = message_id; msg.channel_id = channel_id; msg.content = content;
	m_cluster->message_edit(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to edit message"); });
}

void MessageOperations::EditEmbed(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, const DiscordEmbed* embed, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg; msg.id = message_id; msg.channel_id = channel_id; msg.content = content;
	msg.add_embed(embed->GetEmbed());
	m_cluster->message_edit(msg, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to edit message with embed"); });
}

void MessageOperations::Delete(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_delete(message_id, channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete message"); });
}

void MessageOperations::DeleteBulk(dpp::snowflake channel_id, const std::vector<dpp::snowflake>& message_ids, Callback callback) {
	if (!IsValid()) return;
	if (message_ids.size() < 2 || message_ids.size() > 100) {
		Log.Error("Bulk delete requires between 2 and 100 message IDs (got %zu)", message_ids.size());
		return;
	}
	m_cluster->message_delete_bulk(message_ids, channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to bulk delete messages"); });
}

void MessageOperations::Pin(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_pin(channel_id, message_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to pin message"); });
}

void MessageOperations::Unpin(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_unpin(channel_id, message_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to unpin message"); });
}

void MessageOperations::AddReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji, Callback callback) {
	if (!IsValid() || !emoji) return;
	m_cluster->message_add_reaction(message_id, channel_id, emoji, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to add reaction"); });
}

void MessageOperations::RemoveReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji, Callback callback) {
	if (!IsValid() || !emoji) return;
	m_cluster->message_delete_reaction(message_id, channel_id, 0, emoji, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to remove reaction"); });
}

void MessageOperations::RemoveAllReactions(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_delete_all_reactions(message_id, channel_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to remove all reactions"); });
}

void MessageOperations::Crosspost(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->message_crosspost(channel_id, message_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to crosspost message"); });
}
