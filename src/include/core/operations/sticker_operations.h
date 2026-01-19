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

class StickerOperations : public BaseOperations {
public:
	using BaseOperations::BaseOperations;

	void Create(dpp::snowflake guild_id, const char* name, const char* description, const char* tags, const char* file_path, dpp::sticker_format format, Callback callback = nullptr);
	void Modify(dpp::snowflake guild_id, dpp::snowflake sticker_id, const char* name = nullptr, const char* description = nullptr, const char* tags = nullptr, Callback callback = nullptr);
	void Delete(dpp::snowflake guild_id, dpp::snowflake sticker_id, Callback callback = nullptr);
};
