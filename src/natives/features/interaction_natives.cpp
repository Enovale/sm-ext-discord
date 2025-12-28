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

#include "natives/natives_common.h"
#include "features/discord_interaction.h"
#include "utils/discord_embed.h"

static cell_t interaction_CreateResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->CreateResponse(content);
	return 1;
}

static cell_t interaction_CreateResponseEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	interaction->CreateResponseEmbed(content, embed);
	return 1;
}

static cell_t interaction_GetOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	std::string value;
	if (!interaction->GetOptionValue(name, value)) {
		return 0;
	}

	pContext->StringToLocal(params[3], params[4], value.c_str());
	return 1;
}

static cell_t interaction_GetOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value;
	if (!interaction->GetOptionValueInt(name, value)) {
		return 0;
	}

	// Check if value fits in 32-bit int
	if (value > INT32_MAX || value < INT32_MIN) {
		pContext->ReportError("Integer option '%s' value %lld exceeds 32-bit range, data will be truncated. Use GetOptionValueInt64 instead.", name, value);
		return 0;
	}

	return static_cast<cell_t>(value);
}

static cell_t interaction_GetOptionValueInt64(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value;
	if (!interaction->GetOptionValueInt(name, value)) {
		return 0;
	}

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%lld", value);
	pContext->StringToLocal(params[3], params[4], buffer);
	return 1;
}

static cell_t interaction_GetOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	double value;
	if (!interaction->GetOptionValueDouble(name, value)) {
		return 0;
	}

	return sp_ftoc((float)value);
}

static cell_t interaction_GetOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	bool value;
	if (!interaction->GetOptionValueBool(name, value)) {
		return 0;
	}

	return value;
}

static cell_t interaction_DeferReply(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	interaction->DeferReply(params[2]);
	return 1;
}

static cell_t interaction_EditResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->EditResponse(content);
	return 1;
}

static cell_t interaction_CreateEphemeralResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->CreateEphemeralResponse(content);
	return 1;
}

static cell_t interaction_CreateEphemeralResponseEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	interaction->CreateEphemeralResponseEmbed(content, embed);
	return 1;
}

static cell_t interaction_FollowUp(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->FollowUp(content);
	return 1;
}

static cell_t interaction_FollowUpEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	interaction->FollowUpEmbed(content, embed);
	return 1;
}

static cell_t interaction_FollowUpEphemeral(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->FollowUpEphemeral(content);
	return 1;
}

static cell_t interaction_EditFollowUp(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* content;
	pContext->LocalToString(params[3], &content);

	dpp::snowflake msgFlake;
	if (!ParseSnowflake(pContext, messageId, msgFlake)) return 0;

	interaction->EditFollowUp(msgFlake, content);
	return 1;
}

static cell_t interaction_EditFollowUpEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* content;
	pContext->LocalToString(params[3], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[4]);

	dpp::snowflake msgFlake;
	if (!ParseSnowflake(pContext, messageId, msgFlake)) return 0;

	interaction->EditFollowUpEmbed(msgFlake, content, embed);
	return 1;
}

static cell_t interaction_GetCommandName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	const char* commandName = interaction->GetCommandName();
	pContext->StringToLocal(params[2], params[3], commandName);
	return 1;
}

static cell_t interaction_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string guildId = interaction->GetGuildId();
	pContext->StringToLocal(params[2], params[3], guildId.c_str());
	return 1;
}

static cell_t interaction_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string channelId = interaction->GetChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t interaction_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	Handle_t handle = interaction->GetUserHandle();

	if (!handle) {
		pContext->ReportError("Could not create user handle");
		return 0;
	}

	return handle;
}

static cell_t interaction_GetUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string userId = interaction->GetUserId();
	pContext->StringToLocal(params[2], params[3], userId.c_str());
	return 1;
}

static cell_t interaction_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	const char* userName = interaction->GetUserName();
	pContext->StringToLocal(params[2], params[3], userName);
	return 1;
}

static cell_t interaction_GetUserNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickname().c_str());
	return 1;
}

static cell_t autocomplete_GetCommandName(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	const char* commandName = interaction->GetCommandName();
	pContext->StringToLocal(params[2], params[3], commandName);
	return 1;
}

static cell_t autocomplete_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string guildId = interaction->GetGuildId();
	pContext->StringToLocal(params[2], params[3], guildId.c_str());
	return 1;
}

static cell_t autocomplete_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string channelId = interaction->GetChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t autocomplete_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	Handle_t handle = interaction->GetUserHandle();

	if (!handle) {
		pContext->ReportError("Could not create user handle");
		return 0;
	}

	return handle;
}

static cell_t autocomplete_GetUserNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickname().c_str());
	return 1;
}

static cell_t autocomplete_GetOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	std::string value = interaction->GetOptionValue(name);
	pContext->StringToLocal(params[3], params[4], value.c_str());
	return 1;
}

static cell_t autocomplete_GetOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value = interaction->GetOptionValueInt(name);

	// Check if value fits in 32-bit int
	if (value > INT32_MAX || value < INT32_MIN) {
		pContext->ReportError("Integer option '%s' value %lld exceeds 32-bit range, data will be truncated. Use GetOptionValueInt64 instead.", name, value);
		return 0;
	}

	return static_cast<cell_t>(value);
}

static cell_t autocomplete_GetOptionValueInt64(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value = interaction->GetOptionValueInt(name);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%lld", value);
	pContext->StringToLocal(params[3], params[4], buffer);
	return 1;
}

static cell_t autocomplete_GetOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	double value = interaction->GetOptionValueDouble(name);
	return sp_ftoc((float)value);
}

static cell_t autocomplete_GetOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	bool value = interaction->GetOptionValueBool(name);
	return value;
}

static cell_t autocomplete_CreateAutocompleteResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	if (!interaction->m_client) {
		return 0;
	}

	interaction->m_client->Commands().CreateAutocompleteResponse(interaction->m_command.id, interaction->m_command.token, interaction->m_response);
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	dpp::command_value value;
	dpp::command_option_type type = static_cast<dpp::command_option_type>(params[3]);

	if (type == dpp::co_number) {
		value = static_cast<double>(sp_ctof(params[4]));
	} else {
		value = static_cast<int64_t>(params[4]);
	}

	interaction->m_response.add_autocomplete_choice(dpp::command_option_choice(name, value));
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoiceString(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* str_value;
	pContext->LocalToString(params[3], &str_value);

	interaction->m_response.add_autocomplete_choice(dpp::command_option_choice(name, std::string(str_value)));
	return 1;
}

static cell_t autocomplete_GetFocusedOptionName(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string name = interaction->GetFocusedOptionName();
	pContext->StringToLocal(params[2], params[3], name.c_str());
	return 1;
}

static cell_t autocomplete_GetFocusedOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	std::string value = interaction->GetFocusedOptionValue();
	pContext->StringToLocal(params[2], params[3], value.c_str());
	return 1;
}

static cell_t autocomplete_GetFocusedOptionType(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	return static_cast<cell_t>(interaction->GetFocusedOptionType());
}

static cell_t autocomplete_GetFocusedOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	return static_cast<cell_t>(interaction->GetFocusedOptionValueInt());
}

static cell_t autocomplete_GetFocusedOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	return sp_ftoc((float)interaction->GetFocusedOptionValueDouble());
}

static cell_t autocomplete_GetFocusedOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	return interaction->GetFocusedOptionValueBool();
}

extern const sp_nativeinfo_t interaction_natives[] = {
	{"DiscordInteraction.CreateResponse", interaction_CreateResponse},
	{"DiscordInteraction.CreateResponseEmbed", interaction_CreateResponseEmbed},
	{"DiscordInteraction.GetOptionValue", interaction_GetOptionValue},
	{"DiscordInteraction.GetOptionValueInt", interaction_GetOptionValueInt},
	{"DiscordInteraction.GetOptionValueInt64", interaction_GetOptionValueInt64},
	{"DiscordInteraction.GetOptionValueFloat", interaction_GetOptionValueFloat},
	{"DiscordInteraction.GetOptionValueBool", interaction_GetOptionValueBool},
	{"DiscordInteraction.DeferReply", interaction_DeferReply},
	{"DiscordInteraction.EditResponse", interaction_EditResponse},
	{"DiscordInteraction.CreateEphemeralResponse", interaction_CreateEphemeralResponse},
	{"DiscordInteraction.CreateEphemeralResponseEmbed", interaction_CreateEphemeralResponseEmbed},
	{"DiscordInteraction.FollowUp", interaction_FollowUp},
	{"DiscordInteraction.FollowUpEmbed", interaction_FollowUpEmbed},
	{"DiscordInteraction.FollowUpEphemeral", interaction_FollowUpEphemeral},
	{"DiscordInteraction.EditFollowUp", interaction_EditFollowUp},
	{"DiscordInteraction.EditFollowUpEmbed", interaction_EditFollowUpEmbed},
	{"DiscordInteraction.GetCommandName", interaction_GetCommandName},
	{"DiscordInteraction.GetGuildId", interaction_GetGuildId},
	{"DiscordInteraction.GetChannelId", interaction_GetChannelId},
	{"DiscordInteraction.User.get", interaction_GetUser},
	{"DiscordInteraction.GetUserNickname", interaction_GetUserNickname},
	{"DiscordInteraction.GetUserId", interaction_GetUserId},
	{"DiscordInteraction.GetUserName", interaction_GetUserName},
	{"DiscordAutocompleteInteraction.GetCommandName", autocomplete_GetCommandName},
	{"DiscordAutocompleteInteraction.GetGuildId", autocomplete_GetGuildId},
	{"DiscordAutocompleteInteraction.GetChannelId", autocomplete_GetChannelId},
	{"DiscordAutocompleteInteraction.User.get", autocomplete_GetUser},
	{"DiscordAutocompleteInteraction.GetUserNickname", autocomplete_GetUserNickname},
	{"DiscordAutocompleteInteraction.GetOptionValue", autocomplete_GetOptionValue},
	{"DiscordAutocompleteInteraction.GetOptionValueInt", autocomplete_GetOptionValueInt},
	{"DiscordAutocompleteInteraction.GetOptionValueInt64", autocomplete_GetOptionValueInt64},
	{"DiscordAutocompleteInteraction.GetOptionValueFloat", autocomplete_GetOptionValueFloat},
	{"DiscordAutocompleteInteraction.GetOptionValueBool", autocomplete_GetOptionValueBool},
	{"DiscordAutocompleteInteraction.CreateAutocompleteResponse", autocomplete_CreateAutocompleteResponse},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoice", autocomplete_AddAutocompleteChoice},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceString", autocomplete_AddAutocompleteChoiceString},
	{"DiscordAutocompleteInteraction.GetFocusedOptionName", autocomplete_GetFocusedOptionName},
	{"DiscordAutocompleteInteraction.FocusedOptionType.get", autocomplete_GetFocusedOptionType},
	{"DiscordAutocompleteInteraction.GetFocusedOptionValue", autocomplete_GetFocusedOptionValue},
	{"DiscordAutocompleteInteraction.FocusedOptionValueInt.get", autocomplete_GetFocusedOptionValueInt},
	{"DiscordAutocompleteInteraction.FocusedOptionValueFloat.get", autocomplete_GetFocusedOptionValueFloat},
	{"DiscordAutocompleteInteraction.FocusedOptionValueBool.get", autocomplete_GetFocusedOptionValueBool},
	{nullptr, nullptr}
};
