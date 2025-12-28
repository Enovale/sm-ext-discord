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
#include "entities/discord_guild_member.h"

static cell_t member_GetUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetUserId().c_str());
	return 1;
}

static cell_t member_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetGuildId().c_str());
	return 1;
}

static cell_t member_GetNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetNickname().c_str());
	return 1;
}

static cell_t member_GetAvatarHash(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetAvatarHash().c_str());
	return 1;
}

static cell_t member_GetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetAvatarUrl(params[4]).c_str());
	return 1;
}

static cell_t member_HasAnimatedGuildAvatar(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->HasAnimatedGuildAvatar();
}

static cell_t member_GetJoinedAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return static_cast<cell_t>(member->GetJoinedAt());
}

static cell_t member_GetPremiumSince(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return static_cast<cell_t>(member->GetPremiumSince());
}

static cell_t member_GetCommunicationDisabledUntil(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return static_cast<cell_t>(member->GetCommunicationDisabledUntil());
}

static cell_t member_IsDeaf(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->IsDeaf();
}

static cell_t member_IsMuted(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->IsMuted();
}

static cell_t member_IsPending(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->IsPending();
}

static cell_t member_IsCommunicationDisabled(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->IsCommunicationDisabled();
}

static cell_t member_HasRejoined(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->HasRejoined();
}

static cell_t member_IsGuildOwner(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->IsGuildOwner();
}

static cell_t member_HasCompletedOnboarding(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->HasCompletedOnboarding();
}

static cell_t member_HasStartedOnboarding(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->HasStartedOnboarding();
}

static cell_t member_HasBypassesVerification(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return member->HasBypassesVerification();
}

static cell_t member_GetRoleCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	return static_cast<cell_t>(member->GetRoleCount());
}

static cell_t member_GetRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[3], params[4], member->GetRoleId(params[2]).c_str());
	return 1;
}

static cell_t member_HasRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* role_id_str;
	pContext->LocalToString(params[2], &role_id_str);

	dpp::snowflake role_id;
	if (!ParseSnowflake(pContext, role_id_str, role_id)) return 0;

	return member->HasRole(role_id);
}

static cell_t member_GetPermissions(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetPermissions().c_str());
	return 1;
}

static cell_t member_HasPermission(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* permission;
	pContext->LocalToString(params[2], &permission);

	return member->HasPermission(permission);
}

static cell_t member_AddRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* role_id_str;
	pContext->LocalToString(params[2], &role_id_str);

	dpp::snowflake role_id;
	if (!ParseSnowflake(pContext, role_id_str, role_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	member->AddRole(role_id, callback, data);
	return 1;
}

static cell_t member_RemoveRole(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* role_id_str;
	pContext->LocalToString(params[2], &role_id_str);

	dpp::snowflake role_id;
	if (!ParseSnowflake(pContext, role_id_str, role_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	member->RemoveRole(role_id, callback, data);
	return 1;
}

static cell_t member_SetNickname(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* nickname;
	pContext->LocalToString(params[2], &nickname);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	member->SetNickname(nickname, callback, data);
	return 1;
}

static cell_t member_Kick(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* reason = nullptr;
	if (params[0] >= 2) {
		pContext->LocalToString(params[2], &reason);
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	member->Kick(reason, callback, data);
	return 1;
}

static cell_t member_Ban(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	uint32_t delete_message_seconds = params[2];
	char* reason = nullptr;
	if (params[0] >= 3) {
		pContext->LocalToString(params[3], &reason);
	}

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	cell_t data = params[5];

	member->Ban(delete_message_seconds, reason, callback, data);
	return 1;
}

static cell_t member_Timeout(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	cell_t data = params[4];

	member->Timeout(static_cast<time_t>(params[2]), callback, data);
	return 1;
}

static cell_t member_RemoveTimeout(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);
	cell_t data = params[3];

	member->RemoveTimeout(callback, data);
	return 1;
}

extern const sp_nativeinfo_t guild_member_natives[] = {
	{"DiscordGuildMember.GetUserId", member_GetUserId},
	{"DiscordGuildMember.GetGuildId", member_GetGuildId},
	{"DiscordGuildMember.GetNickname", member_GetNickname},
	{"DiscordGuildMember.GetAvatarHash", member_GetAvatarHash},
	{"DiscordGuildMember.GetAvatarUrl", member_GetAvatarUrl},
	{"DiscordGuildMember.HasAnimatedGuildAvatar.get", member_HasAnimatedGuildAvatar},
	{"DiscordGuildMember.JoinedAt.get", member_GetJoinedAt},
	{"DiscordGuildMember.PremiumSince.get", member_GetPremiumSince},
	{"DiscordGuildMember.CommunicationDisabledUntil.get", member_GetCommunicationDisabledUntil},
	{"DiscordGuildMember.Deaf.get", member_IsDeaf},
	{"DiscordGuildMember.Muted.get", member_IsMuted},
	{"DiscordGuildMember.Pending.get", member_IsPending},
	{"DiscordGuildMember.CommunicationDisabled.get", member_IsCommunicationDisabled},
	{"DiscordGuildMember.HasRejoined.get", member_HasRejoined},
	{"DiscordGuildMember.IsGuildOwner.get", member_IsGuildOwner},
	{"DiscordGuildMember.HasCompletedOnboarding.get", member_HasCompletedOnboarding},
	{"DiscordGuildMember.HasStartedOnboarding.get", member_HasStartedOnboarding},
	{"DiscordGuildMember.HasBypassesVerification.get", member_HasBypassesVerification},
	{"DiscordGuildMember.RoleCount.get", member_GetRoleCount},
	{"DiscordGuildMember.GetRoleId", member_GetRoleId},
	{"DiscordGuildMember.HasRole", member_HasRole},
	{"DiscordGuildMember.GetPermissions", member_GetPermissions},
	{"DiscordGuildMember.HasPermission", member_HasPermission},
	{"DiscordGuildMember.AddRole", member_AddRole},
	{"DiscordGuildMember.RemoveRole", member_RemoveRole},
	{"DiscordGuildMember.SetNickname", member_SetNickname},
	{"DiscordGuildMember.Kick", member_Kick},
	{"DiscordGuildMember.Ban", member_Ban},
	{"DiscordGuildMember.Timeout", member_Timeout},
	{"DiscordGuildMember.RemoveTimeout", member_RemoveTimeout},
	{nullptr, nullptr}
};
