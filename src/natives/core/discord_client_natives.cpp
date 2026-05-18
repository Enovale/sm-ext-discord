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
#include "core/event_registry.h"
#include "entities/discord_message.h"
#include "entities/discord_channel.h"
#include "features/discord_slashcommand.h"
#include "utils/discord_embed.h"
#include "utils/discord_http.h"

#include <exception>

static cell_t discord_CreateClient(IPluginContext* pContext, const cell_t* params)
{
	char* token;
	pContext->LocalToString(params[1], &token);

	uint32_t intents;
	if (!GetNativeUInt32(pContext, params[2], 0xFFFFFFFFu, "Gateway intents", intents)) return 0;

	DiscordClient* pDiscordClient = nullptr;
	try {
		pDiscordClient = new DiscordClient(token, intents);
	} catch (const std::exception& e) {
		pContext->ReportError("Could not create Discord client: %s", e.what());
		return 0;
	} catch (...) {
		pContext->ReportError("Could not create Discord client");
		return 0;
	}

	Handle_t handle = Handles.Create(pContext, pDiscordClient, HandleId::Discord);
	if (!handle) return 0;
	pDiscordClient->SetHandle(handle);
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

	dpp::presence_status status = static_cast<dpp::presence_status>(params[2]);
	dpp::activity_type activity = static_cast<dpp::activity_type>(params[3]);

	dpp::presence presence(status, activity, status_text);
	discord->SetPresence(presence);
	return 1;
}

static cell_t discord_SendMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	char* message;
	pContext->LocalToString(params[2], &channelId);
	pContext->LocalToString(params[3], &message);

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	cell_t data = params[5];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().Send(channel, message, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
	char* message;
	pContext->LocalToString(params[2], &channelId);
	pContext->LocalToString(params[3], &message);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[4]);
	if (!embed) return 0;

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().SendEmbed(channel, message, embed, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
		discord->Messages().SendObject(message, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
		discord->Messages().SendObjectToChannel(channel, message, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
	char* messageId;
	char* content;
	pContext->LocalToString(params[2], &channelId);
	pContext->LocalToString(params[3], &messageId);
	pContext->LocalToString(params[4], &content);

	dpp::snowflake channel, message;
	if (!ParseSnowflake(pContext, channelId, channel) || !ParseSnowflake(pContext, messageId, message)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().Edit(channel, message, content, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
	char* messageId;
	char* content;
	pContext->LocalToString(params[2], &channelId);
	pContext->LocalToString(params[3], &messageId);
	pContext->LocalToString(params[4], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[5]);
	if (!embed) return 0;

	dpp::snowflake channel, message;
	if (!ParseSnowflake(pContext, channelId, channel) || !ParseSnowflake(pContext, messageId, message)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);
	cell_t data = params[7];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().EditEmbed(channel, message, content, embed, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
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
		discord->Messages().Delete(channel, message, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
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
	if (!GetNativeArray(pContext, params[3], &messageIds, "message ID")) return 0;

	int count;
	if (!GetNativeIntInRange(pContext, params[4], 2, 100, "Bulk delete message count", count)) return 0;

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;

	std::vector<dpp::snowflake> message_vec;
	message_vec.reserve(count);

	for (int i = 0; i < count; i++) {
		char* msgId;
		pContext->LocalToString(messageIds[i], &msgId);
		dpp::snowflake msg;
		if (!ParseSnowflake(pContext, msgId, msg)) return 0;
		message_vec.push_back(msg);
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	cell_t data = params[6];

	if (callback) {
		Handle_t client_handle = discord->GetHandle();
		discord->Messages().DeleteBulk(channel, message_vec, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
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

	uint32_t limit;
	if (!GetNativeUInt32(pContext, params[4], 100, "Message limit", limit)) return 0;

	char* around_str;
	char* before_str;
	char* after_str;
	pContext->LocalToString(params[5], &around_str);
	pContext->LocalToString(params[6], &before_str);
	pContext->LocalToString(params[7], &after_str);

	dpp::snowflake channelFlake, around = 0, before = 0, after = 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	if (!ParseOptionalSnowflake(pContext, around_str, around)) return 0;
	if (!ParseOptionalSnowflake(pContext, before_str, before)) return 0;
	if (!ParseOptionalSnowflake(pContext, after_str, after)) return 0;

	cell_t data = params[8];
	Handle_t client_handle = discord->GetHandle();

	discord->Messages().GetMultiple(channelFlake, around, before, after, limit, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordMessage, dpp::message_map>(cb, DiscordResultType::Messages);
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

	discord->Channels().CreateDM(userFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordChannel>(cb);
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

	discord->Users().GetCurrent([callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordUser, dpp::user_identified>(cb);
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

	discord->Threads().Get(threadFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordChannel, dpp::thread>(cb);
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

	char* body;
	char* content_type;
	pContext->LocalToString(params[5], &body);
	pContext->LocalToString(params[6], &content_type);

	HttpHeaders* headers = nullptr;
	if (params[7]) {
		headers = Handles.GetPointer<HttpHeaders>(pContext, params[7]);
		if (!headers) return 0;
	}
	cell_t data = params[8];

	std::string request_body = body && body[0] ? body : "";
	std::string mime_type = content_type && content_type[0] ? content_type : "application/json";
	dpp::http_headers dpp_headers;

	if (headers) {
		dpp_headers = headers->ToDppHeaders();
	}

	Handle_t client_handle = discord->GetHandle();
	discord->Http().Request(url, method, [callback = AsyncCallback(client_handle, callback, data)](const dpp::http_request_completion_t& completion) {
		callback.ResultCustom(DiscordResultType::Http, [completion](DiscordResult& result, DiscordClient*) {
			if (completion.error == dpp::h_success) {
				result.SetSuccess(true);
				result.SetInt("status", completion.status);
					result.SetString("body", completion.body);
					result.SetInt("body_length", static_cast<int>(completion.body.length()));
					auto httpCompletion = std::make_unique<HttpCompletion>(completion);
					Handle_t completionHandle = Handles.CreateCallback(httpCompletion.release(), HandleId::HttpCompletion);
					if (completionHandle) {
						result.SetHandle("completion", completionHandle);
					} else {
						result.SetError("Failed to create HTTP completion handle");
					}
			} else {
				result.SetError(completion.body.empty() ? "HTTP request failed" : completion.body);
			}
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

	const CallbackId* id = GetEventId(eventName);
	if (!id) {
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

	discord->Callbacks().Set(*id, discord->GetHandle(), forward, callback, params[4]);
	return 1;
}

static cell_t discord_UnregisterEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* eventName;
	pContext->LocalToString(params[2], &eventName);

	const CallbackId* id = GetEventId(eventName);
	if (!id) {
		pContext->ReportError("Unknown event: %s", eventName);
		return 0;
	}

	discord->Callbacks().Clear(*id);
	return 1;
}

static cell_t discord_GetGlobalCommands(IPluginContext* pContext, const cell_t* params)
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
	discord->Commands().GetGlobalCommands([callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.ResultList<DiscordSlashCommand, dpp::slashcommand_map>(cb, DiscordResultType::SlashCommands);
	});
	return 1;
}

static cell_t discord_GetGlobalCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* command_id_str;
	pContext->LocalToString(params[2], &command_id_str);

	dpp::snowflake command_id;
	if (!ParseSnowflake(pContext, command_id_str, command_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];

	Handle_t client_handle = discord->GetHandle();
	discord->Commands().GetGlobalCommand(command_id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
		callback.Result<DiscordSlashCommand>(cb);
	});
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
	{"Discord.GetGlobalCommands", discord_GetGlobalCommands},
	{"Discord.GetGlobalCommand", discord_GetGlobalCommand},
	{nullptr, nullptr}
};
