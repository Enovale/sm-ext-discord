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

class DiscordRole
{
private:
	dpp::role m_role;
	dpp::snowflake m_guild_id;
	DiscordClientRef m_client;

public:
	DiscordRole(DiscordClient* client) : m_guild_id(0), m_client(client) {}
	DiscordRole(const dpp::role& role, DiscordClient* client) : m_role(role), m_guild_id(role.guild_id), m_client(client) {}
	DiscordRole(const dpp::role& role, dpp::snowflake guild_id) : m_role(role), m_guild_id(guild_id), m_client(nullptr) {}
	DiscordRole(const dpp::role& role, dpp::snowflake guild_id, DiscordClient* client) : m_role(role), m_guild_id(guild_id), m_client(client) {}
	DiscordRole(const std::pair<dpp::role, dpp::snowflake>& data, DiscordClient* client) : m_role(data.first), m_guild_id(data.second), m_client(client) {}

	// Basic role information
	std::string GetId() const { return m_role.id.str(); }
	const char* GetName() const { return m_role.name.c_str(); }
	uint32_t GetColor() const { return m_role.colour; }
	bool IsHoisted() const { return m_role.is_hoisted(); }
	int32_t GetPosition() const { return m_role.position; }
	uint64_t GetPermissions() const { return static_cast<uint64_t>(m_role.permissions); }
	bool IsManaged() const { return m_role.is_managed(); }
	bool IsMentionable() const { return m_role.is_mentionable(); }

	// Icon
	std::string GetIconHash() const {
		if (m_role.icon.is_iconhash()) {
			return m_role.icon.as_iconhash().to_string();
		}
		return "";
	}
	const char* GetUnicodeEmoji() const { return m_role.unicode_emoji.c_str(); }

	// Tags (stored in flags)
	bool HasTags() const {
		return (m_role.flags & (dpp::r_premium_subscriber | dpp::r_available_for_purchase | dpp::r_guild_connections)) != 0
			|| m_role.bot_id != 0
			|| m_role.integration_id != 0
			|| m_role.subscription_listing_id != 0;
	}
	bool IsPremiumSubscriberRole() const {
		return m_role.flags & dpp::r_premium_subscriber;
	}
	bool IsAvailableForPurchase() const {
		return m_role.flags & dpp::r_available_for_purchase;
	}
	bool IsGuildConnections() const {
		return m_role.flags & dpp::r_guild_connections;
	}

	// Tag IDs
	std::string GetBotId() const {
		if (m_role.bot_id != 0) {
			return m_role.bot_id.str();
		}
		return "";
	}
	std::string GetIntegrationId() const {
		if (m_role.integration_id != 0) {
			return m_role.integration_id.str();
		}
		return "";
	}
	std::string GetSubscriptionListingId() const {
		if (m_role.subscription_listing_id != 0) {
			return m_role.subscription_listing_id.str();
		}
		return "";
	}

	// Permission methods
	bool HasPermission(const char* permission) const;
	std::string GetMention() const { return m_role.get_mention(); }

	// Role management methods
	void Modify(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Delete(IPluginFunction* callback = nullptr, cell_t data = 0);
	void AddToUser(dpp::snowflake user_id);
	void RemoveFromUser(dpp::snowflake user_id);

	// Setters for local role properties (for building new roles)
	void SetName(const char* name) { if (name) m_role.name = name; }
	void SetColor(uint32_t color) { m_role.colour = color; }
	void SetHoist(bool hoist) {
		if (hoist) m_role.flags |= dpp::r_hoist;
		else m_role.flags &= ~dpp::r_hoist;
	}
	void SetMentionable(bool mentionable) {
		if (mentionable) m_role.flags |= dpp::r_mentionable;
		else m_role.flags &= ~dpp::r_mentionable;
	}
	void SetPermissions(uint64_t permissions) { m_role.permissions = permissions; }
	bool SetIcon(const char* filepath, dpp::image_type type);

	// Internal accessor
	const dpp::role& GetDPPRole() const { return m_role; }
	dpp::snowflake GetGuildId() const { return m_guild_id; }
};
