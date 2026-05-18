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

#include <vector>
#include <string>
#include <variant>
#include "smsdk_ext.h"
#include <dpp/snowflake.h>

class DiscordHandleArray {
private:
	std::variant<std::vector<Handle_t>, std::vector<std::string>> m_data;
	bool m_autoFree;

public:
	DiscordHandleArray(bool autoFree = true) : m_data(std::vector<std::string>()), m_autoFree(autoFree) {}
	~DiscordHandleArray();

	// Handle operations
	void Add(Handle_t handle) {
		if (std::holds_alternative<std::vector<std::string>>(m_data) && std::get<std::vector<std::string>>(m_data).empty()) {
			m_data = std::vector<Handle_t>();
		}
		if (auto* vec = std::get_if<std::vector<Handle_t>>(&m_data)) {
			vec->push_back(handle);
		}
	}

	Handle_t GetAt(size_t index) const {
		if (auto* vec = std::get_if<std::vector<Handle_t>>(&m_data)) {
			return index < vec->size() ? (*vec)[index] : 0;
		}
		return 0;
	}

	// String operations
	void AddString(const std::string& str) {
		if (std::holds_alternative<std::vector<Handle_t>>(m_data) && std::get<std::vector<Handle_t>>(m_data).empty()) {
			m_data = std::vector<std::string>();
		}
		if (auto* vec = std::get_if<std::vector<std::string>>(&m_data)) {
			vec->push_back(str);
		}
	}

	const std::string& GetStringAt(size_t index) const {
		static const std::string empty;
		if (const std::string* str = TryGetStringAt(index)) return *str;
		return empty;
	}

	const std::string* TryGetStringAt(size_t index) const {
		if (auto* vec = std::get_if<std::vector<std::string>>(&m_data)) {
			return index < vec->size() ? &(*vec)[index] : nullptr;
		}
		return nullptr;
	}

	// Unified size method
	size_t GetLength() const {
		return std::visit([](const auto& vec) { return vec.size(); }, m_data);
	}

	// Batch add helpers
	void AddStrings(const std::vector<dpp::snowflake>& ids) {
		for (const auto& id : ids) {
			AddString(id.str());
		}
	}

	void AddStrings(const std::vector<std::string>& strings) {
		for (const auto& str : strings) {
			AddString(str);
		}
	}

	bool IsAutoFree() const { return m_autoFree; }
	void SetAutoFree(bool autoFree) { m_autoFree = autoFree; }
};
