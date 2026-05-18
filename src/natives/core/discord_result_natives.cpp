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

#include "core/handle_manager.h"
#include "core/discord_result.h"

static cell_t discord_result_IsSuccess(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	return result->IsSuccess();
}

static cell_t discord_result_GetError(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	pContext->StringToLocal(params[2], params[3], result->GetError());
	return 1;
}

static cell_t discord_result_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	return result->GetTypeInt();
}

static cell_t discord_result_GetInt(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return result->GetInt(key, params[3]);
}

static cell_t discord_result_GetFloat(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return sp_ftoc(result->GetFloat(key, sp_ctof(params[3])));
}

static cell_t discord_result_GetBool(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return result->GetBool(key, params[3] != 0);
}

static cell_t discord_result_GetString(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	const char* value = result->GetString(key);
	pContext->StringToLocal(params[3], params[4], value);
	return 1;
}

static cell_t discord_result_GetHandle(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return BAD_HANDLE;

	char* key;
	pContext->LocalToString(params[2], &key);

	return result->GetHandle(key);
}

static cell_t discord_result_HasKey(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	char* key;
	pContext->LocalToString(params[2], &key);

	return result->HasKey(key);
}

static cell_t discord_result_GetAutoFreeHandles(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	return result->GetAutoFreeHandles();
}

static cell_t discord_result_SetAutoFreeHandles(IPluginContext* pContext, const cell_t* params)
{
	DiscordResult* result = Handles.GetPointer<DiscordResult>(pContext, params[1]);
	if (!result) return 0;

	result->SetAutoFreeHandles(params[2] != 0);
	return 0;
}

extern const sp_nativeinfo_t discord_result_natives[] = {
	{"DiscordResult.IsSuccess.get", discord_result_IsSuccess},
	{"DiscordResult.GetError", discord_result_GetError},
	{"DiscordResult.Type.get", discord_result_GetType},
	{"DiscordResult.GetInt", discord_result_GetInt},
	{"DiscordResult.GetFloat", discord_result_GetFloat},
	{"DiscordResult.GetBool", discord_result_GetBool},
	{"DiscordResult.GetString", discord_result_GetString},
	{"DiscordResult.GetHandle", discord_result_GetHandle},
	{"DiscordResult.HasKey", discord_result_HasKey},
	{"DiscordResult.AutoFreeHandles.get", discord_result_GetAutoFreeHandles},
	{"DiscordResult.AutoFreeHandles.set", discord_result_SetAutoFreeHandles},
	{nullptr, nullptr}
};
