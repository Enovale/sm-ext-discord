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

#pragma once

#include "natives/natives_common.h"
#include "entities/discord_message.h"
#include "utils/discord_embed.h"
#include "utils/discord_component.h"
#include "utils/discord_modal.h"
#include "features/discord_button_interaction.h"
#include "features/discord_select_interaction.h"
#include "features/discord_modal_interaction.h"

// Common interaction native implementations using templates
// These work with any interaction type that inherits from DiscordInteractionBase

// Reply with text content
template<typename T>
static cell_t InteractionReply(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	bool ephemeral = params[3] != 0;

	interaction->Reply(content, ephemeral);
	return 1;
}

// Reply with embed
template<typename T>
static cell_t InteractionReplyEmbed(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	bool ephemeral = params[4] != 0;

	interaction->ReplyEmbed(content, embed, ephemeral);
	return 1;
}

// Reply with message object
template<typename T>
static cell_t InteractionReplyMessage(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	bool ephemeral = params[3] != 0;

	interaction->ReplyMessage(message, ephemeral);
	return 1;
}

// Defer reply
template<typename T>
static cell_t InteractionDeferReply(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	bool ephemeral = params[2] != 0;
	interaction->DeferReply(ephemeral);
	return 1;
}

// Get user handle
template<typename T>
static cell_t InteractionGetUser(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	return interaction->GetUserHandle();
}

// Get channel handle
template<typename T>
static cell_t InteractionGetChannel(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	return interaction->GetChannelHandle();
}

// Component interaction specific - Update message with text
template<typename T>
static cell_t ComponentUpdateMessage(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	interaction->UpdateMessage(content);
	return 1;
}

// Component interaction specific - Defer update
template<typename T>
static cell_t ComponentDeferUpdate(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	interaction->DeferUpdate();
	return 1;
}

// Component interaction specific - Update message with embed
template<typename T>
static cell_t ComponentUpdateMessageEmbed(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	interaction->UpdateMessageEmbed(content, embed);
	return 1;
}

// Component interaction specific - Update with message object
template<typename T>
static cell_t ComponentUpdateDiscordMessage(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!message) return 0;

	interaction->UpdateDiscordMessage(message);
	return 1;
}

// Component interaction specific - Get message handle
template<typename T>
static cell_t ComponentGetMessage(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	return interaction->GetMessageHandle();
}

// Component interaction specific - Get custom ID
template<typename T>
static cell_t ComponentGetCustomId(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	pContext->StringToLocal(params[2], params[3], interaction->GetCustomId());
	return 1;
}

// Component interaction specific - Get component type
template<typename T>
static cell_t ComponentGetType(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	return interaction->GetComponentType();
}

// Component interaction specific - Show modal
template<typename T>
static cell_t ComponentShowModal(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	char* custom_id;
	pContext->LocalToString(params[2], &custom_id);

	char* title;
	pContext->LocalToString(params[3], &title);

	int component_count;
	if (!GetNativeIntInRange(pContext, params[5], 0, 5, "Modal component count", component_count)) return 0;
	cell_t* components_addr = nullptr;
	if (component_count > 0 && !GetNativeArray(pContext, params[4], &components_addr, "modal component")) return 0;

	std::vector<dpp::component> components;
	for (int i = 0; i < component_count; i++) {
		DiscordComponent* comp = Handles.GetPointer<DiscordComponent>(pContext, components_addr[i]);
		if (comp) {
			components.push_back(comp->GetComponent());
		}
	}

	interaction->ShowModal(custom_id, title, components);
	return 1;
}

// Component interaction specific - Show modal from object
template<typename T>
static cell_t ComponentShowModalObject(IPluginContext* pContext, const cell_t* params) {
	T* interaction = Handles.GetPointer<T>(pContext, params[1]);
	if (!interaction) return 0;

	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[2]);
	if (!modal) return 0;

	interaction->ShowModal(modal->GetCustomId(), modal->GetTitle(), modal->GetRows());
	return 1;
}
