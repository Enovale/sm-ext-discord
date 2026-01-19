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
#include "features/discord_invite.h"

static cell_t invite_CreateInvite(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordInvite* invite = new DiscordInvite(discord);
	Handle_t handle = Handles.Create(pContext, invite, HandleId::DiscordInvite);
	if (!handle) return 0;
	return handle;
}

static cell_t invite_GetCode(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetCode());
	return 1;
}

static cell_t invite_GetGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetGuildId().c_str());
	return 1;
}

static cell_t invite_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetChannelId().c_str());
	return 1;
}

static cell_t invite_GetInviterId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetInviterId().c_str());
	return 1;
}

static cell_t invite_GetInviterUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetInviterUserName());
	return 1;
}

static cell_t invite_GetInviterAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetInviterAvatarUrl().c_str());
	return 1;
}

static cell_t invite_GetInviterIsBot(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetInviterIsBot();
}

static cell_t invite_GetGuildName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetGuildName());
	return 1;
}

static cell_t invite_GetChannelName(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	pContext->StringToLocal(params[2], params[3], invite->GetChannelName());
	return 1;
}

static cell_t invite_GetChannelType(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetChannelType();
}

static cell_t invite_GetApproximatePresenceCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetApproximatePresenceCount();
}

static cell_t invite_GetApproximateMemberCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetApproximateMemberCount();
}

static cell_t invite_GetUses(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetUses();
}

static cell_t invite_GetMaxUses(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetMaxUses();
}

static cell_t invite_GetMaxAge(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetMaxAge();
}

static cell_t invite_GetTargetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->GetTargetType();
}

static cell_t invite_IsTemporary(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->IsTemporary();
}

static cell_t invite_IsUnique(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return invite->IsUnique();
}

static cell_t invite_GetCreatedAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return static_cast<cell_t>(invite->GetCreatedAt());
}

static cell_t invite_GetExpiresAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	return static_cast<cell_t>(invite->GetExpiresAt());
}

static cell_t invite_SetMaxAge(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->SetMaxAge(params[2]);
	return 1;
}

static cell_t invite_SetMaxUses(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->SetMaxUses(params[2]);
	return 1;
}

static cell_t invite_SetTargetUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	char* user_id;
	pContext->LocalToString(params[2], &user_id);

	dpp::snowflake user_flake;
	if (!ParseSnowflake(pContext, user_id, user_flake)) return 0;
	invite->SetTargetUserId(user_flake);
	return 1;
}

static cell_t invite_SetTargetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->SetTargetType(params[2]);
	return 1;
}

static cell_t invite_SetTemporary(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->SetTemporary(params[2] != 0);
	return 1;
}

static cell_t invite_SetUnique(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->SetUnique(params[2] != 0);
	return 1;
}

static cell_t invite_Create(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	char* channel_id;
	pContext->LocalToString(params[2], &channel_id);

	dpp::snowflake channel_snowflake;
	if (!ParseSnowflake(pContext, channel_id, channel_snowflake)) return 0;
	invite->Create(channel_snowflake);
	return 1;
}

static cell_t invite_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordInvite* invite = Handles.GetPointer<DiscordInvite>(pContext, params[1]);
	if (!invite) return 0;

	invite->Delete();
	return 1;
}

static cell_t invite_Get(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* code;
	pContext->LocalToString(params[2], &code);

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	Handle_t client_handle = discord->GetHandle();

	discord->Invites().Get(code, [client_handle, discord, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordInvite>(client_handle, discord, callback, data, cb);
	});

	return 1;
}

extern const sp_nativeinfo_t invite_natives[] = {
	{"DiscordInvite.DiscordInvite", invite_CreateInvite},
	{"DiscordInvite.GetCode", invite_GetCode},
	{"DiscordInvite.GetGuildId", invite_GetGuildId},
	{"DiscordInvite.GetChannelId", invite_GetChannelId},
	{"DiscordInvite.GetInviterId", invite_GetInviterId},
	{"DiscordInvite.GetInviterUserName", invite_GetInviterUserName},
	{"DiscordInvite.GetInviterAvatarUrl", invite_GetInviterAvatarUrl},
	{"DiscordInvite.InviterIsBot.get", invite_GetInviterIsBot},
	{"DiscordInvite.GetGuildName", invite_GetGuildName},
	{"DiscordInvite.GetChannelName", invite_GetChannelName},
	{"DiscordInvite.ChannelType.get", invite_GetChannelType},
	{"DiscordInvite.ApproximatePresenceCount.get", invite_GetApproximatePresenceCount},
	{"DiscordInvite.ApproximateMemberCount.get", invite_GetApproximateMemberCount},
	{"DiscordInvite.Uses.get", invite_GetUses},
	{"DiscordInvite.MaxUses.get", invite_GetMaxUses},
	{"DiscordInvite.MaxUses.set", invite_SetMaxUses},
	{"DiscordInvite.MaxAge.get", invite_GetMaxAge},
	{"DiscordInvite.MaxAge.set", invite_SetMaxAge},
	{"DiscordInvite.TargetType.get", invite_GetTargetType},
	{"DiscordInvite.TargetType.set", invite_SetTargetType},
	{"DiscordInvite.IsTemporary.get", invite_IsTemporary},
	{"DiscordInvite.IsTemporary.set", invite_SetTemporary},
	{"DiscordInvite.IsUnique.get", invite_IsUnique},
	{"DiscordInvite.IsUnique.set", invite_SetUnique},
	{"DiscordInvite.CreatedAt.get", invite_GetCreatedAt},
	{"DiscordInvite.ExpiresAt.get", invite_GetExpiresAt},
	{"DiscordInvite.SetTargetUserId", invite_SetTargetUserId},
	{"DiscordInvite.Create", invite_Create},
	{"DiscordInvite.Delete", invite_Delete},
	{"DiscordInvite.Get", invite_Get},
	{nullptr, nullptr}
};
