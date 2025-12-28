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

#include "core/handle_manager.h"
#include "utils/discord_modal.h"
#include "utils/discord_component.h"

static cell_t discord_modal_Create(IPluginContext* pContext, const cell_t* params) {
	char* custom_id;
	pContext->LocalToString(params[1], &custom_id);

	char* title;
	pContext->LocalToString(params[2], &title);

	DiscordModal* modal = new DiscordModal(custom_id, title);

	return Handles.Create(pContext, modal, HandleId::DiscordModal);
}

static cell_t discord_modal_AddRow(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	DiscordComponent* row = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!row) return 0;

	modal->AddRow(row);
	return 1;
}

static cell_t discord_modal_GetCustomId(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	pContext->StringToLocal(params[2], params[3], modal->GetCustomId());
	return 1;
}

static cell_t discord_modal_GetTitle(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	pContext->StringToLocal(params[2], params[3], modal->GetTitle());
	return 1;
}

static cell_t discord_modal_GetRowCount(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	return modal->GetRowCount();
}

static cell_t discord_modal_AddTextInput(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	char *custom_id, *label, *placeholder = nullptr, *default_value = nullptr;
	pContext->LocalToString(params[2], &custom_id);
	pContext->LocalToString(params[3], &label);
	int style = params[4];
	bool required = params[5] != 0;
	pContext->LocalToString(params[6], &placeholder);
	int min_length = params[7];
	int max_length = params[8];
	pContext->LocalToString(params[9], &default_value);

	return modal->AddTextInput(custom_id, label, style, required,
							   placeholder, min_length, max_length, default_value);
}

static cell_t discord_modal_IsFull(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	return modal->IsFull();
}

static cell_t discord_modal_Clear(IPluginContext* pContext, const cell_t* params) {
	DiscordModal* modal = Handles.GetPointer<DiscordModal>(pContext, params[1]);
	if (!modal) return 0;

	modal->Clear();
	return 1;
}

extern const sp_nativeinfo_t modal_natives[] = {
	{"DiscordModal.DiscordModal", discord_modal_Create},
	{"DiscordModal.AddRow", discord_modal_AddRow},
	{"DiscordModal.AddTextInput", discord_modal_AddTextInput},
	{"DiscordModal.Clear", discord_modal_Clear},
	{"DiscordModal.GetCustomId", discord_modal_GetCustomId},
	{"DiscordModal.GetTitle", discord_modal_GetTitle},
	{"DiscordModal.IsFull.get", discord_modal_IsFull},
	{"DiscordModal.RowCount.get", discord_modal_GetRowCount},
	{nullptr, nullptr}
};
