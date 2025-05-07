#include "autocomplete_interaction.h"

static cell_t autocomplete_GetCommandName(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	const char* commandName = interaction->GetCommandName();
	pContext->StringToLocal(params[2], params[3], commandName);
	return 1;
}

static cell_t autocomplete_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	std::string guildId = interaction->GetGuildId();
	pContext->StringToLocal(params[2], params[3], guildId.c_str());
	return 1;
}

static cell_t autocomplete_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	std::string channelId = interaction->GetChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t autocomplete_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	DiscordUser* pDiscordUser = interaction->GetUser();

	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());
	Handle_t handle = g_DiscordUserHandler.CreateHandle(pDiscordUser, &sec, &err);

	if (handle == BAD_HANDLE)
	{
		delete pDiscordUser;
		pContext->ReportError("Could not create user handle (error %d)", err);
		return BAD_HANDLE;
	}

	return handle;
}

static cell_t autocomplete_GetUserNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickname().c_str());
	return 1;
}

static cell_t autocomplete_GetOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	std::string value = interaction->GetOptionValue(name);
	pContext->StringToLocal(params[3], params[4], value.c_str());
	return 1;
}

static cell_t autocomplete_GetOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value = interaction->GetOptionValueInt(name);
	return value;
}

static cell_t autocomplete_GetOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	double value = interaction->GetOptionValueDouble(name);
	return sp_ftoc((float)value);
}

static cell_t autocomplete_GetOptionValueBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	return interaction->GetOptionValueBool(name);
}

static cell_t autocomplete_AddAutocompleteChoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	dpp::command_value value;
	dpp::command_option_type type = static_cast<dpp::command_option_type>(params[3]);
	switch(type) {
		case dpp::co_string:
		{
			char* str_value;
			pContext->LocalToString(params[4], &str_value);
			value = std::string(str_value);
			break;
		}
		case dpp::co_number:
			value = sp_ctof(params[4]);
			break;
		default:
			value = (int64_t)params[4];
			break;
	}

	interaction->m_response.add_autocomplete_choice(dpp::command_option_choice(name, value));
	return 1;
}

static cell_t autocomplete_AddAutocompleteChoiceString(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	char* str_value;
	pContext->LocalToString(params[4], &str_value);

	interaction->m_response.add_autocomplete_choice(dpp::command_option_choice(name, std::string(str_value)));
	return 1;
}

static cell_t autocomplete_CreateAutocompleteResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordAutocompleteInteraction* interaction = g_DiscordAutocompleteInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[2]);
	if (!discord) {
		return 0;
	}

	discord->CreateAutocompleteResponse(interaction->m_command.id, interaction->m_command.token, interaction->m_response);
	return 1;
}

const sp_nativeinfo_t autocomplete_natives[] = {
	{"DiscordAutocompleteInteraction.GetCommandName", autocomplete_GetCommandName},
	{"DiscordAutocompleteInteraction.GetGuildId", autocomplete_GetGuildId},
	{"DiscordAutocompleteInteraction.GetChannelId", autocomplete_GetChannelId},
	{"DiscordAutocompleteInteraction.GetUser",       autocomplete_GetUser},
	{"DiscordAutocompleteInteraction.GetUserNickname", autocomplete_GetUserNickname},
	{"DiscordAutocompleteInteraction.GetOptionValue", autocomplete_GetOptionValue},
	{"DiscordAutocompleteInteraction.GetOptionValueInt", autocomplete_GetOptionValueInt},
	{"DiscordAutocompleteInteraction.GetOptionValueFloat", autocomplete_GetOptionValueFloat},
	{"DiscordAutocompleteInteraction.GetOptionValueBool", autocomplete_GetOptionValueBool},
	{"DiscordAutocompleteInteraction.CreateAutocompleteResponse", autocomplete_CreateAutocompleteResponse},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoice", autocomplete_AddAutocompleteChoice},
	{"DiscordAutocompleteInteraction.AddAutocompleteChoiceString", autocomplete_AddAutocompleteChoiceString}
};