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

#include "core/discord_event.h"
#include "core/handle_manager.h"

DiscordEvent::DiscordEvent(const std::string& name, DiscordClient* client, bool autoFree)
	: m_name(name), m_client(client), m_autoFreeHandles(autoFree) {}

DiscordEvent::~DiscordEvent() {
	if (m_autoFreeHandles) {
		for (auto& [key, value] : m_data) {
			if (std::holds_alternative<Handle_t>(value)) {
				Handle_t h = std::get<Handle_t>(value);
				if (h) Handles.FreeHandle(h);
			}
		}
	}
}

int DiscordEvent::GetInt(const char* key, int defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<int>(it->second)) {
		return std::get<int>(it->second);
	}
	return defaultValue;
}

float DiscordEvent::GetFloat(const char* key, float defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<float>(it->second)) {
		return std::get<float>(it->second);
	}
	return defaultValue;
}

bool DiscordEvent::GetBool(const char* key, bool defaultValue) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<bool>(it->second)) {
		return std::get<bool>(it->second);
	}
	return defaultValue;
}

const char* DiscordEvent::GetString(const char* key) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<std::string>(it->second)) {
		return std::get<std::string>(it->second).c_str();
	}
	return "";
}

Handle_t DiscordEvent::GetHandle(const char* key) const {
	auto it = m_data.find(key);
	if (it != m_data.end() && std::holds_alternative<Handle_t>(it->second)) {
		return std::get<Handle_t>(it->second);
	}
	return BAD_HANDLE;
}

bool DiscordEvent::HasKey(const char* key) const {
	return m_data.find(key) != m_data.end();
}
