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

#include "dpp/dpp.h"
#include "core/handle_manager.h"
#include "core/logger.h"
#include <cstdint>
#include <ctime>
#include <fstream>
#include <limits>
#include <string>

static_assert(std::numeric_limits<time_t>::is_integer, "Discord timestamp APIs require integer time_t");
static_assert(
	std::numeric_limits<time_t>::digits <= std::numeric_limits<int64_t>::digits,
	"Discord timestamp APIs require time_t values to fit in int64_t"
);

// Parse string to uint64_t, returns false if invalid or empty
inline bool ParseUInt64(const char* str, uint64_t& out) {
	if (!str || !*str) return false;
	auto [ptr, ec] = std::from_chars(str, str + std::strlen(str), out);
	return ec == std::errc{} && *ptr == '\0';
}

inline bool ParseInt64(const char* str, int64_t& out) {
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

inline bool FormatTimestamp(time_t value, char* buffer, size_t size) {
	return FormatInt64(static_cast<int64_t>(value), buffer, size);
}

inline bool WriteTimestampString(IPluginContext* ctx, cell_t localAddr, cell_t maxlength, time_t value) {
	if (maxlength <= 0) {
		ctx->ReportError("Timestamp buffer size must be positive");
		return false;
	}

	char buffer[32];
	if (!FormatTimestamp(value, buffer, sizeof(buffer))) {
		ctx->ReportError("Could not format timestamp");
		return false;
	}

	ctx->StringToLocal(localAddr, maxlength, buffer);
	return true;
}

inline bool ReadBinaryFile(const char* path, std::streamoff maxBytes, const char* label, std::string& data, std::string& error) {
	const char* fileLabel = (label && *label) ? label : "file";
	if (!path || !*path) {
		error = std::string(fileLabel) + " path is empty";
		return false;
	}

	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		error = std::string("Failed to open ") + fileLabel + ": " + path;
		return false;
	}

	std::streamoff size = file.tellg();
	if (size <= 0 || size > maxBytes) {
		error = std::string("Invalid ") + fileLabel + " size: " + path;
		return false;
	}
	file.seekg(0, std::ios::beg);

	data.assign(static_cast<size_t>(size), '\0');
	if (!file.read(&data[0], static_cast<std::streamsize>(size))) {
		error = std::string("Failed to read ") + fileLabel + ": " + path;
		data.clear();
		return false;
	}

	return true;
}

inline bool ParseTimestamp(IPluginContext* ctx, const char* str, const char* name, bool allowZero, time_t& out) {
	int64_t value;
	if (!ParseInt64(str, value)) {
		ctx->ReportError("%s must be a Unix timestamp string", name);
		return false;
	}
	if (value < 0 || (!allowZero && value == 0)) {
		ctx->ReportError("%s must be %s", name, allowZero ? "non-negative" : "positive");
		return false;
	}
	if (value > static_cast<int64_t>((std::numeric_limits<time_t>::max)())) {
		ctx->ReportError("%s is outside the supported time_t range", name);
		return false;
	}

	out = static_cast<time_t>(value);
	return true;
}

inline bool ParseOptionalTimestamp(IPluginContext* ctx, const char* str, const char* name, time_t& out) {
	if (!str || !*str) {
		out = 0;
		return true;
	}
	return ParseTimestamp(ctx, str, name, true, out);
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
		ctx->ReportError("Invalid snowflake format: %s", str ? str : "(null)");
		return false;
	}
	out = val;
	return true;
}

// Parse optional string to snowflake. Empty or null values are treated as unset.
inline bool ParseOptionalSnowflake(IPluginContext* ctx, const char* str, dpp::snowflake& out) {
	if (!str || !*str) {
		out = 0;
		return true;
	}

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
