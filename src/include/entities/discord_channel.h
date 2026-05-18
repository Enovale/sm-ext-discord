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
#include "utils/discord_embed.h"
#include "utils/discord_forum.h"

class DiscordChannel
{
private:
	dpp::channel m_channel;
	DiscordClientRef m_client;

public:
	DiscordChannel(const dpp::channel& chnl) : m_channel(chnl), m_client(nullptr) {}
	DiscordChannel(const dpp::channel& chnl, DiscordClient* client) : m_channel(chnl), m_client(client) {}

	// Basic information
	const char* GetName() const { return m_channel.name.c_str(); }
	std::string GetId() const { return m_channel.id.str(); }
	std::string GetGuildId() const { return m_channel.guild_id.str(); }
	std::string GetParentId() const { return m_channel.parent_id.str(); }
	void SetParentId(const char* parent_id) { m_channel.parent_id = parent_id; }
	const char* GetTopic() const { return m_channel.topic.c_str(); }

	// Channel type and properties
	uint8_t GetType() const { return static_cast<uint8_t>(m_channel.get_type()); }
	uint16_t GetPosition() const { return m_channel.position; }
	bool IsNSFW() const { return m_channel.is_nsfw(); }
	bool IsTextChannel() const { return m_channel.is_text_channel(); }
	bool IsVoiceChannel() const { return m_channel.is_voice_channel(); }
	bool IsCategory() const { return m_channel.is_category(); }
	bool IsThread() const {
		dpp::channel_type type = m_channel.get_type();
		return type == dpp::CHANNEL_ANNOUNCEMENT_THREAD ||
			type == dpp::CHANNEL_PUBLIC_THREAD ||
			type == dpp::CHANNEL_PRIVATE_THREAD;
	}
	bool IsForum() const { return m_channel.is_forum(); }
	bool IsNewsChannel() const { return m_channel.is_news_channel(); }
	bool IsStageChannel() const { return m_channel.is_stage_channel(); }

	// Voice channel specific
	uint16_t GetBitrate() const { return m_channel.bitrate; }
	uint8_t GetUserLimit() const { return m_channel.user_limit; }

	// Text channel specific
	uint16_t GetRateLimitPerUser() const { return m_channel.rate_limit_per_user; }

	// Additional channel properties
	uint16_t GetFlags() const { return m_channel.flags; }
	std::string GetOwnerId() const { return m_channel.owner_id.str(); }
	std::string GetLastMessageId() const { return m_channel.last_message_id.str(); }
	time_t GetLastPinTimestamp() const { return m_channel.last_pin_timestamp; }
	uint16_t GetDefaultThreadRateLimitPerUser() const { return m_channel.default_thread_rate_limit_per_user; }
	uint8_t GetDefaultAutoArchiveDuration() const { return static_cast<uint8_t>(m_channel.default_auto_archive_duration); }
	uint8_t GetDefaultSortOrder() const { return static_cast<uint8_t>(m_channel.default_sort_order); }
	uint8_t GetForumLayout() const { return static_cast<uint8_t>(m_channel.get_default_forum_layout()); }
	const char* GetRTCRegion() const { return m_channel.rtc_region.c_str(); }

	// Additional channel type checks
	bool IsDM() const { return m_channel.is_dm(); }
	bool IsGroupDM() const { return m_channel.is_group_dm(); }
	bool IsMediaChannel() const { return m_channel.is_media_channel(); }
	bool IsVideo720p() const { return m_channel.is_video_720p(); }
	bool IsVideoAuto() const { return m_channel.is_video_auto(); }
	bool IsPinnedThread() const { return m_channel.is_pinned_thread(); }
	bool IsTagRequired() const { return m_channel.is_tag_required(); }
	bool IsDownloadOptionsHidden() const { return m_channel.is_download_options_hidden(); }
	bool IsLockedPermissions() const { return m_channel.is_locked_permissions(); }

	// URLs and mentions
	std::string GetMention() const { return m_channel.get_mention(); }
	std::string GetUrl() const { return m_channel.get_url(); }
	std::string GetIconUrl(uint16_t size = 0) const { return m_channel.get_icon_url(size); }

	// Permission overwrites
	size_t GetPermissionOverwriteCount() const { return m_channel.permission_overwrites.size(); }
	std::string GetPermissionOverwriteTargetId(size_t index) const {
		if (index >= m_channel.permission_overwrites.size()) return "";
		return m_channel.permission_overwrites[index].id.str();
	}
	uint8_t GetPermissionOverwriteType(size_t index) const {
		if (index >= m_channel.permission_overwrites.size()) return 0;
		return m_channel.permission_overwrites[index].type;
	}

	// Forum tags
	size_t GetAvailableTagCount() const { return m_channel.available_tags.size(); }
	std::string GetAvailableTagName(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		return m_channel.available_tags[index].name;
	}
	std::string GetAvailableTagId(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		return m_channel.available_tags[index].id.str();
	}

	// New Forum tag methods
	std::string GetAvailableTagEmoji(size_t index) const {
		if (index >= m_channel.available_tags.size()) return "";
		const auto& tag = m_channel.available_tags[index];
		if (std::holds_alternative<dpp::snowflake>(tag.emoji)) {
			return std::get<dpp::snowflake>(tag.emoji).str();
		} else if (std::holds_alternative<std::string>(tag.emoji)) {
			return std::get<std::string>(tag.emoji);
		}
		return "";
	}

	bool GetAvailableTagModerated(size_t index) const {
		if (index >= m_channel.available_tags.size()) return false;
		return m_channel.available_tags[index].moderated;
	}

	bool GetAvailableTagEmojiIsCustom(size_t index) const {
		if (index >= m_channel.available_tags.size()) return false;
		return std::holds_alternative<dpp::snowflake>(m_channel.available_tags[index].emoji);
	}

	// Forum tag management
	void CreateForumTag(const char* name, const char* emoji = "", bool moderated = false);
	void EditForumTag(dpp::snowflake tag_id, const char* name, const char* emoji = "", bool moderated = false);
	void DeleteForumTag(dpp::snowflake tag_id);
	void ApplyForumTag(const class DiscordForumTag* tag);

	// Forum thread creation
	void CreateForumThread(const char* name, const char* message, const std::vector<dpp::snowflake>& tag_ids = {}, dpp::auto_archive_duration_t auto_archive = dpp::arc_1_day, uint16_t rate_limit = 0, IPluginFunction* callback = nullptr, cell_t data = 0);
	void CreateForumThreadEmbed(const char* name, const char* message, const class DiscordEmbed* embed, const std::vector<dpp::snowflake>& tag_ids = {}, dpp::auto_archive_duration_t auto_archive = dpp::arc_1_day, uint16_t rate_limit = 0, IPluginFunction* callback = nullptr, cell_t data = 0);

	// Thread management
	void CreateThread(const char* name, dpp::channel_type type = dpp::CHANNEL_PUBLIC_THREAD, uint16_t auto_archive = 1440, bool invitable = true, uint16_t rate_limit = 0, IPluginFunction* callback = nullptr, cell_t data = 0);
	void CreateThreadWithMessage(dpp::snowflake message_id, const char* name, uint16_t auto_archive = 1440, uint16_t rate_limit = 0, IPluginFunction* callback = nullptr, cell_t data = 0);
	void JoinThread(IPluginFunction* callback = nullptr, cell_t data = 0);
	void LeaveThread(IPluginFunction* callback = nullptr, cell_t data = 0);
	void ModifyThread(const char* name, uint16_t auto_archive = 0, uint16_t rate_limit = 0, bool archived = false, bool locked = false, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ThreadMemberAdd(dpp::snowflake user_id, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ThreadMemberRemove(dpp::snowflake user_id, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetArchived(bool archived, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetLocked(bool locked, IPluginFunction* callback = nullptr, cell_t data = 0);
	bool GetArchivedThreads(bool is_private, time_t before, uint16_t limit, IPluginFunction* callback, cell_t data = 0);
	bool GetThreadMembers(IPluginFunction* callback, cell_t data = 0);

	// Permission management
	void AddPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetPermissionOverwrite(dpp::snowflake target_id, uint8_t type, uint64_t allowed, uint64_t denied, IPluginFunction* callback = nullptr, cell_t data = 0);
	void RemovePermissionOverwrite(dpp::snowflake target_id, uint8_t type, IPluginFunction* callback = nullptr, cell_t data = 0);
	void EditPermissions(dpp::snowflake overwrite_id, uint64_t allow, uint64_t deny, bool is_member, IPluginFunction* callback = nullptr, cell_t data = 0);
	std::string GetUserPermissions(dpp::snowflake user_id) const;

	// Channel actions
	void CreateInvite(int max_age = 86400, int max_uses = 0, bool temporary = false, bool unique = false, IPluginFunction* callback = nullptr, cell_t data = 0);
	void CreateInviteFromObject(const DiscordInvite* invite_obj, IPluginFunction* callback = nullptr, cell_t data = 0);
	bool GetInvites(IPluginFunction* callback, cell_t data = 0);
	bool GetWebhooks(IPluginFunction* callback, cell_t data = 0);
	void SendMessage(const char* content, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SendMessageEmbed(const char* content, const class DiscordEmbed* embed, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SendDiscordMessage(const class DiscordMessage* message, IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetRTCRegion(const char* region);

	// Permission checking
	bool HasUserPermission(const dpp::user& user, const char* permission) const;
	bool HasMemberPermission(const dpp::guild_member& member, const char* permission) const;

	// Channel management methods
	void SetName(const char* name);
	void SetTopic(const char* topic);
	void SetPosition(uint16_t position);
	void SetNSFW(bool nsfw);
	void SetRateLimitPerUser(uint16_t seconds);
	void SetBitrate(uint16_t bitrate);
	void SetUserLimit(uint8_t limit);
	void Delete(IPluginFunction* callback = nullptr, cell_t data = 0);
	void SetParent(dpp::snowflake parent_id);
	void Modify(IPluginFunction* callback = nullptr, cell_t data = 0);

	// Internal accessor
	const dpp::channel& GetDPPChannel() const { return m_channel; }
};
