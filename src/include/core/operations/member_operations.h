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

class MemberOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback);
	void GetUser(dpp::snowflake user_id, Callback callback);
	void Modify(dpp::snowflake guild_id, dpp::snowflake user_id, const std::string& nickname = "", Callback callback = nullptr);
	void AddRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id, Callback callback = nullptr);
	void RemoveRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id, Callback callback = nullptr);
	void Kick(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback = nullptr);
	void Ban(dpp::snowflake guild_id, dpp::snowflake user_id, const char* reason = nullptr, int delete_message_days = 0, Callback callback = nullptr);
	void Timeout(dpp::snowflake guild_id, dpp::snowflake user_id, time_t timeout_until, Callback callback = nullptr);
	void RemoveTimeout(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback = nullptr);
};
