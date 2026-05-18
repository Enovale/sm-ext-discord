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

class DiscordInvite
{
private:
	dpp::invite m_invite;
	DiscordClientRef m_client;

public:
	DiscordInvite(const dpp::invite& invite, DiscordClient* client = nullptr) : m_invite(invite), m_client(client) {}
	DiscordInvite(DiscordClient* client) : m_client(client) {}

	// Basic properties
	const char* GetCode() const { return m_invite.code.c_str(); }
	std::string GetGuildId() const { return m_invite.guild_id.str(); }
	std::string GetChannelId() const { return m_invite.channel_id.str(); }
	std::string GetInviterId() const { return m_invite.inviter.id.str(); }
	std::string GetTargetUserId() const { return m_invite.target_user_id.str(); }

	// Inviter information
	const char* GetInviterUserName() const { return m_invite.inviter.username.c_str(); }
	std::string GetInviterAvatarUrl() const { return m_invite.inviter.get_avatar_url(); }
	bool GetInviterIsBot() const { return m_invite.inviter.is_bot(); }

	// Guild/Channel names (from partial objects)
	const char* GetGuildName() const { return m_invite.destination_guild.name.c_str(); }
	const char* GetChannelName() const { return m_invite.destination_channel.name.c_str(); }
	uint8_t GetChannelType() const { return static_cast<uint8_t>(m_invite.destination_channel.get_type()); }

	// Invite statistics
	uint32_t GetApproximatePresenceCount() const { return m_invite.approximate_presence_count; }
	uint32_t GetApproximateMemberCount() const { return m_invite.approximate_member_count; }
	uint32_t GetUses() const { return m_invite.uses; }
	uint32_t GetMaxUses() const { return m_invite.max_uses; }
	uint32_t GetMaxAge() const { return m_invite.max_age; }

	// Invite properties
	uint8_t GetTargetType() const { return static_cast<uint8_t>(m_invite.target_type); }
	bool IsTemporary() const { return m_invite.temporary; }
	bool IsUnique() const { return m_invite.unique; }
	time_t GetCreatedAt() const { return m_invite.created_at; }
	time_t GetExpiresAt() const { return m_invite.expires_at; }

	// Setters for creating invites
	void SetMaxAge(uint32_t max_age) { m_invite.set_max_age(max_age); }
	void SetMaxUses(uint8_t max_uses) { m_invite.set_max_uses(max_uses); }
	void SetTargetUserId(dpp::snowflake user_id) { m_invite.set_target_user_id(user_id); }
	void SetTargetType(dpp::invite_target_t type) { m_invite.set_target_type(type); }
	void SetTemporary(bool temporary) { m_invite.set_temporary(temporary); }
	void SetUnique(bool unique) { m_invite.set_unique(unique); }

	// Invite management
	void Create(dpp::snowflake channel_id);
	void Delete();

	// Internal accessor
	const dpp::invite& GetDPPInvite() const { return m_invite; }
};
