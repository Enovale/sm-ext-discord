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

#include "callback_manager.h"
#include <string>
#include <unordered_map>

// Event name to id mapping (names match DPP event names)
inline const std::unordered_map<std::string, CallbackId>& GetEventRegistry() {
	static const std::unordered_map<std::string, CallbackId> registry = {
		// Connection events
		{"ready",                            CallbackId::Ready},
		{"resumed",                          CallbackId::Resumed},
		// Message events
		{"message_create",                   CallbackId::MessageCreate},
		{"message_update",                   CallbackId::MessageUpdate},
		{"message_delete",                   CallbackId::MessageDelete},
		{"message_delete_bulk",              CallbackId::MessageDeleteBulk},
		{"message_reaction_add",             CallbackId::MessageReactionAdd},
		{"message_reaction_remove",          CallbackId::MessageReactionRemove},
		{"message_reaction_remove_all",      CallbackId::MessageReactionRemoveAll},
		{"message_reaction_remove_emoji",    CallbackId::MessageReactionRemoveEmoji},
		{"message_poll_vote_add",            CallbackId::MessagePollVoteAdd},
		{"message_poll_vote_remove",         CallbackId::MessagePollVoteRemove},
		// Channel events
		{"channel_create",                   CallbackId::ChannelCreate},
		{"channel_update",                   CallbackId::ChannelUpdate},
		{"channel_delete",                   CallbackId::ChannelDelete},
		{"channel_pins_update",              CallbackId::ChannelPinsUpdate},
		// Thread events
		{"thread_create",                    CallbackId::ThreadCreate},
		{"thread_update",                    CallbackId::ThreadUpdate},
		{"thread_delete",                    CallbackId::ThreadDelete},
		{"thread_list_sync",                 CallbackId::ThreadListSync},
		{"thread_member_update",             CallbackId::ThreadMemberUpdate},
		{"thread_members_update",            CallbackId::ThreadMembersUpdate},
		// Guild events
		{"guild_create",                     CallbackId::GuildCreate},
		{"guild_update",                     CallbackId::GuildUpdate},
		{"guild_delete",                     CallbackId::GuildDelete},
		{"guild_ban_add",                    CallbackId::GuildBanAdd},
		{"guild_ban_remove",                 CallbackId::GuildBanRemove},
		{"guild_emojis_update",              CallbackId::GuildEmojisUpdate},
		{"guild_stickers_update",            CallbackId::GuildStickersUpdate},
		{"guild_integrations_update",        CallbackId::GuildIntegrationsUpdate},
		{"guild_member_add",                 CallbackId::GuildMemberAdd},
		{"guild_member_remove",              CallbackId::GuildMemberRemove},
		{"guild_member_update",              CallbackId::GuildMemberUpdate},
		{"guild_members_chunk",              CallbackId::GuildMembersChunk},
		{"guild_role_create",                CallbackId::GuildRoleCreate},
		{"guild_role_update",                CallbackId::GuildRoleUpdate},
		{"guild_role_delete",                CallbackId::GuildRoleDelete},
		{"guild_scheduled_event_create",     CallbackId::GuildScheduledEventCreate},
		{"guild_scheduled_event_update",     CallbackId::GuildScheduledEventUpdate},
		{"guild_scheduled_event_delete",     CallbackId::GuildScheduledEventDelete},
		{"guild_scheduled_event_user_add",   CallbackId::GuildScheduledEventUserAdd},
		{"guild_scheduled_event_user_remove",CallbackId::GuildScheduledEventUserRemove},
		{"guild_audit_log_entry_create",     CallbackId::GuildAuditLogEntryCreate},
		{"guild_join_request_delete",        CallbackId::GuildJoinRequestDelete},
		// Integration events
		{"integration_create",               CallbackId::IntegrationCreate},
		{"integration_update",               CallbackId::IntegrationUpdate},
		{"integration_delete",               CallbackId::IntegrationDelete},
		// Invite events
		{"invite_create",                    CallbackId::InviteCreate},
		{"invite_delete",                    CallbackId::InviteDelete},
		// Interaction events
		{"interaction_create",               CallbackId::InteractionCreate},
		{"slashcommand",                     CallbackId::Slashcommand},
		{"autocomplete",                     CallbackId::Autocomplete},
		{"button_click",                     CallbackId::ButtonClick},
		{"select_click",                     CallbackId::SelectClick},
		{"form_submit",                      CallbackId::FormSubmit},
		{"user_context_menu",                CallbackId::UserContextMenu},
		{"message_context_menu",             CallbackId::MessageContextMenu},
		// Voice events
		{"voice_state_update",               CallbackId::VoiceStateUpdate},
		{"voice_server_update",              CallbackId::VoiceServerUpdate},
		{"voice_ready",                      CallbackId::VoiceReady},
		{"voice_receive",                    CallbackId::VoiceReceive},
		{"voice_track_marker",               CallbackId::VoiceTrackMarker},
		{"voice_client_disconnect",          CallbackId::VoiceClientDisconnect},
		{"voice_client_speaking",            CallbackId::VoiceClientSpeaking},
		{"voice_buffer_send",                CallbackId::VoiceBufferSend},
		// Stage events
		{"stage_instance_create",            CallbackId::StageInstanceCreate},
		{"stage_instance_update",            CallbackId::StageInstanceUpdate},
		{"stage_instance_delete",            CallbackId::StageInstanceDelete},
		// Presence events
		{"presence_update",                  CallbackId::PresenceUpdate},
		{"typing_start",                     CallbackId::TypingStart},
		{"user_update",                      CallbackId::UserUpdate},
		// Automod events
		{"automod_rule_create",              CallbackId::AutomodRuleCreate},
		{"automod_rule_update",              CallbackId::AutomodRuleUpdate},
		{"automod_rule_delete",              CallbackId::AutomodRuleDelete},
		{"automod_rule_execute",             CallbackId::AutomodRuleExecute},
		// Entitlement events
		{"entitlement_create",               CallbackId::EntitlementCreate},
		{"entitlement_update",               CallbackId::EntitlementUpdate},
		{"entitlement_delete",               CallbackId::EntitlementDelete},
		// Webhook events
		{"webhooks_update",                  CallbackId::WebhooksUpdate},
		// Misc events
		{"log",                              CallbackId::Log},
		{"shutdown",                         CallbackId::Shutdown},
	};
	return registry;
}

// Get event id by name (returns nullptr if not found)
inline const CallbackId* GetEventId(const char* name) {
	const auto& registry = GetEventRegistry();
	auto it = registry.find(name);
	return it != registry.end() ? &it->second : nullptr;
}
