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

#include "features/discord_slashcommand.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"

void DiscordSlashCommand::RegisterToGuild(dpp::snowflake guild_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	m_command.set_application_id(m_client->GetBotIdSnowflake());
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Commands().RegisterGuild(guild_id, m_command, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandRegister);
		});
	} else {
		m_client->Commands().RegisterGuild(guild_id, m_command);
	}
}

void DiscordSlashCommand::RegisterGlobally(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	m_command.set_application_id(m_client->GetBotIdSnowflake());
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Commands().RegisterGlobal(m_command, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandRegister);
		});
	} else {
		m_client->Commands().RegisterGlobal(m_command);
	}
}

bool DiscordSlashCommand::Update(dpp::snowflake guild_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return false;

	dpp::snowflake target_guild = (guild_id != 0) ? guild_id : m_guild_id;

	if (target_guild != 0) {
		if (callback) {
			Handle_t client_handle = m_client->GetHandle();
			m_client->Commands().ModifyGuild(target_guild, m_command, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandUpdate);
			});
		} else {
			m_client->Commands().ModifyGuild(target_guild, m_command);
		}
	} else {
		if (callback) {
			Handle_t client_handle = m_client->GetHandle();
			m_client->Commands().ModifyGlobal(m_command, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandUpdate);
			});
		} else {
			m_client->Commands().ModifyGlobal(m_command);
		}
	}
	return true;
}

void DiscordSlashCommand::Delete(dpp::snowflake guild_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	dpp::snowflake target_guild = (guild_id != 0) ? guild_id : m_guild_id;

	if (target_guild != 0) {
		if (callback) {
			Handle_t client_handle = m_client->GetHandle();
			m_client->Commands().DeleteGuild(target_guild, m_command.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandDelete);
			});
		} else {
			m_client->Commands().DeleteGuild(target_guild, m_command.id);
		}
	} else {
		if (callback) {
			Handle_t client_handle = m_client->GetHandle();
			m_client->Commands().DeleteGlobal(m_command.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
				PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandDelete);
			});
		} else {
			m_client->Commands().DeleteGlobal(m_command.id);
		}
	}
}

bool DiscordSlashCommand::ApplyPermissionOverrides(dpp::snowflake guild_id, IPluginFunction* callback, cell_t data) {
	if (!m_client) return false;
	dpp::snowflake target_guild = (guild_id != 0) ? guild_id : m_guild_id;
	if (target_guild == 0) return false;

	m_command.permissions = m_permissions;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Commands().SetPermissions(target_guild, m_command, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandPermission);
		});
	} else {
		m_client->Commands().SetPermissions(target_guild, m_command);
	}
	return true;
}

void DiscordSlashCommand::BulkDeleteGlobal(DiscordClient* client, IPluginFunction* callback, cell_t data) {
	if (!client) return;
	if (callback) {
		Handle_t client_handle = client->GetHandle();
		client->Commands().BulkDeleteGlobal([client_handle, client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::CommandBulkDelete);
		});
	} else {
		client->Commands().BulkDeleteGlobal();
	}
}
