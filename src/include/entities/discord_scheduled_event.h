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
#include "utils/discord_common.h"

class DiscordClient;

class DiscordScheduledEvent
{
private:
	dpp::scheduled_event m_event;
	DiscordClientRef m_client;

public:
	DiscordScheduledEvent(const dpp::scheduled_event& event) : m_event(event), m_client(nullptr) {}
	DiscordScheduledEvent(const dpp::scheduled_event& event, DiscordClient* client) : m_event(event), m_client(client) {}

	// Basic information
	std::string GetId() const { return m_event.id.str(); }
	std::string GetGuildId() const { return m_event.guild_id.str(); }
	std::string GetChannelId() const { return m_event.channel_id.str(); }
	std::string GetCreatorId() const { return m_event.creator_id.str(); }
	const char* GetName() const { return m_event.name.c_str(); }
	const char* GetDescription() const { return m_event.description.c_str(); }

	// Timing
	time_t GetScheduledStartTime() const { return m_event.scheduled_start_time; }
	time_t GetScheduledEndTime() const { return m_event.scheduled_end_time; }

	// Status and type
	uint8_t GetPrivacyLevel() const { return static_cast<uint8_t>(m_event.privacy_level); }
	uint8_t GetStatus() const { return static_cast<uint8_t>(m_event.status); }
	uint8_t GetEntityType() const { return static_cast<uint8_t>(m_event.entity_type); }
	std::string GetEntityId() const { return m_event.entity_id.str(); }

	// Metadata
	const char* GetLocation() const { return m_event.entity_metadata.location.c_str(); }
	uint32_t GetUserCount() const { return m_event.user_count; }

	// Creator info
	std::string GetCreatorUserId() const { return m_event.creator.id.str(); }
	const char* GetCreatorUserName() const { return m_event.creator.username.c_str(); }

	// Image
	std::string GetImageUrl(uint16_t size = 0) const {
		if (m_event.image.is_iconhash()) {
			return m_event.image.as_iconhash().to_string();
		}
		return "";
	}

	// Management methods (require client)
	void Delete(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Edit(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Start(IPluginFunction* callback = nullptr, cell_t data = 0);
	void End(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Cancel(IPluginFunction* callback = nullptr, cell_t data = 0);

	// Setters for building/editing
	void SetName(const char* name) { if (name) m_event.set_name(name); }
	void SetDescription(const char* desc) { if (desc) m_event.set_description(desc); }
	void SetStartTime(time_t time) { m_event.set_start_time(time); }
	void SetEndTime(time_t time) { m_event.set_end_time(time); }
	void SetLocation(const char* location) { if (location) m_event.set_location(location); }
	void SetChannelId(const char* channel_id);

	// Internal accessor
	const dpp::scheduled_event& GetDPPEvent() const { return m_event; }
	dpp::scheduled_event& GetMutableDPPEvent() { return m_event; }
};
