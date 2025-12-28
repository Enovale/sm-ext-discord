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
#include "entities/discord_reaction.h"

static cell_t reaction_GetCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetCount());
}

static cell_t reaction_GetCountBurst(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetCountBurst());
}

static cell_t reaction_GetCountNormal(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetCountNormal());
}

static cell_t reaction_GetEmojiId(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	pContext->StringToLocal(params[2], params[3], react->GetEmojiId().c_str());
	return 1;
}

static cell_t reaction_GetEmojiName(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	pContext->StringToLocal(params[2], params[3], react->GetEmojiName());
	return 1;
}

static cell_t reaction_GetMe(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return react->GetMe();
}

static cell_t reaction_GetMeBurst(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return react->GetMeBurst();
}

static cell_t reaction_IsCustomEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return react->IsCustomEmoji();
}

static cell_t reaction_GetEmojiMention(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	pContext->StringToLocal(params[2], params[3], react->GetEmojiMention().c_str());
	return 1;
}

static cell_t reaction_GetBurstColorCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetBurstColorCount());
}

static cell_t reaction_GetBurstColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetBurstColor(params[2]));
}

extern const sp_nativeinfo_t reaction_natives[] = {
	{"DiscordReaction.Count.get", reaction_GetCount},
	{"DiscordReaction.CountBurst.get", reaction_GetCountBurst},
	{"DiscordReaction.CountNormal.get", reaction_GetCountNormal},
	{"DiscordReaction.GetEmojiId", reaction_GetEmojiId},
	{"DiscordReaction.GetEmojiName", reaction_GetEmojiName},
	{"DiscordReaction.Me.get", reaction_GetMe},
	{"DiscordReaction.MeBurst.get", reaction_GetMeBurst},
	{"DiscordReaction.IsCustomEmoji.get", reaction_IsCustomEmoji},
	{"DiscordReaction.GetEmojiMention", reaction_GetEmojiMention},
	{"DiscordReaction.BurstColorCount.get", reaction_GetBurstColorCount},
	{"DiscordReaction.GetBurstColor", reaction_GetBurstColor},
	{nullptr, nullptr}
};
