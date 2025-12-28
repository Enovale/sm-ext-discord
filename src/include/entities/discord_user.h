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
	dpp::guild_member m_member;
	bool m_has_member;
	DiscordClient* m_client;

public:
	DiscordUser(const dpp::user& user) : m_user(user), m_has_member(false), m_client(nullptr) {}
	DiscordUser(const dpp::user& user, DiscordClient* client) : m_user(user), m_has_member(false), m_client(client) {}
	DiscordUser(const dpp::user& user, const dpp::guild_member& member) : m_user(user), m_member(member), m_has_member(true), m_client(nullptr) {}
	DiscordUser(const dpp::user& user, const dpp::guild_member& member, DiscordClient* client) : m_user(user), m_member(member), m_has_member(true), m_client(client) {}

	std::string GetId() const { return m_user.id.str(); }
	const char* GetUserName() const { return m_user.username.c_str(); }
	const uint16_t GetDiscriminator() const { return m_user.discriminator; }
	const char* GetGlobalName() const { return m_user.global_name.c_str(); }
	std::string GetAvatarUrl(bool prefer_animated_avatars) const { return m_user.get_avatar_url(0, dpp::i_png, prefer_animated_avatars); }
	std::string GetDefaultAvatarUrl() const { return m_user.get_default_avatar_url(); }
	std::string GetAvatarDecorationUrl(uint16_t size = 0) const { return m_user.get_avatar_decoration_url(size); }
	std::string GetMention() const { return m_user.get_mention(); }
	std::string GetUrl() const { return m_user.get_url(); }
	std::string FormatUsername() const { return m_user.format_username(); }
	bool IsBot() const { return m_user.is_bot(); }
	uint32_t GetFlags() const { return m_user.flags; }
	bool HasFlag(uint32_t flag) const { return (m_user.flags & flag) != 0; }

	// Guild member specific methods
	bool HasGuildMember() const { return m_has_member; }
	std::string GetNickName() const { return m_has_member ? m_member.get_nickname() : ""; }
	time_t GetJoinedAt() const { return m_has_member ? m_member.joined_at : 0; }
	bool IsPending() const { return m_has_member ? m_member.is_pending() : false; }

	// Permission methods
	bool HasPermission(const char* permission) const;
	uint64_t GetPermissions() const;
	bool HasPermissionInChannel(dpp::snowflake channel_id, const char* permission) const;
	uint64_t GetPermissionsInChannel(dpp::snowflake channel_id) const;

	// Role methods
	std::vector<dpp::snowflake> GetRoles() const;
	bool HasRole(dpp::snowflake role_id) const;
	bool HasAnyRole(const std::vector<dpp::snowflake>& role_ids) const;
	bool HasAllRoles(const std::vector<dpp::snowflake>& role_ids) const;
	dpp::snowflake GetHighestRole() const;
	std::string GetRoleName(dpp::snowflake role_id) const;
	std::vector<std::string> GetRoleNames() const;

	// Member management methods
	void SetNickName(const char* nickname);
	void KickFromGuild();
	void BanFromGuild(const char* reason = nullptr, int delete_message_days = 0);
	void UnbanFromGuild();
	void SetTimeout(time_t timeout_until);
	void RemoveTimeout();

	// Internal accessors
	const dpp::user& GetDPPUser() const { return m_user; }
	const dpp::guild_member& GetDPPMember() const { return m_member; }
	DiscordClient* GetClient() const { return m_client; }
	dpp::snowflake GetGuildId() const {
		if (m_has_member) {
			return m_member.guild_id;
		}
		return 0;
	}
};
