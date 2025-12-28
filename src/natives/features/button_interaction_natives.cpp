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

extern const sp_nativeinfo_t button_interaction_natives[] = {
	{"DiscordButtonInteraction.GetCustomId", ComponentGetCustomId<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.Type.get", ComponentGetType<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.User.get", InteractionGetUser<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.Channel.get", InteractionGetChannel<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.Message.get", ComponentGetMessage<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.Reply", InteractionReply<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.ReplyEmbed", InteractionReplyEmbed<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.ReplyFromObject", InteractionReplyMessage<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.DeferReply", InteractionDeferReply<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.UpdateMessage", ComponentUpdateMessage<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.UpdateMessageEmbed", ComponentUpdateMessageEmbed<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.UpdateMessageFromObject", ComponentUpdateDiscordMessage<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.DeferUpdate", ComponentDeferUpdate<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.ShowModal", ComponentShowModal<DiscordButtonInteraction>},
	{"DiscordButtonInteraction.ShowModalFromObject", ComponentShowModalObject<DiscordButtonInteraction>},
	{nullptr, nullptr}
};
