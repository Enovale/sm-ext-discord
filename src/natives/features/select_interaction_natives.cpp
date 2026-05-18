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

#include "natives/interaction_natives_common.h"

static cell_t select_interaction_GetValues(IPluginContext* pContext, const cell_t* params)
{
	DiscordSelectInteraction* interaction = Handles.GetPointer<DiscordSelectInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	const std::vector<std::string>& values = interaction->GetValues();
	return values.size();
}

static cell_t select_interaction_GetValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordSelectInteraction* interaction = Handles.GetPointer<DiscordSelectInteraction>(pContext, params[1]);
	if (!interaction) return 0;

	const std::vector<std::string>& values = interaction->GetValues();
	int index = params[2];

	if (index < 0 || index >= (int)values.size()) {
		pContext->ReportError("Invalid value index %d (count: %d)", index, (int)values.size());
		return 0;
	}

	pContext->StringToLocal(params[3], params[4], values[index].c_str());
	return 1;
}

extern const sp_nativeinfo_t select_interaction_natives[] = {
	{"DiscordSelectInteraction.GetCustomId", ComponentGetCustomId<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.Type.get", ComponentGetType<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.ValueCount.get", select_interaction_GetValues},
	{"DiscordSelectInteraction.User.get", InteractionGetUser<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.Channel.get", InteractionGetChannel<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.Message.get", ComponentGetMessage<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.GetValue", select_interaction_GetValue},
	{"DiscordSelectInteraction.Reply", InteractionReply<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.ReplyEmbed", InteractionReplyEmbed<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.ReplyFromObject", InteractionReplyMessage<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.DeferReply", InteractionDeferReply<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.UpdateMessage", ComponentUpdateMessage<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.UpdateMessageEmbed", ComponentUpdateMessageEmbed<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.UpdateMessageFromObject", ComponentUpdateDiscordMessage<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.DeferUpdate", ComponentDeferUpdate<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.ShowModal", ComponentShowModal<DiscordSelectInteraction>},
	{"DiscordSelectInteraction.ShowModalFromObject", ComponentShowModalObject<DiscordSelectInteraction>},
	{nullptr, nullptr}
};
