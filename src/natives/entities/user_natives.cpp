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
#include "entities/discord_user.h"
#include "entities/discord_guild_member.h"
#include "entities/discord_role.h"

static cell_t user_CreateFromId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* userId;
	pContext->LocalToString(params[2], &userId);

	char* guildId = nullptr;
	if (params[0] >= 3 && params[3] != 0) {
		pContext->LocalToString(params[3], &guildId);
	}

	dpp::snowflake userFlake;
	if (!ParseSnowflake(pContext, userId, userFlake)) return 0;

	dpp::snowflake guildFlake = 0;
	if (guildId && guildId[0] != '\0') {
		ParseSnowflake(pContext, guildId, guildFlake);
	}

		dpp::user* user_ptr = dpp::find_user(userFlake);

		if (!user_ptr) {
			dpp::user user_obj;
			user_obj.id = userFlake;

			DiscordUser* pDiscordUser;
			if (guildFlake != 0) {
				dpp::guild* guild_ptr = dpp::find_guild(guildFlake);
				if (guild_ptr) {
					auto member_it = guild_ptr->members.find(userFlake);
					if (member_it != guild_ptr->members.end()) {
						pDiscordUser = new DiscordUser(user_obj, member_it->second, discord);
					} else {
						dpp::guild_member member_obj;
						member_obj.user_id = userFlake;
						member_obj.guild_id = guildFlake;
						pDiscordUser = new DiscordUser(user_obj, member_obj, discord);
					}
				} else {
					dpp::guild_member member_obj;
					member_obj.user_id = userFlake;
					member_obj.guild_id = guildFlake;
					pDiscordUser = new DiscordUser(user_obj, member_obj, discord);
				}
			} else {
				pDiscordUser = new DiscordUser(user_obj, discord);
			}

			return Handles.Create(pContext, pDiscordUser, HandleId::DiscordUser);
		}

		DiscordUser* pDiscordUser;
		if (guildFlake != 0) {
			dpp::guild* guild_ptr = dpp::find_guild(guildFlake);
			if (guild_ptr) {
				auto member_it = guild_ptr->members.find(userFlake);
				if (member_it != guild_ptr->members.end()) {
					pDiscordUser = new DiscordUser(*user_ptr, member_it->second, discord);
				} else {
					dpp::guild_member member_obj;
					member_obj.user_id = userFlake;
					member_obj.guild_id = guildFlake;
					pDiscordUser = new DiscordUser(*user_ptr, member_obj, discord);
				}
			} else {
				dpp::guild_member member_obj;
				member_obj.user_id = userFlake;
				member_obj.guild_id = guildFlake;
				pDiscordUser = new DiscordUser(*user_ptr, member_obj, discord);
			}
		} else {
			pDiscordUser = new DiscordUser(*user_ptr, discord);
		}

		return Handles.Create(pContext, pDiscordUser, HandleId::DiscordUser);
}

static cell_t user_FetchUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* userId;
	pContext->LocalToString(params[2], &userId);

	char* guildId;
	pContext->LocalToString(params[3], &guildId);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[5];

	dpp::snowflake userFlake;
	if (!ParseSnowflake(pContext, userId, userFlake)) return 0;

	dpp::snowflake guildFlake = 0;
	if (guildId && guildId[0] != '\0') {
		ParseSnowflake(pContext, guildId, guildFlake);
	}

	Handle_t client_handle = discord->GetHandle();
	if (guildFlake != 0) {
		dpp::guild* guild = dpp::find_guild(guildFlake);
		if (guild) {
			auto member_it = guild->members.find(userFlake);
			if (member_it != guild->members.end()) {
				dpp::user* user_ptr = member_it->second.get_user();
				if (user_ptr) {
					DiscordUser* user = new DiscordUser(*user_ptr, member_it->second, discord);
					PushCachedResult<DiscordUser>(client_handle, discord, callback, data, user);
				} else {
					PushError(client_handle, discord, callback, data, "User not found in cache", DiscordResultType::User);
				}
				return 1;
			}
		}
		discord->Members().Get(guildFlake, userFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& confirmation) {
			if (confirmation.is_error()) {
				PushError(client_handle, discord, callback, data, confirmation.get_error().human_readable, DiscordResultType::User);
				return;
			}
			dpp::guild_member member_obj = confirmation.get<dpp::guild_member>();
			dpp::user* user_ptr = member_obj.get_user();
			if (user_ptr) {
				DiscordUser* user = new DiscordUser(*user_ptr, member_obj, discord);
				PushCachedResult<DiscordUser>(client_handle, discord, callback, data, user);
			} else {
				PushError(client_handle, discord, callback, data, "User not found", DiscordResultType::User);
			}
		});
	} else {
		dpp::user* cached_user = dpp::find_user(userFlake);
		if (cached_user) {
			DiscordUser* user = new DiscordUser(*cached_user, discord);
			PushCachedResult<DiscordUser>(client_handle, discord, callback, data, user);
			return 1;
		}
		discord->Members().GetUser(userFlake, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& confirmation) {
			if (confirmation.is_error()) {
				PushError(client_handle, discord, callback, data, confirmation.get_error().human_readable, DiscordResultType::User);
				return;
			}
			DiscordUser* user = nullptr;
			if (std::holds_alternative<dpp::user>(confirmation.value)) {
				user = new DiscordUser(std::get<dpp::user>(confirmation.value), discord);
			} else if (std::holds_alternative<dpp::user_identified>(confirmation.value)) {
				user = new DiscordUser(std::get<dpp::user_identified>(confirmation.value), discord);
			} else {
				user = new DiscordUser(confirmation.get<dpp::user>(), discord);
			}
			PushCachedResult<DiscordUser>(client_handle, discord, callback, data, user);
		});
	}

	return 1;
}

static cell_t user_FindUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	char* guild_id_str = nullptr;
	if (params[0] >= 3) {
		pContext->LocalToString(params[3], &guild_id_str);
	}

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	dpp::user* user_ptr = dpp::find_user(user_id);

	if (!user_ptr) {
		return 0; // User not found in cache
	}

	DiscordUser* pDiscordUser;

	if (guild_id_str && guild_id_str[0] != '\0') {
		dpp::snowflake guild_id;
		if (!ParseSnowflake(pContext, guild_id_str, guild_id)) return 0;
		dpp::guild* guild_ptr = dpp::find_guild(guild_id);

		if (guild_ptr) {
			auto member_it = guild_ptr->members.find(user_id);
			if (member_it != guild_ptr->members.end()) {
				pDiscordUser = new DiscordUser(*user_ptr, member_it->second, discord);
			} else {
				// User exists but no member data - create basic member
				dpp::guild_member member_obj;
				member_obj.user_id = user_id;
				member_obj.guild_id = guild_id;
				pDiscordUser = new DiscordUser(*user_ptr, member_obj, discord);
			}
		} else {
			// Guild not found - create basic member
			dpp::guild_member member_obj;
			member_obj.user_id = user_id;
			member_obj.guild_id = guild_id;
			pDiscordUser = new DiscordUser(*user_ptr, member_obj, discord);
		}
	} else {
		pDiscordUser = new DiscordUser(*user_ptr, discord);
	}

	return Handles.Create(pContext, pDiscordUser, HandleId::DiscordUser);
}

static cell_t user_GetClient(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	DiscordClient* client = user->GetClient();

	if (!client) {
		return 0;
	}

	return client->GetHandle();
}

static cell_t user_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	dpp::snowflake guild_id = user->GetGuildId();

	if (guild_id == 0) {
		return 0;
	}

	char guildIdStr[32];
	snprintf(guildIdStr, sizeof(guildIdStr), "%" PRIu64, static_cast<unsigned long long>(guild_id));
	pContext->StringToLocal(params[2], params[3], guildIdStr);
	return 1;
}

static cell_t user_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[2], params[3], user->GetUserName());
	return 1;
}

static cell_t user_GetDiscriminator(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return user->GetDiscriminator();
}

static cell_t user_GetGlobalName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[2], params[3], user->GetGlobalName());
	return 1;
}

static cell_t user_GetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[3], params[4], user->GetAvatarUrl(params[2]).c_str());
	return 1;
}

static cell_t user_GetDefaultAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[2], params[3], user->GetDefaultAvatarUrl().c_str());
	return 1;
}

static cell_t user_GetAvatarDecorationUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[3], params[4], user->GetAvatarDecorationUrl(params[2]).c_str());
	return 1;
}

static cell_t user_HasFlag(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return user->HasFlag(static_cast<dpp::user_flags>(params[2]));
}

static cell_t user_IsBot(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return user->IsBot();
}

static cell_t user_HasGuildMember(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return user->HasGuildMember();
}

static cell_t user_GetNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	std::string nickname = user->GetNickName();
	pContext->StringToLocal(params[2], params[3], nickname.c_str());
	return 1;
}

static cell_t user_GetJoinedAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return static_cast<cell_t>(user->GetJoinedAt());
}

static cell_t user_IsPending(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	return user->IsPending();
}

static cell_t user_HasPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* permission;
	pContext->LocalToString(params[2], &permission);
	return user->HasPermission(permission);
}

static cell_t user_GetPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char permStr[32];
	snprintf(permStr, sizeof(permStr), "%" PRIu64, user->GetPermissions());
	pContext->StringToLocal(params[2], params[3], permStr);
	return 1;
}

static cell_t user_HasPermissionInChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* permission;
	pContext->LocalToString(params[3], &permission);

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;
	return user->HasPermissionInChannel(channel, permission);
}

static cell_t user_GetPermissionsInChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	dpp::snowflake channel;
	if (!ParseSnowflake(pContext, channelId, channel)) return 0;
	char permStr[32];
	snprintf(permStr, sizeof(permStr), "%" PRIu64, user->GetPermissionsInChannel(channel));
	pContext->StringToLocal(params[3], params[4], permStr);
	return 1;
}

static cell_t user_GetRoles(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	std::vector<dpp::snowflake> roles = user->GetRoles();
	cell_t maxSize = params[3];
	cell_t actualSize = (static_cast<cell_t>(roles.size()) < maxSize) ? static_cast<cell_t>(roles.size()) : maxSize;

	cell_t* rolesArray;
	pContext->LocalToPhysAddr(params[2], &rolesArray);

	for (cell_t i = 0; i < actualSize; i++) {
		char roleStr[32];
		snprintf(roleStr, sizeof(roleStr), "%" PRIu64, static_cast<unsigned long long>(roles[i]));
		pContext->StringToLocalUTF8(rolesArray[i], 21, roleStr, nullptr);
	}

	return actualSize;
}

static cell_t user_HasRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* roleId;
	pContext->LocalToString(params[2], &roleId);

	dpp::snowflake role;
	if (!ParseSnowflake(pContext, roleId, role)) return 0;
	return user->HasRole(role);
}

static cell_t user_HasAnyRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	cell_t* roleIds;
	pContext->LocalToPhysAddr(params[2], &roleIds);
	cell_t roleCount = params[3];

	if (roleCount <= 0) {
		return 0;
	}

	std::vector<dpp::snowflake> roles;

	for (cell_t i = 0; i < roleCount; i++) {
		char* roleIdStr;
		pContext->LocalToString(roleIds[i], &roleIdStr);
		dpp::snowflake roleId;
		if (ParseSnowflake(pContext, roleIdStr, roleId)) {
			roles.push_back(roleId);
		}
	}

	if (roles.empty()) {
		return 0;
	}

	return user->HasAnyRole(roles);
}

static cell_t user_HasAllRoles(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	cell_t* roleIds;
	pContext->LocalToPhysAddr(params[2], &roleIds);
	cell_t roleCount = params[3];

	if (roleCount <= 0) {
		return 0;
	}

	std::vector<dpp::snowflake> roles;

	for (cell_t i = 0; i < roleCount; i++) {
		char* roleIdStr;
		pContext->LocalToString(roleIds[i], &roleIdStr);
		dpp::snowflake roleId;
		if (ParseSnowflake(pContext, roleIdStr, roleId)) {
			roles.push_back(roleId);
		}
	}

	if (roles.empty()) {
		return 0;
	}

	return user->HasAllRoles(roles);
}

static cell_t user_GetHighestRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	dpp::snowflake highest = user->GetHighestRole();
	char roleStr[32];
	snprintf(roleStr, sizeof(roleStr), "%" PRIu64, static_cast<unsigned long long>(highest));
	pContext->StringToLocal(params[2], params[3], roleStr);
	return 1;
}

static cell_t user_GetRoleName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* roleId;
	pContext->LocalToString(params[2], &roleId);

	dpp::snowflake role;
	if (!ParseSnowflake(pContext, roleId, role)) return 0;
	std::string roleName = user->GetRoleName(role);
	pContext->StringToLocal(params[3], params[4], roleName.c_str());
	return !roleName.empty();
}

static cell_t user_GetRoleNames(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	std::vector<std::string> roleNames = user->GetRoleNames();
	cell_t maxSize = params[3];
	cell_t actualSize = (static_cast<cell_t>(roleNames.size()) < maxSize) ? static_cast<cell_t>(roleNames.size()) : maxSize;

	cell_t* namesArray;
	pContext->LocalToPhysAddr(params[2], &namesArray);

	for (cell_t i = 0; i < actualSize; i++) {
		pContext->StringToLocalUTF8(namesArray[i], 64, roleNames[i].c_str(), nullptr);
	}

	return actualSize;
}

static cell_t user_SetNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* nickname;
	pContext->LocalToString(params[2], &nickname);
	user->SetNickName(nickname);
	return 1;
}

static cell_t user_KickFromGuild(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	user->KickFromGuild();
	return 1;
}

static cell_t user_BanFromGuild(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	char* reason;
	pContext->LocalToString(params[2], &reason);

	int delete_days = params[3];
	user->BanFromGuild(reason, delete_days);
	return 1;
}

static cell_t user_UnbanFromGuild(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	user->UnbanFromGuild();
	return 1;
}

static cell_t user_SetTimeout(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	time_t timeout_until = static_cast<time_t>(params[2]);
	user->SetTimeout(timeout_until);
	return 1;
}

static cell_t user_RemoveTimeout(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	user->RemoveTimeout();
	return 1;
}

extern const sp_nativeinfo_t user_natives[] = {
	{"DiscordUser.DiscordUser", user_CreateFromId},
	{"DiscordUser.FetchUser", user_FetchUser},
	{"DiscordUser.FindUser", user_FindUser},
	{"DiscordUser.GetId", EntityGetId<DiscordUser>},
	{"DiscordUser.GetClient", user_GetClient},
	{"DiscordUser.GetGuildId", user_GetGuildId},
	{"DiscordUser.GetUserName", user_GetUserName},
	{"DiscordUser.Discriminator.get", user_GetDiscriminator},
	{"DiscordUser.GetGlobalName", user_GetGlobalName},
	{"DiscordUser.GetAvatarUrl", user_GetAvatarUrl},
	{"DiscordUser.GetDefaultAvatarUrl", user_GetDefaultAvatarUrl},
	{"DiscordUser.GetAvatarDecorationUrl", user_GetAvatarDecorationUrl},
	{"DiscordUser.GetMention", EntityGetMention<DiscordUser>},
	{"DiscordUser.GetUrl", EntityGetUrl<DiscordUser>},
	{"DiscordUser.Flags.get", EntityGetFlags<DiscordUser>},
	{"DiscordUser.HasFlag", user_HasFlag},
	{"DiscordUser.IsBot.get", user_IsBot},
	{"DiscordUser.HasGuildMember.get", user_HasGuildMember},
	{"DiscordUser.GetNickName", user_GetNickName},
	{"DiscordUser.JoinedAt.get", user_GetJoinedAt},
	{"DiscordUser.IsPending.get", user_IsPending},
	{"DiscordUser.HasPermission", user_HasPermission},
	{"DiscordUser.GetPermissions", user_GetPermissions},
	{"DiscordUser.HasPermissionInChannel", user_HasPermissionInChannel},
	{"DiscordUser.GetPermissionsInChannel", user_GetPermissionsInChannel},
	{"DiscordUser.GetRoles", user_GetRoles},
	{"DiscordUser.HasRole", user_HasRole},
	{"DiscordUser.HasAnyRole", user_HasAnyRole},
	{"DiscordUser.HasAllRoles", user_HasAllRoles},
	{"DiscordUser.GetHighestRole", user_GetHighestRole},
	{"DiscordUser.GetRoleName", user_GetRoleName},
	{"DiscordUser.GetRoleNames", user_GetRoleNames},
	{"DiscordUser.SetNickName", user_SetNickName},
	{"DiscordUser.KickFromGuild", user_KickFromGuild},
	{"DiscordUser.BanFromGuild", user_BanFromGuild},
	{"DiscordUser.UnbanFromGuild", user_UnbanFromGuild},
	{"DiscordUser.SetTimeout", user_SetTimeout},
	{"DiscordUser.RemoveTimeout", user_RemoveTimeout},
	{nullptr, nullptr}
};
