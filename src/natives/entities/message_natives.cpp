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

#include "natives/entity_natives_common.h"
#include "entities/discord_message.h"
#include "entities/discord_attachment.h"
#include "entities/discord_reaction.h"
#include "utils/discord_embed.h"
#include "utils/discord_component.h"
#include "utils/discord_poll.h"

static cell_t message_CreateFromId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* channelId;
	pContext->LocalToString(params[3], &channelId);

	dpp::snowflake messageFlake, channelFlake;
	if (!ParseSnowflake(pContext, messageId, messageFlake)) return 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	dpp::message message_obj;
	message_obj.id = messageFlake;
	message_obj.channel_id = channelFlake;

	DiscordMessage* pDiscordMessage = new DiscordMessage(message_obj, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateEmpty(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordMessage* pDiscordMessage = new DiscordMessage(content, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* content;
	pContext->LocalToString(params[3], &content);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	DiscordMessage* pDiscordMessage = new DiscordMessage(channelFlake, content, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[2]);
	if (!embed) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(embed, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithChannelEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	DiscordMessage* pDiscordMessage = new DiscordMessage(channelFlake, embed, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_FetchMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* channelId;
	pContext->LocalToString(params[3], &channelId);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[5];

	dpp::snowflake messageFlake, channelFlake;
	if (!ParseSnowflake(pContext, messageId, messageFlake)) return 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	discord->Messages().Get(messageFlake, channelFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& confirmation) {
		PushResult<DiscordMessage>(client_handle, discord, callback, data, confirmation);
	});

	return 1;
}

static cell_t message_GetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetContent());
	return 1;
}

static cell_t message_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetChannelId().c_str());
	return 1;
}

static cell_t message_GetAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	Handle_t handle = message->GetAuthorHandle();

	if (!handle) {
		pContext->ReportError("Could not create author handle");
		return 0;
	}

	return handle;
}

static cell_t message_GetAuthorNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetAuthorNickName().c_str());
	return 1;
}

static cell_t message_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetType());
}

static cell_t message_GetMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetMessageId().c_str());
	return 1;
}

static cell_t message_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->Edit(content, callback, data);
	return 1;
}

static cell_t message_EditEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->EditEmbed(content, embed, callback, data);
	return 1;
}

static cell_t message_EditFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->EditFromObject(callback, data);
	return 1;
}

static cell_t message_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Delete(callback, data);
	return 1;
}

static cell_t message_Pin(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Pin(callback, data);
	return 1;
}

static cell_t message_Unpin(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Unpin(callback, data);
	return 1;
}

static cell_t message_AddReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* emoji;
	pContext->LocalToString(params[2], &emoji);
	message->AddReaction(emoji);
	return 1;
}

static cell_t message_RemoveReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* emoji;
	pContext->LocalToString(params[2], &emoji);
	message->RemoveReaction(emoji);
	return 1;
}

static cell_t message_RemoveAllReactions(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->RemoveAllReactions(); return 1;
}

static cell_t message_Reply(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->Reply(content, callback, data);
	return 1;
}

static cell_t message_ReplyEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->ReplyEmbed(content, embed, callback, data);
	return 1;
}

static cell_t message_ReplyFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordMessage* reply_message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!reply_message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->ReplyFromObject(reply_message, callback, data);
	return 1;
}

static cell_t message_Crosspost(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->Crosspost(); return 1;
}

static cell_t message_CreateThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int auto_archive_duration = params[3];
	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->CreateThread(name, auto_archive_duration, callback, data);
	return 1;
}

static cell_t message_GetTimestamp64(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char buffer[32];
	FormatInt64(static_cast<int64_t>(message->GetTimestamp()), buffer, sizeof(buffer));
	pContext->StringToLocal(params[2], params[3], buffer);
	return 1;
}

static cell_t message_GetEditedTimestamp64(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char buffer[32];
	FormatInt64(static_cast<int64_t>(message->GetEditedTimestamp()), buffer, sizeof(buffer));
	pContext->StringToLocal(params[2], params[3], buffer);
	return 1;
}

static cell_t message_GetReferencedMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedMessageId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetReferencedChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedChannelId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetReferencedGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedGuildId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetMentionedUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string userId = message->GetMentionedUserId(params[2]);
	pContext->StringToLocal(params[3], params[4], userId.c_str());
	return 1;
}

static cell_t message_GetMentionedRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string roleId = message->GetMentionedRoleId(params[2]);
	pContext->StringToLocal(params[3], params[4], roleId.c_str());
	return 1;
}

static cell_t message_GetMentionedChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string channelId = message->GetMentionedChannelId(params[2]);
	pContext->StringToLocal(params[3], params[4], channelId.c_str());
	return 1;
}

static cell_t message_GetMentionedUserIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& mentions = message->GetMentionedUsers();
	for (const auto& mention : mentions) {
		array->AddString(mention.first.id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetMentionedRoleIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& roles = message->GetMentionedRoles();
	for (const auto& role_id : roles) {
		array->AddString(role_id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetMentionedChannelIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& channels = message->GetMentionedChannels();
	for (const auto& channel : channels) {
		array->AddString(channel.id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetWebhookId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string webhookId = message->GetWebhookId();
	pContext->StringToLocal(params[2], params[3], webhookId.c_str());
	return 1;
}

static cell_t message_GetNonce(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	const char* nonce = message->GetNonce();
	pContext->StringToLocal(params[2], params[3], nonce);
	return 1;
}

static cell_t message_GetURL(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string url = message->GetURL();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t message_SetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	message->SetChannelId(channelFlake);
	return 1;
}

static cell_t message_SetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->SetType(static_cast<dpp::message_type>(params[2]));
	return 1;
}

static cell_t message_SetFlags(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->SetFlags(static_cast<uint16_t>(params[2]));
	return 1;
}

static cell_t message_SetTTS(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->SetTTS(params[2]);
	return 1;
}

static cell_t message_SetNonce(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* nonce;
	pContext->LocalToString(params[2], &nonce);
	message->SetNonce(nonce);
	return 1;
}

static cell_t message_ClearEmbeds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->ClearEmbeds();
	return 1;
}

static cell_t message_Send(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Send(callback, data);
	return 1;
}

static cell_t message_SetPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[2]);
	if (!poll) return 0;

	message->SetPoll(poll);
	return 1;
}

static cell_t message_GetPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	Handle_t handle = message->GetPollHandle();
	if (!handle) return 0;

	return handle;
}

static cell_t message_EndPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->EndPoll();
	return 1;
}

static cell_t message_GetPollAnswerVoters(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	uint32_t answer_id = static_cast<uint32_t>(params[2]);
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	message->GetPollAnswerVoters(answer_id, callback, data);
	return 1;
}

static cell_t message_SetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	message->SetContent(content);
	return 1;
}

static cell_t message_AddEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[2]);
	if (!embed) return 0;

	message->AddEmbed(embed);
	return 1;
}

static cell_t message_SetAllowedMentions(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	cell_t* users_array;
	cell_t* roles_array;

	pContext->LocalToPhysAddr(params[3], &users_array);
	pContext->LocalToPhysAddr(params[5], &roles_array);

	std::vector<dpp::snowflake> users(params[4]);
	std::vector<dpp::snowflake> roles(params[6]);

	for (size_t i = 0; i < users.size(); i++) {
		char* str;
		pContext->LocalToString(users_array[i], &str);
		ParseSnowflake(str, users[i]);
	}

	for (size_t i = 0; i < roles.size(); i++) {
		char* str;
		pContext->LocalToString(roles_array[i], &str);
		ParseSnowflake(str, roles[i]);
	}

	message->SetAllowedMentions(params[2], users, roles);
	return 1;
}

static cell_t message_AddComponent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!component) return 0;

	message->AddComponent(component);
	return 1;
}

static cell_t message_ClearComponents(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->ClearComponents();
	return 1;
}

static cell_t message_GetAttachment(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordAttachment* attachment = message->GetAttachment(params[2]);
	if (!attachment) return 0;

	Handle_t handle = Handles.Create(pContext, attachment, HandleId::DiscordAttachment);
	if (!handle) return 0;
	return handle;
}

static cell_t message_GetReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordReaction* reaction = message->GetReaction(params[2]);
	if (!reaction) return 0;

	Handle_t handle = Handles.Create(pContext, reaction, HandleId::DiscordReaction);
	if (!handle) return 0;
	return handle;
}

extern const sp_nativeinfo_t message_natives[] = {
	{"DiscordMessage.DiscordMessage", message_CreateFromId},
	{"DiscordMessage.CreateEmpty", message_CreateEmpty},
	{"DiscordMessage.CreateWithContent", message_CreateWithContent},
	{"DiscordMessage.CreateWithChannel", message_CreateWithChannel},
	{"DiscordMessage.CreateWithEmbed", message_CreateWithEmbed},
	{"DiscordMessage.CreateWithChannelEmbed", message_CreateWithChannelEmbed},
	{"DiscordMessage.FetchMessage", message_FetchMessage},
	{"DiscordMessage.GetContent", message_GetContent},
	{"DiscordMessage.ContentLength.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetContentLength>},
	{"DiscordMessage.GetMessageId", message_GetMessageId},
	{"DiscordMessage.GetChannelId", message_GetChannelId},
	{"DiscordMessage.GetGuildId", EntityGetGuildId<DiscordMessage>},
	{"DiscordMessage.Author.get", message_GetAuthor},
	{"DiscordMessage.GetAuthorNickName", message_GetAuthorNickName},
	{"DiscordMessage.Type.get", message_GetType},
	{"DiscordMessage.Type.set", message_SetType},
	{"DiscordMessage.IsPinned.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsPinned>},
	{"DiscordMessage.IsTTS.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsTTS>},
	{"DiscordMessage.IsTTS.set", message_SetTTS},
	{"DiscordMessage.IsMentionEveryone.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsMentionEveryone>},
	{"DiscordMessage.IsBot.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsBot>},
	{"DiscordMessage.Edit", message_Edit},
	{"DiscordMessage.EditEmbed", message_EditEmbed},
	{"DiscordMessage.EditFromObject", message_EditFromObject},
	{"DiscordMessage.Delete", message_Delete},
	{"DiscordMessage.Pin", message_Pin},
	{"DiscordMessage.Unpin", message_Unpin},
	{"DiscordMessage.AddReaction", message_AddReaction},
	{"DiscordMessage.RemoveReaction", message_RemoveReaction},
	{"DiscordMessage.RemoveAllReactions", message_RemoveAllReactions},
	{"DiscordMessage.Reply", message_Reply},
	{"DiscordMessage.ReplyEmbed", message_ReplyEmbed},
	{"DiscordMessage.ReplyFromObject", message_ReplyFromObject},
	{"DiscordMessage.Crosspost", message_Crosspost},
	{"DiscordMessage.CreateThread", message_CreateThread},
	{"DiscordMessage.Flags.get", EntityGetFlags<DiscordMessage>},
	{"DiscordMessage.Flags.set", message_SetFlags},
	{"DiscordMessage.Timestamp.get", EntityGetInt<DiscordMessage, time_t, &DiscordMessage::GetTimestamp>},
	{"DiscordMessage.GetTimestamp", message_GetTimestamp64},
	{"DiscordMessage.EditedTimestamp.get", EntityGetInt<DiscordMessage, time_t, &DiscordMessage::GetEditedTimestamp>},
	{"DiscordMessage.GetEditedTimestamp", message_GetEditedTimestamp64},
	{"DiscordMessage.IsDM.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsDM>},
	{"DiscordMessage.HasRemixAttachment.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasRemixAttachment>},
	{"DiscordMessage.HasReference.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasReference>},
	{"DiscordMessage.GetReferencedMessageId", message_GetReferencedMessageId},
	{"DiscordMessage.GetReferencedChannelId", message_GetReferencedChannelId},
	{"DiscordMessage.GetReferencedGuildId", message_GetReferencedGuildId},
	{"DiscordMessage.AttachmentCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetAttachmentCount>},
	{"DiscordMessage.EmbedCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetEmbedCount>},
	{"DiscordMessage.ReactionCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetReactionCount>},
	{"DiscordMessage.StickerCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetStickerCount>},
	{"DiscordMessage.MentionedUserCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedUserCount>},
	{"DiscordMessage.MentionedRoleCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedRoleCount>},
	{"DiscordMessage.MentionedChannelCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedChannelCount>},
	{"DiscordMessage.GetMentionedUserId", message_GetMentionedUserId},
	{"DiscordMessage.GetMentionedRoleId", message_GetMentionedRoleId},
	{"DiscordMessage.GetMentionedChannelId", message_GetMentionedChannelId},
	{"DiscordMessage.GetMentionedUserIds", message_GetMentionedUserIds},
	{"DiscordMessage.GetMentionedRoleIds", message_GetMentionedRoleIds},
	{"DiscordMessage.GetMentionedChannelIds", message_GetMentionedChannelIds},
	{"DiscordMessage.GetWebhookId", message_GetWebhookId},
	{"DiscordMessage.GetNonce", message_GetNonce},
	{"DiscordMessage.GetUrl", message_GetURL},
	{"DiscordMessage.SetContent", message_SetContent},
	{"DiscordMessage.SetChannelId", message_SetChannelId},
	{"DiscordMessage.SetNonce", message_SetNonce},
	{"DiscordMessage.SetAllowedMentions", message_SetAllowedMentions},
	{"DiscordMessage.AddEmbed", message_AddEmbed},
	{"DiscordMessage.ClearEmbeds", message_ClearEmbeds},
	{"DiscordMessage.AddComponent", message_AddComponent},
	{"DiscordMessage.ClearComponents", message_ClearComponents},
	{"DiscordMessage.Send", message_Send},
	{"DiscordMessage.HasPoll.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasPoll>},
	{"DiscordMessage.SetPoll", message_SetPoll},
	{"DiscordMessage.Poll.get", message_GetPoll},
	{"DiscordMessage.EndPoll", message_EndPoll},
	{"DiscordMessage.GetPollAnswerVoters", message_GetPollAnswerVoters},
	{"DiscordMessage.GetAttachment", message_GetAttachment},
	{"DiscordMessage.GetReaction", message_GetReaction},
	{nullptr, nullptr}
};
