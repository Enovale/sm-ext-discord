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

#include "dpp/dpp.h"
#include "core/handle_manager.h"
#include "core/logger.h"

// Parse string to uint64 using std::from_chars (faster, no exceptions)
inline bool ParseUInt64(const char* str, uint64_t& out) {
	if (!str || !*str) return false;
	auto [ptr, ec] = std::from_chars(str, str + std::strlen(str), out);
	return ec == std::errc{};
}

inline bool ParseSnowflake(const char* str, dpp::snowflake& out) {
	uint64_t val;
	if (!ParseUInt64(str, val)) return false;
	out = val;
	return true;
}

inline bool ParseSnowflake(IPluginContext* ctx, char* str, dpp::snowflake& out) {
	uint64_t val;
	if (!ParseUInt64(str, val)) {
		ctx->ReportError("Invalid snowflake format: %s", str);
		return false;
	}
	out = val;
	return true;
}
