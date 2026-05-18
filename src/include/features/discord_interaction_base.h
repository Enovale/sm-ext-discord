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
#include "core/handle_manager.h"
#include "entities/discord_user.h"
#include "entities/discord_channel.h"
#include "entities/discord_message.h"
#include "utils/discord_embed.h"

// CRTP base class for interaction types
template<typename Derived, typename EventType>
class DiscordInteractionBase
{
protected:
	EventType m_event;
	DiscordClientRef m_client;
	mutable CachedHandle<DiscordUser> m_userHandle;
	mutable CachedHandle<DiscordChannel> m_channelHandle;

	EventType& GetEvent() { return m_event; }
	const EventType& GetEvent() const { return m_event; }

public:
	DiscordInteractionBase(const EventType& event, DiscordClient* client)
		: m_event(event), m_client(client) {}

	// Common methods for all interactions
	const char* GetCustomId() const { return m_event.custom_id.c_str(); }
	dpp::snowflake GetUserId() const { return m_event.command.usr.id; }
	dpp::snowflake GetChannelId() const { return m_event.command.channel_id; }
	dpp::snowflake GetGuildId() const { return m_event.command.guild_id; }

	void Reply(const char* content, bool ephemeral = false) {
		if (!content) return;
		dpp::message msg(content);
		if (ephemeral) msg.set_flags(dpp::m_ephemeral);
		m_event.reply(msg);
	}

	void ReplyEmbed(const char* content, const DiscordEmbed* embed, bool ephemeral = false) {
		if (!content || !embed) return;
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		if (ephemeral) msg.set_flags(dpp::m_ephemeral);
		m_event.reply(msg);
	}

	void ReplyMessage(const DiscordMessage* message, bool ephemeral = false) {
		if (!message) return;
		dpp::message msg = message->GetDPPMessage();
		if (ephemeral) msg.set_flags(dpp::m_ephemeral);
		m_event.reply(msg);
	}

	void DeferReply(bool ephemeral = false) {
		m_event.thinking(ephemeral);
	}

	DiscordUser* GetUser() const {
		if (!m_client) return nullptr;
		return new DiscordUser(m_event.command.usr, m_client.Get());
	}

	Handle_t GetUserHandle() const {
		return m_userHandle.GetOrCreate([this]() { return GetUser(); });
	}

	DiscordChannel* GetChannel() const {
		if (!m_client) return nullptr;
		dpp::channel* ch = dpp::find_channel(m_event.command.channel_id);
		if (!ch) return nullptr;
		return new DiscordChannel(*ch, m_client.Get());
	}

	Handle_t GetChannelHandle() const {
		return m_channelHandle.GetOrCreate([this]() { return GetChannel(); });
	}
};

// Mixin for component interactions (Button, Select) that support message updates
template<typename Derived, typename EventType>
class DiscordComponentInteractionBase : public DiscordInteractionBase<Derived, EventType>
{
	using Base = DiscordInteractionBase<Derived, EventType>;
	mutable CachedHandle<DiscordMessage> m_messageHandle;

public:
	DiscordComponentInteractionBase(const EventType& event, DiscordClient* client)
		: Base(event, client) {}

	dpp::snowflake GetMessageId() const { return this->m_event.command.message_id; }
	uint8_t GetComponentType() const { return static_cast<uint8_t>(this->m_event.component_type); }

	void UpdateMessage(const char* content) {
		if (!content) return;
		dpp::message msg(content);
		this->m_event.reply(dpp::ir_update_message, msg);
	}

	void UpdateMessageEmbed(const char* content, const DiscordEmbed* embed) {
		if (!content || !embed) return;
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		this->m_event.reply(dpp::ir_update_message, msg);
	}

	void UpdateDiscordMessage(const DiscordMessage* message) {
		if (!message) return;
		dpp::message msg = message->GetDPPMessage();
		this->m_event.reply(dpp::ir_update_message, msg);
	}

	void DeferUpdate() {
		this->m_event.reply(dpp::ir_deferred_update_message, dpp::message());
	}

	DiscordMessage* GetMessage() const {
		if (!this->m_client) return nullptr;
		return new DiscordMessage(this->m_event.command.msg, this->m_client.Get());
	}

	Handle_t GetMessageHandle() const {
		return m_messageHandle.GetOrCreate([this]() { return GetMessage(); });
	}

	void ShowModal(const char* custom_id, const char* title, const std::vector<dpp::component>& components) {
		if (!custom_id || !title) return;
		dpp::interaction_modal_response modal(custom_id, title);

		for (size_t i = 0; i < components.size(); i++) {
			const auto& row = components[i];
			if (!row.components.empty()) {
				modal.add_component(row.components[0]);
				if (i < components.size() - 1) {
					modal.add_row();
				}
			}
		}

		this->m_event.dialog(modal);
	}
};
