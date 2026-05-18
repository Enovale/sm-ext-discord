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

#include "smsdk_ext.h"
#include "async_callback.h"
#include <array>
#include <functional>
#include <utility>

enum class CallbackId {
	// Connection events
	Ready,
	Resumed,
	// Message events
	MessageCreate,
	MessageUpdate,
	MessageDelete,
	MessageDeleteBulk,
	MessageReactionAdd,
	MessageReactionRemove,
	MessageReactionRemoveAll,
	MessageReactionRemoveEmoji,
	MessagePollVoteAdd,
	MessagePollVoteRemove,
	// Channel events
	ChannelCreate,
	ChannelUpdate,
	ChannelDelete,
	ChannelPinsUpdate,
	// Thread events
	ThreadCreate,
	ThreadUpdate,
	ThreadDelete,
	ThreadListSync,
	ThreadMemberUpdate,
	ThreadMembersUpdate,
	// Guild events
	GuildCreate,
	GuildUpdate,
	GuildDelete,
	GuildBanAdd,
	GuildBanRemove,
	GuildEmojisUpdate,
	GuildStickersUpdate,
	GuildIntegrationsUpdate,
	GuildMemberAdd,
	GuildMemberRemove,
	GuildMemberUpdate,
	GuildMembersChunk,
	GuildRoleCreate,
	GuildRoleUpdate,
	GuildRoleDelete,
	GuildScheduledEventCreate,
	GuildScheduledEventUpdate,
	GuildScheduledEventDelete,
	GuildScheduledEventUserAdd,
	GuildScheduledEventUserRemove,
	GuildAuditLogEntryCreate,
	GuildJoinRequestDelete,
	// Integration events
	IntegrationCreate,
	IntegrationUpdate,
	IntegrationDelete,
	// Invite events
	InviteCreate,
	InviteDelete,
	// Interaction events
	InteractionCreate,
	Slashcommand,
	Autocomplete,
	ButtonClick,
	SelectClick,
	FormSubmit,
	UserContextMenu,
	MessageContextMenu,
	// Voice events
	VoiceStateUpdate,
	VoiceServerUpdate,
	VoiceReady,
	VoiceReceive,
	VoiceTrackMarker,
	VoiceClientDisconnect,
	VoiceClientSpeaking,
	VoiceBufferSend,
	// Stage events
	StageInstanceCreate,
	StageInstanceUpdate,
	StageInstanceDelete,
	// Presence events
	PresenceUpdate,
	TypingStart,
	UserUpdate,
	// Automod events
	AutomodRuleCreate,
	AutomodRuleUpdate,
	AutomodRuleDelete,
	AutomodRuleExecute,
	// Entitlement events
	EntitlementCreate,
	EntitlementUpdate,
	EntitlementDelete,
	// Webhook events
	WebhooksUpdate,
	// Misc events
	Log,
	Shutdown,
	Count
};

class DiscordEvent;

// Unified callback data - all events use same signature: (Discord, DiscordEvent, any)
struct CallbackData {
	IChangeableForward* forward = nullptr;
	AsyncCallback callback;

	void Set(Handle_t clientHandle, IChangeableForward* fwd, IPluginFunction* func, cell_t d = 0) {
		Release();
		forward = fwd;
		callback = AsyncCallback(clientHandle, func, d);
	}

	void Release() {
		if (forward) {
			forwards->ReleaseForward(forward);
			forward = nullptr;
		}
		callback = AsyncCallback();
	}

	bool IsValid() const {
		return forward && callback.GetFunction() && forward->GetFunctionCount() > 0;
	}

	bool IsForPlugin(unsigned int pluginSerial) const {
		return callback.IsForPlugin(pluginSerial);
	}
};

bool DispatchCallbackEvent(CallbackData& callback, DiscordEvent* event);

class CallbackManager {
private:
	std::array<CallbackData, static_cast<size_t>(CallbackId::Count)> m_callbacks;
	std::function<void(CallbackId)> m_onRegister;
	std::function<void(CallbackId)> m_onClear;

	void ReleaseAll() {
		for (auto& callback : m_callbacks) {
			callback.Release();
		}
	}

public:
	~CallbackManager() {
		ReleaseAll();
	}

	void SetRegisterCallback(std::function<void(CallbackId)> cb) {
		m_onRegister = std::move(cb);
	}

	void SetClearCallback(std::function<void(CallbackId)> cb) {
		m_onClear = std::move(cb);
	}

	CallbackData& Get(CallbackId id) {
		return m_callbacks[static_cast<size_t>(id)];
	}

	const CallbackData& Get(CallbackId id) const {
		return m_callbacks[static_cast<size_t>(id)];
	}

	void Set(CallbackId id, Handle_t clientHandle, IChangeableForward* fwd, IPluginFunction* func, cell_t d = 0) {
		m_callbacks[static_cast<size_t>(id)].Set(clientHandle, fwd, func, d);
		if (m_onRegister) m_onRegister(id);
	}

	void Clear(CallbackId id) {
		if (m_onClear) m_onClear(id);
		m_callbacks[static_cast<size_t>(id)].Release();
	}

	void ClearForPlugin(unsigned int pluginSerial) {
		if (!pluginSerial) return;

		for (size_t i = 0; i < m_callbacks.size(); ++i) {
			if (m_callbacks[i].IsForPlugin(pluginSerial)) {
				Clear(static_cast<CallbackId>(i));
			}
		}
	}
};
