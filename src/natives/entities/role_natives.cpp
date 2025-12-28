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
#include "entities/discord_role.h"

static cell_t role_CreateEmpty(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordRole* pDiscordRole = new DiscordRole(discord);

	return Handles.Create(pContext, pDiscordRole, HandleId::DiscordRole);
}

static cell_t role_FetchRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* guildId;
	char* roleId;
	pContext->LocalToString(params[2], &guildId);
	pContext->LocalToString(params[3], &roleId);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid create role callback function");
		return 0;
	}

	cell_t data = params[5];

	dpp::snowflake guildFlake, roleFlake;
	if (!ParseSnowflake(pContext, guildId, guildFlake)) return 0;
	if (!ParseSnowflake(pContext, roleId, roleFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	dpp::role* cached_role = dpp::find_role(roleFlake);
	if (cached_role) {
		DiscordRole* role = new DiscordRole(*cached_role, guildFlake, discord);
		PushCachedResult<DiscordRole>(client_handle, discord, callback, data, role);
		return 1;
	}
	discord->Roles().GetAll(guildFlake, [client_handle, discord, callback, data, guildFlake, roleFlake](const dpp::confirmation_callback_t& confirmation) {
		if (confirmation.is_error()) {
			PushError(client_handle, discord, callback, data, confirmation.get_error().human_readable, DiscordResultType::Role);
			return;
		}
		auto roles = confirmation.get<dpp::role_map>();
		auto it = roles.find(roleFlake);
		if (it == roles.end()) {
			PushError(client_handle, discord, callback, data, "Role not found", DiscordResultType::Role);
			return;
		}
		DiscordRole* role = new DiscordRole(it->second, guildFlake, discord);
		PushCachedResult<DiscordRole>(client_handle, discord, callback, data, role);
	});

	return 1;
}

static cell_t role_FindRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* guild_id_str;
	char* role_id_str;
	pContext->LocalToString(params[2], &guild_id_str);
	pContext->LocalToString(params[3], &role_id_str);

	dpp::snowflake guild_id, role_id;
	if (!ParseSnowflake(pContext, guild_id_str, guild_id)) return 0;
	if (!ParseSnowflake(pContext, role_id_str, role_id)) return 0;

	dpp::role* role_ptr = dpp::find_role(role_id);

	if (!role_ptr) {
		return 0; // Role not found in cache
	}

	DiscordRole* pDiscordRole = new DiscordRole(*role_ptr, guild_id, discord);

	return Handles.Create(pContext, pDiscordRole, HandleId::DiscordRole);
}

static cell_t role_GetColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return static_cast<cell_t>(role->GetColor());
}

static cell_t role_GetHoist(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsHoisted();
}

static cell_t role_GetIconHash(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	pContext->StringToLocal(params[2], params[3], role->GetIconHash().c_str());
	return 1;
}

static cell_t role_GetUnicodeEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	pContext->StringToLocal(params[2], params[3], role->GetUnicodeEmoji());
	return 1;
}

static cell_t role_GetPosition(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->GetPosition();
}

static cell_t role_GetPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char perms_str[32];
	snprintf(perms_str, sizeof(perms_str), "%" PRIu64, role->GetPermissions());
	pContext->StringToLocal(params[2], params[3], perms_str);
	return 1;
}

static cell_t role_GetManaged(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsManaged();
}

static cell_t role_GetMentionable(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsMentionable();
}

static cell_t role_GetHasTags(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->HasTags();
}

static cell_t role_GetIsPremiumSubscriberRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsPremiumSubscriberRole();
}

static cell_t role_GetIsAvailableForPurchase(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsAvailableForPurchase();
}

static cell_t role_GetIsGuildConnections(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	return role->IsGuildConnections();
}

static cell_t role_GetBotId(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	std::string bot_id = role->GetBotId();
	if (bot_id.empty()) {
		return 0;
	}
	pContext->StringToLocal(params[2], params[3], bot_id.c_str());
	return 1;
}

static cell_t role_GetIntegrationId(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	std::string integration_id = role->GetIntegrationId();
	if (integration_id.empty()) {
		return 0;
	}
	pContext->StringToLocal(params[2], params[3], integration_id.c_str());
	return 1;
}

static cell_t role_GetSubscriptionListingId(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	std::string subscription_id = role->GetSubscriptionListingId();
	if (subscription_id.empty()) {
		return 0;
	}
	pContext->StringToLocal(params[2], params[3], subscription_id.c_str());
	return 1;
}

static cell_t role_HasPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char* permission;
	pContext->LocalToString(params[2], &permission);
	return role->HasPermission(permission);
}

static cell_t role_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	role->SetName(name);
	return 1;
}

static cell_t role_SetColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	uint32_t color = static_cast<uint32_t>(params[2]);
	role->SetColor(color);
	return 1;
}

static cell_t role_SetHoist(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	role->SetHoist(params[2] != 0);
	return 1;
}

static cell_t role_SetMentionable(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	role->SetMentionable(params[2] != 0);
	return 1;
}

static cell_t role_Modify(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	role->Modify();
	return 1;
}

static cell_t role_AddToUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char* user_id;
	pContext->LocalToString(params[2], &user_id);

	dpp::snowflake user_flake;
	if (!ParseSnowflake(pContext, user_id, user_flake)) return 0;
	role->AddToUser(user_flake);
	return 1;
}

static cell_t role_RemoveFromUser(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char* user_id;
	pContext->LocalToString(params[2], &user_id);

	dpp::snowflake user_flake;
	if (!ParseSnowflake(pContext, user_id, user_flake)) return 0;
	role->RemoveFromUser(user_flake);
	return 1;
}

static cell_t role_SetIcon(IPluginContext* pContext, const cell_t* params)
{
	DiscordRole* role = Handles.GetPointer<DiscordRole>(pContext, params[1]);
	if (!role) return 0;

	char* filepath;
	pContext->LocalToString(params[2], &filepath);

	dpp::image_type type = static_cast<dpp::image_type>(params[3]);
	return role->SetIcon(filepath, type);
}

extern const sp_nativeinfo_t role_natives[] = {
	{"DiscordRole.DiscordRole", role_CreateEmpty},
	{"DiscordRole.FetchRole", role_FetchRole},
	{"DiscordRole.FindRole", role_FindRole},
	{"DiscordRole.GetId", EntityGetId<DiscordRole>},
	{"DiscordRole.GetName", EntityGetName<DiscordRole>},
	{"DiscordRole.SetName", role_SetName},
	{"DiscordRole.Color.get", role_GetColor},
	{"DiscordRole.Color.set", role_SetColor},
	{"DiscordRole.Hoist.get", role_GetHoist},
	{"DiscordRole.Hoist.set", role_SetHoist},
	{"DiscordRole.GetIconHash", role_GetIconHash},
	{"DiscordRole.GetUnicodeEmoji", role_GetUnicodeEmoji},
	{"DiscordRole.Position.get", role_GetPosition},
	{"DiscordRole.GetPermissions", role_GetPermissions},
	{"DiscordRole.Managed.get", role_GetManaged},
	{"DiscordRole.Mentionable.get", role_GetMentionable},
	{"DiscordRole.Mentionable.set", role_SetMentionable},
	{"DiscordRole.HasTags.get", role_GetHasTags},
	{"DiscordRole.IsPremiumSubscriberRole.get", role_GetIsPremiumSubscriberRole},
	{"DiscordRole.IsAvailableForPurchase.get", role_GetIsAvailableForPurchase},
	{"DiscordRole.IsGuildConnections.get", role_GetIsGuildConnections},
	{"DiscordRole.GetBotId", role_GetBotId},
	{"DiscordRole.GetIntegrationId", role_GetIntegrationId},
	{"DiscordRole.GetSubscriptionListingId", role_GetSubscriptionListingId},
	{"DiscordRole.HasPermission", role_HasPermission},
	{"DiscordRole.GetMention", EntityGetMention<DiscordRole>},
	{"DiscordRole.Modify", role_Modify},
	{"DiscordRole.Delete", EntityDelete<DiscordRole>},
	{"DiscordRole.AddToUser", role_AddToUser},
	{"DiscordRole.RemoveFromUser", role_RemoveFromUser},
	{"DiscordRole.SetIcon", role_SetIcon},
	{nullptr, nullptr}
};
