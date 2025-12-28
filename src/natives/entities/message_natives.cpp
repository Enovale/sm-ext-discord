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

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
}

static cell_t message_CreateEmpty(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(discord);

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
}

static cell_t message_CreateWithContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordMessage* pDiscordMessage = new DiscordMessage(content, discord);

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
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

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
}

static cell_t message_CreateWithEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[2]);
	if (!embed) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(embed, discord);

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
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

	return Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
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

static cell_t message_GetContentLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetContentLength());
}

static cell_t message_GetMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetMessageId().c_str());
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

static cell_t message_GetAuthorNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetAuthorNickname().c_str());
	return 1;
}

static cell_t message_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetType());
}

static cell_t message_IsPinned(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->IsPinned();
}

static cell_t message_IsTTS(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->IsTTS();
}

static cell_t message_IsMentionEveryone(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->IsMentionEveryone();
}

static cell_t message_IsBot(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->IsBot();
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

static cell_t message_GetTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetTimestamp());
}

static cell_t message_GetEditedTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetEditedTimestamp());
}

static cell_t message_IsDM(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->IsDM();
}

static cell_t message_HasRemixAttachment(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->HasRemixAttachment();
}

static cell_t message_HasReference(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->HasReference();
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

static cell_t message_GetAttachmentCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetAttachmentCount());
}

static cell_t message_GetEmbedCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetEmbedCount());
}

static cell_t message_GetReactionCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetReactionCount());
}

static cell_t message_GetStickerCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetStickerCount());
}

static cell_t message_GetMentionedUserCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetMentionedUserCount());
}

static cell_t message_GetMentionedRoleCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetMentionedRoleCount());
}

static cell_t message_GetMentionedChannelCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetMentionedChannelCount());
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

static cell_t message_HasPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return message->HasPoll();
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

	message->GetPollAnswerVoters(answer_id, callback, params[4]);
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

	return Handles.Create(pContext, attachment, HandleId::DiscordAttachment);
}

static cell_t message_GetReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordReaction* reaction = message->GetReaction(params[2]);
	if (!reaction) return 0;

	return Handles.Create(pContext, reaction, HandleId::DiscordReaction);
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
	{"DiscordMessage.ContentLength.get", message_GetContentLength},
	{"DiscordMessage.GetMessageId", message_GetMessageId},
	{"DiscordMessage.GetChannelId", message_GetChannelId},
	{"DiscordMessage.GetGuildId", EntityGetGuildId<DiscordMessage>},
	{"DiscordMessage.Author.get", message_GetAuthor},
	{"DiscordMessage.GetAuthorNickname", message_GetAuthorNickname},
	{"DiscordMessage.Type.get", message_GetType},
	{"DiscordMessage.Type.set", message_SetType},
	{"DiscordMessage.IsPinned.get", message_IsPinned},
	{"DiscordMessage.IsTTS.get", message_IsTTS},
	{"DiscordMessage.IsTTS.set", message_SetTTS},
	{"DiscordMessage.IsMentionEveryone.get", message_IsMentionEveryone},
	{"DiscordMessage.IsBot.get", message_IsBot},
	{"DiscordMessage.Edit", message_Edit},
	{"DiscordMessage.EditEmbed", message_EditEmbed},
	{"DiscordMessage.Delete", message_Delete},
	{"DiscordMessage.Pin", message_Pin},
	{"DiscordMessage.Unpin", message_Unpin},
	{"DiscordMessage.AddReaction", message_AddReaction},
	{"DiscordMessage.RemoveReaction", message_RemoveReaction},
	{"DiscordMessage.RemoveAllReactions", message_RemoveAllReactions},
	{"DiscordMessage.Reply", message_Reply},
	{"DiscordMessage.ReplyEmbed", message_ReplyEmbed},
	{"DiscordMessage.Crosspost", message_Crosspost},
	{"DiscordMessage.CreateThread", message_CreateThread},
	{"DiscordMessage.Flags.get", EntityGetFlags<DiscordMessage>},
	{"DiscordMessage.Flags.set", message_SetFlags},
	{"DiscordMessage.Timestamp.get", message_GetTimestamp},
	{"DiscordMessage.EditedTimestamp.get", message_GetEditedTimestamp},
	{"DiscordMessage.IsDM.get", message_IsDM},
	{"DiscordMessage.HasRemixAttachment.get", message_HasRemixAttachment},
	{"DiscordMessage.HasReference.get", message_HasReference},
	{"DiscordMessage.GetReferencedMessageId", message_GetReferencedMessageId},
	{"DiscordMessage.GetReferencedChannelId", message_GetReferencedChannelId},
	{"DiscordMessage.GetReferencedGuildId", message_GetReferencedGuildId},
	{"DiscordMessage.AttachmentCount.get", message_GetAttachmentCount},
	{"DiscordMessage.EmbedCount.get", message_GetEmbedCount},
	{"DiscordMessage.ReactionCount.get", message_GetReactionCount},
	{"DiscordMessage.StickerCount.get", message_GetStickerCount},
	{"DiscordMessage.MentionedUserCount.get", message_GetMentionedUserCount},
	{"DiscordMessage.MentionedRoleCount.get", message_GetMentionedRoleCount},
	{"DiscordMessage.MentionedChannelCount.get", message_GetMentionedChannelCount},
	{"DiscordMessage.GetMentionedUserId", message_GetMentionedUserId},
	{"DiscordMessage.GetMentionedRoleId", message_GetMentionedRoleId},
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
	{"DiscordMessage.HasPoll.get", message_HasPoll},
	{"DiscordMessage.SetPoll", message_SetPoll},
	{"DiscordMessage.Poll.get", message_GetPoll},
	{"DiscordMessage.EndPoll", message_EndPoll},
	{"DiscordMessage.GetPollAnswerVoters", message_GetPollAnswerVoters},
	{"DiscordMessage.GetAttachment", message_GetAttachment},
	{"DiscordMessage.GetReaction", message_GetReaction},
	{nullptr, nullptr}
};
