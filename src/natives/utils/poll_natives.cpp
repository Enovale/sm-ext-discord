/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2026 ProjectSky
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

#include "core/handle_manager.h"
#include "utils/discord_poll.h"
#include <cstdio>

static cell_t poll_Create(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = new DiscordPoll();
	Handle_t handle = Handles.Create(pContext, poll, HandleId::DiscordPoll);
	if (!handle) return 0;
	return handle;
}

static cell_t poll_SetQuestion(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	char* text;
	pContext->LocalToString(params[2], &text);
	poll->SetQuestion(text);
	return 1;
}

static cell_t poll_SetDuration(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	poll->SetDuration(params[2]);
	return 1;
}

static cell_t poll_SetMultiSelect(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	poll->SetMultiSelect(params[2] != 0);
	return 1;
}

static cell_t poll_AddAnswer(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	char* text;
	char* emoji;
	pContext->LocalToString(params[2], &text);
	pContext->LocalToString(params[3], &emoji);

	if (emoji && emoji[0] != '\0') {
		poll->AddAnswerWithEmoji(text, emoji);
	} else {
		poll->AddAnswer(text);
	}
	return 1;
}

static cell_t poll_GetQuestion(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	pContext->StringToLocal(params[2], params[3], poll->GetQuestion());
	return 1;
}

static cell_t poll_GetExpiry(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	if (params[3] <= 0) {
		pContext->ReportError("Poll expiry buffer size must be positive");
		return 0;
	}

	char buffer[64];
	std::snprintf(buffer, sizeof(buffer), "%.0f", poll->GetExpiry());
	pContext->StringToLocal(params[2], params[3], buffer);
	return 1;
}

static cell_t poll_IsMultiSelect(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->IsMultiSelect();
}

static cell_t poll_IsFinalized(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->IsFinalized();
}

static cell_t poll_GetAnswerCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return static_cast<cell_t>(poll->GetAnswerCount());
}

static cell_t poll_GetAnswerId(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->GetAnswerId(params[2]);
}

static cell_t poll_GetAnswerText(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	pContext->StringToLocal(params[3], params[4], poll->GetAnswerText(params[2]));
	return 1;
}

static cell_t poll_HasResults(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->HasResults();
}

static cell_t poll_GetVoteCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->GetVoteCount(params[2]);
}

static cell_t poll_DidVote(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->DidVote(params[2]);
}

static cell_t poll_GetTotalVoteCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[1]);
	if (!poll) return 0;

	return poll->GetTotalVoteCount();
}

extern const sp_nativeinfo_t poll_natives[] = {
	{"DiscordPoll.DiscordPoll", poll_Create},
	{"DiscordPoll.SetQuestion", poll_SetQuestion},
	{"DiscordPoll.Duration.set", poll_SetDuration},
	{"DiscordPoll.MultiSelect.set", poll_SetMultiSelect},
	{"DiscordPoll.AddAnswer", poll_AddAnswer},
	{"DiscordPoll.GetQuestion", poll_GetQuestion},
	{"DiscordPoll.GetExpiry", poll_GetExpiry},
	{"DiscordPoll.MultiSelect.get", poll_IsMultiSelect},
	{"DiscordPoll.Finalized.get", poll_IsFinalized},
	{"DiscordPoll.AnswerCount.get", poll_GetAnswerCount},
	{"DiscordPoll.GetAnswerId", poll_GetAnswerId},
	{"DiscordPoll.GetAnswerText", poll_GetAnswerText},
	{"DiscordPoll.HasResults.get", poll_HasResults},
	{"DiscordPoll.GetVoteCount", poll_GetVoteCount},
	{"DiscordPoll.TotalVoteCount.get", poll_GetTotalVoteCount},
	{"DiscordPoll.DidVote", poll_DidVote},
	{nullptr, nullptr}
};
