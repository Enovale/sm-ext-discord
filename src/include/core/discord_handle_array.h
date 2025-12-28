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

#include <vector>
#include <string>
#include "smsdk_ext.h"

class DiscordHandleArray {
private:
	std::vector<Handle_t> m_handles;
	std::vector<std::string> m_strings;
	bool m_autoFree;

public:
	DiscordHandleArray(bool autoFree = true) : m_autoFree(autoFree) {}
	~DiscordHandleArray();

	// Handle operations
	void Add(Handle_t handle) { m_handles.push_back(handle); }
	Handle_t GetAt(size_t index) const { return index < m_handles.size() ? m_handles[index] : 0; }
	size_t GetLength() const { return m_handles.size(); }

	// String operations
	void AddString(const std::string& str) { m_strings.push_back(str); }
	const std::string& GetStringAt(size_t index) const {
		static const std::string empty;
		return index < m_strings.size() ? m_strings[index] : empty;
	}
	size_t GetStringCount() const { return m_strings.size(); }

	bool IsAutoFree() const { return m_autoFree; }
	void SetAutoFree(bool autoFree) { m_autoFree = autoFree; }
};
