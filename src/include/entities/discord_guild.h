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

class DiscordRole;

class DiscordGuild
{
private:
	dpp::guild m_guild;
	DiscordClient* m_client;

public:
	DiscordGuild(const dpp::guild& guild) : m_guild(guild), m_client(nullptr) {}
	DiscordGuild(const dpp::guild& guild, DiscordClient* client) : m_guild(guild), m_client(client) {}

	// Basic guild information
	std::string GetId() const { return m_guild.id.str(); }
	const char* GetName() const { return m_guild.name.c_str(); }
	void SetName(const char* name) { if (name) m_guild.name = name; }
	const char* GetDescription() const { return m_guild.description.c_str(); }
	void SetDescription(const char* desc) { if (desc) m_guild.description = desc; }
	std::string GetOwnerId() const { return m_guild.owner_id.str(); }
	std::string GetApplicationId() const { return m_guild.application_id.str(); }

	// Guild URLs and icons
	std::string GetIconUrl(uint16_t size = 0, bool prefer_animated = true) const {
		return m_guild.get_icon_url(size, dpp::i_png, prefer_animated);
	}
	std::string GetBannerUrl(uint16_t size = 0, bool prefer_animated = true) const {
		return m_guild.get_banner_url(size, dpp::i_png, prefer_animated);
	}
	std::string GetSplashUrl(uint16_t size = 0) const {
		return m_guild.get_splash_url(size);
	}
	std::string GetDiscoverySplashUrl(uint16_t size = 0) const {
		return m_guild.get_discovery_splash_url(size);
	}

	// Guild features and flags
	bool IsLarge() const { return m_guild.is_large(); }
	bool IsVerified() const { return m_guild.is_verified(); }
	bool IsPartnered() const { return m_guild.is_partnered(); }

	// Guild properties
	uint32_t GetMaxPresences() const { return m_guild.max_presences; }
	uint32_t GetMaxMembers() const { return m_guild.max_members; }
	uint16_t GetPremiumSubscriptionCount() const { return m_guild.premium_subscription_count; }
	uint8_t GetMaxVideoChannelUsers() const { return m_guild.max_video_channel_users; }
	uint8_t GetPremiumTier() const { return static_cast<uint8_t>(m_guild.premium_tier); }
	uint8_t GetVerificationLevel() const { return static_cast<uint8_t>(m_guild.verification_level); }
	uint8_t GetExplicitContentFilter() const { return static_cast<uint8_t>(m_guild.explicit_content_filter); }
	uint8_t GetMfaLevel() const { return static_cast<uint8_t>(m_guild.mfa_level); }
	uint8_t GetNsfwLevel() const { return static_cast<uint8_t>(m_guild.nsfw_level); }
	uint8_t GetAfkTimeout() const { return static_cast<uint8_t>(m_guild.afk_timeout); }
	uint8_t GetDefaultMessageNotifications() const { return static_cast<uint8_t>(m_guild.default_message_notifications); }

	// Channel IDs
	std::string GetAfkChannelId() const { return m_guild.afk_channel_id.str(); }
	std::string GetSystemChannelId() const { return m_guild.system_channel_id.str(); }
	std::string GetRulesChannelId() const { return m_guild.rules_channel_id.str(); }
	std::string GetPublicUpdatesChannelId() const { return m_guild.public_updates_channel_id.str(); }
	std::string GetWidgetChannelId() const { return m_guild.widget_channel_id.str(); }
	std::string GetSafetyAlertsChannelId() const { return m_guild.safety_alerts_channel_id.str(); }

	// Collections
	size_t GetMemberCount() const { return m_guild.members.size(); }
	size_t GetRoleCount() const { return m_guild.roles.size(); }
	size_t GetEmojiCount() const { return m_guild.emojis.size(); }
	size_t GetChannelCount() const { return m_guild.channels.size(); }
	size_t GetVoiceMemberCount() const { return m_guild.voice_members.size(); }
	size_t GetThreadCount() const { return m_guild.threads.size(); }

	// Collection accessors
	std::string GetRoleId(size_t index) const {
		if (index >= m_guild.roles.size()) return "";
		return m_guild.roles[index].str();
	}
	std::string GetChannelId(size_t index) const {
		if (index >= m_guild.channels.size()) return "";
		return m_guild.channels[index].str();
	}
	std::string GetThreadId(size_t index) const {
		if (index >= m_guild.threads.size()) return "";
		return m_guild.threads[index].str();
	}
	std::string GetEmojiId(size_t index) const {
		if (index >= m_guild.emojis.size()) return "";
		return m_guild.emojis[index].str();
	}

	// Additional properties
	const char* GetVanityUrlCode() const { return m_guild.vanity_url_code.c_str(); }
	uint32_t GetFlags() const { return m_guild.flags; }
	uint16_t GetFlagsExtra() const { return m_guild.flags_extra; }
	uint16_t GetShardId() const { return m_guild.shard_id; }

	// Welcome screen
	bool HasWelcomeScreen() const { return !m_guild.welcome_screen.description.empty(); }
	const char* GetWelcomeScreenDescription() const { return m_guild.welcome_screen.description.c_str(); }
	size_t GetWelcomeChannelCount() const { return m_guild.welcome_screen.welcome_channels.size(); }
	std::string GetWelcomeChannelId(size_t index) const {
		if (index >= m_guild.welcome_screen.welcome_channels.size()) return "";
		return m_guild.welcome_screen.welcome_channels[index].channel_id.str();
	}
	const char* GetWelcomeChannelDescription(size_t index) const {
		if (index >= m_guild.welcome_screen.welcome_channels.size()) return "";
		return m_guild.welcome_screen.welcome_channels[index].description.c_str();
	}

	// Permission methods for users
	uint64_t GetBasePermissions(dpp::snowflake user_id) const;
	uint64_t GetPermissionsInChannel(dpp::snowflake user_id, dpp::snowflake channel_id) const;
	bool HasPermission(dpp::snowflake user_id, const char* permission) const;
	bool HasPermissionInChannel(dpp::snowflake user_id, dpp::snowflake channel_id, const char* permission) const;

	// Guild management methods
	void Modify();
	bool GetInvites(IPluginFunction* callback, cell_t data = 0);
	bool GetWebhooks(IPluginFunction* callback, cell_t data = 0);
	bool GetMember(dpp::snowflake user_id, IPluginFunction* callback, cell_t data = 0);
	bool GetMembers(uint16_t limit, dpp::snowflake after, IPluginFunction* callback, cell_t data = 0);
	bool SearchMembers(const char* query, uint16_t limit, IPluginFunction* callback, cell_t data = 0);
	bool GetActiveThreads(IPluginFunction* callback, cell_t data = 0);
	bool GetBans(dpp::snowflake before, dpp::snowflake after, uint16_t limit, IPluginFunction* callback, cell_t data = 0);
	void CreateEmoji(const char* name, const char* image_path, dpp::image_type type);
	void CreateSticker(const char* name, const char* description, const char* tags, const char* file_path, dpp::sticker_format format);
	void CreateScheduledEvent(const char* name, const char* description, dpp::snowflake channel_id, time_t start_time, time_t end_time, dpp::event_entity_type type);
	void CreateExternalScheduledEvent(const char* name, const char* description, const char* location, time_t start_time, time_t end_time);
	void UnbanUser(dpp::snowflake user_id);
	void CreateRole(const char* name, uint32_t color, bool hoist, bool mentionable, uint64_t permissions, IPluginFunction* callback, cell_t data);
	void CreateRoleFromObject(DiscordRole* role, IPluginFunction* callback, cell_t data);
	void BulkDeleteCommands();

	// Internal accessor
	const dpp::guild& GetDPPGuild() const { return m_guild; }
};
