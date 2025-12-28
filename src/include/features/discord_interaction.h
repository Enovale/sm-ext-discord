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

#include "utils/discord_embed.h"
#include "entities/discord_user.h"
#include "core/handle_manager.h"
#include "core/discord_client.h"

class DiscordInteraction
{
private:
	dpp::slashcommand_t m_interaction;
	std::string m_commandName;
	DiscordClient* m_client;
	mutable CachedHandle<DiscordUser> m_userHandle;

public:
	DiscordInteraction(const dpp::slashcommand_t& interaction, DiscordClient* client) :
		m_interaction(interaction),
		m_commandName(interaction.command.get_command_name()),
		m_client(client)
	{
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return m_interaction.command.guild_id.str(); }
	std::string GetChannelId() const { return m_interaction.command.channel_id.str(); }

	Handle_t GetUserHandle() const {
		return m_userHandle.GetOrCreate([this]() { return GetUser(); });
	}

	DiscordUser* GetUser() const {
		if (!m_client) return nullptr;
		if (m_interaction.command.guild_id != 0 && m_interaction.command.member.user_id != 0) {
			return new DiscordUser(m_interaction.command.usr, m_interaction.command.member, m_client);
		}
		return new DiscordUser(m_interaction.command.usr, m_client);
	}
	std::string GetUserId() const { return m_interaction.command.usr.id.str(); }
	const char* GetUserName() const { return m_interaction.command.usr.username.c_str(); }
	std::string GetUserNickname() const { return m_interaction.command.member.get_nickname(); }

	bool GetOptionValue(const char* name, std::string& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<std::string>(param);
		return true;
	}

	bool GetOptionValueInt(const char* name, int64_t& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<int64_t>(param);
		return true;
	}

	bool GetOptionValueDouble(const char* name, double& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<double>(param);
		return true;
	}

	bool GetOptionValueBool(const char* name, bool& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<bool>(param);
		return true;
	}

	void CreateResponse(const char* content) const {
		m_interaction.reply(dpp::message(content));
	}

	void CreateResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}

	void DeferReply(bool ephemeral = false) const {
		m_interaction.thinking(ephemeral);
	}

	void EditResponse(const char* content) const {
		m_interaction.edit_response(dpp::message(content));
	}

	void CreateEphemeralResponse(const char* content) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		m_interaction.reply(msg);
	}

	void CreateEphemeralResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}

	void FollowUp(const char* content) const {
		m_interaction.follow_up(dpp::message(content));
	}

	void FollowUpEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content ? content : "");
		if (embed) msg.add_embed(embed->GetEmbed());
		m_interaction.follow_up(msg);
	}

	void FollowUpEphemeral(const char* content) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		m_interaction.follow_up(msg);
	}

	void EditFollowUp(dpp::snowflake message_id, const char* content) const {
		if (!m_client || !m_client->GetCluster()) return;
		dpp::message msg(content);
		msg.id = message_id;
		m_client->GetCluster()->interaction_followup_edit(m_interaction.command.token, msg);
	}

	void EditFollowUpEmbed(dpp::snowflake message_id, const char* content, const DiscordEmbed* embed) const {
		if (!m_client || !m_client->GetCluster()) return;
		dpp::message msg(content ? content : "");
		msg.id = message_id;
		if (embed) msg.add_embed(embed->GetEmbed());
		m_client->GetCluster()->interaction_followup_edit(m_interaction.command.token, msg);
	}

	std::string GetToken() const { return m_interaction.command.token; }
	dpp::snowflake GetInteractionId() const { return m_interaction.command.id; }
};

class DiscordAutocompleteInteraction
{
private:
	mutable CachedHandle<DiscordUser> m_userHandle;

public:
	dpp::interaction_response m_response;
	std::string m_commandName;
	dpp::interaction m_command;
	dpp::autocomplete_t m_autocomplete;
	DiscordClient* m_client;

	DiscordAutocompleteInteraction(const dpp::autocomplete_t& autocomplete, DiscordClient* client) :
		m_response(dpp::ir_autocomplete_reply),
		m_commandName(autocomplete.name),
		m_command(autocomplete.command),
		m_autocomplete(autocomplete),
		m_client(client)
	{
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return m_command.guild_id.str(); }
	std::string GetChannelId() const { return m_command.channel_id.str(); }

	Handle_t GetUserHandle() const {
		return m_userHandle.GetOrCreate([this]() { return GetUser(); });
	}

	DiscordUser* GetUser() const {
		if (!m_client) return nullptr;
		if (m_command.guild_id != 0 && m_command.member.user_id != 0) {
			return new DiscordUser(m_command.usr, m_command.member, m_client);
		}
		return new DiscordUser(m_command.usr, m_client);
	}
	std::string GetUserNickname() const { return m_command.member.get_nickname(); }

	dpp::command_option GetOption(const char* name) const {
		for (auto & opt : m_autocomplete.options) {
			if (opt.name == name) return opt;
		}

		throw std::runtime_error("Option not found");
	}

	std::string GetOptionValue(const char* name) const {
		auto opt = GetOption(name);
		if (auto* str = std::get_if<std::string>(&opt.value)) return *str;
		return "";
	}

	int64_t GetOptionValueInt(const char* name) const {
		auto opt = GetOption(name);
		if (auto* val = std::get_if<int64_t>(&opt.value)) return *val;
		return 0;
	}

	double GetOptionValueDouble(const char* name) const {
		auto opt = GetOption(name);
		if (auto* val = std::get_if<double>(&opt.value)) return *val;
		return 0.0;
	}

	bool GetOptionValueBool(const char* name) const {
		auto opt = GetOption(name);
		if (auto* val = std::get_if<bool>(&opt.value)) return *val;
		return false;
	}

	void AddAutocompleteOption(dpp::command_option_choice choice) {
		m_response.add_autocomplete_choice(choice);
	}

	const dpp::command_option* GetFocusedOption() const {
		for (const auto& opt : m_autocomplete.options) {
			if (opt.focused) return &opt;
		}
		return nullptr;
	}

	std::string GetFocusedOptionName() const {
		auto* opt = GetFocusedOption();
		return opt ? opt->name : "";
	}

	dpp::command_option_type GetFocusedOptionType() const {
		auto* opt = GetFocusedOption();
		return opt ? opt->type : dpp::co_string;
	}

	std::string GetFocusedOptionValue() const {
		auto* opt = GetFocusedOption();
		if (!opt) return "";
		if (auto* str = std::get_if<std::string>(&opt->value)) return *str;
		return "";
	}

	int64_t GetFocusedOptionValueInt() const {
		auto* opt = GetFocusedOption();
		if (!opt) return 0;
		if (auto* val = std::get_if<int64_t>(&opt->value)) return *val;
		return 0;
	}

	double GetFocusedOptionValueDouble() const {
		auto* opt = GetFocusedOption();
		if (!opt) return 0.0;
		if (auto* val = std::get_if<double>(&opt->value)) return *val;
		return 0.0;
	}

	bool GetFocusedOptionValueBool() const {
		auto* opt = GetFocusedOption();
		if (!opt) return false;
		if (auto* val = std::get_if<bool>(&opt->value)) return *val;
		return false;
	}
};
