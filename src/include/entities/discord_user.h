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

class DiscordUser
{
private:
	dpp::user m_user;
	DiscordClient* m_client;

public:
	DiscordUser(const dpp::user& user, DiscordClient* client) : m_user(user), m_client(client) {}

	std::string GetId() const { return m_user.id.str(); }
	const char* GetUserName() const { return m_user.username.c_str(); }
	uint16_t GetDiscriminator() const { return m_user.discriminator; }
	const char* GetGlobalName() const { return m_user.global_name.c_str(); }
	std::string GetAvatarUrl(bool prefer_animated_avatars) const { return m_user.get_avatar_url(0, dpp::i_png, prefer_animated_avatars); }
	std::string GetDefaultAvatarUrl() const { return m_user.get_default_avatar_url(); }
	std::string GetAvatarDecorationUrl(uint16_t size = 0) const { return m_user.get_avatar_decoration_url(size); }
	std::string GetMention() const { return m_user.get_mention(); }
	std::string GetUrl() const { return m_user.get_url(); }
	std::string FormatUserName() const { return m_user.format_username(); }
	bool IsBot() const { return m_user.is_bot(); }
	uint32_t GetFlags() const { return m_user.flags; }

	// Internal accessors
	const dpp::user& GetDPPUser() const { return m_user; }
	DiscordClient* GetClient() const { return m_client; }
};
