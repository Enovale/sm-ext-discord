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
#include "entities/discord_guild_member.h"

static cell_t member_GetNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetNickName().c_str());
	return 1;
}

static cell_t member_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetUserName());
	return 1;
}

static cell_t member_GetGlobalName(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetGlobalName());
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

static cell_t member_GetRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[3], params[4], member->GetRoleId(params[2]).c_str());
	return 1;
}

static cell_t member_GetRoles(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	array->AddStrings(member->GetRoles());

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
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

static cell_t member_SetNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* nickname;
	pContext->LocalToString(params[2], &nickname);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	member->SetNickName(nickname, callback, data);
	return 1;
}

static cell_t member_Kick(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* reason;
	pContext->LocalToString(params[2], &reason);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	member->Kick(reason && reason[0] ? reason : nullptr, callback, data);
	return 1;
}

static cell_t member_Ban(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	uint32_t delete_message_seconds = params[2];
	char* reason;
	pContext->LocalToString(params[3], &reason);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	member->Ban(delete_message_seconds, reason && reason[0] ? reason : nullptr, callback, data);
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

static cell_t member_MoveToVoiceChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	char* channel_id_str;
	pContext->LocalToString(params[2], &channel_id_str);

	dpp::snowflake channel_id;
	if (!ParseSnowflake(pContext, channel_id_str, channel_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	member->MoveToVoiceChannel(channel_id, callback, data);
	return 1;
}

static cell_t member_DisconnectFromVoice(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	member->DisconnectFromVoice(callback, data);
	return 1;
}

static cell_t member_SetMute(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	bool mute = params[2] != 0;
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	member->SetMute(mute, callback, data);
	return 1;
}

static cell_t member_SetDeaf(IPluginContext* pContext, const cell_t* params)
{
	DiscordGuildMember* member = Handles.GetPointer<DiscordGuildMember>(pContext, params[1]);
	if (!member) return 0;

	bool deaf = params[2] != 0;
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	member->SetDeaf(deaf, callback, data);
	return 1;
}

extern const sp_nativeinfo_t guild_member_natives[] = {
	{"DiscordGuildMember.GetUserId", EntityGetUserId<DiscordGuildMember>},
	{"DiscordGuildMember.GetGuildId", EntityGetGuildId<DiscordGuildMember>},
	{"DiscordGuildMember.GetNickName", member_GetNickName},
	{"DiscordGuildMember.GetUserName", member_GetUserName},
	{"DiscordGuildMember.GetGlobalName", member_GetGlobalName},
	{"DiscordGuildMember.GetAvatarHash", member_GetAvatarHash},
	{"DiscordGuildMember.GetAvatarUrl", member_GetAvatarUrl},
	{"DiscordGuildMember.HasAnimatedGuildAvatar.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::HasAnimatedGuildAvatar>},
	{"DiscordGuildMember.JoinedAt.get", EntityGetInt<DiscordGuildMember, time_t, &DiscordGuildMember::GetJoinedAt>},
	{"DiscordGuildMember.PremiumSince.get", EntityGetInt<DiscordGuildMember, time_t, &DiscordGuildMember::GetPremiumSince>},
	{"DiscordGuildMember.CommunicationDisabledUntil.get", EntityGetInt<DiscordGuildMember, time_t, &DiscordGuildMember::GetCommunicationDisabledUntil>},
	{"DiscordGuildMember.Deaf.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::IsDeaf>},
	{"DiscordGuildMember.Muted.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::IsMuted>},
	{"DiscordGuildMember.Pending.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::IsPending>},
	{"DiscordGuildMember.CommunicationDisabled.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::IsCommunicationDisabled>},
	{"DiscordGuildMember.HasRejoined.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::HasRejoined>},
	{"DiscordGuildMember.IsGuildOwner.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::IsGuildOwner>},
	{"DiscordGuildMember.HasCompletedOnboarding.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::HasCompletedOnboarding>},
	{"DiscordGuildMember.HasStartedOnboarding.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::HasStartedOnboarding>},
	{"DiscordGuildMember.HasBypassesVerification.get", EntityGetBool<DiscordGuildMember, &DiscordGuildMember::HasBypassesVerification>},
	{"DiscordGuildMember.RoleCount.get", EntityGetInt<DiscordGuildMember, size_t, &DiscordGuildMember::GetRoleCount>},
	{"DiscordGuildMember.GetRoleId", member_GetRoleId},
	{"DiscordGuildMember.GetRoles", member_GetRoles},
	{"DiscordGuildMember.HasRole", member_HasRole},
	{"DiscordGuildMember.GetPermissions", member_GetPermissions},
	{"DiscordGuildMember.HasPermission", member_HasPermission},
	{"DiscordGuildMember.AddRole", member_AddRole},
	{"DiscordGuildMember.RemoveRole", member_RemoveRole},
	{"DiscordGuildMember.SetNickName", member_SetNickName},
	{"DiscordGuildMember.Kick", member_Kick},
	{"DiscordGuildMember.Ban", member_Ban},
	{"DiscordGuildMember.Timeout", member_Timeout},
	{"DiscordGuildMember.RemoveTimeout", member_RemoveTimeout},
	{"DiscordGuildMember.MoveToVoiceChannel", member_MoveToVoiceChannel},
	{"DiscordGuildMember.DisconnectFromVoice", member_DisconnectFromVoice},
	{"DiscordGuildMember.SetMute", member_SetMute},
	{"DiscordGuildMember.SetDeaf", member_SetDeaf},
	{nullptr, nullptr}
};
