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

#include "natives/natives_common.h"
#include "features/discord_interaction.h"
#include "utils/discord_embed.h"
#include "entities/discord_message.h"

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
	if (!IsInt32Range(value)) {
		pContext->ReportError("Integer option exceeds 32-bit range. Use GetOptionValueInt64() instead.");
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
	FormatInt64(value, buffer, sizeof(buffer));
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

static cell_t interaction_CreateResponseFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	interaction->CreateResponseFromObject(message);
	return 1;
}

static cell_t interaction_EditResponseFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	interaction->EditResponseFromObject(message);
	return 1;
}

static cell_t interaction_FollowUpFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	interaction->FollowUpFromObject(message);
	return 1;
}

static cell_t interaction_EditFollowUpFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[3]);
	if (!message) return 0;

	dpp::snowflake msgFlake;
	if (!ParseSnowflake(pContext, messageId, msgFlake)) return 0;

	interaction->EditFollowUpFromObject(msgFlake, message);
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

static cell_t interaction_GetUserNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = Handles.GetPointer<DiscordInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickName().c_str());
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

static cell_t autocomplete_GetUserNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickName().c_str());
	return 1;
}

static cell_t autocomplete_GetOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
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

static cell_t autocomplete_GetOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value;
	if (!interaction->GetOptionValueInt(name, value)) {
		return 0;
	}

	// Check if value fits in 32-bit int
	if (!IsInt32Range(value)) {
		pContext->ReportError("Integer option exceeds 32-bit range. Use GetOptionValueInt64() instead.");
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

	int64_t value;
	if (!interaction->GetOptionValueInt(name, value)) {
		return 0;
	}

	char buffer[32];
	FormatInt64(value, buffer, sizeof(buffer));
	pContext->StringToLocal(params[3], params[4], buffer);
	return 1;
}

static cell_t autocomplete_GetOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	double value;
	if (!interaction->GetOptionValueDouble(name, value)) {
		return 0;
	}

	return sp_ftoc((float)value);
}

static cell_t autocomplete_GetOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	bool value;
	if (!interaction->GetOptionValueBool(name, value)) {
		return 0;
	}

	return value;
}

static cell_t autocomplete_CreateAutocompleteResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	if (!interaction->GetClient()) {
		return 0;
	}

	interaction->CreateAutocompleteResponse();
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

	interaction->GetResponse().add_autocomplete_choice(dpp::command_option_choice(name, std::string(str_value)));
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoiceInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value = static_cast<int64_t>(params[3]);
	if (!IsInt32Range(value)) {
		pContext->ReportError("Autocomplete integer choice value exceeds 32-bit range. Use AddAutocompleteChoiceInt64() instead.");
		return 0;
	}

	interaction->GetResponse().add_autocomplete_choice(dpp::command_option_choice(name, value));
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoiceInt64(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* value_str;
	pContext->LocalToString(params[3], &value_str);

	int64_t value;
	if (!ParseInt64(value_str, value)) {
		const char* display_value = "(null)";
		if (value_str) {
			display_value = value_str;
		}
		pContext->ReportError("Invalid autocomplete integer choice value: %s", display_value);
		return 0;
	}

	interaction->GetResponse().add_autocomplete_choice(dpp::command_option_choice(name, value));
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoiceFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	interaction->GetResponse().add_autocomplete_choice(dpp::command_option_choice(name, static_cast<double>(sp_ctof(params[3]))));
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

	std::string value;
	if (!interaction->GetFocusedOptionValue(value)) {
		return 0;
	}
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

	int64_t value;
	if (!interaction->GetFocusedOptionValueInt(value)) {
		return 0;
	}
	if (!IsInt32Range(value)) {
		pContext->ReportError("Integer option exceeds 32-bit range. Use GetFocusedOptionValueInt64() instead.");
		return 0;
	}
	return static_cast<cell_t>(value);
}

static cell_t autocomplete_GetFocusedOptionValueInt64(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	int64_t value;
	if (!interaction->GetFocusedOptionValueInt(value)) {
		return 0;
	}
	char buffer[32];
	FormatInt64(value, buffer, sizeof(buffer));
	pContext->StringToLocal(params[2], params[3], buffer);
	return 1;
}

static cell_t autocomplete_GetFocusedOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	double value;
	if (!interaction->GetFocusedOptionValueDouble(value)) {
		return 0;
	}
	return sp_ftoc((float)value);
}

static cell_t autocomplete_GetFocusedOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = Handles.GetPointer<DiscordAutocompleteInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	bool value;
	if (!interaction->GetFocusedOptionValueBool(value)) {
		return 0;
	}
	return value;
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
	{"DiscordInteraction.CreateResponseFromObject", interaction_CreateResponseFromObject},
	{"DiscordInteraction.EditResponseFromObject", interaction_EditResponseFromObject},
	{"DiscordInteraction.FollowUpFromObject", interaction_FollowUpFromObject},
	{"DiscordInteraction.EditFollowUpFromObject", interaction_EditFollowUpFromObject},
	{"DiscordInteraction.GetCommandName", interaction_GetCommandName},
	{"DiscordInteraction.GetGuildId", interaction_GetGuildId},
	{"DiscordInteraction.GetChannelId", interaction_GetChannelId},
	{"DiscordInteraction.User.get", interaction_GetUser},
	{"DiscordInteraction.GetUserNickName", interaction_GetUserNickName},
	{"DiscordInteraction.GetUserId", interaction_GetUserId},
	{"DiscordInteraction.GetUserName", interaction_GetUserName},
	{"DiscordAutocompleteInteraction.GetCommandName", autocomplete_GetCommandName},
	{"DiscordAutocompleteInteraction.GetGuildId", autocomplete_GetGuildId},
	{"DiscordAutocompleteInteraction.GetChannelId", autocomplete_GetChannelId},
	{"DiscordAutocompleteInteraction.User.get", autocomplete_GetUser},
	{"DiscordAutocompleteInteraction.GetUserNickName", autocomplete_GetUserNickName},
	{"DiscordAutocompleteInteraction.GetOptionValue", autocomplete_GetOptionValue},
	{"DiscordAutocompleteInteraction.GetOptionValueInt", autocomplete_GetOptionValueInt},
	{"DiscordAutocompleteInteraction.GetOptionValueInt64", autocomplete_GetOptionValueInt64},
	{"DiscordAutocompleteInteraction.GetOptionValueFloat", autocomplete_GetOptionValueFloat},
	{"DiscordAutocompleteInteraction.GetOptionValueBool", autocomplete_GetOptionValueBool},
	{"DiscordAutocompleteInteraction.CreateAutocompleteResponse", autocomplete_CreateAutocompleteResponse},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceString", autocomplete_AddAutocompleteChoiceString},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceInt", autocomplete_AddAutocompleteChoiceInt},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceInt64", autocomplete_AddAutocompleteChoiceInt64},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceFloat", autocomplete_AddAutocompleteChoiceFloat},
	{"DiscordAutocompleteInteraction.GetFocusedOptionName", autocomplete_GetFocusedOptionName},
	{"DiscordAutocompleteInteraction.FocusedOptionType.get", autocomplete_GetFocusedOptionType},
	{"DiscordAutocompleteInteraction.GetFocusedOptionValue", autocomplete_GetFocusedOptionValue},
	{"DiscordAutocompleteInteraction.FocusedOptionValueInt.get", autocomplete_GetFocusedOptionValueInt},
	{"DiscordAutocompleteInteraction.GetFocusedOptionValueInt64", autocomplete_GetFocusedOptionValueInt64},
	{"DiscordAutocompleteInteraction.FocusedOptionValueFloat.get", autocomplete_GetFocusedOptionValueFloat},
	{"DiscordAutocompleteInteraction.FocusedOptionValueBool.get", autocomplete_GetFocusedOptionValueBool},
	{nullptr, nullptr}
};
