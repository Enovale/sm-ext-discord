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
#include "core/discord_handle_array.h"

static cell_t handle_array_GetLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordHandleArray* arr = Handles.GetPointer<DiscordHandleArray>(pContext, params[1]);
	if (!arr) return 0;

	return static_cast<cell_t>(arr->GetLength());
}

static cell_t handle_array_GetAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordHandleArray* arr = Handles.GetPointer<DiscordHandleArray>(pContext, params[1]);
	if (!arr) return 0;

	return static_cast<cell_t>(arr->GetAt(static_cast<size_t>(params[2])));
}

static cell_t handle_array_GetStringAt(IPluginContext* pContext, const cell_t* params)
{
	DiscordHandleArray* arr = Handles.GetPointer<DiscordHandleArray>(pContext, params[1]);
	if (!arr) return 0;

	const std::string& str = arr->GetStringAt(static_cast<size_t>(params[2]));
	pContext->StringToLocal(params[3], params[4], str.c_str());
	return 1;
}

extern const sp_nativeinfo_t discord_handle_array_natives[] = {
	{"DiscordHandleArray.Length.get", handle_array_GetLength},
	{"DiscordHandleArray.GetAt", handle_array_GetAt},
	{"DiscordHandleArray.GetStringAt", handle_array_GetStringAt},
	{nullptr, nullptr}
};
