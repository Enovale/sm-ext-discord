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

// Parse string to uint64_t, returns false if invalid or empty
inline bool ParseUInt64(const char* str, uint64_t& out) {
	if (!str || !*str) return false;
	auto [ptr, ec] = std::from_chars(str, str + std::strlen(str), out);
	return ec == std::errc{} && *ptr == '\0';
}

// Format int64_t to string, returns false if buffer too small
inline bool FormatInt64(int64_t value, char* buffer, size_t size) {
	if (!buffer || size < 2) return false;
	auto [ptr, ec] = std::to_chars(buffer, buffer + size - 1, value);
	if (ec != std::errc{}) return false;
	*ptr = '\0';
	return true;
}

// Parse string to snowflake
inline bool ParseSnowflake(const char* str, dpp::snowflake& out) {
	uint64_t val;
	if (!ParseUInt64(str, val)) return false;
	out = val;
	return true;
}

// Parse string to snowflake with error reporting
inline bool ParseSnowflake(IPluginContext* ctx, char* str, dpp::snowflake& out) {
	uint64_t val;
	if (!ParseUInt64(str, val)) {
		ctx->ReportError("Invalid snowflake format: %s", str);
		return false;
	}
	out = val;
	return true;
}

// Check if int64_t fits in int32_t range using sign extension comparison
inline bool IsInt32Range(int64_t value) {
	return (value >> 31) == (value >> 63);
}
