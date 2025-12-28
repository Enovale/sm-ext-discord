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

#include "dpp/dpp.h"
#include "callback_manager.h"
#include "discord_event.h"
#include <bitset>

class DiscordClient;

class EventHandler {
public:
	EventHandler(DiscordClient* client, dpp::cluster* cluster, CallbackManager& callbacks);
	~EventHandler();
	void Detach();
	void RegisterEvent(CallbackId id);
	void UnregisterEvent(CallbackId id);

private:
	DiscordClient* m_client;
	dpp::cluster* m_cluster;
	CallbackManager& m_callbacks;
	std::array<std::function<void()>, static_cast<size_t>(CallbackId::Count)> m_detachFuncs;
	std::bitset<static_cast<size_t>(CallbackId::Count)> m_registeredEvents;

	// Helper to dispatch event with DiscordEvent object
	void DispatchEvent(CallbackId id, DiscordEvent* event);

	// Connection events
	void OnReady(const dpp::ready_t& event);
	void OnResumed(const dpp::resumed_t& event);
	// Message events
	void OnMessageCreate(const dpp::message_create_t& event);
	void OnMessageUpdate(const dpp::message_update_t& event);
	void OnMessageDelete(const dpp::message_delete_t& event);
	void OnMessageDeleteBulk(const dpp::message_delete_bulk_t& event);
	void OnMessageReactionAdd(const dpp::message_reaction_add_t& event);
	void OnMessageReactionRemove(const dpp::message_reaction_remove_t& event);
	void OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t& event);
	void OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t& event);
	void OnMessagePollVoteAdd(const dpp::message_poll_vote_add_t& event);
	void OnMessagePollVoteRemove(const dpp::message_poll_vote_remove_t& event);
	// Channel events
	void OnChannelCreate(const dpp::channel_create_t& event);
	void OnChannelUpdate(const dpp::channel_update_t& event);
	void OnChannelDelete(const dpp::channel_delete_t& event);
	void OnChannelPinsUpdate(const dpp::channel_pins_update_t& event);
	// Thread events
	void OnThreadCreate(const dpp::thread_create_t& event);
	void OnThreadUpdate(const dpp::thread_update_t& event);
	void OnThreadDelete(const dpp::thread_delete_t& event);
	void OnThreadListSync(const dpp::thread_list_sync_t& event);
	void OnThreadMemberUpdate(const dpp::thread_member_update_t& event);
	void OnThreadMembersUpdate(const dpp::thread_members_update_t& event);
	// Guild events
	void OnGuildCreate(const dpp::guild_create_t& event);
	void OnGuildUpdate(const dpp::guild_update_t& event);
	void OnGuildDelete(const dpp::guild_delete_t& event);
	void OnGuildBanAdd(const dpp::guild_ban_add_t& event);
	void OnGuildBanRemove(const dpp::guild_ban_remove_t& event);
	void OnGuildEmojisUpdate(const dpp::guild_emojis_update_t& event);
	void OnGuildStickersUpdate(const dpp::guild_stickers_update_t& event);
	void OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t& event);
	void OnGuildMemberAdd(const dpp::guild_member_add_t& event);
	void OnGuildMemberRemove(const dpp::guild_member_remove_t& event);
	void OnGuildMemberUpdate(const dpp::guild_member_update_t& event);
	void OnGuildMembersChunk(const dpp::guild_members_chunk_t& event);
	void OnGuildRoleCreate(const dpp::guild_role_create_t& event);
	void OnGuildRoleUpdate(const dpp::guild_role_update_t& event);
	void OnGuildRoleDelete(const dpp::guild_role_delete_t& event);
	void OnGuildScheduledEventCreate(const dpp::guild_scheduled_event_create_t& event);
	void OnGuildScheduledEventUpdate(const dpp::guild_scheduled_event_update_t& event);
	void OnGuildScheduledEventDelete(const dpp::guild_scheduled_event_delete_t& event);
	void OnGuildScheduledEventUserAdd(const dpp::guild_scheduled_event_user_add_t& event);
	void OnGuildScheduledEventUserRemove(const dpp::guild_scheduled_event_user_remove_t& event);
	void OnGuildAuditLogEntryCreate(const dpp::guild_audit_log_entry_create_t& event);
	void OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t& event);
	// Integration events
	void OnIntegrationCreate(const dpp::integration_create_t& event);
	void OnIntegrationUpdate(const dpp::integration_update_t& event);
	void OnIntegrationDelete(const dpp::integration_delete_t& event);
	// Invite events
	void OnInviteCreate(const dpp::invite_create_t& event);
	void OnInviteDelete(const dpp::invite_delete_t& event);
	// Interaction events
	void OnInteractionCreate(const dpp::interaction_create_t& event);
	void OnSlashcommand(const dpp::slashcommand_t& event);
	void OnAutocomplete(const dpp::autocomplete_t& event);
	void OnButtonClick(const dpp::button_click_t& event);
	void OnSelectClick(const dpp::select_click_t& event);
	void OnFormSubmit(const dpp::form_submit_t& event);
	void OnUserContextMenu(const dpp::user_context_menu_t& event);
	void OnMessageContextMenu(const dpp::message_context_menu_t& event);
	// Voice events
	void OnVoiceStateUpdate(const dpp::voice_state_update_t& event);
	void OnVoiceServerUpdate(const dpp::voice_server_update_t& event);
	void OnVoiceReady(const dpp::voice_ready_t& event);
	void OnVoiceReceive(const dpp::voice_receive_t& event);
	void OnVoiceTrackMarker(const dpp::voice_track_marker_t& event);
	void OnVoiceClientDisconnect(const dpp::voice_client_disconnect_t& event);
	void OnVoiceClientSpeaking(const dpp::voice_client_speaking_t& event);
	void OnVoiceBufferSend(const dpp::voice_buffer_send_t& event);
	// Stage events
	void OnStageInstanceCreate(const dpp::stage_instance_create_t& event);
	void OnStageInstanceUpdate(const dpp::stage_instance_update_t& event);
	void OnStageInstanceDelete(const dpp::stage_instance_delete_t& event);
	// Presence events
	void OnPresenceUpdate(const dpp::presence_update_t& event);
	void OnTypingStart(const dpp::typing_start_t& event);
	void OnUserUpdate(const dpp::user_update_t& event);
	// Automod events
	void OnAutomodRuleCreate(const dpp::automod_rule_create_t& event);
	void OnAutomodRuleUpdate(const dpp::automod_rule_update_t& event);
	void OnAutomodRuleDelete(const dpp::automod_rule_delete_t& event);
	void OnAutomodRuleExecute(const dpp::automod_rule_execute_t& event);
	// Entitlement events
	void OnEntitlementCreate(const dpp::entitlement_create_t& event);
	void OnEntitlementUpdate(const dpp::entitlement_update_t& event);
	void OnEntitlementDelete(const dpp::entitlement_delete_t& event);
	// Webhook events
	void OnWebhooksUpdate(const dpp::webhooks_update_t& event);
	// Misc events
	void OnLog(const dpp::log_t& event);
};
