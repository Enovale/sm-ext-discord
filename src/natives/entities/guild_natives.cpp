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

	return Handles.Create(pContext, pDiscordGuild, HandleId::DiscordGuild);
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

static cell_t guild_GetMemberCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetMemberCount());
}

static cell_t guild_GetVerificationLevel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetVerificationLevel();
}

static cell_t guild_GetPremiumTier(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetPremiumTier();
}

static cell_t guild_IsLarge(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->IsLarge();
}

static cell_t guild_IsVerified(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->IsVerified();
}

static cell_t guild_IsPartnered(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->IsPartnered();
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

static cell_t guild_GetMaxMembers(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetMaxMembers();
}

static cell_t guild_GetMaxPresences(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetMaxPresences();
}

static cell_t guild_GetPremiumSubscriptionCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetPremiumSubscriptionCount();
}

static cell_t guild_GetExplicitContentFilter(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetExplicitContentFilter();
}

static cell_t guild_GetMfaLevel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetMfaLevel();
}

static cell_t guild_GetNsfwLevel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetNsfwLevel();
}

static cell_t guild_GetAfkTimeout(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetAfkTimeout();
}

static cell_t guild_GetDefaultMessageNotifications(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetDefaultMessageNotifications();
}

static cell_t guild_GetShardId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetShardId();
}

static cell_t guild_GetFlagsExtra(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return guild->GetFlagsExtra();
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
	snprintf(permStr, sizeof(permStr), "%" PRIu64, permissions);
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
	snprintf(permStr, sizeof(permStr), "%" PRIu64, permissions);
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

	guild->Modify();
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

	guild->GetInvites(callback, params[3]);
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

	guild->GetWebhooks(callback, params[3]);
	return 1;
}

static cell_t guild_GetRoleCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetRoleCount());
}

static cell_t guild_GetChannelCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetChannelCount());
}

static cell_t guild_GetThreadCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetThreadCount());
}

static cell_t guild_GetEmojiCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetEmojiCount());
}

static cell_t guild_GetVoiceMemberCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	return static_cast<cell_t>(guild->GetVoiceMemberCount());
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

	guild->GetMember(user_id, callback, params[4]);
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

	guild->GetMembers(limit, after, callback, params[5]);
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
	guild->SearchMembers(query, limit, callback, params[5]);
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

	guild->GetActiveThreads(callback, params[3]);
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
	guild->GetBans(before, after, limit, callback, params[6]);
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

	guild->CreateEmoji(name, image_path, static_cast<dpp::image_type>(params[4]));
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

	guild->CreateSticker(name, description, tags, file_path, static_cast<dpp::sticker_format>(params[6]));
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

	guild->CreateScheduledEvent(name, description, channel_id, params[5], params[6], static_cast<dpp::event_entity_type>(params[7]));
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

	guild->CreateExternalScheduledEvent(name, description, location, params[5], params[6]);
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

	guild->UnbanUser(user_id);
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

	guild->CreateRole(name, color, hoist, mentionable, permissions, callback, params[8]);
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

	guild->CreateRoleFromObject(role, callback, params[4]);
	return 1;
}

static cell_t guild_BulkDeleteCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuild* guild = Handles.GetPointer<DiscordGuild>(pContext, params[1]);
	if (!guild) return 0;

	guild->BulkDeleteCommands();
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
	{"DiscordGuild.MemberCount.get", guild_GetMemberCount},
	{"DiscordGuild.VerificationLevel.get", guild_GetVerificationLevel},
	{"DiscordGuild.PremiumTier.get", guild_GetPremiumTier},
	{"DiscordGuild.IsLarge.get", guild_IsLarge},
	{"DiscordGuild.IsVerified.get", guild_IsVerified},
	{"DiscordGuild.IsPartnered.get", guild_IsPartnered},
	{"DiscordGuild.GetIconUrl", guild_GetIconUrl},
	{"DiscordGuild.GetBannerUrl", guild_GetBannerUrl},
	{"DiscordGuild.GetSplashUrl", guild_GetSplashUrl},
	{"DiscordGuild.GetDiscoverySplashUrl", guild_GetDiscoverySplashUrl},
	{"DiscordGuild.GetVanityUrlCode", guild_GetVanityUrlCode},
	{"DiscordGuild.MaxMembers.get", guild_GetMaxMembers},
	{"DiscordGuild.MaxPresences.get", guild_GetMaxPresences},
	{"DiscordGuild.PremiumSubscriptionCount.get", guild_GetPremiumSubscriptionCount},
	{"DiscordGuild.ExplicitContentFilter.get", guild_GetExplicitContentFilter},
	{"DiscordGuild.MfaLevel.get", guild_GetMfaLevel},
	{"DiscordGuild.NsfwLevel.get", guild_GetNsfwLevel},
	{"DiscordGuild.AfkTimeout.get", guild_GetAfkTimeout},
	{"DiscordGuild.DefaultMessageNotifications.get", guild_GetDefaultMessageNotifications},
	{"DiscordGuild.ShardId.get", guild_GetShardId},
	{"DiscordGuild.Flags.get", EntityGetFlags<DiscordGuild>},
	{"DiscordGuild.FlagsExtra.get", guild_GetFlagsExtra},
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
	{"DiscordGuild.RoleCount.get", guild_GetRoleCount},
	{"DiscordGuild.ChannelCount.get", guild_GetChannelCount},
	{"DiscordGuild.ThreadCount.get", guild_GetThreadCount},
	{"DiscordGuild.EmojiCount.get", guild_GetEmojiCount},
	{"DiscordGuild.VoiceMemberCount.get", guild_GetVoiceMemberCount},
	{"DiscordGuild.GetMember", guild_GetMember},
	{"DiscordGuild.GetMembers", guild_GetMembers},
	{"DiscordGuild.SearchMembers", guild_SearchMembers},
	{"DiscordGuild.GetActiveThreads", guild_GetActiveThreads},
	{"DiscordGuild.GetBans", guild_GetBans},
	{"DiscordGuild.CreateEmoji", guild_CreateEmoji},
	{"DiscordGuild.CreateSticker", guild_CreateSticker},
	{"DiscordGuild.CreateScheduledEvent", guild_CreateScheduledEvent},
	{"DiscordGuild.CreateExternalScheduledEvent", guild_CreateExternalScheduledEvent},
	{"DiscordGuild.UnbanUser", guild_UnbanUser},
	{"DiscordGuild.CreateRole", guild_CreateRole},
	{"DiscordGuild.CreateRoleFromObject", guild_CreateRoleFromObject},
	{"DiscordGuild.BulkDeleteCommands", guild_BulkDeleteCommands},
	{nullptr, nullptr}
};
