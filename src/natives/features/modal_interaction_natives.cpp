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

#include "natives/interaction_natives_common.h"

static cell_t modal_interaction_GetCustomId(IPluginContext* pContext, const cell_t* params)
{
	DiscordModalInteraction* interaction = Handles.GetPointer<DiscordModalInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetCustomId());
	return 1;
}

static cell_t modal_interaction_GetValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordModalInteraction* interaction = Handles.GetPointer<DiscordModalInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	char* custom_id;
	pContext->LocalToString(params[2], &custom_id);

	std::string value = interaction->GetValue(custom_id);
	pContext->StringToLocal(params[3], params[4], value.c_str());
	return 1;
}

extern const sp_nativeinfo_t modal_interaction_natives[] = {
	{"DiscordModalInteraction.GetCustomId", modal_interaction_GetCustomId},
	{"DiscordModalInteraction.GetValue", modal_interaction_GetValue},
	{"DiscordModalInteraction.User.get", InteractionGetUser<DiscordModalInteraction>},
	{"DiscordModalInteraction.Channel.get", InteractionGetChannel<DiscordModalInteraction>},
	{"DiscordModalInteraction.Reply", InteractionReply<DiscordModalInteraction>},
	{"DiscordModalInteraction.ReplyEmbed", InteractionReplyEmbed<DiscordModalInteraction>},
	{"DiscordModalInteraction.ReplyFromObject", InteractionReplyMessage<DiscordModalInteraction>},
	{"DiscordModalInteraction.DeferReply", InteractionDeferReply<DiscordModalInteraction>},
	{nullptr, nullptr}
};
