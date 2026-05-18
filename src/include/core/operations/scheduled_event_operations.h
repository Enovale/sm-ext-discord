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

class DiscordScheduledEvent;

class ScheduledEventOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void GetAll(dpp::snowflake guild_id, Callback callback);
	void Get(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback);
	void GetUsers(dpp::snowflake guild_id, dpp::snowflake event_id, uint16_t limit, dpp::snowflake before, dpp::snowflake after, Callback callback);
	void Create(dpp::snowflake guild_id, const char* name, const char* description, dpp::snowflake channel_id, time_t start_time, time_t end_time, dpp::event_entity_type type, Callback callback = nullptr);
	void CreateExternal(dpp::snowflake guild_id, const char* name, const char* description, const char* location, time_t start_time, time_t end_time, Callback callback = nullptr);
	void Modify(dpp::snowflake guild_id, dpp::snowflake event_id, const char* name = nullptr, const char* description = nullptr, time_t start_time = 0, time_t end_time = 0, Callback callback = nullptr);
	void ModifyFromObject(const DiscordScheduledEvent* event_obj, Callback callback = nullptr);
	void Delete(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void Start(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void End(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void Cancel(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
};
