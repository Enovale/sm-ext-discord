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

class WebhookOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake webhook_id, Callback callback);
	void GetByChannel(dpp::snowflake channel_id, Callback callback);
	void GetByGuild(dpp::snowflake guild_id, Callback callback);
	void Create(dpp::snowflake channel_id, const std::string& name, const std::string& avatar = "", Callback callback = nullptr);
	void Create(const dpp::webhook& wh, Callback callback);
	void Execute(dpp::webhook wh, const char* message, dpp::snowflake thread_id, const std::string& thread_name, Callback callback = nullptr);
	void ExecuteEmbed(dpp::webhook wh, const char* message, const DiscordEmbed* embed, dpp::snowflake thread_id, const std::string& thread_name, Callback callback = nullptr);
	void ExecuteMessage(dpp::webhook wh, const DiscordMessage* message, dpp::snowflake thread_id, const std::string& thread_name, Callback callback = nullptr);
	void Modify(const dpp::webhook& wh, Callback callback = nullptr);
	void Delete(dpp::snowflake webhook_id, Callback callback = nullptr);
};
