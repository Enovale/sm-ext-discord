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

#include "core/natives_manager.h"

// Forward declarations for all natives arrays
extern const sp_nativeinfo_t discord_core_natives[];
extern const sp_nativeinfo_t user_natives[];
extern const sp_nativeinfo_t message_natives[];
extern const sp_nativeinfo_t channel_natives[];
extern const sp_nativeinfo_t guild_natives[];
extern const sp_nativeinfo_t role_natives[];
extern const sp_nativeinfo_t webhook_natives[];
extern const sp_nativeinfo_t interaction_natives[];
extern const sp_nativeinfo_t invite_natives[];
extern const sp_nativeinfo_t slashcommand_natives[];
extern const sp_nativeinfo_t embed_natives[];
extern const sp_nativeinfo_t http_natives[];
extern const sp_nativeinfo_t forum_natives[];
extern const sp_nativeinfo_t component_natives[];
extern const sp_nativeinfo_t button_interaction_natives[];
extern const sp_nativeinfo_t select_interaction_natives[];
extern const sp_nativeinfo_t modal_interaction_natives[];
extern const sp_nativeinfo_t modal_natives[];
extern const sp_nativeinfo_t poll_natives[];
extern const sp_nativeinfo_t discord_event_natives[];
extern const sp_nativeinfo_t discord_result_natives[];
extern const sp_nativeinfo_t discord_handle_array_natives[];
extern const sp_nativeinfo_t guild_member_natives[];
extern const sp_nativeinfo_t emoji_natives[];
extern const sp_nativeinfo_t sticker_natives[];
extern const sp_nativeinfo_t scheduled_event_natives[];
extern const sp_nativeinfo_t attachment_natives[];
extern const sp_nativeinfo_t reaction_natives[];
extern const sp_nativeinfo_t ban_natives[];
extern const sp_nativeinfo_t thread_member_natives[];

static const sp_nativeinfo_t* s_all_natives[] = {
	discord_core_natives,
	user_natives,
	message_natives,
	channel_natives,
	guild_natives,
	role_natives,
	webhook_natives,
	interaction_natives,
	invite_natives,
	slashcommand_natives,
	embed_natives,
	http_natives,
	forum_natives,
	component_natives,
	button_interaction_natives,
	select_interaction_natives,
	modal_interaction_natives,
	modal_natives,
	poll_natives,
	discord_event_natives,
	discord_result_natives,
	discord_handle_array_natives,
	guild_member_natives,
	emoji_natives,
	sticker_natives,
	scheduled_event_natives,
	attachment_natives,
	reaction_natives,
	ban_natives,
	thread_member_natives
};

NativesManager& NativesManager::Instance() {
	static NativesManager instance;
	return instance;
}

void NativesManager::Initialize() {
	for (auto natives : s_all_natives) {
		sharesys->AddNatives(myself, natives);
	}
}
