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

#include "core/discord_client_ref.h"
#include "smsdk_ext.h"
#include <unordered_map>
#include <variant>

class DiscordClient;

// Event value types
using EventValue = std::variant<int, float, bool, std::string, Handle_t>;

class DiscordEvent {
private:
	std::string m_name;
	std::unordered_map<std::string, EventValue> m_data;
	DiscordClientRef m_client;
	bool m_autoFreeHandles;

public:
	DiscordEvent(const std::string& name, DiscordClient* client, bool autoFree = true);
	~DiscordEvent();

	// Setters
	void SetInt(const char* key, int value) { m_data[key] = value; }
	void SetFloat(const char* key, float value) { m_data[key] = value; }
	void SetBool(const char* key, bool value) { m_data[key] = value; }
	void SetString(const char* key, const std::string& value) { m_data[key] = value; }
	void SetHandle(const char* key, Handle_t value) { if (value) m_data[key] = value; }

	// Getters
	int GetInt(const char* key, int defaultValue = 0) const;
	float GetFloat(const char* key, float defaultValue = 0.0f) const;
	bool GetBool(const char* key, bool defaultValue = false) const;
	const char* GetString(const char* key) const;
	Handle_t GetHandle(const char* key) const;
	bool HasKey(const char* key) const;

	const std::string& GetName() const { return m_name; }
	DiscordClient* GetClient() const { return m_client.Get(); }
	void SetAutoFreeHandles(bool autoFree) { m_autoFreeHandles = autoFree; }
	bool GetAutoFreeHandles() const { return m_autoFreeHandles; }
};
