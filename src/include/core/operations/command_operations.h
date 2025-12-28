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

class CommandOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void RegisterGuild(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback = nullptr);
	void RegisterGlobal(const dpp::slashcommand& command, Callback callback = nullptr);
	void ModifyGuild(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback = nullptr);
	void ModifyGlobal(const dpp::slashcommand& command, Callback callback = nullptr);
	void SetPermissions(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback = nullptr);
	void CreateAutocompleteResponse(dpp::snowflake id, const std::string& token, const dpp::interaction_response& response, Callback callback = nullptr);
	void DeleteGuild(dpp::snowflake guild_id, dpp::snowflake command_id, Callback callback = nullptr);
	void DeleteGlobal(dpp::snowflake command_id, Callback callback = nullptr);
	void BulkDeleteGuild(dpp::snowflake guild_id, Callback callback = nullptr);
	void BulkDeleteGlobal(Callback callback = nullptr);
	void GetGuildCommands(dpp::snowflake guild_id, Callback callback);
	void GetGlobalCommands(Callback callback);
};
