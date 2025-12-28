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

#include "utils/discord_common.h"

class DiscordClient;

class DiscordEmoji
{
private:
	dpp::emoji m_emoji;
	dpp::snowflake m_guild_id;
	DiscordClient* m_client;

public:
	DiscordEmoji(const dpp::emoji& emoji) : m_emoji(emoji), m_guild_id(0), m_client(nullptr) {}
	DiscordEmoji(const dpp::emoji& emoji, dpp::snowflake guild_id) : m_emoji(emoji), m_guild_id(guild_id), m_client(nullptr) {}
	DiscordEmoji(const dpp::emoji& emoji, dpp::snowflake guild_id, DiscordClient* client) : m_emoji(emoji), m_guild_id(guild_id), m_client(client) {}
	DiscordEmoji(dpp::snowflake emoji_id, dpp::snowflake guild_id, DiscordClient* client) : m_guild_id(guild_id), m_client(client) {
		m_emoji.id = emoji_id;
	}

	// Basic information
	std::string GetId() const { return m_emoji.id.str(); }
	const char* GetName() const { return m_emoji.name.c_str(); }
	std::string GetUserId() const { return m_emoji.user_id.str(); }
	std::string GetGuildId() const { return m_guild_id.str(); }

	// Flags
	bool RequiresColons() const { return m_emoji.requires_colons(); }
	bool IsManaged() const { return m_emoji.is_managed(); }
	bool IsAnimated() const { return m_emoji.is_animated(); }
	bool IsAvailable() const { return m_emoji.is_available(); }

	// Roles
	size_t GetRoleCount() const { return m_emoji.roles.size(); }
	std::string GetRoleId(size_t index) const {
		if (index >= m_emoji.roles.size()) return "";
		return m_emoji.roles[index].str();
	}

	// URLs and mentions
	std::string GetMention() const { return m_emoji.get_mention(); }
	std::string GetUrl(uint16_t size = 0) const { return m_emoji.get_url(size); }
	std::string GetFormat() const { return m_emoji.format(); }

	// Management methods (require client)
	void Delete();
	void Edit(const char* name);

	// Internal accessor
	const dpp::emoji& GetDPPEmoji() const { return m_emoji; }
};
