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

#include "features/discord_interaction.h"
#include "entities/discord_message.h"

DiscordInteraction::DiscordInteraction(const dpp::slashcommand_t& interaction, DiscordClient* client) :
	m_interaction(interaction),
	m_commandName(interaction.command.get_command_name()),
	m_client(client)
{
}

const char* DiscordInteraction::GetCommandName() const {
	return m_commandName.c_str();
}

std::string DiscordInteraction::GetGuildId() const {
	return m_interaction.command.guild_id.str();
}

std::string DiscordInteraction::GetChannelId() const {
	return m_interaction.command.channel_id.str();
}

Handle_t DiscordInteraction::GetUserHandle() const {
	return m_userHandle.GetOrCreate([this]() { return GetUser(); });
}

DiscordUser* DiscordInteraction::GetUser() const {
	if (!m_client) return nullptr;
	return new DiscordUser(m_interaction.command.usr, m_client.Get());
}

std::string DiscordInteraction::GetUserId() const {
	return m_interaction.command.usr.id.str();
}

const char* DiscordInteraction::GetUserName() const {
	return m_interaction.command.usr.username.c_str();
}

std::string DiscordInteraction::GetUserNickName() const {
	return m_interaction.command.member.get_nickname();
}

bool DiscordInteraction::GetOptionValue(const char* name, std::string& value) const {
	auto param = m_interaction.get_parameter(name);
	if (param.index() == 0) return false;
	if (auto* str = std::get_if<std::string>(&param)) {
		value = *str;
		return true;
	}
	return false;
}

bool DiscordInteraction::GetOptionValueInt(const char* name, int64_t& value) const {
	auto param = m_interaction.get_parameter(name);
	if (param.index() == 0) return false;
	if (auto* val = std::get_if<int64_t>(&param)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordInteraction::GetOptionValueDouble(const char* name, double& value) const {
	auto param = m_interaction.get_parameter(name);
	if (param.index() == 0) return false;
	if (auto* val = std::get_if<double>(&param)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordInteraction::GetOptionValueBool(const char* name, bool& value) const {
	auto param = m_interaction.get_parameter(name);
	if (param.index() == 0) return false;
	if (auto* val = std::get_if<bool>(&param)) {
		value = *val;
		return true;
	}
	return false;
}

void DiscordInteraction::CreateResponse(const char* content) const {
	m_interaction.reply(dpp::message(content));
}

void DiscordInteraction::CreateResponseEmbed(const char* content, const DiscordEmbed* embed) const {
	dpp::message msg(content);
	msg.add_embed(embed->GetEmbed());
	m_interaction.reply(msg);
}

void DiscordInteraction::DeferReply(bool ephemeral) const {
	m_interaction.thinking(ephemeral);
}

void DiscordInteraction::EditResponse(const char* content) const {
	m_interaction.edit_response(dpp::message(content));
}

void DiscordInteraction::CreateEphemeralResponse(const char* content) const {
	dpp::message msg(content);
	msg.set_flags(dpp::m_ephemeral);
	m_interaction.reply(msg);
}

void DiscordInteraction::CreateEphemeralResponseEmbed(const char* content, const DiscordEmbed* embed) const {
	dpp::message msg(content);
	msg.set_flags(dpp::m_ephemeral);
	msg.add_embed(embed->GetEmbed());
	m_interaction.reply(msg);
}

void DiscordInteraction::FollowUp(const char* content) const {
	m_interaction.follow_up(dpp::message(content));
}

void DiscordInteraction::FollowUpEmbed(const char* content, const DiscordEmbed* embed) const {
	dpp::message msg(content ? content : "");
	if (embed) msg.add_embed(embed->GetEmbed());
	m_interaction.follow_up(msg);
}

void DiscordInteraction::FollowUpEphemeral(const char* content) const {
	dpp::message msg(content);
	msg.set_flags(dpp::m_ephemeral);
	m_interaction.follow_up(msg);
}

void DiscordInteraction::EditFollowUp(dpp::snowflake message_id, const char* content) const {
	if (!m_client) return;
	dpp::message msg(content);
	msg.id = message_id;
	m_client->Interactions().EditFollowUp(m_interaction.command.token, msg);
}

void DiscordInteraction::EditFollowUpEmbed(dpp::snowflake message_id, const char* content, const DiscordEmbed* embed) const {
	if (!m_client) return;
	dpp::message msg(content ? content : "");
	msg.id = message_id;
	if (embed) msg.add_embed(embed->GetEmbed());
	m_client->Interactions().EditFollowUp(m_interaction.command.token, msg);
}

void DiscordInteraction::CreateResponseFromObject(const DiscordMessage* message) const {
	if (!message) return;
	m_interaction.reply(message->GetDPPMessage());
}

void DiscordInteraction::EditResponseFromObject(const DiscordMessage* message) const {
	if (!message) return;
	m_interaction.edit_response(message->GetDPPMessage());
}

void DiscordInteraction::FollowUpFromObject(const DiscordMessage* message) const {
	if (!message) return;
	m_interaction.follow_up(message->GetDPPMessage());
}

void DiscordInteraction::EditFollowUpFromObject(dpp::snowflake message_id, const DiscordMessage* message) const {
	if (!m_client || !message) return;
	dpp::message msg = message->GetDPPMessage();
	msg.id = message_id;
	m_client->Interactions().EditFollowUp(m_interaction.command.token, msg);
}

std::string DiscordInteraction::GetToken() const {
	return m_interaction.command.token;
}

dpp::snowflake DiscordInteraction::GetInteractionId() const {
	return m_interaction.command.id;
}

DiscordAutocompleteInteraction::DiscordAutocompleteInteraction(const dpp::autocomplete_t& autocomplete, DiscordClient* client) :
	m_response(dpp::ir_autocomplete_reply),
	m_commandName(autocomplete.name),
	m_command(autocomplete.command),
	m_autocomplete(autocomplete),
	m_client(client)
{
}

const char* DiscordAutocompleteInteraction::GetCommandName() const {
	return m_commandName.c_str();
}

std::string DiscordAutocompleteInteraction::GetGuildId() const {
	return m_command.guild_id.str();
}

std::string DiscordAutocompleteInteraction::GetChannelId() const {
	return m_command.channel_id.str();
}

Handle_t DiscordAutocompleteInteraction::GetUserHandle() const {
	return m_userHandle.GetOrCreate([this]() { return GetUser(); });
}

DiscordUser* DiscordAutocompleteInteraction::GetUser() const {
	if (!m_client) return nullptr;
	return new DiscordUser(m_command.usr, m_client.Get());
}

std::string DiscordAutocompleteInteraction::GetUserNickName() const {
	return m_command.member.get_nickname();
}

const dpp::command_option* DiscordAutocompleteInteraction::GetOption(const char* name) const {
	for (const auto& opt : m_autocomplete.options) {
		if (opt.name == name) return &opt;
	}
	return nullptr;
}

bool DiscordAutocompleteInteraction::GetOptionValue(const char* name, std::string& value) const {
	auto* opt = GetOption(name);
	if (!opt) return false;
	if (auto* str = std::get_if<std::string>(&opt->value)) {
		value = *str;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetOptionValueInt(const char* name, int64_t& value) const {
	auto* opt = GetOption(name);
	if (!opt) return false;
	if (auto* val = std::get_if<int64_t>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetOptionValueDouble(const char* name, double& value) const {
	auto* opt = GetOption(name);
	if (!opt) return false;
	if (auto* val = std::get_if<double>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetOptionValueBool(const char* name, bool& value) const {
	auto* opt = GetOption(name);
	if (!opt) return false;
	if (auto* val = std::get_if<bool>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}

void DiscordAutocompleteInteraction::AddAutocompleteOption(dpp::command_option_choice choice) {
	m_response.add_autocomplete_choice(choice);
}

void DiscordAutocompleteInteraction::CreateAutocompleteResponse() {
	if (!m_client) return;
	m_client->Commands().CreateAutocompleteResponse(m_command.id, m_command.token, m_response);
}

const dpp::command_option* DiscordAutocompleteInteraction::GetFocusedOption() const {
	for (const auto& opt : m_autocomplete.options) {
		if (opt.focused) return &opt;
	}
	return nullptr;
}

std::string DiscordAutocompleteInteraction::GetFocusedOptionName() const {
	auto* opt = GetFocusedOption();
	return opt ? opt->name : "";
}

dpp::command_option_type DiscordAutocompleteInteraction::GetFocusedOptionType() const {
	auto* opt = GetFocusedOption();
	return opt ? opt->type : dpp::co_string;
}

bool DiscordAutocompleteInteraction::GetFocusedOptionValue(std::string& value) const {
	auto* opt = GetFocusedOption();
	if (!opt) return false;
	if (auto* str = std::get_if<std::string>(&opt->value)) {
		value = *str;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetFocusedOptionValueInt(int64_t& value) const {
	auto* opt = GetFocusedOption();
	if (!opt) return false;
	if (auto* val = std::get_if<int64_t>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetFocusedOptionValueDouble(double& value) const {
	auto* opt = GetFocusedOption();
	if (!opt) return false;
	if (auto* val = std::get_if<double>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}

bool DiscordAutocompleteInteraction::GetFocusedOptionValueBool(bool& value) const {
	auto* opt = GetFocusedOption();
	if (!opt) return false;
	if (auto* val = std::get_if<bool>(&opt->value)) {
		value = *val;
		return true;
	}
	return false;
}
