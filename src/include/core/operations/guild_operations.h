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

#pragma once

#include "base_operations.h"

class DiscordGuild;

class GuildOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake guild_id, Callback callback);
	void ModifyFromObject(const DiscordGuild* guild_obj, Callback callback = nullptr);
	void GetMembers(dpp::snowflake guild_id, uint16_t limit = 1000, dpp::snowflake after = 0, Callback callback = nullptr);
	void SearchMembers(dpp::snowflake guild_id, const char* query, uint16_t limit = 1, Callback callback = nullptr);
	void GetBans(dpp::snowflake guild_id, dpp::snowflake before = 0, dpp::snowflake after = 0, uint16_t limit = 1000, Callback callback = nullptr);
	void GetInvites(dpp::snowflake guild_id, Callback callback);
	void GetWebhooks(dpp::snowflake guild_id, Callback callback);
	void BulkDeleteCommands(dpp::snowflake guild_id, Callback callback = nullptr);
	void Leave(dpp::snowflake guild_id, Callback callback = nullptr);
	void ModifyCurrentMember(dpp::snowflake guild_id, const char* nickname, Callback callback = nullptr);
	void GetPruneCount(dpp::snowflake guild_id, uint16_t days, Callback callback);
	void BeginPrune(dpp::snowflake guild_id, uint16_t days, Callback callback = nullptr);
};
