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

class ScheduledEventOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Modify(dpp::snowflake guild_id, dpp::snowflake event_id, const char* name = nullptr, const char* description = nullptr, time_t start_time = 0, time_t end_time = 0, Callback callback = nullptr);
	void Delete(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void Start(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void End(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
	void Cancel(dpp::snowflake guild_id, dpp::snowflake event_id, Callback callback = nullptr);
};
