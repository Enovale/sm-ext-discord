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

#include "smsdk_ext.h"
#include <dpp/dpp.h>

constexpr size_t LOG_BUFFER_SIZE = 1024;

class Logger {
public:
	static Logger& Instance();
	void Error(const char* fmt, ...);
	void Message(const char* fmt, ...);
	void DppError(const dpp::confirmation_callback_t& cb, const char* fmt, ...);
};

inline Logger& Log = Logger::Instance();
