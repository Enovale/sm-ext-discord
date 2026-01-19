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

#include "natives/entity_natives_common.h"
#include "entities/discord_sticker.h"

static cell_t sticker_GetPackId(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	pContext->StringToLocal(params[2], params[3], sticker->GetPackId().c_str());
	return 1;
}

static cell_t sticker_GetTags(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	pContext->StringToLocal(params[2], params[3], sticker->GetTags());
	return 1;
}

static cell_t sticker_GetUserName(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	pContext->StringToLocal(params[2], params[3], sticker->GetUserName());
	return 1;
}

static cell_t sticker_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	sticker->Delete(callback, data);
	return 1;
}

static cell_t sticker_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	char *name, *description, *tags;
	pContext->LocalToString(params[2], &name);
	pContext->LocalToString(params[3], &description);
	pContext->LocalToString(params[4], &tags);

	IPluginFunction* callback = pContext->GetFunctionById(params[5]);

	cell_t data = params[6];
	sticker->Edit(name, description, tags, callback, data);
	return 1;
}

extern const sp_nativeinfo_t sticker_natives[] = {
	{"DiscordSticker.GetId", EntityGetId<DiscordSticker>},
	{"DiscordSticker.GetPackId", sticker_GetPackId},
	{"DiscordSticker.GetName", EntityGetName<DiscordSticker>},
	{"DiscordSticker.GetDescription", EntityGetDescription<DiscordSticker>},
	{"DiscordSticker.GetTags", sticker_GetTags},
	{"DiscordSticker.GetGuildId", EntityGetGuildId<DiscordSticker>},
	{"DiscordSticker.Type.get", EntityGetInt<DiscordSticker, uint8_t, &DiscordSticker::GetType>},
	{"DiscordSticker.FormatType.get", EntityGetInt<DiscordSticker, uint8_t, &DiscordSticker::GetFormatType>},
	{"DiscordSticker.Available.get", EntityGetBool<DiscordSticker, &DiscordSticker::IsAvailable>},
	{"DiscordSticker.SortValue.get", EntityGetInt<DiscordSticker, uint8_t, &DiscordSticker::GetSortValue>},
	{"DiscordSticker.GetUserId", EntityGetUserId<DiscordSticker>},
	{"DiscordSticker.GetUserName", sticker_GetUserName},
	{"DiscordSticker.GetUrl", EntityGetUrl<DiscordSticker>},
	{"DiscordSticker.Delete", sticker_Delete},
	{"DiscordSticker.Edit", sticker_Edit},
	{nullptr, nullptr}
};
