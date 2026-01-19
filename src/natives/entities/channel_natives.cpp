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
#include "entities/discord_channel.h"
#include "entities/discord_message.h"
#include "entities/discord_thread_member.h"
#include "features/discord_invite.h"
#include "utils/discord_embed.h"
#include "utils/discord_forum.h"

static cell_t channel_CreateFromId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	dpp::channel* channel_ptr = dpp::find_channel(channelFlake);

	if (!channel_ptr) {
		dpp::channel channel_obj;
		channel_obj.id = channelFlake;

		DiscordChannel* pDiscordChannel = new DiscordChannel(channel_obj, discord);
		Handle_t handle = Handles.Create(pContext, pDiscordChannel, HandleId::DiscordChannel);
		if (!handle) return 0;
		return handle;
	}

	DiscordChannel* pDiscordChannel = new DiscordChannel(*channel_ptr, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordChannel, HandleId::DiscordChannel);
	if (!handle) return 0;
	return handle;
}

static cell_t channel_FetchChannel(IPluginContext* pContext, const cell_t* params)
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

	cell_t data = params[4];

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	dpp::channel* cached_channel = dpp::find_channel(channelFlake);
	if (cached_channel) {
		DiscordChannel* channel = new DiscordChannel(*cached_channel, discord);
		PushCachedResult<DiscordChannel>(client_handle, discord, callback, data, channel);
		return 1;
	}
	discord->Channels().Get(channelFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& confirmation) {
		PushResult<DiscordChannel>(client_handle, discord, callback, data, confirmation);
	});

	return 1;
}

static cell_t channel_FindChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channel_id_str;
	pContext->LocalToString(params[2], &channel_id_str);

	dpp::snowflake channel_id;
	if (!ParseSnowflake(pContext, channel_id_str, channel_id)) return 0;

	dpp::channel* channel_ptr = dpp::find_channel(channel_id);

	if (!channel_ptr) {
		return 0; // Channel not found in cache
	}

	DiscordChannel* pDiscordChannel = new DiscordChannel(*channel_ptr, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordChannel, HandleId::DiscordChannel);
	if (!handle) return 0;
	return handle;
}

static cell_t channel_GetParentId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	pContext->StringToLocal(params[2], params[3], channel->GetParentId().c_str());
	return 1;
}

static cell_t channel_SetParentId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* parent_id;
	pContext->LocalToString(params[2], &parent_id);

	channel->SetParentId(parent_id);
	return 1;
}

static cell_t channel_GetTopic(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	pContext->StringToLocal(params[2], params[3], channel->GetTopic());
	return 1;
}


static cell_t channel_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	channel->SetName(name);
	return 1;
}

static cell_t channel_SetTopic(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* topic;
	pContext->LocalToString(params[2], &topic);
	channel->SetTopic(topic);
	return 1;
}

static cell_t channel_SetNSFW(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	bool nsfw = params[2];
	channel->SetNSFW(nsfw);
	return 1;
}

static cell_t channel_SetPosition(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	uint16_t position = static_cast<uint16_t>(params[2]);
	channel->SetPosition(position);
	return 1;
}

static cell_t channel_SetRateLimitPerUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	uint16_t seconds = static_cast<uint16_t>(params[2]);
	channel->SetRateLimitPerUser(seconds);
	return 1;
}

static cell_t channel_SetBitrate(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	uint16_t bitrate = static_cast<uint16_t>(params[2]);
	channel->SetBitrate(bitrate);
	return 1;
}

static cell_t channel_SetUserLimit(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	uint8_t limit = static_cast<uint8_t>(params[2]);
	channel->SetUserLimit(limit);
	return 1;
}

static cell_t channel_SetParent(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* parentId;
	pContext->LocalToString(params[2], &parentId);

	dpp::snowflake parent;
	if (!ParseSnowflake(pContext, parentId, parent)) return 0;
	channel->SetParent(parent);
	return 1;
}

static cell_t channel_AddPermissionOverwrite(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* targetId;
	pContext->LocalToString(params[2], &targetId);
	uint8_t type = static_cast<uint8_t>(params[3]);

	char* allowed_str;
	char* denied_str;
	pContext->LocalToString(params[4], &allowed_str);
	pContext->LocalToString(params[5], &denied_str);

	dpp::snowflake target;
	if (!ParseSnowflake(pContext, targetId, target)) return 0;
	uint64_t allowed, denied;
	if (!ParseUInt64(allowed_str, allowed)) return 0;
	if (!ParseUInt64(denied_str, denied)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);

	cell_t data = params[7];
	channel->AddPermissionOverwrite(target, type, allowed, denied, callback, data);
	return 1;
}

static cell_t channel_SetPermissionOverwrite(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* targetId;
	pContext->LocalToString(params[2], &targetId);
	uint8_t type = static_cast<uint8_t>(params[3]);

	char* allowed_str;
	char* denied_str;
	pContext->LocalToString(params[4], &allowed_str);
	pContext->LocalToString(params[5], &denied_str);

	dpp::snowflake target;
	if (!ParseSnowflake(pContext, targetId, target)) return 0;
	uint64_t allowed, denied;
	if (!ParseUInt64(allowed_str, allowed)) return 0;
	if (!ParseUInt64(denied_str, denied)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);

	cell_t data = params[7];
	channel->SetPermissionOverwrite(target, type, allowed, denied, callback, data);
	return 1;
}

static cell_t channel_RemovePermissionOverwrite(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* targetId;
	pContext->LocalToString(params[2], &targetId);
	uint8_t type = static_cast<uint8_t>(params[3]);

	dpp::snowflake target;
	if (!ParseSnowflake(pContext, targetId, target)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	channel->RemovePermissionOverwrite(target, type, callback, data);
	return 1;
}

static cell_t channel_EditPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* overwrite_id_str;
	pContext->LocalToString(params[2], &overwrite_id_str);

	dpp::snowflake overwrite_id;
	if (!ParseSnowflake(pContext, overwrite_id_str, overwrite_id)) return 0;

	uint64_t allow = static_cast<uint64_t>(params[3]);
	uint64_t deny = static_cast<uint64_t>(params[4]);
	bool is_member = params[5];

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);

	cell_t data = params[7];
	channel->EditPermissions(overwrite_id, allow, deny, is_member, callback, data);
	return 1;
}

static cell_t channel_GetUserPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* userId;
	pContext->LocalToString(params[2], &userId);

	dpp::snowflake user;
	if (!ParseSnowflake(pContext, userId, user)) return 0;
	std::string permissions = channel->GetUserPermissions(user);
	pContext->StringToLocal(params[3], params[4], permissions.c_str());
	return 1;
}

static cell_t channel_CreateInvite(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int max_age = params[2];
	int max_uses = params[3];
	bool temporary = params[4];
	bool unique = params[5];

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);

	cell_t data = params[7];
	channel->CreateInvite(max_age, max_uses, temporary, unique, callback, data);
	return 1;
}

static cell_t channel_CreateInviteFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[2]);
	if (!invite) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->CreateInviteFromObject(invite, callback, data);
	return 1;
}

static cell_t channel_GetInvites(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	channel->GetInvites(callback, data);
	return 1;
}

static cell_t channel_GetWebhooks(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	channel->GetWebhooks(callback, data);
	return 1;
}

static cell_t channel_SendMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->SendMessage(content, callback, data);
	return 1;
}

static cell_t channel_SendMessageEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	channel->SendMessageEmbed(content, embed, callback, data);
	return 1;
}

static cell_t channel_SendDiscordMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->SendDiscordMessage(message, callback, data);
	return 1;
}

static cell_t channel_SetRTCRegion(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* region;
	pContext->LocalToString(params[2], &region);

	channel->SetRTCRegion(region);
	return 1;
}

static cell_t channel_GetOwnerId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	std::string ownerId = channel->GetOwnerId();
	pContext->StringToLocal(params[2], params[3], ownerId.c_str());
	return 1;
}

static cell_t channel_GetLastMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	std::string lastMessageId = channel->GetLastMessageId();
	pContext->StringToLocal(params[2], params[3], lastMessageId.c_str());
	return 1;
}


static cell_t channel_GetLastPinTimestamp64(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char buffer[32];
	FormatInt64(static_cast<int64_t>(channel->GetLastPinTimestamp()), buffer, sizeof(buffer));
	pContext->StringToLocal(params[2], params[3], buffer);
	return 1;
}


static cell_t channel_GetRTCRegion(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	std::string region = channel->GetRTCRegion();
	pContext->StringToLocal(params[2], params[3], region.c_str());
	return 1;
}


static cell_t channel_GetPermissionOverwriteTargetId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;


	std::string targetId = channel->GetPermissionOverwriteTargetId(params[2]);
	pContext->StringToLocal(params[3], params[4], targetId.c_str());
	return 1;
}

static cell_t channel_GetPermissionOverwriteType(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	return static_cast<cell_t>(channel->GetPermissionOverwriteType(params[2]));
}


static cell_t channel_GetAvailableTagName(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int index = params[2];
	std::string tagName = channel->GetAvailableTagName(index);
	pContext->StringToLocal(params[3], params[4], tagName.c_str());
	return 1;
}

static cell_t channel_GetAvailableTagId(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int index = params[2];
	std::string tagId = channel->GetAvailableTagId(index);
	pContext->StringToLocal(params[3], params[4], tagId.c_str());
	return 1;
}

static cell_t channel_GetAvailableTagEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int index = params[2];
	std::string tagEmoji = channel->GetAvailableTagEmoji(index);
	pContext->StringToLocal(params[3], params[4], tagEmoji.c_str());
	return 1;
}

static cell_t channel_GetAvailableTagModerated(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int index = params[2];
	return channel->GetAvailableTagModerated(index);
}

static cell_t channel_GetAvailableTagEmojiIsCustom(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int index = params[2];
	return channel->GetAvailableTagEmojiIsCustom(index);
}

static cell_t channel_CreateForumTag(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	char* emoji;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &emoji);
	bool moderated = params[4];

	channel->CreateForumTag(name, emoji ? emoji : "", moderated);
	return 1;
}

static cell_t channel_EditForumTag(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* tag_id;
	char* name;
	char* emoji;
	pContext->LocalToString(params[2], &tag_id);
	pContext->LocalToString(params[3], &name);
	pContext->LocalToString(params[4], &emoji);
	bool moderated = params[5];

	dpp::snowflake tagId;
	if (!ParseSnowflake(pContext, tag_id, tagId)) return 0;

	channel->EditForumTag(tagId, name, emoji ? emoji : "", moderated);
	return 1;
}

static cell_t channel_DeleteForumTag(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* tag_id;
	pContext->LocalToString(params[2], &tag_id);

	dpp::snowflake tagId;
	if (!ParseSnowflake(pContext, tag_id, tagId)) return 0;

	channel->DeleteForumTag(tagId);
	return 1;
}

static cell_t channel_CreateForumThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	char* message;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &message);

	std::vector<dpp::snowflake> tag_ids;
	int tag_count = params[5];

	if (params[4] != 0 && tag_count > 0) {
		cell_t* tag_array;
		pContext->LocalToPhysAddr(params[4], &tag_array);

		for (int i = 0; i < tag_count; i++) {
			char* tag_id;
			pContext->LocalToString(tag_array[i], &tag_id);
			dpp::snowflake tag_snowflake;
			if (ParseSnowflake(pContext, tag_id, tag_snowflake)) {
				tag_ids.push_back(tag_snowflake);
			}
		}
	}

	int auto_archive = params[6];
	int rate_limit = params[7];

	IPluginFunction* callback = pContext->GetFunctionById(params[8]);

	cell_t data = params[9];
	channel->CreateForumThread(name, message, tag_ids, auto_archive, rate_limit, callback, data);
	return 1;
}

static cell_t channel_CreateForumThreadEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	char* message;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &message);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[4]);
	if (!embed) return 0;

	std::vector<dpp::snowflake> tag_ids;
	int tag_count = params[6];

	if (params[5] != 0 && tag_count > 0) {
		cell_t* tag_array;
		pContext->LocalToPhysAddr(params[5], &tag_array);

		for (int i = 0; i < tag_count; i++) {
			char* tag_id;
			pContext->LocalToString(tag_array[i], &tag_id);
			dpp::snowflake tag_snowflake;
			if (ParseSnowflake(pContext, tag_id, tag_snowflake)) {
				tag_ids.push_back(tag_snowflake);
			}
		}
	}

	int auto_archive = params[7];
	int rate_limit = params[8];

	IPluginFunction* callback = pContext->GetFunctionById(params[9]);

	cell_t data = params[10];
	channel->CreateForumThreadEmbed(name, message, embed, tag_ids, auto_archive, rate_limit, callback, data);
	return 1;
}

static cell_t channel_GetIconUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	int size = params[4];
	std::string iconUrl = channel->GetIconUrl(size);
	pContext->StringToLocal(params[2], params[3], iconUrl.c_str());
	return 1;
}

static cell_t channel_CreateThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	uint8_t type = params[3];
	int auto_archive = params[4];
	bool invitable = params[5];
	int rate_limit = params[6];
	IPluginFunction* callback = pContext->GetFunctionById(params[7]);

	cell_t data = params[8];
	channel->CreateThread(name, type, auto_archive, invitable, rate_limit, callback, data);
	return 1;
}

static cell_t channel_CreateThreadWithMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* message_id_str;
	pContext->LocalToString(params[2], &message_id_str);

	dpp::snowflake message_id;
	if (!ParseSnowflake(pContext, message_id_str, message_id)) return 0;

	char* name;
	pContext->LocalToString(params[3], &name);

	int auto_archive = params[4];
	int rate_limit = params[5];
	IPluginFunction* callback = pContext->GetFunctionById(params[6]);

	cell_t data = params[7];
	channel->CreateThreadWithMessage(message_id, name, auto_archive, rate_limit, callback, data);
	return 1;
}

static cell_t channel_JoinThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	channel->JoinThread(callback, data);
	return 1;
}

static cell_t channel_LeaveThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	channel->LeaveThread(callback, data);
	return 1;
}

static cell_t channel_ModifyThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	int auto_archive = params[3];
	int rate_limit = params[4];
	bool archived = params[5];
	bool locked = params[6];
	IPluginFunction* callback = pContext->GetFunctionById(params[7]);

	cell_t data = params[8];
	channel->ModifyThread(name, auto_archive, rate_limit, archived, locked, callback, data);
	return 1;
}

static cell_t channel_ThreadMemberAdd(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* user_id;
	pContext->LocalToString(params[2], &user_id);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->ThreadMemberAdd(dpp::snowflake(user_id), callback, data);
	return 1;
}

static cell_t channel_ThreadMemberRemove(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	char* user_id;
	pContext->LocalToString(params[2], &user_id);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->ThreadMemberRemove(dpp::snowflake(user_id), callback, data);
	return 1;
}

static cell_t channel_SetArchived(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->SetArchived(params[2], callback, data);
	return 1;
}

static cell_t channel_SetLocked(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	channel->SetLocked(params[2], callback, data);
	return 1;
}

static cell_t channel_GetArchivedThreads(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	bool is_private = params[2];
	time_t before = static_cast<time_t>(params[3]);
	uint16_t limit = static_cast<uint16_t>(params[4]);

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[6];
	channel->GetArchivedThreads(is_private, before, limit, callback, data);
	return 1;
}

static cell_t channel_GetThreadMembers(IPluginContext* pContext, const cell_t* params)
{
	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[1]);
	if (!channel) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	channel->GetThreadMembers(callback, data);
	return 1;
}

extern const sp_nativeinfo_t channel_natives[] = {
	{"DiscordChannel.DiscordChannel", channel_CreateFromId},
	{"DiscordChannel.FetchChannel", channel_FetchChannel},
	{"DiscordChannel.FindChannel", channel_FindChannel},
	{"DiscordChannel.GetWebhooks", channel_GetWebhooks},
	{"DiscordChannel.GetName", EntityGetName<DiscordChannel>},
	{"DiscordChannel.GetId", EntityGetId<DiscordChannel>},
	{"DiscordChannel.GetGuildId", EntityGetGuildId<DiscordChannel>},
	{"DiscordChannel.GetParentId", channel_GetParentId},
	{"DiscordChannel.SetParentId", channel_SetParentId},
	{"DiscordChannel.GetTopic", channel_GetTopic},
	{"DiscordChannel.Type.get", EntityGetInt<DiscordChannel, uint8_t, &DiscordChannel::GetType>},
	{"DiscordChannel.Position.get", EntityGetInt<DiscordChannel, uint16_t, &DiscordChannel::GetPosition>},
	{"DiscordChannel.IsNSFW.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsNSFW>},
	{"DiscordChannel.IsTextChannel.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsTextChannel>},
	{"DiscordChannel.IsVoiceChannel.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsVoiceChannel>},
	{"DiscordChannel.IsCategory.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsCategory>},
	{"DiscordChannel.IsThread.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsThread>},
	{"DiscordChannel.IsForum.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsForum>},
	{"DiscordChannel.IsNewsChannel.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsNewsChannel>},
	{"DiscordChannel.IsStageChannel.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsStageChannel>},
	{"DiscordChannel.Bitrate.get", EntityGetInt<DiscordChannel, uint16_t, &DiscordChannel::GetBitrate>},
	{"DiscordChannel.UserLimit.get", EntityGetInt<DiscordChannel, uint8_t, &DiscordChannel::GetUserLimit>},
	{"DiscordChannel.RateLimitPerUser.get", EntityGetInt<DiscordChannel, uint16_t, &DiscordChannel::GetRateLimitPerUser>},
	{"DiscordChannel.GetMention", EntityGetMention<DiscordChannel>},
	{"DiscordChannel.GetUrl", EntityGetUrl<DiscordChannel>},
	{"DiscordChannel.SetName", channel_SetName},
	{"DiscordChannel.SetTopic", channel_SetTopic},
	{"DiscordChannel.Position.set", channel_SetPosition},
	{"DiscordChannel.IsNSFW.set", channel_SetNSFW},
	{"DiscordChannel.RateLimitPerUser.set", channel_SetRateLimitPerUser},
	{"DiscordChannel.Bitrate.set", channel_SetBitrate},
	{"DiscordChannel.UserLimit.set", channel_SetUserLimit},
	{"DiscordChannel.Delete", EntityDelete<DiscordChannel>},
	{"DiscordChannel.SetParent", channel_SetParent},
	{"DiscordChannel.Flags.get", EntityGetFlags<DiscordChannel>},
	{"DiscordChannel.GetOwnerId", channel_GetOwnerId},
	{"DiscordChannel.GetLastMessageId", channel_GetLastMessageId},
	{"DiscordChannel.LastPinTimestamp.get", EntityGetInt<DiscordChannel, time_t, &DiscordChannel::GetLastPinTimestamp>},
	{"DiscordChannel.GetLastPinTimestamp", channel_GetLastPinTimestamp64},
	{"DiscordChannel.DefaultThreadRateLimitPerUser.get", EntityGetInt<DiscordChannel, uint16_t, &DiscordChannel::GetDefaultThreadRateLimitPerUser>},
	{"DiscordChannel.DefaultAutoArchiveDuration.get", EntityGetInt<DiscordChannel, uint8_t, &DiscordChannel::GetDefaultAutoArchiveDuration>},
	{"DiscordChannel.DefaultSortOrder.get", EntityGetInt<DiscordChannel, uint8_t, &DiscordChannel::GetDefaultSortOrder>},
	{"DiscordChannel.ForumLayout.get", EntityGetInt<DiscordChannel, uint8_t, &DiscordChannel::GetForumLayout>},
	{"DiscordChannel.GetRTCRegion", channel_GetRTCRegion},
	{"DiscordChannel.IsDM.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsDM>},
	{"DiscordChannel.IsGroupDM.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsGroupDM>},
	{"DiscordChannel.IsMediaChannel.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsMediaChannel>},
	{"DiscordChannel.IsVideo720p.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsVideo720p>},
	{"DiscordChannel.IsVideoAuto.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsVideoAuto>},
	{"DiscordChannel.IsPinnedThread.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsPinnedThread>},
	{"DiscordChannel.IsTagRequired.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsTagRequired>},
	{"DiscordChannel.IsDownloadOptionsHidden.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsDownloadOptionsHidden>},
	{"DiscordChannel.IsLockedPermissions.get", EntityGetBool<DiscordChannel, &DiscordChannel::IsLockedPermissions>},
	{"DiscordChannel.PermissionOverwriteCount.get", EntityGetInt<DiscordChannel, size_t, &DiscordChannel::GetPermissionOverwriteCount>},
	{"DiscordChannel.GetPermissionOverwriteTargetId", channel_GetPermissionOverwriteTargetId},
	{"DiscordChannel.GetPermissionOverwriteType", channel_GetPermissionOverwriteType},
	{"DiscordChannel.AvailableTagCount.get", EntityGetInt<DiscordChannel, size_t, &DiscordChannel::GetAvailableTagCount>},
	{"DiscordChannel.GetAvailableTagName", channel_GetAvailableTagName},
	{"DiscordChannel.GetAvailableTagId", channel_GetAvailableTagId},
	{"DiscordChannel.GetAvailableTagEmoji", channel_GetAvailableTagEmoji},
	{"DiscordChannel.GetAvailableTagModerated", channel_GetAvailableTagModerated},
	{"DiscordChannel.GetAvailableTagEmojiIsCustom", channel_GetAvailableTagEmojiIsCustom},
	{"DiscordChannel.CreateForumTag", channel_CreateForumTag},
	{"DiscordChannel.EditForumTag", channel_EditForumTag},
	{"DiscordChannel.DeleteForumTag", channel_DeleteForumTag},
	{"DiscordChannel.CreateForumThread", channel_CreateForumThread},
	{"DiscordChannel.CreateForumThreadEmbed", channel_CreateForumThreadEmbed},
	{"DiscordChannel.GetIconUrl", channel_GetIconUrl},
	{"DiscordChannel.AddPermissionOverwrite", channel_AddPermissionOverwrite},
	{"DiscordChannel.SetPermissionOverwrite", channel_SetPermissionOverwrite},
	{"DiscordChannel.RemovePermissionOverwrite", channel_RemovePermissionOverwrite},
	{"DiscordChannel.EditPermissions", channel_EditPermissions},
	{"DiscordChannel.GetUserPermissions", channel_GetUserPermissions},
	{"DiscordChannel.CreateInvite", channel_CreateInvite},
	{"DiscordChannel.CreateInviteFromObject", channel_CreateInviteFromObject},
	{"DiscordChannel.GetInvites", channel_GetInvites},
	{"DiscordChannel.SendMessage", channel_SendMessage},
	{"DiscordChannel.SendMessageEmbed", channel_SendMessageEmbed},
	{"DiscordChannel.SendMessageFromObject", channel_SendDiscordMessage},
	{"DiscordChannel.SetRTCRegion", channel_SetRTCRegion},
	{"DiscordChannel.CreateThread", channel_CreateThread},
	{"DiscordChannel.CreateThreadWithMessage", channel_CreateThreadWithMessage},
	{"DiscordChannel.JoinThread", channel_JoinThread},
	{"DiscordChannel.LeaveThread", channel_LeaveThread},
	{"DiscordChannel.ModifyThread", channel_ModifyThread},
	{"DiscordChannel.ThreadMemberAdd", channel_ThreadMemberAdd},
	{"DiscordChannel.ThreadMemberRemove", channel_ThreadMemberRemove},
	{"DiscordChannel.SetArchived", channel_SetArchived},
	{"DiscordChannel.SetLocked", channel_SetLocked},
	{"DiscordChannel.GetArchivedThreads", channel_GetArchivedThreads},
	{"DiscordChannel.GetThreadMembers", channel_GetThreadMembers},
	{nullptr, nullptr}
};
