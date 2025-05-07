#include "interaction.h"

static cell_t interaction_CreateResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->CreateResponse(content);
	return 1;
}

static cell_t interaction_CreateResponseEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[3]);

	interaction->CreateResponseEmbed(content, embed);
	return 1;
}

static cell_t interaction_GetOptionValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	std::string value;
	if (!interaction->GetOptionValue(name, value)) {
		return 0;
	}

	pContext->StringToLocal(params[3], params[4], value.c_str());
	return 1;
}

// TODO: process int64_t
static cell_t interaction_GetOptionValueInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	int64_t value;
	if (!interaction->GetOptionValueInt(name, value)) {
		return 0;
	}

	return value;
}

static cell_t interaction_GetOptionValueFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

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
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	bool value;
	if (!interaction->GetOptionValueBool(name, value)) {
		return 0;
	}

	return value ? 1 : 0;
}

static cell_t interaction_DeferReply(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	interaction->DeferReply(params[2] ? true : false);
	return 1;
}

static cell_t interaction_EditResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->EditResponse(content);
	return 1;
}

static cell_t interaction_CreateEphemeralResponse(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->CreateEphemeralResponse(content);
	return 1;
}

static cell_t interaction_CreateEphemeralResponseEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[3]);

	interaction->CreateEphemeralResponseEmbed(content, embed);
	return 1;
}

static cell_t interaction_GetCommandName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	const char* commandName = interaction->GetCommandName();
	pContext->StringToLocal(params[2], params[3], commandName);
	return 1;
}

static cell_t interaction_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	std::string guildId = interaction->GetGuildId();
	pContext->StringToLocal(params[2], params[3], guildId.c_str());
	return 1;
}

static cell_t interaction_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	std::string channelId = interaction->GetChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t interaction_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
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

static cell_t interaction_GetUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	std::string userId = interaction->GetUserId();
	pContext->StringToLocal(params[2], params[3], userId.c_str());
	return 1;
}

static cell_t interaction_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	const char* userName = interaction->GetUserName();
	pContext->StringToLocal(params[2], params[3], userName);
	return 1;
}

static cell_t interaction_GetUserNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordInteraction* interaction = g_DiscordInteractionHandler.ReadHandle(params[1]);
	if (!interaction) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], interaction->GetUserNickname().c_str());
	return 1;
}

const sp_nativeinfo_t interaction_natives[] = {
	{"DiscordInteraction.CreateResponse", interaction_CreateResponse},
	{"DiscordInteraction.CreateResponseEmbed", interaction_CreateResponseEmbed},
	{"DiscordInteraction.GetOptionValue", interaction_GetOptionValue},
	{"DiscordInteraction.GetOptionValueInt", interaction_GetOptionValueInt},
	{"DiscordInteraction.GetOptionValueFloat", interaction_GetOptionValueFloat},
	{"DiscordInteraction.GetOptionValueBool", interaction_GetOptionValueBool},
	{"DiscordInteraction.DeferReply", interaction_DeferReply},
	{"DiscordInteraction.EditResponse", interaction_EditResponse},
	{"DiscordInteraction.CreateEphemeralResponse", interaction_CreateEphemeralResponse},
	{"DiscordInteraction.CreateEphemeralResponseEmbed", interaction_CreateEphemeralResponseEmbed},
	{"DiscordInteraction.GetCommandName", interaction_GetCommandName},
	{"DiscordInteraction.GetGuildId", interaction_GetGuildId},
	{"DiscordInteraction.GetChannelId", interaction_GetChannelId},
	{"DiscordInteraction.GetUser",       interaction_GetUser},
	{"DiscordInteraction.GetUserNickname", interaction_GetUserNickname},
	{"DiscordInteraction.GetUserId", interaction_GetUserId},
	{"DiscordInteraction.GetUserName", interaction_GetUserName}
};