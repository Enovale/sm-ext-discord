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
#include "features/discord_webhook.h"
#include "entities/discord_message.h"
#include "utils/discord_embed.h"

static cell_t webhook_CreateWebhookFromUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* webhook_url;
	pContext->LocalToString(params[2], &webhook_url);

	dpp::webhook webhook;
	try
	{
		webhook = dpp::webhook(webhook_url);
	}
	catch (const std::exception& e)
	{
		pContext->ReportError("Webhook url invalid: %s", e.what());
		return 0;
	}

	DiscordWebhook* pDiscordWebhook = new DiscordWebhook(webhook, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordWebhook, HandleId::DiscordWebhook);
	if (!handle) return 0;
	return handle;
}

static cell_t webhook_CreateWebhookFromIdToken(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* webhook_id;
	pContext->LocalToString(params[2], &webhook_id);

	char* webhook_token;
	pContext->LocalToString(params[3], &webhook_token);

	dpp::snowflake id;
	if (!ParseSnowflake(pContext, webhook_id, id)) return 0;

	DiscordWebhook* pDiscordWebhook = new DiscordWebhook(id, std::string(webhook_token), discord);
	Handle_t handle = Handles.Create(pContext, pDiscordWebhook, HandleId::DiscordWebhook);
	if (!handle) return 0;
	return handle;
}

static cell_t webhook_FetchWebhook(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* webhookId;
	pContext->LocalToString(params[2], &webhookId);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];

	dpp::snowflake webhookFlake;
	if (!ParseSnowflake(pContext, webhookId, webhookFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	discord->Webhooks().Get(webhookFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& confirmation) {
		callback.Result<DiscordWebhook>(confirmation);
	});

	return 1;
}

static cell_t webhook_GetId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetId().c_str());
	return 1;
}

static cell_t webhook_GetUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	Handle_t handle = webhook->GetUserHandle();

	if (!handle) {
		pContext->ReportError("Could not create user handle");
		return 0;
	}

	return handle;
}

static cell_t webhook_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetName());
	return 1;
}

static cell_t webhook_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	webhook->SetName(name);
	return 1;
}

static cell_t webhook_GetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetAvatarUrl());
	return 1;
}

static cell_t webhook_SetAvatarFromFile(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* filepath;
	pContext->LocalToString(params[2], &filepath);

	dpp::image_type type = static_cast<dpp::image_type>(params[3]);
	return webhook->SetAvatarFromFile(filepath, type);
}

static cell_t webhook_SetAvatarFromUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);

	webhook->SetAvatarFromUrl(url);
	return 1;
}

static cell_t webhook_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	return static_cast<cell_t>(webhook->GetType());
}

static cell_t webhook_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetGuildId().c_str());
	return 1;
}

static cell_t webhook_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetChannelId().c_str());
	return 1;
}

static cell_t webhook_GetToken(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetToken());
	return 1;
}

static cell_t webhook_GetApplicationId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetApplicationId().c_str());
	return 1;
}

static cell_t webhook_GetSourceGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetSourceGuildId().c_str());
	return 1;
}

static cell_t webhook_GetSourceChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetSourceChannelId().c_str());
	return 1;
}

static cell_t webhook_GetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetUrl());
	return 1;
}

static cell_t webhook_GetImageData(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	pContext->StringToLocal(params[2], params[3], webhook->GetImageData().c_str());
	return 1;
}

static cell_t webhook_SetThreadId(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* threadId;
	pContext->LocalToString(params[2], &threadId);

	dpp::snowflake threadFlake;
	if (!ParseSnowflake(pContext, threadId, threadFlake)) return 0;

	webhook->SetThreadId(threadFlake);
	return 1;
}

static cell_t webhook_SetThreadName(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* threadName;
	pContext->LocalToString(params[2], &threadName);

	webhook->SetThreadName(threadName);
	return 1;
}

static cell_t webhook_Modify(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	webhook->Modify(callback, data);
	return 1;
}

static cell_t webhook_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	webhook->Delete(callback, data);
	return 1;
}

static cell_t webhook_Execute(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* message;
	pContext->LocalToString(params[2], &message);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	webhook->Execute(message, callback, data);
	return 1;
}

static cell_t webhook_ExecuteEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	char* message;
	pContext->LocalToString(params[2], &message);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	webhook->ExecuteEmbed(message, embed, callback, data);
	return 1;
}

static cell_t webhook_ExecuteMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordWebhook* webhook = Handles.GetPointer<DiscordWebhook>(pContext, params[1]);
	if (!webhook) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	webhook->ExecuteMessage(message, callback, data);
	return 1;
}

static cell_t webhook_CreateWebhook(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* name;
	pContext->LocalToString(params[3], &name);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	dpp::webhook webhook;
	webhook.name = name;
	webhook.channel_id = channelFlake;

	Handle_t client_handle = discord->GetHandle();
	cell_t data = params[5];
	discord->Webhooks().Create(webhook, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& confirmation) {
		callback.Result<DiscordWebhook>(confirmation);
	});
	return 1;
}

static cell_t webhook_GetChannelWebhooks(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	cell_t data = params[4];
	discord->Webhooks().GetByChannel(channelFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& confirmation) {
		callback.ResultList<DiscordWebhook, dpp::webhook_map>(confirmation, DiscordResultType::Webhooks);
	});
	return 1;
}

static cell_t webhook_GetGuildWebhooks(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	dpp::snowflake guildFlake;
	if (!ParseSnowflake(pContext, guildId, guildFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	cell_t data = params[4];
	discord->Webhooks().GetByGuild(guildFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& confirmation) {
		callback.ResultList<DiscordWebhook, dpp::webhook_map>(confirmation, DiscordResultType::Webhooks);
	});
	return 1;
}

extern const sp_nativeinfo_t webhook_natives[] = {
	{"DiscordWebhook.DiscordWebhook", webhook_CreateWebhookFromUrl},
	{"DiscordWebhook.CreateFromIdToken", webhook_CreateWebhookFromIdToken},
	{"DiscordWebhook.FetchWebhook", webhook_FetchWebhook},
	{"DiscordWebhook.GetId", webhook_GetId},
	{"DiscordWebhook.User.get", webhook_GetUser},
	{"DiscordWebhook.GetName", webhook_GetName},
	{"DiscordWebhook.SetName", webhook_SetName},
	{"DiscordWebhook.GetAvatarUrl", webhook_GetAvatarUrl},
	{"DiscordWebhook.SetAvatarFromFile", webhook_SetAvatarFromFile},
	{"DiscordWebhook.SetAvatarFromUrl", webhook_SetAvatarFromUrl},
	{"DiscordWebhook.Type.get", webhook_GetType},
	{"DiscordWebhook.GetGuildId", webhook_GetGuildId},
	{"DiscordWebhook.GetChannelId", webhook_GetChannelId},
	{"DiscordWebhook.GetToken", webhook_GetToken},
	{"DiscordWebhook.GetApplicationId", webhook_GetApplicationId},
	{"DiscordWebhook.GetSourceGuildId", webhook_GetSourceGuildId},
	{"DiscordWebhook.GetSourceChannelId", webhook_GetSourceChannelId},
	{"DiscordWebhook.GetUrl", webhook_GetUrl},
	{"DiscordWebhook.GetImageData", webhook_GetImageData},
	{"DiscordWebhook.SetThreadId", webhook_SetThreadId},
	{"DiscordWebhook.SetThreadName", webhook_SetThreadName},
	{"DiscordWebhook.Modify", webhook_Modify},
	{"DiscordWebhook.Delete", webhook_Delete},
	{"DiscordWebhook.Execute", webhook_Execute},
	{"DiscordWebhook.ExecuteEmbed", webhook_ExecuteEmbed},
	{"DiscordWebhook.ExecuteFromObject", webhook_ExecuteMessage},
	{"DiscordWebhook.CreateWebhook", webhook_CreateWebhook},
	{"DiscordWebhook.GetChannelWebhooks", webhook_GetChannelWebhooks},
	{"DiscordWebhook.GetGuildWebhooks", webhook_GetGuildWebhooks},
	{nullptr, nullptr}
};
