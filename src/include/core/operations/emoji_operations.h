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

#pragma once

#include "base_operations.h"

class EmojiOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Create(dpp::snowflake guild_id, const char* name, const char* image_path, dpp::image_type type, Callback callback = nullptr);
	void Modify(dpp::snowflake guild_id, dpp::snowflake emoji_id, const char* name, Callback callback = nullptr);
	void Delete(dpp::snowflake guild_id, dpp::snowflake emoji_id, Callback callback = nullptr);
	void Get(dpp::snowflake guild_id, dpp::snowflake emoji_id, Callback callback);
	void GetAll(dpp::snowflake guild_id, Callback callback);
};
