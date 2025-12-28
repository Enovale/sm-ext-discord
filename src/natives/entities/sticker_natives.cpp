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

static cell_t sticker_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	return static_cast<cell_t>(sticker->GetType());
}

static cell_t sticker_GetFormatType(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	return static_cast<cell_t>(sticker->GetFormatType());
}

static cell_t sticker_IsAvailable(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	return sticker->IsAvailable();
}

static cell_t sticker_GetSortValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	return static_cast<cell_t>(sticker->GetSortValue());
}

static cell_t sticker_GetUsername(IPluginContext* pContext, const cell_t* params)
{
	DiscordSticker* sticker = Handles.GetPointer<DiscordSticker>(pContext, params[1]);
	if (!sticker) return 0;

	pContext->StringToLocal(params[2], params[3], sticker->GetUsername());
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

	sticker->Edit(name, description, tags);
	return 1;
}

extern const sp_nativeinfo_t sticker_natives[] = {
	{"DiscordSticker.GetId", EntityGetId<DiscordSticker>},
	{"DiscordSticker.GetPackId", sticker_GetPackId},
	{"DiscordSticker.GetName", EntityGetName<DiscordSticker>},
	{"DiscordSticker.GetDescription", EntityGetDescription<DiscordSticker>},
	{"DiscordSticker.GetTags", sticker_GetTags},
	{"DiscordSticker.GetGuildId", EntityGetGuildId<DiscordSticker>},
	{"DiscordSticker.Type.get", sticker_GetType},
	{"DiscordSticker.FormatType.get", sticker_GetFormatType},
	{"DiscordSticker.Available.get", sticker_IsAvailable},
	{"DiscordSticker.SortValue.get", sticker_GetSortValue},
	{"DiscordSticker.GetUserId", EntityGetUserId<DiscordSticker>},
	{"DiscordSticker.GetUsername", sticker_GetUsername},
	{"DiscordSticker.GetUrl", EntityGetUrl<DiscordSticker>},
	{"DiscordSticker.Delete", EntityDelete<DiscordSticker>},
	{"DiscordSticker.Edit", sticker_Edit},
	{nullptr, nullptr}
};
