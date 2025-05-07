#include "message.h"

static cell_t message_GetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetContent());
	return 1;
}

static cell_t message_GetMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetMessageId().c_str());
	return 1;
}

static cell_t message_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetChannelId().c_str());
	return 1;
}

static cell_t message_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetGuildId().c_str());
	return 1;
}

static cell_t message_GetAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	DiscordUser* pDiscordUser = message->GetAuthor();

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

static cell_t message_GetAuthorId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetAuthorId().c_str());
	return 1;
}

static cell_t message_GetAuthorName(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetAuthorName());
	return 1;
}

static cell_t message_GetAuthorDisplayName(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetAuthorDisplayName());
	return 1;
}

static cell_t message_GetAuthorNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], message->GetAuthorNickname().c_str());
	return 1;
}

static cell_t message_GetAuthorDiscriminator(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	return message->GetAuthorDiscriminator();
}

static cell_t message_IsBot(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = g_DiscordMessageHandler.ReadHandle(params[1]);
	if (!message) {
		return 0;
	}

	return message->IsBot() ? 1 : 0;
}

const sp_nativeinfo_t message_natives[] = {
	{"DiscordMessage.GetContent",    message_GetContent},
	{"DiscordMessage.GetMessageId",  message_GetMessageId},
	{"DiscordMessage.GetChannelId",  message_GetChannelId},
	{"DiscordMessage.GetGuildId",    message_GetGuildId},
	{"DiscordMessage.GetAuthor",       message_GetAuthor},
	{"DiscordMessage.GetAuthorId",   message_GetAuthorId},
	{"DiscordMessage.GetAuthorName", message_GetAuthorName},
	{"DiscordMessage.GetAuthorDisplayName", message_GetAuthorDisplayName},
	{"DiscordMessage.GetAuthorNickname", message_GetAuthorNickname},
	{"DiscordMessage.GetAuthorDiscriminator", message_GetAuthorDiscriminator},
	{"DiscordMessage.IsBot",         message_IsBot}
};