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

#include "core/operations/command_operations.h"
#include "utils/discord_common.h"

void CommandOperations::RegisterGuild(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_command_create(command, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to register slash command"); });
}

void CommandOperations::RegisterGlobal(const dpp::slashcommand& command, Callback callback) {
	if (!IsValid()) return;
	m_cluster->global_command_create(command, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to register global slash command"); });
}

void CommandOperations::ModifyGuild(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_command_edit(command, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify guild command"); });
}

void CommandOperations::ModifyGlobal(const dpp::slashcommand& command, Callback callback) {
	if (!IsValid()) return;
	m_cluster->global_command_edit(command, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify global command"); });
}

void CommandOperations::SetPermissions(dpp::snowflake guild_id, const dpp::slashcommand& command, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_command_edit_permissions(command, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to set command permissions"); });
}

void CommandOperations::CreateAutocompleteResponse(dpp::snowflake id, const std::string& token, const dpp::interaction_response& response, Callback callback) {
	if (!IsValid()) return;
	m_cluster->interaction_response_create(id, token, response, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create autocomplete response"); });
}

void CommandOperations::DeleteGuild(dpp::snowflake guild_id, dpp::snowflake command_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_command_delete(command_id, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete guild command"); });
}

void CommandOperations::DeleteGlobal(dpp::snowflake command_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->global_command_delete(command_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete global command"); });
}

void CommandOperations::BulkDeleteGuild(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_bulk_command_delete(guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to bulk delete guild commands"); });
}

void CommandOperations::BulkDeleteGlobal(Callback callback) {
	if (!IsValid()) return;
	m_cluster->global_bulk_command_delete(callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to bulk delete global commands"); });
}

void CommandOperations::GetGuildCommands(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_commands_get(guild_id, callback);
}

void CommandOperations::GetGlobalCommands(Callback callback) {
	if (!IsValid()) return;
	m_cluster->global_commands_get(callback);
}
