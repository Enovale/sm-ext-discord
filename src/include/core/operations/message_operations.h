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

#pragma once

#include "base_operations.h"

class DiscordEmbed;
class DiscordMessage;

class MessageOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake message_id, dpp::snowflake channel_id, Callback callback);
	void GetMultiple(dpp::snowflake channel_id, dpp::snowflake around = 0, dpp::snowflake before = 0, dpp::snowflake after = 0, uint32_t limit = 50, Callback callback = nullptr);
	void Send(dpp::snowflake channel_id, const char* message, Callback callback = nullptr);
	void SendEmbed(dpp::snowflake channel_id, const char* message, const DiscordEmbed* embed, Callback callback = nullptr);
	void SendObject(const DiscordMessage* message, Callback callback = nullptr);
	void SendObjectToChannel(dpp::snowflake channel_id, const DiscordMessage* message, Callback callback = nullptr);
	void SendDPPMessage(const dpp::message& message, Callback callback = nullptr);
	void Edit(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, Callback callback = nullptr);
	void EditEmbed(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, const DiscordEmbed* embed, Callback callback = nullptr);
	void EditMessage(const DiscordMessage* message, Callback callback = nullptr);
	void Delete(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback = nullptr);
	void DeleteBulk(dpp::snowflake channel_id, const std::vector<dpp::snowflake>& message_ids, Callback callback = nullptr);
	void Pin(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback = nullptr);
	void Unpin(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback = nullptr);
	void AddReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji, Callback callback = nullptr);
	void RemoveReaction(dpp::snowflake channel_id, dpp::snowflake message_id, const char* emoji, Callback callback = nullptr);
	void RemoveAllReactions(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback = nullptr);
	void Crosspost(dpp::snowflake channel_id, dpp::snowflake message_id, Callback callback = nullptr);
	void EndPoll(const dpp::message& message, Callback callback = nullptr);
	void GetPollAnswerVoters(const dpp::message& message, uint32_t answer_id, Callback callback = nullptr);
};
