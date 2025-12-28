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
#include "entities/discord_thread_member.h"

static cell_t thread_member_GetThreadId(IPluginContext* pContext, const cell_t* params)
{
	DiscordThreadMember* member = Handles.GetPointer<DiscordThreadMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetThreadId().c_str());
	return 1;
}

static cell_t thread_member_GetUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordThreadMember* member = Handles.GetPointer<DiscordThreadMember>(pContext, params[1]);
	if (!member) return 0;

	pContext->StringToLocal(params[2], params[3], member->GetUserId().c_str());
	return 1;
}

static cell_t thread_member_GetJoinedTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordThreadMember* member = Handles.GetPointer<DiscordThreadMember>(pContext, params[1]);
	if (!member) return 0;

	return member->GetJoinedTimestamp();
}

extern const sp_nativeinfo_t thread_member_natives[] = {
	{"DiscordThreadMember.GetThreadId", thread_member_GetThreadId},
	{"DiscordThreadMember.GetUserId", thread_member_GetUserId},
	{"DiscordThreadMember.JoinedTimestamp.get", thread_member_GetJoinedTimestamp},
	{"DiscordThreadMember.Flags.get", EntityGetFlags<DiscordThreadMember>},
	{nullptr, nullptr}
};
