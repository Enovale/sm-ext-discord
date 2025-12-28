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
#include "smsdk_ext.h"

class ChannelOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Get(dpp::snowflake channel_id, Callback callback);
	void Create(dpp::snowflake guild_id, const char* name, dpp::channel_type type, const char* topic = "", dpp::snowflake parent_id = 0, IPluginFunction* callback = nullptr, cell_t data = 0);
	void CreateDM(dpp::snowflake user_id, Callback callback);
	void Modify(dpp::snowflake channel_id, const std::string& name = "", const std::string& topic = "", uint16_t position = 0, bool nsfw = false, uint16_t rate_limit = 0, uint16_t bitrate = 0, uint8_t user_limit = 0, dpp::snowflake parent_id = 0, Callback callback = nullptr);
	void Delete(dpp::snowflake channel_id, Callback callback = nullptr);
	void Typing(dpp::snowflake channel_id, Callback callback = nullptr);
};
