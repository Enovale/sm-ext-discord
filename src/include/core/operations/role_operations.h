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

class DiscordRole;

class RoleOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void GetAll(dpp::snowflake guild_id, Callback callback);
	void Create(dpp::snowflake guild_id, const char* name, uint32_t color = 0, bool hoist = false, bool mentionable = false, uint64_t permissions = 0, IPluginFunction* callback = nullptr, cell_t data = 0);
	void CreateFromObject(dpp::snowflake guild_id, const DiscordRole* role_obj, IPluginFunction* callback = nullptr, cell_t data = 0);
	void Modify(dpp::snowflake guild_id, dpp::snowflake role_id, const std::string& name = "", uint32_t color = 0, bool hoist = false, bool mentionable = false, uint64_t permissions = 0, Callback callback = nullptr);
	void Delete(dpp::snowflake guild_id, dpp::snowflake role_id, Callback callback = nullptr);
};
