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
#include "core/discord_event.h"

static cell_t discord_event_GetInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return event->GetInt(key, params[3]);
}

static cell_t discord_event_GetFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return sp_ftoc(event->GetFloat(key, sp_ctof(params[3])));
}

static cell_t discord_event_GetBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return event->GetBool(key, params[3] != 0);
}

static cell_t discord_event_GetString(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	const char* value = event->GetString(key);
	pContext->StringToLocal(params[3], params[4], value);
	return 1;
}

static cell_t discord_event_GetHandle(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return BAD_HANDLE;

	char* key;
	pContext->LocalToString(params[2], &key);

	return event->GetHandle(key);
}

static cell_t discord_event_HasKey(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return event->HasKey(key);
}

static cell_t discord_event_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	pContext->StringToLocal(params[2], params[3], event->GetName().c_str());
	return 1;
}

static cell_t discord_event_GetAutoFreeHandles(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	return event->GetAutoFreeHandles();
}

static cell_t discord_event_SetAutoFreeHandles(IPluginContext* pContext, const cell_t* params)
{
	DiscordEvent* event = Handles.GetPointer<DiscordEvent>(pContext, params[1]);
	if (!event) return 0;

	event->SetAutoFreeHandles(params[2] != 0);
	return 0;
}

extern const sp_nativeinfo_t discord_event_natives[] = {
	{"DiscordEvent.GetInt", discord_event_GetInt},
	{"DiscordEvent.GetFloat", discord_event_GetFloat},
	{"DiscordEvent.GetBool", discord_event_GetBool},
	{"DiscordEvent.GetString", discord_event_GetString},
	{"DiscordEvent.GetHandle", discord_event_GetHandle},
	{"DiscordEvent.HasKey", discord_event_HasKey},
	{"DiscordEvent.GetName", discord_event_GetName},
	{"DiscordEvent.AutoFreeHandles.get", discord_event_GetAutoFreeHandles},
	{"DiscordEvent.AutoFreeHandles.set", discord_event_SetAutoFreeHandles},
	{nullptr, nullptr}
};
