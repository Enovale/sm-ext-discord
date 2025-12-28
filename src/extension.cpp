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

#include "extension.h"
#include "core/handle_manager.h"
#include "core/natives_manager.h"
#include "core/task_manager.h"

DiscordExtension g_DiscordExt;
SMEXT_LINK(&g_DiscordExt);

static void OnGameFrame(bool simulating) {
	Tasks.ProcessFrame();
}

bool DiscordExtension::SDK_OnLoad(char* error, size_t maxlen, bool late) {
	if (!Handles.Initialize(error, maxlen)) {
		return false;
	}

	Natives.Initialize();
  sharesys->RegisterLibrary(myself, "discord");

	smutils->AddGameFrameHook(&OnGameFrame);
	return true;
}

void DiscordExtension::SDK_OnUnload() {
	smutils->RemoveGameFrameHook(&OnGameFrame);
	Tasks.Clear();
	Handles.Shutdown();
}
