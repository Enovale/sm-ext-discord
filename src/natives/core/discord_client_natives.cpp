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
#include "core/event_registry.h"
#include "entities/discord_message.h"
#include "entities/discord_channel.h"
#include "utils/discord_embed.h"
#include "utils/discord_http.h"

static cell_t discord_CreateClient(IPluginContext* pContext, const cell_t* params)
{
	char* token;
	pContext->LocalToString(params[1], &token);

	DiscordClient* pDiscordClient = new DiscordClient(token, static_cast<uint32_t>(params[2]));
	Handle_t handle = Handles.Create(pContext, pDiscordClient, HandleId::Discord);
	if (handle) {
		pDiscordClient->SetHandle(handle);
	}
	return handle;
}

static cell_t discord_Start(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	discord->Start();
	return 1;
}

static cell_t discord_Stop(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	discord->Stop();
	return 1;
}

static cell_t discord_GetBotId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	std::string botId = discord->GetBotId();
	if (botId.empty()) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], botId.c_str());
	return 1;
}

static cell_t discord_GetBotName(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	const char* botName = discord->GetBotName();
	if (!botName) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], botName);
	return 1;
}

static cell_t discord_GetBotDiscriminator(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	return discord->GetBotDiscriminator();
}

static cell_t discord_GetBotAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	std::string avatarUrl = discord->GetBotAvatarUrl();
	if (avatarUrl.empty()) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], avatarUrl.c_str());
	return 1;
}

static cell_t discord_GetUptime(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	return static_cast<cell_t>(discord->GetUptime());
}

static cell_t discord_SetPresence(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* status_text;
	pContext->LocalToString(params[4], &status_text);

	dpp::presence presence(static_cast<dpp::presence_status>(params[2]), static_cast<dpp::activity_type>(params[3]), status_text);
	discord->SetPresence(presence);
	return 1;
}

static cell_t discord_SendMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* message;
	pContext->LocalToString(params[3], &message);

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	cell_t data = params[5];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().Send(channel, message, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().Send(channel, message);
	}
	return 1;
}

static cell_t discord_SendMessageEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* message;
	pContext->LocalToString(params[3], &message);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[4]);
	if (!embed) return 0;

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().SendEmbed(channel, message, embed, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().SendEmbed(channel, message, embed);
	}
	return 1;
}

static cell_t discord_SendDiscordMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().SendObject(message, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().SendObject(message);
	}
	return 1;
}

static cell_t discord_SendDiscordMessageToChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[3]);
	if (!message) return 0;

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	cell_t data = params[5];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().SendObjectToChannel(channel, message, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().SendObjectToChannel(channel, message);
	}
	return 1;
}

static cell_t discord_IsRunning(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	return discord->IsRunning();
}

static cell_t discord_EditMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	char* content;
	pContext->LocalToString(params[4], &content);

	dpp::snowflake channel, message;
	if (!ParseSnowflake(pContext, channelId, channel) || !ParseSnowflake(pContext, messageId, message)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().Edit(channel, message, content, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().Edit(channel, message, content);
	}
	return 1;
}

static cell_t discord_EditMessageEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	char* content;
	pContext->LocalToString(params[4], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[5]);
	if (!embed) return 0;

	dpp::snowflake channel, message;
	if (!ParseSnowflake(pContext, channelId, channel) || !ParseSnowflake(pContext, messageId, message)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);
	cell_t data = params[7];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().EditEmbed(channel, message, content, embed, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, discord, callback, data, cb);
		});
	} else {
		discord->Messages().EditEmbed(channel, message, content, embed);
	}
	return 1;
}

static cell_t discord_DeleteMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	dpp::snowflake channel, message;
	if (!ParseSnowflake(pContext, channelId, channel) || !ParseSnowflake(pContext, messageId, message)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	cell_t data = params[5];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().Delete(channel, message, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, discord, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		discord->Messages().Delete(channel, message);
	}
	return 1;
}

static cell_t discord_DeleteMessageBulk(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	cell_t* messageIds;
	pContext->LocalToPhysAddr(params[3], &messageIds);

	unsigned int count = params[4];

	if (count < 2 || count > 100) {
		pContext->ReportError("Bulk delete requires between 2 and 100 message IDs (got %u)", count);
		return 0;
	}

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	std::vector<dpp::snowflake> message_vec;
	message_vec.reserve(count);

	for (unsigned int i = 0; i < count; i++) {
		char* msgId;
		pContext->LocalToString(messageIds[i], &msgId);
		dpp::snowflake msg;
		if (ParseSnowflake(pContext, msgId, msg)) {
			message_vec.push_back(msg);
		}
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().DeleteBulk(channel, message_vec, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, discord, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		discord->Messages().DeleteBulk(channel, message_vec);
	}
	return 1;
}

static cell_t discord_GetMessages(IPluginContext* pContext, const cell_t* params)
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

	uint32_t limit = static_cast<uint32_t>(params[4]);

	char* around_str;
	char* before_str;
	char* after_str;
	pContext->LocalToString(params[5], &around_str);
	pContext->LocalToString(params[6], &before_str);
	pContext->LocalToString(params[7], &after_str);

	dpp::snowflake channelFlake, around = 0, before = 0, after = 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	if (around_str && around_str[0] != '\0') ParseSnowflake(around_str, around);
	if (before_str && before_str[0] != '\0') ParseSnowflake(before_str, before);
	if (after_str && after_str[0] != '\0') ParseSnowflake(after_str, after);

	cell_t data = params[8];
	Handle_t client_handle = discord->GetHandle();

	discord->Messages().GetMultiple(channelFlake, around, before, after, limit, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordMessage, dpp::message_map>(client_handle, discord, callback, data, cb, DiscordResultType::Messages);
	});

	return 1;
}

static cell_t discord_CreateDMChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* userId;
	pContext->LocalToString(params[2], &userId);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	dpp::snowflake userFlake;
	if (!ParseSnowflake(pContext, userId, userFlake)) return 0;

	cell_t data = params[4];
	Handle_t client_handle = discord->GetHandle();

	discord->Channels().CreateDM(userFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordChannel>(client_handle, discord, callback, data, cb);
	});

	return 1;
}

static cell_t discord_FetchCurrentUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	Handle_t client_handle = discord->GetHandle();

	discord->Users().GetCurrent([client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordUser, dpp::user_identified>(client_handle, discord, callback, data, cb);
	});

	return 1;
}

static cell_t discord_SendTyping(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	discord->Channels().Typing(channelFlake);
	return 1;
}

static cell_t discord_FetchThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* threadId;
	pContext->LocalToString(params[2], &threadId);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	dpp::snowflake threadFlake;
	if (!ParseSnowflake(pContext, threadId, threadFlake)) return 0;

	cell_t data = params[4];
	Handle_t client_handle = discord->GetHandle();

	discord->Threads().Get(threadFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordChannel, dpp::thread>(client_handle, discord, callback, data, cb);
	});

	return 1;
}

static cell_t discord_HttpRequest(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	if (!discord->IsRunning()) {
		pContext->ReportError("Bot instance not running, unable to send request");
		return 0;
	}

	char* url;
	pContext->LocalToString(params[2], &url);

	dpp::http_method method = static_cast<dpp::http_method>(params[3]);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	char* body = nullptr;
	if (params[0] >= 5) {
		pContext->LocalToString(params[5], &body);
	}

	char* content_type = nullptr;
	if (params[0] >= 6) {
		pContext->LocalToString(params[6], &content_type);
	}

	HttpHeaders* headers = nullptr;
	if (params[0] >= 7) {
		headers = Handles.GetPointer<HttpHeaders>(pContext, params[7]);
	}

	cell_t data = params[8];

	std::string request_body = body ? body : "";
	std::string mime_type = content_type ? content_type : "application/json";
	dpp::http_headers dpp_headers;

	if (headers) {
		dpp_headers = headers->ToDppHeaders();
	}

	Handle_t client_handle = discord->GetHandle();
	discord->Http().Request(url, method, [client_handle, discord, callback, data](const dpp::http_request_completion_t& completion) {
		Tasks.Push([client_handle, discord, callback, data, completion]() {
			DiscordResult* result = new DiscordResult(DiscordResultType::Http, discord);

			if (completion.error == dpp::h_success) {
				result->SetSuccess(true);
				result->SetInt("status", completion.status);
				result->SetString("body", completion.body);
				result->SetInt("body_length", static_cast<int>(completion.body.length()));
				auto* httpCompletion = new HttpCompletion(completion);
				Handle_t completionHandle = Handles.CreateCallback(httpCompletion, HandleId::HttpCompletion);
				result->SetHandle("completion", completionHandle);
			} else {
				result->SetError(completion.body.empty() ? "HTTP request failed" : completion.body);
			}

			Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);

			callback->PushCell(client_handle);
			callback->PushCell(resultHandle);
			callback->PushCell(data);
			callback->Execute(nullptr);

			Handles.FreeHandle(resultHandle);
		});
	}, request_body, mime_type, dpp_headers);

	return 1;
}

static cell_t discord_RegisterEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* eventName;
	pContext->LocalToString(params[2], &eventName);

	const EventMeta* meta = GetEventMeta(eventName);
	if (!meta) {
		pContext->ReportError("Unknown event: %s", eventName);
		return 0;
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	IChangeableForward* forward = forwards->CreateForwardEx(nullptr, ET_Ignore, 3, nullptr, Param_Cell, Param_Cell, Param_Any);
	if (!forward) {
		pContext->ReportError("Could not create event forward");
		return 0;
	}

	if (!forward->AddFunction(callback)) {
		forwards->ReleaseForward(forward);
		pContext->ReportError("Could not add callback to forward");
		return 0;
	}

	discord->Callbacks().Set(meta->id, forward, params[4]);
	return 1;
}

static cell_t discord_UnregisterEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* eventName;
	pContext->LocalToString(params[2], &eventName);

	const EventMeta* meta = GetEventMeta(eventName);
	if (!meta) {
		pContext->ReportError("Unknown event: %s", eventName);
		return 0;
	}

	discord->Events().UnregisterEvent(meta->id);
	return 1;
}

// Global cache count natives
static cell_t discord_GetCachedUserCount(IPluginContext* pContext, const cell_t* params)
{
	return static_cast<cell_t>(DiscordClient::GetCachedUserCount());
}

static cell_t discord_GetCachedGuildCount(IPluginContext* pContext, const cell_t* params)
{
	return static_cast<cell_t>(DiscordClient::GetCachedGuildCount());
}

static cell_t discord_GetCachedChannelCount(IPluginContext* pContext, const cell_t* params)
{
	return static_cast<cell_t>(DiscordClient::GetCachedChannelCount());
}

static cell_t discord_GetCachedRoleCount(IPluginContext* pContext, const cell_t* params)
{
	return static_cast<cell_t>(DiscordClient::GetCachedRoleCount());
}

static cell_t discord_GetCachedEmojiCount(IPluginContext* pContext, const cell_t* params)
{
	return static_cast<cell_t>(DiscordClient::GetCachedEmojiCount());
}

extern const sp_nativeinfo_t discord_core_natives[] = {
	{"Discord.Discord", discord_CreateClient},
	{"Discord.Start", discord_Start},
	{"Discord.Stop", discord_Stop},
	{"Discord.GetBotId", discord_GetBotId},
	{"Discord.GetBotName", discord_GetBotName},
	{"Discord.BotDiscriminator.get", discord_GetBotDiscriminator},
	{"Discord.GetBotAvatarUrl", discord_GetBotAvatarUrl},
	{"Discord.Uptime.get", discord_GetUptime},
	{"Discord.SetPresence", discord_SetPresence},
	{"Discord.IsRunning.get", discord_IsRunning},
	{"Discord.SendMessage", discord_SendMessage},
	{"Discord.SendMessageEmbed", discord_SendMessageEmbed},
	{"Discord.SendMessageFromObject", discord_SendDiscordMessage},
	{"Discord.SendMessageFromObjectToChannel", discord_SendDiscordMessageToChannel},
	{"Discord.EditMessage", discord_EditMessage},
	{"Discord.EditMessageEmbed", discord_EditMessageEmbed},
	{"Discord.DeleteMessage", discord_DeleteMessage},
	{"Discord.DeleteMessageBulk", discord_DeleteMessageBulk},
	{"Discord.GetMessages", discord_GetMessages},
	{"Discord.CreateDMChannel", discord_CreateDMChannel},
	{"Discord.FetchCurrentUser", discord_FetchCurrentUser},
	{"Discord.FetchThread", discord_FetchThread},
	{"Discord.SendTyping", discord_SendTyping},
	{"Discord.HttpRequest", discord_HttpRequest},
	{"Discord.RegisterEvent", discord_RegisterEvent},
	{"Discord.UnregisterEvent", discord_UnregisterEvent},
	{"Discord.CachedUserCount.get", discord_GetCachedUserCount},
	{"Discord.CachedGuildCount.get", discord_GetCachedGuildCount},
	{"Discord.CachedChannelCount.get", discord_GetCachedChannelCount},
	{"Discord.CachedRoleCount.get", discord_GetCachedRoleCount},
	{"Discord.CachedEmojiCount.get", discord_GetCachedEmojiCount},
	{nullptr, nullptr}
};
