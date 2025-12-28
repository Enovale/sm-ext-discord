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

#include "callback_manager.h"
#include <string>
#include <unordered_map>

// Event metadata for unified event registration
struct EventMeta {
	CallbackId id;
	bool hasObjectHandle;  // true if event provides a Handle (message, interaction, etc.)
};

// Event name to metadata mapping (names match DPP event names)
inline const std::unordered_map<std::string, EventMeta>& GetEventRegistry() {
	static const std::unordered_map<std::string, EventMeta> registry = {
		// Connection events
		{"ready",                            {CallbackId::Ready,                       false}},
		{"resumed",                          {CallbackId::Resumed,                     false}},
		// Message events
		{"message_create",                   {CallbackId::MessageCreate,               true}},
		{"message_update",                   {CallbackId::MessageUpdate,               true}},
		{"message_delete",                   {CallbackId::MessageDelete,               false}},
		{"message_delete_bulk",              {CallbackId::MessageDeleteBulk,           false}},
		{"message_reaction_add",             {CallbackId::MessageReactionAdd,          false}},
		{"message_reaction_remove",          {CallbackId::MessageReactionRemove,       false}},
		{"message_reaction_remove_all",      {CallbackId::MessageReactionRemoveAll,    false}},
		{"message_reaction_remove_emoji",    {CallbackId::MessageReactionRemoveEmoji,  false}},
		{"message_poll_vote_add",            {CallbackId::MessagePollVoteAdd,          false}},
		{"message_poll_vote_remove",         {CallbackId::MessagePollVoteRemove,       false}},
		// Channel events
		{"channel_create",                   {CallbackId::ChannelCreate,               true}},
		{"channel_update",                   {CallbackId::ChannelUpdate,               true}},
		{"channel_delete",                   {CallbackId::ChannelDelete,               true}},
		{"channel_pins_update",              {CallbackId::ChannelPinsUpdate,           false}},
		// Thread events
		{"thread_create",                    {CallbackId::ThreadCreate,                true}},
		{"thread_update",                    {CallbackId::ThreadUpdate,                true}},
		{"thread_delete",                    {CallbackId::ThreadDelete,                false}},
		{"thread_list_sync",                 {CallbackId::ThreadListSync,              false}},
		{"thread_member_update",             {CallbackId::ThreadMemberUpdate,          false}},
		{"thread_members_update",            {CallbackId::ThreadMembersUpdate,         false}},
		// Guild events
		{"guild_create",                     {CallbackId::GuildCreate,                 true}},
		{"guild_update",                     {CallbackId::GuildUpdate,                 true}},
		{"guild_delete",                     {CallbackId::GuildDelete,                 false}},
		{"guild_ban_add",                    {CallbackId::GuildBanAdd,                 false}},
		{"guild_ban_remove",                 {CallbackId::GuildBanRemove,              false}},
		{"guild_emojis_update",              {CallbackId::GuildEmojisUpdate,           false}},
		{"guild_stickers_update",            {CallbackId::GuildStickersUpdate,         false}},
		{"guild_integrations_update",        {CallbackId::GuildIntegrationsUpdate,     false}},
		{"guild_member_add",                 {CallbackId::GuildMemberAdd,              false}},
		{"guild_member_remove",              {CallbackId::GuildMemberRemove,           false}},
		{"guild_member_update",              {CallbackId::GuildMemberUpdate,           false}},
		{"guild_members_chunk",              {CallbackId::GuildMembersChunk,           false}},
		{"guild_role_create",                {CallbackId::GuildRoleCreate,             true}},
		{"guild_role_update",                {CallbackId::GuildRoleUpdate,             true}},
		{"guild_role_delete",                {CallbackId::GuildRoleDelete,             false}},
		{"guild_scheduled_event_create",     {CallbackId::GuildScheduledEventCreate,   false}},
		{"guild_scheduled_event_update",     {CallbackId::GuildScheduledEventUpdate,   false}},
		{"guild_scheduled_event_delete",     {CallbackId::GuildScheduledEventDelete,   false}},
		{"guild_scheduled_event_user_add",   {CallbackId::GuildScheduledEventUserAdd,  false}},
		{"guild_scheduled_event_user_remove",{CallbackId::GuildScheduledEventUserRemove,false}},
		{"guild_audit_log_entry_create",     {CallbackId::GuildAuditLogEntryCreate,    false}},
		{"guild_join_request_delete",        {CallbackId::GuildJoinRequestDelete,      false}},
		// Integration events
		{"integration_create",               {CallbackId::IntegrationCreate,           false}},
		{"integration_update",               {CallbackId::IntegrationUpdate,           false}},
		{"integration_delete",               {CallbackId::IntegrationDelete,           false}},
		// Invite events
		{"invite_create",                    {CallbackId::InviteCreate,                true}},
		{"invite_delete",                    {CallbackId::InviteDelete,                false}},
		// Interaction events
		{"interaction_create",               {CallbackId::InteractionCreate,           true}},
		{"slashcommand",                     {CallbackId::Slashcommand,                true}},
		{"autocomplete",                     {CallbackId::Autocomplete,                true}},
		{"button_click",                     {CallbackId::ButtonClick,                 true}},
		{"select_click",                     {CallbackId::SelectClick,                 true}},
		{"form_submit",                      {CallbackId::FormSubmit,                  true}},
		{"user_context_menu",                {CallbackId::UserContextMenu,             true}},
		{"message_context_menu",             {CallbackId::MessageContextMenu,          true}},
		// Voice events
		{"voice_state_update",               {CallbackId::VoiceStateUpdate,            false}},
		{"voice_server_update",              {CallbackId::VoiceServerUpdate,           false}},
		{"voice_ready",                      {CallbackId::VoiceReady,                  false}},
		{"voice_receive",                    {CallbackId::VoiceReceive,                false}},
		{"voice_track_marker",               {CallbackId::VoiceTrackMarker,            false}},
		{"voice_client_disconnect",          {CallbackId::VoiceClientDisconnect,       false}},
		{"voice_client_speaking",            {CallbackId::VoiceClientSpeaking,         false}},
		{"voice_buffer_send",                {CallbackId::VoiceBufferSend,             false}},
		// Stage events
		{"stage_instance_create",            {CallbackId::StageInstanceCreate,         false}},
		{"stage_instance_update",            {CallbackId::StageInstanceUpdate,         false}},
		{"stage_instance_delete",            {CallbackId::StageInstanceDelete,         false}},
		// Presence events
		{"presence_update",                  {CallbackId::PresenceUpdate,              false}},
		{"typing_start",                     {CallbackId::TypingStart,                 false}},
		{"user_update",                      {CallbackId::UserUpdate,                  true}},
		// Automod events
		{"automod_rule_create",              {CallbackId::AutomodRuleCreate,           false}},
		{"automod_rule_update",              {CallbackId::AutomodRuleUpdate,           false}},
		{"automod_rule_delete",              {CallbackId::AutomodRuleDelete,           false}},
		{"automod_rule_execute",             {CallbackId::AutomodRuleExecute,          false}},
		// Entitlement events
		{"entitlement_create",               {CallbackId::EntitlementCreate,           false}},
		{"entitlement_update",               {CallbackId::EntitlementUpdate,           false}},
		{"entitlement_delete",               {CallbackId::EntitlementDelete,           false}},
		// Webhook events
		{"webhooks_update",                  {CallbackId::WebhooksUpdate,              false}},
		// Misc events
		{"log",                              {CallbackId::Log,                         false}},
		{"shutdown",                         {CallbackId::Shutdown,                    false}},
	};
	return registry;
}

// Get event metadata by name (returns nullptr if not found)
inline const EventMeta* GetEventMeta(const char* name) {
	const auto& registry = GetEventRegistry();
	auto it = registry.find(name);
	return it != registry.end() ? &it->second : nullptr;
}
