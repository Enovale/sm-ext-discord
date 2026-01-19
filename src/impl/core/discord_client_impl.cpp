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

#include "core/discord_client.h"
#include "core/logger.h"

DiscordClient::DiscordClient(const char* token, uint32_t intents) : m_discord_handle(0) {
	try {
		m_cluster = std::make_unique<dpp::cluster>(token, intents);
		// Initialize operations
		m_messageOps = std::make_unique<MessageOperations>(m_cluster.get(), this);
		m_webhookOps = std::make_unique<WebhookOperations>(m_cluster.get(), this);
		m_commandOps = std::make_unique<CommandOperations>(m_cluster.get(), this);
		m_channelOps = std::make_unique<ChannelOperations>(m_cluster.get(), this);
		m_roleOps = std::make_unique<RoleOperations>(m_cluster.get(), this);
		m_memberOps = std::make_unique<MemberOperations>(m_cluster.get(), this);
		m_guildOps = std::make_unique<GuildOperations>(m_cluster.get(), this);
		m_threadOps = std::make_unique<ThreadOperations>(m_cluster.get(), this);
		m_inviteOps = std::make_unique<InviteOperations>(m_cluster.get(), this);
		m_emojiOps = std::make_unique<EmojiOperations>(m_cluster.get(), this);
		m_stickerOps = std::make_unique<StickerOperations>(m_cluster.get(), this);
		m_scheduledEventOps = std::make_unique<ScheduledEventOperations>(m_cluster.get(), this);
		m_voiceOps = std::make_unique<VoiceOperations>(m_cluster.get(), this);
		m_httpOps = std::make_unique<HttpOperations>(m_cluster.get(), this);
		m_userOps = std::make_unique<UserOperations>(m_cluster.get(), this);
		m_interactionOps = std::make_unique<InteractionOperations>(m_cluster.get(), this);
		// Initialize event handler
		m_eventHandler = std::make_unique<EventHandler>(this, m_cluster.get(), m_callbacks);
		m_callbacks.SetRegisterCallback([this](CallbackId id) {
			if (m_eventHandler) m_eventHandler->RegisterEvent(id);
		});
	} catch (const std::exception& e) {
		Log.Error("Failed to create Discord cluster: %s", e.what());
	}
}

DiscordClient::~DiscordClient() {
	Stop();
}

void DiscordClient::Start() {
	if (!m_cluster) {
		return;
	}

	try {
		m_cluster->start(dpp::st_return);
		m_startCalled = true;
	} catch (const std::exception &e) {
		Log.Error("Failed to start Discord bot: %s", e.what());
	}
}

void DiscordClient::Stop() {
	if (!m_cluster) {
		return;
	}

	// Detach event handlers before shutdown to prevent callbacks to destroyed objects
	if (m_eventHandler) {
		m_eventHandler->Detach();
	}

	SendClosePacket();

	try {
		m_cluster->shutdown();
	} catch (...) {
		// Ignore exceptions during shutdown
	}

	if (m_startCalled) {
		m_startCalled = false;
		auto& shutdown = m_callbacks.Get(CallbackId::Shutdown);
		if (shutdown.IsValid()) {
			Handle_t clientHandle = GetHandle();
			DiscordEvent* ev = new DiscordEvent("shutdown", this);
			Handle_t eventHandle = Handles.CreateCallback(ev, HandleId::DiscordEvent);
			shutdown.forward->PushCell(clientHandle);
			shutdown.forward->PushCell(eventHandle);
			shutdown.forward->PushCell(shutdown.data);
			shutdown.forward->Execute(nullptr);
			Handles.FreeHandle(eventHandle);
		}
	}
}

void DiscordClient::SetPresence(dpp::presence presence) {
	if (!IsRunning()) return;
	m_cluster->set_presence(presence);
}

bool DiscordClient::IsRunning() const {
	if (!m_cluster) return false;

	for (const auto& [id, shard] : m_cluster->get_shards()) {
		if (shard && shard->is_connected()) return true;
	}
	return false;
}

void DiscordClient::SendClosePacket() {
	for (const auto& [id, shard] : m_cluster->get_shards()) {
		if (shard && shard->is_connected()) {
			shard->send_close_packet();
		}
	}
}

uint64_t DiscordClient::GetUptime() const {
	if (!IsRunning()) return 0;
	return m_cluster->uptime().to_secs();
}
