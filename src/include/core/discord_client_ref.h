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
#include <cstddef>

class DiscordClient;

class DiscordClientRef {
public:
	DiscordClientRef() = default;
	DiscordClientRef(std::nullptr_t) {}
	DiscordClientRef(DiscordClient* client);

	DiscordClient* Get() const;
	Handle_t GetHandle() const { return m_handle; }

	explicit operator bool() const { return Get() != nullptr; }
	DiscordClient* operator->() const { return Get(); }

private:
	Handle_t m_handle{BAD_HANDLE};
};
