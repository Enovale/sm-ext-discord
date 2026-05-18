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

#include "core/operations/user_operations.h"
#include "utils/discord_common.h"

void UserOperations::Get(dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->user_get(user_id, callback);
}

void UserOperations::GetCurrent(Callback callback) {
	if (!IsValid()) return;
	m_cluster->current_user_get(callback);
}
