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

#include "natives/entity_natives_common.h"
#include "entities/discord_emoji.h"

static cell_t emoji_GetRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	pContext->StringToLocal(params[3], params[4], emoji->GetRoleId(params[2]).c_str());
	return 1;
}

static cell_t emoji_GetRoles(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& roles = emoji->GetRoles();
	for (const auto& role_id : roles) {
		array->AddString(role_id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
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

static cell_t emoji_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	char* guild_id_str;
	pContext->LocalToString(params[2], &guild_id_str);

	dpp::snowflake guild_id;
	if (!ParseSnowflake(pContext, guild_id_str, guild_id)) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	emoji->Delete(guild_id, callback, data);
	return 1;
}

static cell_t emoji_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmoji* emoji = Handles.GetPointer<DiscordEmoji>(pContext, params[1]);
	if (!emoji) return 0;

	char* guild_id_str;
	pContext->LocalToString(params[2], &guild_id_str);

	dpp::snowflake guild_id;
	if (!ParseSnowflake(pContext, guild_id_str, guild_id)) return 0;

	char* name;
	pContext->LocalToString(params[3], &name);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	emoji->Edit(guild_id, name, callback, data);
	return 1;
}

extern const sp_nativeinfo_t emoji_natives[] = {
	{"DiscordEmoji.GetId", EntityGetId<DiscordEmoji>},
	{"DiscordEmoji.GetName", EntityGetName<DiscordEmoji>},
	{"DiscordEmoji.GetUserId", EntityGetUserId<DiscordEmoji>},
	{"DiscordEmoji.GetGuildId", EntityGetGuildId<DiscordEmoji>},
	{"DiscordEmoji.RequiresColons.get", EntityGetBool<DiscordEmoji, &DiscordEmoji::RequiresColons>},
	{"DiscordEmoji.Managed.get", EntityGetBool<DiscordEmoji, &DiscordEmoji::IsManaged>},
	{"DiscordEmoji.Animated.get", EntityGetBool<DiscordEmoji, &DiscordEmoji::IsAnimated>},
	{"DiscordEmoji.Available.get", EntityGetBool<DiscordEmoji, &DiscordEmoji::IsAvailable>},
	{"DiscordEmoji.RoleCount.get", EntityGetInt<DiscordEmoji, size_t, &DiscordEmoji::GetRoleCount>},
	{"DiscordEmoji.GetRoleId", emoji_GetRoleId},
	{"DiscordEmoji.GetRoles", emoji_GetRoles},
	{"DiscordEmoji.GetMention", EntityGetMention<DiscordEmoji>},
	{"DiscordEmoji.GetUrl", emoji_GetUrl},
	{"DiscordEmoji.GetFormat", emoji_GetFormat},
	{"DiscordEmoji.Delete", emoji_Delete},
	{"DiscordEmoji.Edit", emoji_Edit},
	{nullptr, nullptr}
};
