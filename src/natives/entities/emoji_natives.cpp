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
#include "entities/discord_emoji.h"

static cell_t emoji_RequiresColons(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	return emoji->RequiresColons();
}

static cell_t emoji_IsManaged(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	return emoji->IsManaged();
}

static cell_t emoji_IsAnimated(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	return emoji->IsAnimated();
}

static cell_t emoji_IsAvailable(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	return emoji->IsAvailable();
}

static cell_t emoji_GetRoleCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	return static_cast<cell_t>(emoji->GetRoleCount());
}

static cell_t emoji_GetRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	pContext->StringToLocal(params[3], params[4], emoji->GetRoleId(params[2]).c_str());
	return 1;
}

static cell_t emoji_GetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	pContext->StringToLocal(params[2], params[3], emoji->GetUrl(params[4]).c_str());
	return 1;
}

static cell_t emoji_GetFormat(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	pContext->StringToLocal(params[2], params[3], emoji->GetFormat().c_str());
	return 1;
}

static cell_t emoji_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	emoji->Edit(name);
	return 1;
}

extern const sp_nativeinfo_t emoji_natives[] = {
	{"DiscordEmoji.GetId", EntityGetId<DiscordEmoji>},
	{"DiscordEmoji.GetName", EntityGetName<DiscordEmoji>},
	{"DiscordEmoji.GetUserId", EntityGetUserId<DiscordEmoji>},
	{"DiscordEmoji.GetGuildId", EntityGetGuildId<DiscordEmoji>},
	{"DiscordEmoji.RequiresColons.get", emoji_RequiresColons},
	{"DiscordEmoji.Managed.get", emoji_IsManaged},
	{"DiscordEmoji.Animated.get", emoji_IsAnimated},
	{"DiscordEmoji.Available.get", emoji_IsAvailable},
	{"DiscordEmoji.RoleCount.get", emoji_GetRoleCount},
	{"DiscordEmoji.GetRoleId", emoji_GetRoleId},
	{"DiscordEmoji.GetMention", EntityGetMention<DiscordEmoji>},
	{"DiscordEmoji.GetUrl", emoji_GetUrl},
	{"DiscordEmoji.GetFormat", emoji_GetFormat},
	{"DiscordEmoji.Delete", EntityDelete<DiscordEmoji>},
	{"DiscordEmoji.Edit", emoji_Edit},
	{nullptr, nullptr}
};
