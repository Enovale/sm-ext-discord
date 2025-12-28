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

#include "utils/discord_common.h"

class DiscordSlashCommand
{
private:
	dpp::slashcommand m_command;
	std::vector<dpp::command_option> m_options;
	DiscordClient* m_client;
	dpp::snowflake m_guild_id; // Store guild ID for guild-specific commands
	std::vector<dpp::command_permission> m_permissions; // Store command permissions

public:
	DiscordSlashCommand(DiscordClient* client) : m_client(client), m_guild_id(0) {}

	void SetName(const char* name) { m_command.set_name(name); }
	void SetDescription(const char* description) { m_command.set_description(description); }
	void SetDefaultPermissions(const char* permissions) {
		if (permissions && permissions[0] != '\0') {
			uint64_t perm;
			if (ParseUInt64(permissions, perm)) {
				m_command.set_default_permissions(perm);
			}
		}
	}
	void SetApplicationId(dpp::snowflake app_id) { m_command.set_application_id(app_id); }

	// Methods for managing existing commands
	void SetCommandId(dpp::snowflake command_id) { m_command.id = command_id; }
	void SetGuildId(dpp::snowflake guild_id) { m_guild_id = guild_id; }
	dpp::snowflake GetCommandId() const { return m_command.id; }
	dpp::snowflake GetGuildId() const { return m_guild_id; }

	const char* GetName() const { return m_command.name.c_str(); }
	const char* GetDescription() const { return m_command.description.c_str(); }
	std::string GetDefaultPermissions() const {
		char permStr[32];
		snprintf(permStr, sizeof(permStr), "%" PRIu64, static_cast<uint64_t>(m_command.default_member_permissions));
		return std::string(permStr);
	}

	void AddOption(const char* name, const char* description, dpp::command_option_type type, bool required = false, bool autocomplete = false) {
		dpp::command_option option(type, name, description, required);
		option.set_auto_complete(autocomplete);
		m_options.push_back(option);
		m_command.options = m_options;
	}

	void AddChoiceOption(const char* name, const char* description, dpp::command_option_type type, bool required = false) {
		dpp::command_option option(type, name, description, required);
		m_options.push_back(option);
		m_command.options = m_options;
	}

	void AddStringChoice(const char* choice_name, const char* choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, std::string(choice_value)));
			m_command.options = m_options;
		}
	}

	void AddIntChoice(const char* choice_name, int64_t choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, choice_value));
			m_command.options = m_options;
		}
	}

	void AddFloatChoice(const char* choice_name, double choice_value) {
		if (!m_options.empty()) {
			m_options.back().add_choice(dpp::command_option_choice(choice_name, choice_value));
			m_command.options = m_options;
		}
	}

	void RegisterToGuild(dpp::snowflake guild_id);
	void RegisterGlobally();

	// New advanced functionality methods
	void SetContextMenuType(dpp::slashcommand_contextmenu_type type) { m_command.set_type(type); }
	dpp::slashcommand_contextmenu_type GetContextMenuType() const { return m_command.type; }

	void SetNSFW(bool nsfw) { m_command.set_nsfw(nsfw); }
	bool GetNSFW() const { return m_command.nsfw; }

	void SetDMPermission(bool dm_permission) { m_command.set_dm_permission(dm_permission); }
	bool GetDMPermission() const { return m_command.dm_permission; }

	void AddLocalization(const char* language, const char* name, const char* description = nullptr) {
		if (description) {
			m_command.add_localization(language, name, description);
		} else {
			m_command.add_localization(language, name);
		}
	}

	void SetInteractionContexts(const std::vector<dpp::interaction_context_type>& contexts) {
		m_command.set_interaction_contexts(contexts);
	}

	void SetIntegrationTypes(const std::vector<dpp::application_integration_types>& types) {
		m_command.integration_types.clear();
		for (const auto& type : types) {
			m_command.integration_types.push_back(type);
		}
	}

	// Option-specific methods for the last added option
	void SetLastOptionMinValue(double min_value) {
		if (!m_options.empty()) {
			m_options.back().set_min_value(min_value);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMaxValue(double max_value) {
		if (!m_options.empty()) {
			m_options.back().set_max_value(max_value);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMinLength(int64_t min_length) {
		if (!m_options.empty()) {
			m_options.back().set_min_length(min_length);
			m_command.options = m_options;
		}
	}

	void SetLastOptionMaxLength(int64_t max_length) {
		if (!m_options.empty()) {
			m_options.back().set_max_length(max_length);
			m_command.options = m_options;
		}
	}

	void AddLastOptionChannelType(dpp::channel_type channel_type) {
		if (!m_options.empty()) {
			m_options.back().add_channel_type(channel_type);
			m_command.options = m_options;
		}
	}

	std::string GetMention() const { return m_command.get_mention(); }

	// Permission override management methods
	void AddPermissionOverride(dpp::snowflake target_id, dpp::command_permission_type type, bool permission) {
		dpp::command_permission perm;
		perm.id = target_id;
		perm.type = type;
		perm.permission = permission;

		RemovePermissionOverride(target_id, type);
		m_permissions.push_back(perm);
	}

	void RemovePermissionOverride(dpp::snowflake target_id, dpp::command_permission_type type) {
		m_permissions.erase(
			std::remove_if(m_permissions.begin(), m_permissions.end(),
				[target_id, type](const dpp::command_permission& perm) {
					return perm.id == target_id && perm.type == type;
				}),
			m_permissions.end()
		);
	}

	void ClearPermissionOverrides() { m_permissions.clear(); }

	size_t GetPermissionOverrideCount() const { return m_permissions.size(); }

	bool GetPermissionOverride(size_t index, dpp::snowflake& target_id, dpp::command_permission_type& type, bool& permission) const {
		if (index >= m_permissions.size()) return false;
		target_id = m_permissions[index].id;
		type = m_permissions[index].type;
		permission = m_permissions[index].permission;
		return true;
	}


	bool Update(dpp::snowflake guild_id = 0);
	void Delete(dpp::snowflake guild_id = 0);
	bool ApplyPermissionOverrides(dpp::snowflake guild_id);

	const dpp::slashcommand& GetCommand() const { return m_command; }
};
