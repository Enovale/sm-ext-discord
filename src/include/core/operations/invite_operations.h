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

#pragma once

#include "base_operations.h"

class DiscordInvite;

class InviteOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Create(dpp::snowflake channel_id, uint32_t max_age = 86400, uint32_t max_uses = 0, bool temporary = false, bool unique = false, Callback callback = nullptr);
	void CreateFromObject(dpp::snowflake channel_id, const DiscordInvite* invite, Callback callback = nullptr);
	void CreateWithChannel(const dpp::channel& channel, const dpp::invite& invite, Callback callback = nullptr);
	void Delete(const char* code, const char* reason = nullptr, Callback callback = nullptr);
	void Get(const char* code, Callback callback);
};
