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
#include "entities/discord_reaction.h"

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

static cell_t reaction_GetEmojiMention(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	pContext->StringToLocal(params[2], params[3], react->GetEmojiMention().c_str());
	return 1;
}

static cell_t reaction_GetBurstColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordReaction* react = Handles.GetPointer<DiscordReaction>(pContext, params[1]);
	if (!react) return 0;

	return static_cast<cell_t>(react->GetBurstColor(params[2]));
}

extern const sp_nativeinfo_t reaction_natives[] = {
	{"DiscordReaction.Count.get", EntityGetInt<DiscordReaction, uint32_t, &DiscordReaction::GetCount>},
	{"DiscordReaction.CountBurst.get", EntityGetInt<DiscordReaction, uint32_t, &DiscordReaction::GetCountBurst>},
	{"DiscordReaction.CountNormal.get", EntityGetInt<DiscordReaction, uint32_t, &DiscordReaction::GetCountNormal>},
	{"DiscordReaction.GetEmojiId", reaction_GetEmojiId},
	{"DiscordReaction.GetEmojiName", reaction_GetEmojiName},
	{"DiscordReaction.Me.get", EntityGetBool<DiscordReaction, &DiscordReaction::GetMe>},
	{"DiscordReaction.MeBurst.get", EntityGetBool<DiscordReaction, &DiscordReaction::GetMeBurst>},
	{"DiscordReaction.IsCustomEmoji.get", EntityGetBool<DiscordReaction, &DiscordReaction::IsCustomEmoji>},
	{"DiscordReaction.GetEmojiMention", reaction_GetEmojiMention},
	{"DiscordReaction.BurstColorCount.get", EntityGetInt<DiscordReaction, size_t, &DiscordReaction::GetBurstColorCount>},
	{"DiscordReaction.GetBurstColor", reaction_GetBurstColor},
	{nullptr, nullptr}
};
