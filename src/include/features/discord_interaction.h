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

#include "core/discord_client_ref.h"
#include "utils/discord_embed.h"
#include "entities/discord_user.h"
#include "core/handle_manager.h"
#include "core/discord_client.h"

class DiscordInteraction
{
private:
	dpp::slashcommand_t m_interaction;
	std::string m_commandName;
	DiscordClientRef m_client;
	mutable CachedHandle<DiscordUser> m_userHandle;

public:
	DiscordInteraction(const dpp::slashcommand_t& interaction, DiscordClient* client);

	const char* GetCommandName() const;
	std::string GetGuildId() const;
	std::string GetChannelId() const;
	Handle_t GetUserHandle() const;
	DiscordUser* GetUser() const;
	std::string GetUserId() const;
	const char* GetUserName() const;
	std::string GetUserNickName() const;

	bool GetOptionValue(const char* name, std::string& value) const;
	bool GetOptionValueInt(const char* name, int64_t& value) const;
	bool GetOptionValueDouble(const char* name, double& value) const;
	bool GetOptionValueBool(const char* name, bool& value) const;

	void CreateResponse(const char* content) const;
	void CreateResponseEmbed(const char* content, const DiscordEmbed* embed) const;
	void DeferReply(bool ephemeral = false) const;
	void EditResponse(const char* content) const;
	void CreateEphemeralResponse(const char* content) const;
	void CreateEphemeralResponseEmbed(const char* content, const DiscordEmbed* embed) const;
	void FollowUp(const char* content) const;
	void FollowUpEmbed(const char* content, const DiscordEmbed* embed) const;
	void FollowUpEphemeral(const char* content) const;
	void EditFollowUp(dpp::snowflake message_id, const char* content) const;
	void EditFollowUpEmbed(dpp::snowflake message_id, const char* content, const DiscordEmbed* embed) const;

	void CreateResponseFromObject(const class DiscordMessage* message) const;
	void EditResponseFromObject(const class DiscordMessage* message) const;
	void FollowUpFromObject(const class DiscordMessage* message) const;
	void EditFollowUpFromObject(dpp::snowflake message_id, const class DiscordMessage* message) const;

	std::string GetToken() const;
	dpp::snowflake GetInteractionId() const;
};

class DiscordAutocompleteInteraction
{
private:
	dpp::interaction_response m_response;
	std::string m_commandName;
	dpp::interaction m_command;
	dpp::autocomplete_t m_autocomplete;
	DiscordClientRef m_client;
	mutable CachedHandle<DiscordUser> m_userHandle;

public:
	DiscordAutocompleteInteraction(const dpp::autocomplete_t& autocomplete, DiscordClient* client);

	DiscordClient* GetClient() const { return m_client.Get(); }
	dpp::interaction_response& GetResponse() { return m_response; }
	const dpp::interaction& GetCommand() const { return m_command; }

	const char* GetCommandName() const;
	std::string GetGuildId() const;
	std::string GetChannelId() const;
	Handle_t GetUserHandle() const;
	DiscordUser* GetUser() const;
	std::string GetUserNickName() const;

	const dpp::command_option* GetOption(const char* name) const;
	bool GetOptionValue(const char* name, std::string& value) const;
	bool GetOptionValueInt(const char* name, int64_t& value) const;
	bool GetOptionValueDouble(const char* name, double& value) const;
	bool GetOptionValueBool(const char* name, bool& value) const;

	void AddAutocompleteOption(dpp::command_option_choice choice);
	void CreateAutocompleteResponse();

	const dpp::command_option* GetFocusedOption() const;
	std::string GetFocusedOptionName() const;
	dpp::command_option_type GetFocusedOptionType() const;
	bool GetFocusedOptionValue(std::string& value) const;
	bool GetFocusedOptionValueInt(int64_t& value) const;
	bool GetFocusedOptionValueDouble(double& value) const;
	bool GetFocusedOptionValueBool(bool& value) const;
};
