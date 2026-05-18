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

#include "core/operations/webhook_operations.h"
#include "utils/discord_common.h"
#include "utils/discord_embed.h"
#include "entities/discord_message.h"

void WebhookOperations::Get(dpp::snowflake webhook_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->get_webhook(webhook_id, callback);
}

void WebhookOperations::GetByChannel(dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->get_channel_webhooks(channel_id, callback);
}

void WebhookOperations::GetByGuild(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->get_guild_webhooks(guild_id, callback);
}

void WebhookOperations::Create(dpp::snowflake channel_id, const std::string& name, const std::string& avatar, Callback callback) {
	if (!IsValid()) return;
	dpp::webhook wh;
	wh.channel_id = channel_id;
	wh.name = name;
	if (!avatar.empty()) wh.avatar = avatar;
	m_cluster->create_webhook(wh, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create webhook"); });
}

void WebhookOperations::Execute(dpp::webhook wh, const char* message, dpp::snowflake thread_id, const std::string& thread_name, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg(message);
	m_cluster->execute_webhook(wh, msg, false, thread_id, thread_name, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to execute webhook"); });
}

void WebhookOperations::ExecuteEmbed(dpp::webhook wh, const char* message, const DiscordEmbed* embed, dpp::snowflake thread_id, const std::string& thread_name, Callback callback) {
	if (!IsValid()) return;
	dpp::message msg(message ? message : "");
	msg.add_embed(embed->GetEmbed());
	m_cluster->execute_webhook(wh, msg, false, thread_id, thread_name, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to execute webhook with embed"); });
}

void WebhookOperations::ExecuteMessage(dpp::webhook wh, const DiscordMessage* message, dpp::snowflake thread_id, const std::string& thread_name, Callback callback) {
	if (!IsValid() || !message) return;
	m_cluster->execute_webhook(wh, message->GetDPPMessage(), false, thread_id, thread_name, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to execute webhook with message"); });
}

void WebhookOperations::Create(const dpp::webhook& wh, Callback callback) {
	if (!IsValid()) return;
	m_cluster->create_webhook(wh, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create webhook"); });
}

void WebhookOperations::Modify(const dpp::webhook& wh, Callback callback) {
	if (!IsValid()) return;
	m_cluster->edit_webhook(wh, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify webhook"); });
}

void WebhookOperations::Delete(dpp::snowflake webhook_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->delete_webhook(webhook_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete webhook"); });
}
