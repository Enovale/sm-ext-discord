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

class DiscordSticker
{
private:
	dpp::sticker m_sticker;
	DiscordClient* m_client;

public:
	DiscordSticker(const dpp::sticker& sticker) : m_sticker(sticker), m_client(nullptr) {}
	DiscordSticker(const dpp::sticker& sticker, DiscordClient* client) : m_sticker(sticker), m_client(client) {}

	// Basic information
	std::string GetId() const { return m_sticker.id.str(); }
	std::string GetPackId() const { return m_sticker.pack_id.str(); }
	const char* GetName() const { return m_sticker.name.c_str(); }
	const char* GetDescription() const { return m_sticker.description.c_str(); }
	const char* GetTags() const { return m_sticker.tags.c_str(); }
	std::string GetGuildId() const { return m_sticker.guild_id.str(); }

	// Type information
	uint8_t GetType() const { return static_cast<uint8_t>(m_sticker.type); }
	uint8_t GetFormatType() const { return static_cast<uint8_t>(m_sticker.format_type); }
	bool IsAvailable() const { return m_sticker.available; }
	uint8_t GetSortValue() const { return m_sticker.sort_value; }

	// User who uploaded (for guild stickers)
	std::string GetUserId() const { return m_sticker.sticker_user.id.str(); }
	const char* GetUsername() const { return m_sticker.sticker_user.username.c_str(); }

	// URL
	std::string GetUrl() const { return m_sticker.get_url(); }

	// Management methods (require client)
	void Delete();
	void Edit(const char* name, const char* description, const char* tags);

	// Internal accessor
	const dpp::sticker& GetDPPSticker() const { return m_sticker; }
};
