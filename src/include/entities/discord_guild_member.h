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

class DiscordGuildMember
{
private:
	dpp::guild_member m_member;
	DiscordClient* m_client;

public:
	DiscordGuildMember(const dpp::guild_member& member) : m_member(member), m_client(nullptr) {}
	DiscordGuildMember(const dpp::guild_member& member, DiscordClient* client) : m_member(member), m_client(client) {}

	// Basic information
	std::string GetUserId() const { return m_member.user_id.str(); }
	std::string GetGuildId() const { return m_member.guild_id.str(); }
	std::string GetNickname() const { return m_member.get_nickname(); }

	// Avatar
	std::string GetAvatarHash() const { return m_member.avatar.to_string(); }
	std::string GetAvatarUrl(uint16_t size = 0) const { return m_member.get_avatar_url(size); }
	bool HasAnimatedGuildAvatar() const { return m_member.has_animated_guild_avatar(); }

	// Timestamps
	time_t GetJoinedAt() const { return m_member.joined_at; }
	time_t GetPremiumSince() const { return m_member.premium_since; }
	time_t GetCommunicationDisabledUntil() const { return m_member.communication_disabled_until; }

	// Status flags
	bool IsDeaf() const { return m_member.is_deaf(); }
	bool IsMuted() const { return m_member.is_muted(); }
	bool IsPending() const { return m_member.is_pending(); }
	bool IsCommunicationDisabled() const { return m_member.is_communication_disabled(); }
	bool HasRejoined() const { return m_member.has_rejoined(); }
	bool IsGuildOwner() const { return m_member.is_guild_owner(); }
	bool HasCompletedOnboarding() const { return m_member.has_completed_onboarding(); }
	bool HasStartedOnboarding() const { return m_member.has_started_onboarding(); }
	bool HasBypassesVerification() const { return m_member.has_bypasses_verification(); }

	// Roles
	size_t GetRoleCount() const { return m_member.get_roles().size(); }
	std::string GetRoleId(size_t index) const {
		const auto& roles = m_member.get_roles();
		if (index >= roles.size()) return "";
		return roles[index].str();
	}
	bool HasRole(dpp::snowflake role_id) const {
		const auto& roles = m_member.get_roles();
		return std::find(roles.begin(), roles.end(), role_id) != roles.end();
	}

	// Permission methods
	std::string GetPermissions() const;
	bool HasPermission(const char* permission) const;

	// Management methods (require client)
	void AddRole(dpp::snowflake role_id, IPluginFunction* callback = nullptr, cell_t data = 0);
	void RemoveRole(dpp::snowflake role_id, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetNickname(const char* nickname, IPluginFunction* callback = nullptr, cell_t data = 0);
	void Kick(const char* reason = nullptr, IPluginFunction* callback = nullptr, cell_t data = 0);
	void Ban(uint32_t delete_message_seconds = 0, const char* reason = nullptr, IPluginFunction* callback = nullptr, cell_t data = 0);
	void Timeout(time_t until, IPluginFunction* callback = nullptr, cell_t data = 0);
	void RemoveTimeout(IPluginFunction* callback = nullptr, cell_t data = 0);

	// Internal accessor
	const dpp::guild_member& GetDPPMember() const { return m_member; }
};
