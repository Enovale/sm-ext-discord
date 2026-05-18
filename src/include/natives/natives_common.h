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

#include "core/handle_manager.h"
#include "utils/discord_common.h"
#include "core/async_callback.h"
#include "core/discord_client.h"
#include <cstddef>

inline bool GetNativeArray(IPluginContext* ctx, cell_t localAddr, cell_t** out, const char* name) {
	if (!out) return false;
	*out = nullptr;
	if (!localAddr) {
		ctx->ReportError("%s array is null", name);
		return false;
	}
	if (ctx->LocalToPhysAddr(localAddr, out) != SP_ERROR_NONE || !*out) {
		ctx->ReportError("Could not read %s array", name);
		return false;
	}
	return true;
}

inline bool GetNativeIntInRange(IPluginContext* ctx, cell_t value, int minValue, int maxValue, const char* name, int& out) {
	if (value < minValue || value > maxValue) {
		ctx->ReportError("%s must be between %d and %d (got %d)", name, minValue, maxValue, value);
		return false;
	}
	out = static_cast<int>(value);
	return true;
}

template <std::size_t N>
inline bool GetNativeIntInList(IPluginContext* ctx, cell_t value, const int (&allowedValues)[N], const char* name, int& out) {
	for (std::size_t i = 0; i < N; ++i) {
		if (value == allowedValues[i]) {
			out = static_cast<int>(value);
			return true;
		}
	}

	ctx->ReportError("%s has invalid value %d", name, value);
	return false;
}

template <typename T>
inline bool GetNativeEnumInRange(IPluginContext* ctx, cell_t value, int minValue, int maxValue, const char* name, T& out) {
	int enumValue;
	if (!GetNativeIntInRange(ctx, value, minValue, maxValue, name, enumValue)) return false;
	out = static_cast<T>(enumValue);
	return true;
}

template <typename T, std::size_t N>
inline bool GetNativeEnumInList(IPluginContext* ctx, cell_t value, const int (&allowedValues)[N], const char* name, T& out) {
	int enumValue;
	if (!GetNativeIntInList(ctx, value, allowedValues, name, enumValue)) return false;
	out = static_cast<T>(enumValue);
	return true;
}

inline bool GetNativeAutoArchiveMinutes(IPluginContext* ctx, cell_t value, bool allowZero, const char* name, uint16_t& out) {
	if (allowZero && value == 0) {
		out = 0;
		return true;
	}

	switch (value) {
		case dpp::arc_1_hour:
			out = 60;
			return true;
		case dpp::arc_1_day:
			out = 1440;
			return true;
		case dpp::arc_3_days:
			out = 4320;
			return true;
		case dpp::arc_1_week:
			out = 10080;
			return true;
		default:
			ctx->ReportError("%s must be AutoArchive_1Hour, AutoArchive_1Day, AutoArchive_3Days, or AutoArchive_1Week%s (got %d)",
				name, allowZero ? ", or 0" : "", value);
			return false;
	}
}

inline bool GetNativeAutoArchiveEnum(IPluginContext* ctx, cell_t value, const char* name, dpp::auto_archive_duration_t& out) {
	uint16_t minutes;
	if (!GetNativeAutoArchiveMinutes(ctx, value, false, name, minutes)) return false;
	out = static_cast<dpp::auto_archive_duration_t>(value);
	return true;
}

inline bool GetNativeUInt8(IPluginContext* ctx, cell_t value, uint8_t maxValue, const char* name, uint8_t& out) {
	if (value < 0 || value > maxValue) {
		ctx->ReportError("%s must be between 0 and %u (got %d)", name, static_cast<unsigned>(maxValue), value);
		return false;
	}
	out = static_cast<uint8_t>(value);
	return true;
}

inline bool GetNativeUInt16(IPluginContext* ctx, cell_t value, uint16_t maxValue, const char* name, uint16_t& out) {
	if (value < 0 || value > maxValue) {
		ctx->ReportError("%s must be between 0 and %u (got %d)", name, static_cast<unsigned>(maxValue), value);
		return false;
	}
	out = static_cast<uint16_t>(value);
	return true;
}

inline bool GetNativeUInt32(IPluginContext* ctx, cell_t value, uint32_t maxValue, const char* name, uint32_t& out) {
	if (value < 0 || static_cast<uint64_t>(value) > maxValue) {
		ctx->ReportError("%s must be between 0 and %u (got %d)", name, static_cast<unsigned>(maxValue), value);
		return false;
	}
	out = static_cast<uint32_t>(value);
	return true;
}
