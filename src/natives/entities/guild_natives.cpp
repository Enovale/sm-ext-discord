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
#include "entities/discord_guild.h"
#include "entities/discord_role.h"
#include "entities/discord_channel.h"

static cell_t guild_FetchGuild(IPluginContext* pContext, const cell_t* params)
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

	cell_t data = params[4];

	dpp::snowflake guildFlake;
	if (!ParseSnowflake(pContext, guildId, guildFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	dpp::guild* cached_guild = dpp::find_guild(guildFlake);
	if (cached_guild) {
		DiscordGuild* guild = new DiscordGuild(*cached_guild, discord);
		PushCachedResult<DiscordGuild>(client_handle, discord, callback, data, guild);
		return 1;
	}
	discord->Guilds().Get(guildFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& confirmation) {
		PushResult<DiscordGuild>(client_handle, discord, callback, data, confirmation);
	});

	return 1;
}

static cell_t guild_FindGuild(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* guild_id_str;
	pContext->LocalToString(params[2], &guild_id_str);

	dpp::snowflake guild_id;
	if (!ParseSnowflake(pContext, guild_id_str, guild_id)) return 0;

	dpp::guild* guild_ptr = dpp::find_guild(guild_id);

	if (!guild_ptr) {
		return 0; // Guild not found in cache
	}

	DiscordGuild* pDiscordGuild = new DiscordGuild(*guild_ptr, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordGuild, HandleId::DiscordGuild);
	if (!handle) return 0;
	return handle;
}

static cell_t guild_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	guild->SetName(name);
	return 1;
}

static cell_t guild_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* description;
	pContext->LocalToString(params[2], &description);
	guild->SetDescription(description);
	return 1;
}

static cell_t guild_GetOwnerId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	pContext->StringToLocal(params[2], params[3], guild->GetOwnerId().c_str());
	return 1;
}


static cell_t guild_GetIconUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t size = static_cast<uint16_t>(params[4]);
	bool prefer_animated = params[5] != 0;
	std::string iconUrl = guild->GetIconUrl(size, prefer_animated);

	pContext->StringToLocal(params[2], params[3], iconUrl.c_str());
	return 1;
}

static cell_t guild_GetBannerUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t size = static_cast<uint16_t>(params[4]);
	bool prefer_animated = params[5] != 0;
	std::string bannerUrl = guild->GetBannerUrl(size, prefer_animated);

	pContext->StringToLocal(params[2], params[3], bannerUrl.c_str());
	return 1;
}

static cell_t guild_GetSplashUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t size = static_cast<uint16_t>(params[4]);
	std::string splashUrl = guild->GetSplashUrl(size);

	pContext->StringToLocal(params[2], params[3], splashUrl.c_str());
	return 1;
}

static cell_t guild_GetDiscoverySplashUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t size = static_cast<uint16_t>(params[4]);
	std::string splashUrl = guild->GetDiscoverySplashUrl(size);

	pContext->StringToLocal(params[2], params[3], splashUrl.c_str());
	return 1;
}

static cell_t guild_GetVanityUrlCode(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	const char* vanityUrl = guild->GetVanityUrlCode();
	pContext->StringToLocal(params[2], params[3], vanityUrl);
	return 1;
}


static cell_t guild_GetAfkChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	std::string channelId = guild->GetAfkChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t guild_GetSystemChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	std::string channelId = guild->GetSystemChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t guild_GetRulesChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	std::string channelId = guild->GetRulesChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t guild_GetPublicUpdatesChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	std::string channelId = guild->GetPublicUpdatesChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t guild_GetSafetyAlertsChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	std::string channelId = guild->GetSafetyAlertsChannelId();
	pContext->StringToLocal(params[2], params[3], channelId.c_str());
	return 1;
}

static cell_t guild_GetBasePermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	uint64_t permissions = guild->GetBasePermissions(user_id);

	char permStr[32];
	FormatInt64(static_cast<int64_t>(permissions), permStr, sizeof(permStr));
	pContext->StringToLocal(params[3], params[4], permStr);
	return 1;
}

static cell_t guild_GetPermissionsInChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	char* channel_id_str;
	pContext->LocalToString(params[2], &user_id_str);
	pContext->LocalToString(params[3], &channel_id_str);

	dpp::snowflake user_id, channel_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;
	if (!ParseSnowflake(pContext, channel_id_str, channel_id)) return 0;

	uint64_t permissions = guild->GetPermissionsInChannel(user_id, channel_id);

	char permStr[32];
	FormatInt64(static_cast<int64_t>(permissions), permStr, sizeof(permStr));
	pContext->StringToLocal(params[4], params[5], permStr);
	return 1;
}

static cell_t guild_HasPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	char* permission;
	pContext->LocalToString(params[2], &user_id_str);
	pContext->LocalToString(params[3], &permission);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;
	return guild->HasPermission(user_id, permission);
}

static cell_t guild_HasPermissionInChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	char* channel_id_str;
	char* permission;
	pContext->LocalToString(params[2], &user_id_str);
	pContext->LocalToString(params[3], &channel_id_str);
	pContext->LocalToString(params[4], &permission);

	dpp::snowflake user_id, channel_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;
	if (!ParseSnowflake(pContext, channel_id_str, channel_id)) return 0;
	return guild->HasPermissionInChannel(user_id, channel_id, permission);
}

static cell_t guild_Modify(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	guild->Modify(callback, data);
	return 1;
}

static cell_t guild_GetInvites(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetInvites(callback, data);
	return 1;
}

static cell_t guild_GetWebhooks(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetWebhooks(callback, data);
	return 1;
}

static cell_t guild_GetRoles(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetRoles(callback, data);
	return 1;
}


static cell_t guild_GetMember(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	guild->GetMember(user_id, callback, data);
	return 1;
}

static cell_t guild_GetMembers(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	uint16_t limit = static_cast<uint16_t>(params[3]);
	char* after_str;
	pContext->LocalToString(params[4], &after_str);

	dpp::snowflake after = 0;
	if (after_str && after_str[0] != '\0') {
		ParseSnowflake(after_str, after);
	}

	cell_t data = params[5];
	guild->GetMembers(limit, after, callback, data);
	return 1;
}

static cell_t guild_SearchMembers(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* query;
	pContext->LocalToString(params[2], &query);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	uint16_t limit = static_cast<uint16_t>(params[4]);
	cell_t data = params[5];
	guild->SearchMembers(query, limit, callback, data);
	return 1;
}

static cell_t guild_GetActiveThreads(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetActiveThreads(callback, data);
	return 1;
}

static cell_t guild_GetChannels(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetChannels(callback, data);
	return 1;
}

static cell_t guild_GetBans(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	char* before_str;
	char* after_str;
	pContext->LocalToString(params[3], &before_str);
	pContext->LocalToString(params[4], &after_str);

	dpp::snowflake before = 0, after = 0;
	if (before_str && before_str[0] != '\0') ParseSnowflake(before_str, before);
	if (after_str && after_str[0] != '\0') ParseSnowflake(after_str, after);

	uint16_t limit = static_cast<uint16_t>(params[5]);
	cell_t data = params[6];
	guild->GetBans(before, after, limit, callback, data);
	return 1;
}

static cell_t guild_CreateEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	char* image_path;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &image_path);

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", image_path);

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);

	cell_t data = params[6];
	guild->CreateEmoji(name, fullpath, static_cast<dpp::image_type>(params[4]), callback, data);
	return 1;
}

static cell_t guild_GetEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* emoji_id_str;
	pContext->LocalToString(params[2], &emoji_id_str);

	dpp::snowflake emoji_id;
	if (!ParseSnowflake(pContext, emoji_id_str, emoji_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	guild->GetEmoji(emoji_id, callback, data);
	return 1;
}

static cell_t guild_GetAllEmojis(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetAllEmojis(callback, data);
	return 1;
}

static cell_t guild_CreateSticker(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	char* description;
	char* tags;
	char* file_path;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &description);
	pContext->LocalToString(params[4], &tags);
	pContext->LocalToString(params[5], &file_path);

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", file_path);

	IPluginFunction* callback = pContext->GetFunctionById(params[7]);

	cell_t data = params[8];
	guild->CreateSticker(name, description, tags, fullpath, static_cast<dpp::sticker_format>(params[6]), callback, data);
	return 1;
}

static cell_t guild_CreateScheduledEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	char* description;
	char* channel_id_str;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &description);
	pContext->LocalToString(params[4], &channel_id_str);

	dpp::snowflake channel_id;
	if (!ParseSnowflake(pContext, channel_id_str, channel_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[8]);

	cell_t data = params[9];
	guild->CreateScheduledEvent(name, description, channel_id, params[5], params[6], static_cast<dpp::event_entity_type>(params[7]), callback, data);
	return 1;
}

static cell_t guild_CreateExternalScheduledEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	char* description;
	char* location;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &description);
	pContext->LocalToString(params[4], &location);

	IPluginFunction* callback = pContext->GetFunctionById(params[7]);

	cell_t data = params[8];
	guild->CreateExternalScheduledEvent(name, description, location, params[5], params[6], callback, data);
	return 1;
}

static cell_t guild_GetScheduledEvents(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetScheduledEvents(callback, data);
	return 1;
}

static cell_t guild_GetScheduledEvent(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* event_id_str;
	pContext->LocalToString(params[2], &event_id_str);

	dpp::snowflake event_id;
	if (!ParseSnowflake(pContext, event_id_str, event_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	guild->GetScheduledEvent(event_id, callback, data);
	return 1;
}

static cell_t guild_GetScheduledEventUsers(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* event_id_str;
	pContext->LocalToString(params[2], &event_id_str);

	dpp::snowflake event_id;
	if (!ParseSnowflake(pContext, event_id_str, event_id)) return 0;

	uint16_t limit = static_cast<uint16_t>(params[3]);

	char* before_str;
	char* after_str;
	pContext->LocalToString(params[4], &before_str);
	pContext->LocalToString(params[5], &after_str);

	dpp::snowflake before, after;
	ParseSnowflake(before_str, before);
	ParseSnowflake(after_str, after);

	IPluginFunction* callback = pContext->GetFunctionById(params[6]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[7];
	guild->GetScheduledEventUsers(event_id, limit, before, after, callback, data);
	return 1;
}

static cell_t guild_BanUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	uint32_t delete_message_seconds = params[3];

	char* reason;
	pContext->LocalToString(params[4], &reason);

	IPluginFunction* callback = params[5] ? pContext->GetFunctionById(params[5]) : nullptr;

	cell_t data = params[6];
	guild->BanUser(user_id, delete_message_seconds, reason, callback, data);
	return 1;
}

static cell_t guild_UnbanUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	guild->UnbanUser(user_id, callback, data);
	return 1;
}

static cell_t guild_CreateRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	uint32_t color = params[3];
	bool hoist = params[4] != 0;
	bool mentionable = params[5] != 0;

	char* permissionsStr;
	pContext->LocalToString(params[6], &permissionsStr);

	IPluginFunction* callback = pContext->GetFunctionById(params[7]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	uint64_t permissions;
	if (!ParseUInt64(permissionsStr, permissions)) {
		pContext->ReportError("Invalid permissions string");
		return 0;
	}

	cell_t data = params[8];
	guild->CreateRole(name, color, hoist, mentionable, permissions, callback, data);
	return 1;
}

static cell_t guild_CreateRoleFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[2]);
	if (!role) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	guild->CreateRoleFromObject(role, callback, data);
	return 1;
}

static cell_t guild_BulkDeleteCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	guild->BulkDeleteCommands(callback, data);
	return 1;
}

static cell_t guild_GetCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[3];
	guild->GetCommands(callback, data);
	return 1;
}

static cell_t guild_GetCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

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
	guild->GetCommand(command_id, callback, data);
	return 1;
}

static cell_t guild_ModifyCurrentMember(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	char* nickname;
	pContext->LocalToString(params[2], &nickname);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	guild->ModifyCurrentMember(nickname, callback, data);
	return 1;
}

static cell_t guild_GetPruneCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t days = static_cast<uint16_t>(params[2]);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	guild->GetPruneCount(days, callback, data);
	return 1;
}

static cell_t guild_BeginPrune(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	uint16_t days = static_cast<uint16_t>(params[2]);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	guild->BeginPrune(days, callback, data);
	return 1;
}

static cell_t guild_EditRolePositions(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	int role_count = params[3];
	if (role_count <= 0) return 0;

	std::vector<dpp::role> roles;
	cell_t* role_array;
	pContext->LocalToPhysAddr(params[2], &role_array);

	for (int i = 0; i < role_count; i++) {
		DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, role_array[i]);
		if (role) {
			roles.push_back(role->GetDPPRole());
		}
	}

	if (roles.empty()) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	guild->EditRolePositions(roles, callback, data);
	return 1;
}

static cell_t guild_EditChannelPositions(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	int channel_count = params[3];
	if (channel_count <= 0) return 0;

	std::vector<dpp::channel> channels;
	cell_t* channel_array;
	pContext->LocalToPhysAddr(params[2], &channel_array);

	for (int i = 0; i < channel_count; i++) {
		DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, channel_array[i]);
		if (channel) {
			channels.push_back(channel->GetDPPChannel());
		}
	}

	if (channels.empty()) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	guild->EditChannelPositions(channels, callback, data);
	return 1;
}

extern const sp_nativeinfo_t guild_natives[] = {
	{"DiscordGuild.FetchGuild", guild_FetchGuild},
	{"DiscordGuild.FindGuild", guild_FindGuild},
	{"DiscordGuild.GetId", EntityGetId<DiscordGuild>},
	{"DiscordGuild.GetName", EntityGetName<DiscordGuild>},
	{"DiscordGuild.SetName", guild_SetName},
	{"DiscordGuild.GetDescription", EntityGetDescription<DiscordGuild>},
	{"DiscordGuild.SetDescription", guild_SetDescription},
	{"DiscordGuild.GetOwnerId", guild_GetOwnerId},
	{"DiscordGuild.MemberCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetMemberCount>},
	{"DiscordGuild.VerificationLevel.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetVerificationLevel>},
	{"DiscordGuild.PremiumTier.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetPremiumTier>},
	{"DiscordGuild.IsLarge.get", EntityGetBool<DiscordGuild, &DiscordGuild::IsLarge>},
	{"DiscordGuild.IsVerified.get", EntityGetBool<DiscordGuild, &DiscordGuild::IsVerified>},
	{"DiscordGuild.IsPartnered.get", EntityGetBool<DiscordGuild, &DiscordGuild::IsPartnered>},
	{"DiscordGuild.GetIconUrl", guild_GetIconUrl},
	{"DiscordGuild.GetBannerUrl", guild_GetBannerUrl},
	{"DiscordGuild.GetSplashUrl", guild_GetSplashUrl},
	{"DiscordGuild.GetDiscoverySplashUrl", guild_GetDiscoverySplashUrl},
	{"DiscordGuild.GetVanityUrlCode", guild_GetVanityUrlCode},
	{"DiscordGuild.MaxMembers.get", EntityGetInt<DiscordGuild, uint32_t, &DiscordGuild::GetMaxMembers>},
	{"DiscordGuild.MaxPresences.get", EntityGetInt<DiscordGuild, uint32_t, &DiscordGuild::GetMaxPresences>},
	{"DiscordGuild.PremiumSubscriptionCount.get", EntityGetInt<DiscordGuild, uint16_t, &DiscordGuild::GetPremiumSubscriptionCount>},
	{"DiscordGuild.ExplicitContentFilter.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetExplicitContentFilter>},
	{"DiscordGuild.MfaLevel.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetMfaLevel>},
	{"DiscordGuild.NsfwLevel.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetNsfwLevel>},
	{"DiscordGuild.AfkTimeout.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetAfkTimeout>},
	{"DiscordGuild.DefaultMessageNotifications.get", EntityGetInt<DiscordGuild, uint8_t, &DiscordGuild::GetDefaultMessageNotifications>},
	{"DiscordGuild.ShardId.get", EntityGetInt<DiscordGuild, uint16_t, &DiscordGuild::GetShardId>},
	{"DiscordGuild.Flags.get", EntityGetFlags<DiscordGuild>},
	{"DiscordGuild.FlagsExtra.get", EntityGetInt<DiscordGuild, uint16_t, &DiscordGuild::GetFlagsExtra>},
	{"DiscordGuild.GetInvites", guild_GetInvites},
	{"DiscordGuild.GetWebhooks", guild_GetWebhooks},
	{"DiscordGuild.GetAfkChannelId", guild_GetAfkChannelId},
	{"DiscordGuild.GetSystemChannelId", guild_GetSystemChannelId},
	{"DiscordGuild.GetRulesChannelId", guild_GetRulesChannelId},
	{"DiscordGuild.GetPublicUpdatesChannelId", guild_GetPublicUpdatesChannelId},
	{"DiscordGuild.GetSafetyAlertsChannelId", guild_GetSafetyAlertsChannelId},
	{"DiscordGuild.GetBasePermissions", guild_GetBasePermissions},
	{"DiscordGuild.GetPermissionsInChannel", guild_GetPermissionsInChannel},
	{"DiscordGuild.HasPermission", guild_HasPermission},
	{"DiscordGuild.HasPermissionInChannel", guild_HasPermissionInChannel},
	{"DiscordGuild.Modify", guild_Modify},
	{"DiscordGuild.RoleCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetRoleCount>},
	{"DiscordGuild.GetRoles", guild_GetRoles},
	{"DiscordGuild.ChannelCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetChannelCount>},
	{"DiscordGuild.ThreadCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetThreadCount>},
	{"DiscordGuild.EmojiCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetEmojiCount>},
	{"DiscordGuild.VoiceMemberCount.get", EntityGetInt<DiscordGuild, size_t, &DiscordGuild::GetVoiceMemberCount>},
	{"DiscordGuild.GetMember", guild_GetMember},
	{"DiscordGuild.GetMembers", guild_GetMembers},
	{"DiscordGuild.SearchMembers", guild_SearchMembers},
	{"DiscordGuild.GetActiveThreads", guild_GetActiveThreads},
	{"DiscordGuild.GetChannels", guild_GetChannels},
	{"DiscordGuild.GetBans", guild_GetBans},
	{"DiscordGuild.CreateEmoji", guild_CreateEmoji},
	{"DiscordGuild.GetEmoji", guild_GetEmoji},
	{"DiscordGuild.GetAllEmojis", guild_GetAllEmojis},
	{"DiscordGuild.CreateSticker", guild_CreateSticker},
	{"DiscordGuild.CreateScheduledEvent", guild_CreateScheduledEvent},
	{"DiscordGuild.CreateExternalScheduledEvent", guild_CreateExternalScheduledEvent},
	{"DiscordGuild.GetScheduledEvents", guild_GetScheduledEvents},
	{"DiscordGuild.GetScheduledEvent", guild_GetScheduledEvent},
	{"DiscordGuild.GetScheduledEventUsers", guild_GetScheduledEventUsers},
	{"DiscordGuild.BanUser", guild_BanUser},
	{"DiscordGuild.UnbanUser", guild_UnbanUser},
	{"DiscordGuild.CreateRole", guild_CreateRole},
	{"DiscordGuild.CreateRoleFromObject", guild_CreateRoleFromObject},
	{"DiscordGuild.BulkDeleteCommands", guild_BulkDeleteCommands},
	{"DiscordGuild.GetCommands", guild_GetCommands},
	{"DiscordGuild.GetCommand", guild_GetCommand},
	{"DiscordGuild.ModifyCurrentMember", guild_ModifyCurrentMember},
	{"DiscordGuild.GetPruneCount", guild_GetPruneCount},
	{"DiscordGuild.BeginPrune", guild_BeginPrune},
	{"DiscordGuild.EditRolePositions", guild_EditRolePositions},
	{"DiscordGuild.EditChannelPositions", guild_EditChannelPositions},
	{nullptr, nullptr}
};
