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

	dpp::snowflake userFlake;
	if (!ParseSnowflake(pContext, userId, userFlake)) return 0;

	dpp::user* user_ptr = dpp::find_user(userFlake);

	DiscordUser* pDiscordUser;
	if (!user_ptr) {
		dpp::user user_obj;
		user_obj.id = userFlake;
		pDiscordUser = new DiscordUser(user_obj, discord);
	} else {
		pDiscordUser = new DiscordUser(*user_ptr, discord);
	}

	Handle_t handle = Handles.Create(pContext, pDiscordUser, HandleId::DiscordUser);
	if (!handle) return 0;
	return handle;
}

static cell_t user_FetchUser(IPluginContext* pContext, const cell_t* params)
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

	cell_t data = params[4];

	dpp::snowflake userFlake;
	if (!ParseSnowflake(pContext, userId, userFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();

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

	return 1;
}

static cell_t user_FindUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* user_id_str;
	pContext->LocalToString(params[2], &user_id_str);

	dpp::snowflake user_id;
	if (!ParseSnowflake(pContext, user_id_str, user_id)) return 0;

	dpp::user* user_ptr = dpp::find_user(user_id);

	if (!user_ptr) {
		return 0; // User not found in cache
	}

	DiscordUser* pDiscordUser = new DiscordUser(*user_ptr, discord);

	Handle_t handle = Handles.Create(pContext, pDiscordUser, HandleId::DiscordUser);
	if (!handle) return 0;
	return handle;
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

static cell_t user_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[2], params[3], user->GetUserName());
	return 1;
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

	pContext->StringToLocal(params[2], params[3], user->GetAvatarUrl(params[4]).c_str());
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

static cell_t user_GetFormattedUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordUser* user = Handles.GetPointer<DiscordUser>(pContext, params[1]);
	if (!user) return 0;

	pContext->StringToLocal(params[2], params[3], user->FormatUserName().c_str());
	return 1;
}

extern const sp_nativeinfo_t user_natives[] = {
	{"DiscordUser.DiscordUser", user_CreateFromId},
	{"DiscordUser.FetchUser", user_FetchUser},
	{"DiscordUser.FindUser", user_FindUser},
	{"DiscordUser.GetId", EntityGetId<DiscordUser>},
	{"DiscordUser.GetClient", user_GetClient},
	{"DiscordUser.GetUserName", user_GetUserName},
	{"DiscordUser.Discriminator.get", EntityGetInt<DiscordUser, uint16_t, &DiscordUser::GetDiscriminator>},
	{"DiscordUser.GetGlobalName", user_GetGlobalName},
	{"DiscordUser.GetAvatarUrl", user_GetAvatarUrl},
	{"DiscordUser.GetDefaultAvatarUrl", user_GetDefaultAvatarUrl},
	{"DiscordUser.GetAvatarDecorationUrl", user_GetAvatarDecorationUrl},
	{"DiscordUser.GetMention", EntityGetMention<DiscordUser>},
	{"DiscordUser.GetUrl", EntityGetUrl<DiscordUser>},
	{"DiscordUser.Flags.get", EntityGetFlags<DiscordUser>},
	{"DiscordUser.IsBot.get", EntityGetBool<DiscordUser, &DiscordUser::IsBot>},
	{"DiscordUser.GetFormattedUserName", user_GetFormattedUserName},
	{nullptr, nullptr}
};
