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

#include "core/discord_result.h"
#include "core/handle_manager.h"

DiscordResult::DiscordResult(DiscordResultType type, DiscordClient* client, bool autoFree)
	: m_type(type), m_client(client), m_autoFreeHandles(autoFree), m_success(true) {}

DiscordResult::~DiscordResult() {
	if (m_autoFreeHandles) {
		for (auto& [key, value] : m_data) {
			if (std::holds_alternative<Handle_t>(value)) {
				Handle_t h = std::get<Handle_t>(value);
				if (h) Handles.FreeHandle(h);
			}
		}
	}
}

int DiscordResult::GetInt(const char* key, int defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<int>(it->second)) {
		return std::get<int>(it->second);
	}
	return defaultValue;
}

float DiscordResult::GetFloat(const char* key, float defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<float>(it->second)) {
		return std::get<float>(it->second);
	}
	return defaultValue;
}

bool DiscordResult::GetBool(const char* key, bool defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<bool>(it->second)) {
		return std::get<bool>(it->second);
	}
	return defaultValue;
}

const char* DiscordResult::GetString(const char* key) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<std::string>(it->second)) {
		return std::get<std::string>(it->second).c_str();
	}
	return "";
}

Handle_t DiscordResult::GetHandle(const char* key) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<Handle_t>(it->second)) {
		return std::get<Handle_t>(it->second);
	}
	return BAD_HANDLE;
}

bool DiscordResult::HasKey(const char* key) const {
	return m_data.find(key) != m_data.end();
}
