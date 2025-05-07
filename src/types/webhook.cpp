#include "webhook.h"

static cell_t webhook_CreateWebhook(IPluginContext* pContext, const cell_t* params)
{
	char* webhook_url;
	pContext->LocalToString(params[1], &webhook_url);

	dpp::webhook webhook;
	try
	{
    	webhook = dpp::webhook(webhook_url);
	}
	catch (const std::exception& e)
	{
		pContext->ReportError("Webhook url invalid: %s", e.what());
		return BAD_HANDLE;
	}

	DiscordWebhook* pDiscordWebhook = new DiscordWebhook(webhook);

	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());
	Handle_t handle = g_DiscordWebhookHandler.CreateHandle(pDiscordWebhook, &sec, &err);

	if (handle == BAD_HANDLE)
	{
		delete pDiscordWebhook;
		pContext->ReportError("Could not create webhook handle (error %d)", err);
		return BAD_HANDLE;
	}

	return handle;
}

static cell_t webhook_GetId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], webhook->GetId().c_str());
	return 1;
}

static cell_t webhook_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	DiscordUser* pDiscordUser = webhook->GetUser();

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

static cell_t webhook_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], webhook->GetName());
	return 1;
}

static cell_t webhook_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);
	webhook->SetName(name);
	return 1;
}

static cell_t webhook_GetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], webhook->GetAvatarUrl());
	return 1;
}

static cell_t webhook_SetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	char* avatar_url;
	pContext->LocalToString(params[2], &avatar_url);
	webhook->SetAvatarUrl(avatar_url);
	return 1;
}

static cell_t webhook_GetAvatarData(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], webhook->GetAvatarData().c_str());
	return 1;
}

static cell_t webhook_SetAvatarData(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[1]);
	if (!webhook) {
		return 0;
	}

	char* avatar_data;
	pContext->LocalToString(params[2], &avatar_data);
	webhook->SetAvatarData(avatar_data);
	return 1;
}

const sp_nativeinfo_t webhook_natives[] = {
	{"DiscordWebhook.DiscordWebhook",webhook_CreateWebhook},
	{"DiscordWebhook.GetId",       webhook_GetId},
	{"DiscordWebhook.GetUser",       webhook_GetUser},
	{"DiscordWebhook.GetName",       webhook_GetName},
	{"DiscordWebhook.SetName",       webhook_SetName},
	{"DiscordWebhook.GetAvatarUrl",       webhook_GetAvatarUrl},
	{"DiscordWebhook.SetAvatarUrl",       webhook_SetAvatarUrl},
	{"DiscordWebhook.GetAvatarData",       webhook_GetAvatarData},
	{"DiscordWebhook.SetAvatarData",       webhook_SetAvatarData}
};