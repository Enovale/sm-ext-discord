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
#include "entities/discord_ban.h"

static cell_t ban_GetReason(IPluginContext* pContext, const cell_t* params)
{
	DiscordBan* ban = Handles.GetPointer<DiscordBan>(pContext, params[1]);
	if (!ban) return 0;

	pContext->StringToLocal(params[2], params[3], ban->GetReason());
	return 1;
}

extern const sp_nativeinfo_t ban_natives[] = {
	{"DiscordBan.GetUserId", EntityGetUserId<DiscordBan>},
	{"DiscordBan.GetReason", ban_GetReason},
	{nullptr, nullptr}
};
