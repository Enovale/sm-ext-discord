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
#include "core/callback_manager.h"
#include "core/event_handler.h"
#include "core/operations/operations.h"

class DiscordClient
{
private:
	std::unique_ptr<dpp::cluster> m_cluster;
	Handle_t m_discord_handle = 0;
	CallbackManager m_callbacks;
	std::unique_ptr<EventHandler> m_eventHandler;
	bool m_startCalled = false;

	// Operations
	std::unique_ptr<MessageOperations> m_messageOps;
	std::unique_ptr<WebhookOperations> m_webhookOps;
	std::unique_ptr<CommandOperations> m_commandOps;
	std::unique_ptr<ChannelOperations> m_channelOps;
	std::unique_ptr<RoleOperations> m_roleOps;
	std::unique_ptr<MemberOperations> m_memberOps;
	std::unique_ptr<GuildOperations> m_guildOps;
	std::unique_ptr<ThreadOperations> m_threadOps;
	std::unique_ptr<InviteOperations> m_inviteOps;
	std::unique_ptr<EmojiOperations> m_emojiOps;
	std::unique_ptr<StickerOperations> m_stickerOps;
	std::unique_ptr<ScheduledEventOperations> m_scheduledEventOps;
	std::unique_ptr<StageOperations> m_stageOps;
	std::unique_ptr<VoiceOperations> m_voiceOps;
	std::unique_ptr<HttpOperations> m_httpOps;
	std::unique_ptr<UserOperations> m_userOps;

public:
	DiscordClient(const char* token, uint32_t intents = 0);
	~DiscordClient();

	void Start();
	void Stop();
	bool IsRunning() const;
	void SendClosePacket();

	void SetHandle(Handle_t handle) { m_discord_handle = handle; }
	Handle_t GetHandle() const { return m_discord_handle; }
	dpp::cluster* GetCluster() const { return m_cluster.get(); }
	CallbackManager& Callbacks() { return m_callbacks; }
	EventHandler& Events() { return *m_eventHandler; }

	// Operations accessors
	MessageOperations& Messages() { return *m_messageOps; }
	WebhookOperations& Webhooks() { return *m_webhookOps; }
	CommandOperations& Commands() { return *m_commandOps; }
	ChannelOperations& Channels() { return *m_channelOps; }
	RoleOperations& Roles() { return *m_roleOps; }
	MemberOperations& Members() { return *m_memberOps; }
	GuildOperations& Guilds() { return *m_guildOps; }
	ThreadOperations& Threads() { return *m_threadOps; }
	InviteOperations& Invites() { return *m_inviteOps; }
	EmojiOperations& Emojis() { return *m_emojiOps; }
	StickerOperations& Stickers() { return *m_stickerOps; }
	ScheduledEventOperations& ScheduledEvents() { return *m_scheduledEventOps; }
	StageOperations& Stages() { return *m_stageOps; }
	VoiceOperations& Voice() { return *m_voiceOps; }
	HttpOperations& Http() { return *m_httpOps; }
	UserOperations& Users() { return *m_userOps; }

	// Bot info
	std::string GetBotId() const { return m_cluster ? m_cluster->me.id.str() : ""; }
	const char* GetBotName() const { return m_cluster ? m_cluster->me.username.c_str() : ""; }
	uint16_t GetBotDiscriminator() const { return m_cluster ? m_cluster->me.discriminator : 0; }
	std::string GetBotAvatarUrl() const { return m_cluster ? m_cluster->me.get_avatar_url() : ""; }
	uint64_t GetUptime() const;

	// Global cache counts (static - not instance specific)
	static uint64_t GetCachedUserCount() { return dpp::get_user_count(); }
	static uint64_t GetCachedGuildCount() { return dpp::get_guild_count(); }
	static uint64_t GetCachedChannelCount() { return dpp::get_channel_count(); }
	static uint64_t GetCachedRoleCount() { return dpp::get_role_count(); }
	static uint64_t GetCachedEmojiCount() { return dpp::get_emoji_count(); }

	// Presence
	void SetPresence(dpp::presence presence);
};
