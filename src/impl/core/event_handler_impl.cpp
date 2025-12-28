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

#include "discord.h"

EventHandler::EventHandler(DiscordClient* client, dpp::cluster* cluster, CallbackManager& callbacks)
	: m_client(client), m_cluster(cluster), m_callbacks(callbacks) {}

EventHandler::~EventHandler() {
	Detach();
}

void EventHandler::RegisterEvent(CallbackId id) {
	if (!m_cluster) return;

	size_t idx = static_cast<size_t>(id);
	if (m_registeredEvents.test(idx)) return;
	m_registeredEvents.set(idx);

	auto reg = [this, idx](auto& router, auto handler) {
		auto h = router(handler);
		auto* ptr = &router;
		m_detachFuncs[idx] = [ptr, h]() { ptr->detach(h); };
	};

	switch (id) {
		case CallbackId::Ready:
			reg(m_cluster->on_ready, [this](const dpp::ready_t& e) { OnReady(e); });
			break;
		case CallbackId::Resumed:
			reg(m_cluster->on_resumed, [this](const dpp::resumed_t& e) { OnResumed(e); });
			break;
		case CallbackId::MessageCreate:
			reg(m_cluster->on_message_create, [this](const dpp::message_create_t& e) { OnMessageCreate(e); });
			break;
		case CallbackId::MessageUpdate:
			reg(m_cluster->on_message_update, [this](const dpp::message_update_t& e) { OnMessageUpdate(e); });
			break;
		case CallbackId::MessageDelete:
			reg(m_cluster->on_message_delete, [this](const dpp::message_delete_t& e) { OnMessageDelete(e); });
			break;
		case CallbackId::MessageDeleteBulk:
			reg(m_cluster->on_message_delete_bulk, [this](const dpp::message_delete_bulk_t& e) { OnMessageDeleteBulk(e); });
			break;
		case CallbackId::MessageReactionAdd:
			reg(m_cluster->on_message_reaction_add, [this](const dpp::message_reaction_add_t& e) { OnMessageReactionAdd(e); });
			break;
		case CallbackId::MessageReactionRemove:
			reg(m_cluster->on_message_reaction_remove, [this](const dpp::message_reaction_remove_t& e) { OnMessageReactionRemove(e); });
			break;
		case CallbackId::MessageReactionRemoveAll:
			reg(m_cluster->on_message_reaction_remove_all, [this](const dpp::message_reaction_remove_all_t& e) { OnMessageReactionRemoveAll(e); });
			break;
		case CallbackId::MessageReactionRemoveEmoji:
			reg(m_cluster->on_message_reaction_remove_emoji, [this](const dpp::message_reaction_remove_emoji_t& e) { OnMessageReactionRemoveEmoji(e); });
			break;
		case CallbackId::MessagePollVoteAdd:
			reg(m_cluster->on_message_poll_vote_add, [this](const dpp::message_poll_vote_add_t& e) { OnMessagePollVoteAdd(e); });
			break;
		case CallbackId::MessagePollVoteRemove:
			reg(m_cluster->on_message_poll_vote_remove, [this](const dpp::message_poll_vote_remove_t& e) { OnMessagePollVoteRemove(e); });
			break;
		case CallbackId::ChannelCreate:
			reg(m_cluster->on_channel_create, [this](const dpp::channel_create_t& e) { OnChannelCreate(e); });
			break;
		case CallbackId::ChannelUpdate:
			reg(m_cluster->on_channel_update, [this](const dpp::channel_update_t& e) { OnChannelUpdate(e); });
			break;
		case CallbackId::ChannelDelete:
			reg(m_cluster->on_channel_delete, [this](const dpp::channel_delete_t& e) { OnChannelDelete(e); });
			break;
		case CallbackId::ChannelPinsUpdate:
			reg(m_cluster->on_channel_pins_update, [this](const dpp::channel_pins_update_t& e) { OnChannelPinsUpdate(e); });
			break;
		case CallbackId::ThreadCreate:
			reg(m_cluster->on_thread_create, [this](const dpp::thread_create_t& e) { OnThreadCreate(e); });
			break;
		case CallbackId::ThreadUpdate:
			reg(m_cluster->on_thread_update, [this](const dpp::thread_update_t& e) { OnThreadUpdate(e); });
			break;
		case CallbackId::ThreadDelete:
			reg(m_cluster->on_thread_delete, [this](const dpp::thread_delete_t& e) { OnThreadDelete(e); });
			break;
		case CallbackId::ThreadListSync:
			reg(m_cluster->on_thread_list_sync, [this](const dpp::thread_list_sync_t& e) { OnThreadListSync(e); });
			break;
		case CallbackId::ThreadMemberUpdate:
			reg(m_cluster->on_thread_member_update, [this](const dpp::thread_member_update_t& e) { OnThreadMemberUpdate(e); });
			break;
		case CallbackId::ThreadMembersUpdate:
			reg(m_cluster->on_thread_members_update, [this](const dpp::thread_members_update_t& e) { OnThreadMembersUpdate(e); });
			break;
		case CallbackId::GuildCreate:
			reg(m_cluster->on_guild_create, [this](const dpp::guild_create_t& e) { OnGuildCreate(e); });
			break;
		case CallbackId::GuildUpdate:
			reg(m_cluster->on_guild_update, [this](const dpp::guild_update_t& e) { OnGuildUpdate(e); });
			break;
		case CallbackId::GuildDelete:
			reg(m_cluster->on_guild_delete, [this](const dpp::guild_delete_t& e) { OnGuildDelete(e); });
			break;
		case CallbackId::GuildBanAdd:
			reg(m_cluster->on_guild_ban_add, [this](const dpp::guild_ban_add_t& e) { OnGuildBanAdd(e); });
			break;
		case CallbackId::GuildBanRemove:
			reg(m_cluster->on_guild_ban_remove, [this](const dpp::guild_ban_remove_t& e) { OnGuildBanRemove(e); });
			break;
		case CallbackId::GuildEmojisUpdate:
			reg(m_cluster->on_guild_emojis_update, [this](const dpp::guild_emojis_update_t& e) { OnGuildEmojisUpdate(e); });
			break;
		case CallbackId::GuildStickersUpdate:
			reg(m_cluster->on_guild_stickers_update, [this](const dpp::guild_stickers_update_t& e) { OnGuildStickersUpdate(e); });
			break;
		case CallbackId::GuildIntegrationsUpdate:
			reg(m_cluster->on_guild_integrations_update, [this](const dpp::guild_integrations_update_t& e) { OnGuildIntegrationsUpdate(e); });
			break;
		case CallbackId::GuildMemberAdd:
			reg(m_cluster->on_guild_member_add, [this](const dpp::guild_member_add_t& e) { OnGuildMemberAdd(e); });
			break;
		case CallbackId::GuildMemberRemove:
			reg(m_cluster->on_guild_member_remove, [this](const dpp::guild_member_remove_t& e) { OnGuildMemberRemove(e); });
			break;
		case CallbackId::GuildMemberUpdate:
			reg(m_cluster->on_guild_member_update, [this](const dpp::guild_member_update_t& e) { OnGuildMemberUpdate(e); });
			break;
		case CallbackId::GuildMembersChunk:
			reg(m_cluster->on_guild_members_chunk, [this](const dpp::guild_members_chunk_t& e) { OnGuildMembersChunk(e); });
			break;
		case CallbackId::GuildRoleCreate:
			reg(m_cluster->on_guild_role_create, [this](const dpp::guild_role_create_t& e) { OnGuildRoleCreate(e); });
			break;
		case CallbackId::GuildRoleUpdate:
			reg(m_cluster->on_guild_role_update, [this](const dpp::guild_role_update_t& e) { OnGuildRoleUpdate(e); });
			break;
		case CallbackId::GuildRoleDelete:
			reg(m_cluster->on_guild_role_delete, [this](const dpp::guild_role_delete_t& e) { OnGuildRoleDelete(e); });
			break;
		case CallbackId::GuildScheduledEventCreate:
			reg(m_cluster->on_guild_scheduled_event_create, [this](const dpp::guild_scheduled_event_create_t& e) { OnGuildScheduledEventCreate(e); });
			break;
		case CallbackId::GuildScheduledEventUpdate:
			reg(m_cluster->on_guild_scheduled_event_update, [this](const dpp::guild_scheduled_event_update_t& e) { OnGuildScheduledEventUpdate(e); });
			break;
		case CallbackId::GuildScheduledEventDelete:
			reg(m_cluster->on_guild_scheduled_event_delete, [this](const dpp::guild_scheduled_event_delete_t& e) { OnGuildScheduledEventDelete(e); });
			break;
		case CallbackId::GuildScheduledEventUserAdd:
			reg(m_cluster->on_guild_scheduled_event_user_add, [this](const dpp::guild_scheduled_event_user_add_t& e) { OnGuildScheduledEventUserAdd(e); });
			break;
		case CallbackId::GuildScheduledEventUserRemove:
			reg(m_cluster->on_guild_scheduled_event_user_remove, [this](const dpp::guild_scheduled_event_user_remove_t& e) { OnGuildScheduledEventUserRemove(e); });
			break;
		case CallbackId::GuildAuditLogEntryCreate:
			reg(m_cluster->on_guild_audit_log_entry_create, [this](const dpp::guild_audit_log_entry_create_t& e) { OnGuildAuditLogEntryCreate(e); });
			break;
		case CallbackId::GuildJoinRequestDelete:
			reg(m_cluster->on_guild_join_request_delete, [this](const dpp::guild_join_request_delete_t& e) { OnGuildJoinRequestDelete(e); });
			break;
		case CallbackId::IntegrationCreate:
			reg(m_cluster->on_integration_create, [this](const dpp::integration_create_t& e) { OnIntegrationCreate(e); });
			break;
		case CallbackId::IntegrationUpdate:
			reg(m_cluster->on_integration_update, [this](const dpp::integration_update_t& e) { OnIntegrationUpdate(e); });
			break;
		case CallbackId::IntegrationDelete:
			reg(m_cluster->on_integration_delete, [this](const dpp::integration_delete_t& e) { OnIntegrationDelete(e); });
			break;
		case CallbackId::InviteCreate:
			reg(m_cluster->on_invite_create, [this](const dpp::invite_create_t& e) { OnInviteCreate(e); });
			break;
		case CallbackId::InviteDelete:
			reg(m_cluster->on_invite_delete, [this](const dpp::invite_delete_t& e) { OnInviteDelete(e); });
			break;
		case CallbackId::InteractionCreate:
			reg(m_cluster->on_interaction_create, [this](const dpp::interaction_create_t& e) { OnInteractionCreate(e); });
			break;
		case CallbackId::Slashcommand:
			reg(m_cluster->on_slashcommand, [this](const dpp::slashcommand_t& e) { OnSlashcommand(e); });
			break;
		case CallbackId::Autocomplete:
			reg(m_cluster->on_autocomplete, [this](const dpp::autocomplete_t& e) { OnAutocomplete(e); });
			break;
		case CallbackId::ButtonClick:
			reg(m_cluster->on_button_click, [this](const dpp::button_click_t& e) { OnButtonClick(e); });
			break;
		case CallbackId::SelectClick:
			reg(m_cluster->on_select_click, [this](const dpp::select_click_t& e) { OnSelectClick(e); });
			break;
		case CallbackId::FormSubmit:
			reg(m_cluster->on_form_submit, [this](const dpp::form_submit_t& e) { OnFormSubmit(e); });
			break;
		case CallbackId::UserContextMenu:
			reg(m_cluster->on_user_context_menu, [this](const dpp::user_context_menu_t& e) { OnUserContextMenu(e); });
			break;
		case CallbackId::MessageContextMenu:
			reg(m_cluster->on_message_context_menu, [this](const dpp::message_context_menu_t& e) { OnMessageContextMenu(e); });
			break;
		case CallbackId::VoiceStateUpdate:
			reg(m_cluster->on_voice_state_update, [this](const dpp::voice_state_update_t& e) { OnVoiceStateUpdate(e); });
			break;
		case CallbackId::VoiceServerUpdate:
			reg(m_cluster->on_voice_server_update, [this](const dpp::voice_server_update_t& e) { OnVoiceServerUpdate(e); });
			break;
		case CallbackId::VoiceReady:
			reg(m_cluster->on_voice_ready, [this](const dpp::voice_ready_t& e) { OnVoiceReady(e); });
			break;
		case CallbackId::VoiceReceive:
			reg(m_cluster->on_voice_receive, [this](const dpp::voice_receive_t& e) { OnVoiceReceive(e); });
			break;
		case CallbackId::VoiceTrackMarker:
			reg(m_cluster->on_voice_track_marker, [this](const dpp::voice_track_marker_t& e) { OnVoiceTrackMarker(e); });
			break;
		case CallbackId::VoiceClientDisconnect:
			reg(m_cluster->on_voice_client_disconnect, [this](const dpp::voice_client_disconnect_t& e) { OnVoiceClientDisconnect(e); });
			break;
		case CallbackId::VoiceClientSpeaking:
			reg(m_cluster->on_voice_client_speaking, [this](const dpp::voice_client_speaking_t& e) { OnVoiceClientSpeaking(e); });
			break;
		case CallbackId::VoiceBufferSend:
			reg(m_cluster->on_voice_buffer_send, [this](const dpp::voice_buffer_send_t& e) { OnVoiceBufferSend(e); });
			break;
		case CallbackId::StageInstanceCreate:
			reg(m_cluster->on_stage_instance_create, [this](const dpp::stage_instance_create_t& e) { OnStageInstanceCreate(e); });
			break;
		case CallbackId::StageInstanceUpdate:
			reg(m_cluster->on_stage_instance_update, [this](const dpp::stage_instance_update_t& e) { OnStageInstanceUpdate(e); });
			break;
		case CallbackId::StageInstanceDelete:
			reg(m_cluster->on_stage_instance_delete, [this](const dpp::stage_instance_delete_t& e) { OnStageInstanceDelete(e); });
			break;
		case CallbackId::PresenceUpdate:
			reg(m_cluster->on_presence_update, [this](const dpp::presence_update_t& e) { OnPresenceUpdate(e); });
			break;
		case CallbackId::TypingStart:
			reg(m_cluster->on_typing_start, [this](const dpp::typing_start_t& e) { OnTypingStart(e); });
			break;
		case CallbackId::UserUpdate:
			reg(m_cluster->on_user_update, [this](const dpp::user_update_t& e) { OnUserUpdate(e); });
			break;
		case CallbackId::AutomodRuleCreate:
			reg(m_cluster->on_automod_rule_create, [this](const dpp::automod_rule_create_t& e) { OnAutomodRuleCreate(e); });
			break;
		case CallbackId::AutomodRuleUpdate:
			reg(m_cluster->on_automod_rule_update, [this](const dpp::automod_rule_update_t& e) { OnAutomodRuleUpdate(e); });
			break;
		case CallbackId::AutomodRuleDelete:
			reg(m_cluster->on_automod_rule_delete, [this](const dpp::automod_rule_delete_t& e) { OnAutomodRuleDelete(e); });
			break;
		case CallbackId::AutomodRuleExecute:
			reg(m_cluster->on_automod_rule_execute, [this](const dpp::automod_rule_execute_t& e) { OnAutomodRuleExecute(e); });
			break;
		case CallbackId::EntitlementCreate:
			reg(m_cluster->on_entitlement_create, [this](const dpp::entitlement_create_t& e) { OnEntitlementCreate(e); });
			break;
		case CallbackId::EntitlementUpdate:
			reg(m_cluster->on_entitlement_update, [this](const dpp::entitlement_update_t& e) { OnEntitlementUpdate(e); });
			break;
		case CallbackId::EntitlementDelete:
			reg(m_cluster->on_entitlement_delete, [this](const dpp::entitlement_delete_t& e) { OnEntitlementDelete(e); });
			break;
		case CallbackId::WebhooksUpdate:
			reg(m_cluster->on_webhooks_update, [this](const dpp::webhooks_update_t& e) { OnWebhooksUpdate(e); });
			break;
		case CallbackId::Log:
			reg(m_cluster->on_log, [this](const dpp::log_t& e) { OnLog(e); });
			break;
		default:
			break;
	}
}

void EventHandler::Detach() {
	for (size_t i = 0; i < m_detachFuncs.size(); ++i) {
		if (m_detachFuncs[i]) {
			m_detachFuncs[i]();
			m_detachFuncs[i] = nullptr;
		}
	}
	m_registeredEvents.reset();
}

void EventHandler::UnregisterEvent(CallbackId id) {
	size_t idx = static_cast<size_t>(id);
	if (!m_registeredEvents.test(idx)) return;

	if (m_detachFuncs[idx]) {
		m_detachFuncs[idx]();
		m_detachFuncs[idx] = nullptr;
	}
	m_registeredEvents.reset(idx);
	m_callbacks.Get(id).Release();
}

void EventHandler::DispatchEvent(CallbackId id, DiscordEvent* event) {
	auto& cb = m_callbacks.Get(id);
	if (!cb.IsValid()) {
		delete event;
		return;
	}

	Handle_t clientHandle = m_client->GetHandle();
	IChangeableForward* forward = cb.forward;
	cell_t data = cb.data;

	Tasks.Push([clientHandle, forward, data, event]() {
		Handle_t eventHandle = Handles.CreateCallback(event, HandleId::DiscordEvent);
		if (!eventHandle) return;

		forward->PushCell(clientHandle);
		forward->PushCell(eventHandle);
		forward->PushCell(data);
		forward->Execute(nullptr);

		Handles.FreeHandle(eventHandle);
	});
}

void EventHandler::OnReady(const dpp::ready_t& event) {
	DiscordEvent* ev = new DiscordEvent("ready", m_client);
	ev->SetString("session_id", event.session_id);
	ev->SetInt("shard_id", event.shard_id);
	ev->SetInt("guild_count", event.guild_count);

	DiscordHandleArray* guild_ids = new DiscordHandleArray(false);
	for (const auto& guild : event.guilds) {
		guild_ids->AddString(guild.str());
	}
	Handle_t guild_ids_handle = Handles.CreateCallback(guild_ids, HandleId::DiscordHandleArray);
	ev->SetHandle("guild_ids", guild_ids_handle);

	DispatchEvent(CallbackId::Ready, ev);
}

void EventHandler::OnResumed(const dpp::resumed_t& event) {
	DiscordEvent* ev = new DiscordEvent("resumed", m_client);
	ev->SetString("session_id", event.session_id);
	ev->SetInt("shard_id", event.shard_id);
	DispatchEvent(CallbackId::Resumed, ev);
}

void EventHandler::OnMessageCreate(const dpp::message_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_create", m_client);

	DiscordMessage* msg = new DiscordMessage(event.msg, m_client);
	Handle_t msgHandle = Handles.CreateCallback(msg, HandleId::DiscordMessage);
	ev->SetHandle("message", msgHandle);

	ev->SetString("message_id", event.msg.id.str());
	ev->SetString("channel_id", event.msg.channel_id.str());
	ev->SetString("guild_id", event.msg.guild_id.str());
	ev->SetString("author_id", event.msg.author.id.str());
	ev->SetString("content", event.msg.content);

	DispatchEvent(CallbackId::MessageCreate, ev);
}

void EventHandler::OnMessageDelete(const dpp::message_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_delete", m_client);
	ev->SetString("message_id", event.id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.guild_id.str());
	DispatchEvent(CallbackId::MessageDelete, ev);
}

void EventHandler::OnMessageUpdate(const dpp::message_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_update", m_client);

	DiscordMessage* msg = new DiscordMessage(event.msg, m_client);
	Handle_t msgHandle = Handles.CreateCallback(msg, HandleId::DiscordMessage);
	ev->SetHandle("message", msgHandle);

	ev->SetString("message_id", event.msg.id.str());
	ev->SetString("channel_id", event.msg.channel_id.str());
	ev->SetString("guild_id", event.msg.guild_id.str());

	DispatchEvent(CallbackId::MessageUpdate, ev);
}

void EventHandler::OnMessageReactionAdd(const dpp::message_reaction_add_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_reaction_add", m_client);
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.reacting_guild.id ? event.reacting_guild.id.str() : "");
	ev->SetString("user_id", event.reacting_user.id.str());
	ev->SetString("message_author_id", event.message_author_id.str());

	if (event.reacting_guild.id) {
		DiscordGuild* guild = new DiscordGuild(event.reacting_guild, m_client);
		Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
		ev->SetHandle("guild", guildHandle);
	}

	std::string emoji = event.reacting_emoji.id
		? (event.reacting_emoji.name + ":" + event.reacting_emoji.id.str())
		: event.reacting_emoji.name;
	ev->SetString("emoji", emoji);

	DispatchEvent(CallbackId::MessageReactionAdd, ev);
}

void EventHandler::OnMessageReactionRemove(const dpp::message_reaction_remove_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_reaction_remove", m_client);
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.reacting_guild.id ? event.reacting_guild.id.str() : "");
	ev->SetString("user_id", event.reacting_user_id.str());

	if (event.reacting_guild.id) {
		DiscordGuild* guild = new DiscordGuild(event.reacting_guild, m_client);
		Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
		ev->SetHandle("guild", guildHandle);
	}

	std::string emoji = event.reacting_emoji.id
		? (event.reacting_emoji.name + ":" + event.reacting_emoji.id.str())
		: event.reacting_emoji.name;
	ev->SetString("emoji", emoji);

	DispatchEvent(CallbackId::MessageReactionRemove, ev);
}

void EventHandler::OnMessageReactionRemoveAll(const dpp::message_reaction_remove_all_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_reaction_remove_all", m_client);
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.reacting_guild.id.str());

	if (event.reacting_guild.id) {
		DiscordGuild* guild = new DiscordGuild(event.reacting_guild, m_client);
		Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
		ev->SetHandle("guild", guildHandle);
	}

	DispatchEvent(CallbackId::MessageReactionRemoveAll, ev);
}

void EventHandler::OnMessageReactionRemoveEmoji(const dpp::message_reaction_remove_emoji_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_reaction_remove_emoji", m_client);
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.reacting_guild.id.str());

	if (event.reacting_guild.id) {
		DiscordGuild* guild = new DiscordGuild(event.reacting_guild, m_client);
		Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
		ev->SetHandle("guild", guildHandle);
	}

	std::string emoji = event.reacting_emoji.id
		? (event.reacting_emoji.name + ":" + event.reacting_emoji.id.str())
		: event.reacting_emoji.name;
	ev->SetString("emoji", emoji);

	DispatchEvent(CallbackId::MessageReactionRemoveEmoji, ev);
}

void EventHandler::OnGuildMemberAdd(const dpp::guild_member_add_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_member_add", m_client);
	ev->SetString("user_id", event.added.user_id.str());
	ev->SetString("guild_id", event.added.guild_id.str());

	dpp::user* user = event.added.get_user();
	ev->SetString("username", user ? user->username : "");

	DiscordGuildMember* member = new DiscordGuildMember(event.added, m_client);
	Handle_t memberHandle = Handles.CreateCallback(member, HandleId::DiscordGuildMember);
	ev->SetHandle("member", memberHandle);

	DispatchEvent(CallbackId::GuildMemberAdd, ev);
}

void EventHandler::OnGuildMemberRemove(const dpp::guild_member_remove_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_member_remove", m_client);
	ev->SetString("user_id", event.removed.id.str());
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("username", event.removed.username);

	DiscordUser* user = new DiscordUser(event.removed, m_client);
	Handle_t userHandle = Handles.CreateCallback(user, HandleId::DiscordUser);
	ev->SetHandle("user", userHandle);

	DispatchEvent(CallbackId::GuildMemberRemove, ev);
}

void EventHandler::OnLog(const dpp::log_t& event) {
	DiscordEvent* ev = new DiscordEvent("log", m_client);
	ev->SetInt("severity", static_cast<int>(event.severity));
	ev->SetString("message", event.message);
	DispatchEvent(CallbackId::Log, ev);
}

void EventHandler::OnSlashcommand(const dpp::slashcommand_t& event) {
	DiscordEvent* ev = new DiscordEvent("slashcommand", m_client);

	DiscordInteraction* interaction = new DiscordInteraction(event, m_client);
	Handle_t interactionHandle = Handles.CreateCallback(interaction, HandleId::DiscordInteraction);
	ev->SetHandle("interaction", interactionHandle);

	ev->SetString("command_name", event.command.get_command_name());
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());

	DispatchEvent(CallbackId::Slashcommand, ev);
}

void EventHandler::OnAutocomplete(const dpp::autocomplete_t& event) {
	DiscordEvent* ev = new DiscordEvent("autocomplete", m_client);

	DiscordAutocompleteInteraction* interaction = new DiscordAutocompleteInteraction(event, m_client);
	Handle_t interactionHandle = Handles.CreateCallback(interaction, HandleId::DiscordAutocompleteInteraction);
	ev->SetHandle("interaction", interactionHandle);

	ev->SetString("command_name", event.command.get_command_name());
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());

	DispatchEvent(CallbackId::Autocomplete, ev);
}

void EventHandler::OnButtonClick(const dpp::button_click_t& event) {
	DiscordEvent* ev = new DiscordEvent("button_click", m_client);

	DiscordButtonInteraction* interaction = new DiscordButtonInteraction(event, m_client);
	Handle_t interactionHandle = Handles.CreateCallback(interaction, HandleId::DiscordButtonInteraction);
	ev->SetHandle("interaction", interactionHandle);

	ev->SetString("custom_id", event.custom_id);
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());

	DispatchEvent(CallbackId::ButtonClick, ev);
}

void EventHandler::OnSelectClick(const dpp::select_click_t& event) {
	DiscordEvent* ev = new DiscordEvent("select_click", m_client);

	DiscordSelectInteraction* interaction = new DiscordSelectInteraction(event, m_client);
	Handle_t interactionHandle = Handles.CreateCallback(interaction, HandleId::DiscordSelectInteraction);
	ev->SetHandle("interaction", interactionHandle);

	ev->SetString("custom_id", event.custom_id);
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());

	DispatchEvent(CallbackId::SelectClick, ev);
}

void EventHandler::OnFormSubmit(const dpp::form_submit_t& event) {
	DiscordEvent* ev = new DiscordEvent("form_submit", m_client);

	DiscordModalInteraction* interaction = new DiscordModalInteraction(event, m_client);
	Handle_t interactionHandle = Handles.CreateCallback(interaction, HandleId::DiscordModalInteraction);
	ev->SetHandle("interaction", interactionHandle);

	ev->SetString("custom_id", event.custom_id);
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());

	DispatchEvent(CallbackId::FormSubmit, ev);
}

void EventHandler::OnMessagePollVoteAdd(const dpp::message_poll_vote_add_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_poll_vote_add", m_client);
	ev->SetString("user_id", event.user_id.str());
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetInt("answer_id", event.answer_id);
	DispatchEvent(CallbackId::MessagePollVoteAdd, ev);
}

void EventHandler::OnMessagePollVoteRemove(const dpp::message_poll_vote_remove_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_poll_vote_remove", m_client);
	ev->SetString("user_id", event.user_id.str());
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetInt("answer_id", event.answer_id);
	DispatchEvent(CallbackId::MessagePollVoteRemove, ev);
}

void EventHandler::OnMessageDeleteBulk(const dpp::message_delete_bulk_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_delete_bulk", m_client);
	ev->SetString("channel_id", event.deleting_channel.id.str());
	ev->SetString("guild_id", event.deleting_guild.id.str());

	DiscordHandleArray* message_ids = new DiscordHandleArray(false);
	for (const auto& id : event.deleted) {
		message_ids->AddString(id.str());
	}
	Handle_t message_ids_handle = Handles.CreateCallback(message_ids, HandleId::DiscordHandleArray);
	ev->SetHandle("message_ids", message_ids_handle);

	DispatchEvent(CallbackId::MessageDeleteBulk, ev);
}

void EventHandler::OnChannelCreate(const dpp::channel_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("channel_create", m_client);
	DiscordChannel* channel = new DiscordChannel(event.created, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("channel", channelHandle);
	ev->SetString("channel_id", event.created.id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetString("name", event.created.name);
	ev->SetInt("type", static_cast<int>(event.created.get_type()));
	DispatchEvent(CallbackId::ChannelCreate, ev);
}

void EventHandler::OnChannelUpdate(const dpp::channel_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("channel_update", m_client);
	DiscordChannel* channel = new DiscordChannel(event.updated, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("channel", channelHandle);
	ev->SetString("channel_id", event.updated.id.str());
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("name", event.updated.name);
	DispatchEvent(CallbackId::ChannelUpdate, ev);
}

void EventHandler::OnChannelDelete(const dpp::channel_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("channel_delete", m_client);
	DiscordChannel* channel = new DiscordChannel(event.deleted, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("channel", channelHandle);
	ev->SetString("channel_id", event.deleted.id.str());
	ev->SetString("guild_id", event.deleted.guild_id.str());
	DispatchEvent(CallbackId::ChannelDelete, ev);
}

void EventHandler::OnChannelPinsUpdate(const dpp::channel_pins_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("channel_pins_update", m_client);
	ev->SetString("channel_id", event.pin_channel.id.str());
	ev->SetString("guild_id", event.pin_guild.id.str());
	ev->SetString("timestamp", std::to_string(event.timestamp));
	DispatchEvent(CallbackId::ChannelPinsUpdate, ev);
}

void EventHandler::OnThreadCreate(const dpp::thread_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_create", m_client);
	ev->SetString("thread_id", event.created.id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetString("parent_id", event.created.parent_id.str());
	ev->SetString("name", event.created.name);
	ev->SetString("owner_id", event.created.owner_id.str());
	ev->SetInt("type", static_cast<int>(event.created.get_type()));

	DiscordChannel* channel = new DiscordChannel(event.created, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("thread", channelHandle);

	DispatchEvent(CallbackId::ThreadCreate, ev);
}

void EventHandler::OnThreadUpdate(const dpp::thread_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_update", m_client);
	ev->SetString("thread_id", event.updated.id.str());
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("name", event.updated.name);

	DiscordChannel* channel = new DiscordChannel(event.updated, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("thread", channelHandle);

	DispatchEvent(CallbackId::ThreadUpdate, ev);
}

void EventHandler::OnThreadDelete(const dpp::thread_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_delete", m_client);
	ev->SetString("thread_id", event.deleted.id.str());
	ev->SetString("guild_id", event.deleted.guild_id.str());
	ev->SetString("parent_id", event.deleted.parent_id.str());

	DiscordChannel* channel = new DiscordChannel(event.deleted, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("thread", channelHandle);

	DispatchEvent(CallbackId::ThreadDelete, ev);
}

void EventHandler::OnThreadListSync(const dpp::thread_list_sync_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_list_sync", m_client);
	ev->SetString("guild_id", event.updating_guild.id.str());
	ev->SetInt("thread_count", event.threads.size());
	DispatchEvent(CallbackId::ThreadListSync, ev);
}

void EventHandler::OnThreadMemberUpdate(const dpp::thread_member_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_member_update", m_client);
	ev->SetString("thread_id", event.updated.thread_id.str());
	ev->SetString("user_id", event.updated.user_id.str());

	DiscordThreadMember* member = new DiscordThreadMember(event.updated);
	Handle_t memberHandle = Handles.CreateCallback(member, HandleId::DiscordThreadMember);
	ev->SetHandle("member", memberHandle);

	DispatchEvent(CallbackId::ThreadMemberUpdate, ev);
}

void EventHandler::OnThreadMembersUpdate(const dpp::thread_members_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("thread_members_update", m_client);
	ev->SetString("thread_id", event.thread_id.str());
	ev->SetString("guild_id", event.updating_guild ? event.updating_guild->id.str() : "");
	ev->SetInt("member_count", event.member_count);

	DiscordHandleArray* added_members = new DiscordHandleArray(false);
	for (const auto& member : event.added) {
		DiscordThreadMember* threadMember = new DiscordThreadMember(member);
		Handle_t h = Handles.CreateCallback(threadMember, HandleId::DiscordThreadMember);
		added_members->Add(h);
	}
	Handle_t added_handle = Handles.CreateCallback(added_members, HandleId::DiscordHandleArray);
	ev->SetHandle("added_members", added_handle);

	DiscordHandleArray* removed_user_ids = new DiscordHandleArray(false);
	for (const auto& id : event.removed_ids) {
		removed_user_ids->AddString(id.str());
	}
	Handle_t removed_handle = Handles.CreateCallback(removed_user_ids, HandleId::DiscordHandleArray);
	ev->SetHandle("removed_user_ids", removed_handle);

	DispatchEvent(CallbackId::ThreadMembersUpdate, ev);
}

void EventHandler::OnGuildCreate(const dpp::guild_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_create", m_client);
	DiscordGuild* guild = new DiscordGuild(event.created, m_client);
	Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
	ev->SetHandle("guild", guildHandle);
	ev->SetString("guild_id", event.created.id.str());
	ev->SetString("name", event.created.name);
	ev->SetInt("member_count", event.created.member_count);
	DispatchEvent(CallbackId::GuildCreate, ev);
}

void EventHandler::OnGuildUpdate(const dpp::guild_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_update", m_client);
	DiscordGuild* guild = new DiscordGuild(event.updated, m_client);
	Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
	ev->SetHandle("guild", guildHandle);
	ev->SetString("guild_id", event.updated.id.str());
	ev->SetString("name", event.updated.name);
	DispatchEvent(CallbackId::GuildUpdate, ev);
}

void EventHandler::OnGuildDelete(const dpp::guild_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_delete", m_client);
	ev->SetString("guild_id", event.deleted.id.str());
	ev->SetBool("unavailable", event.deleted.is_unavailable());
	DispatchEvent(CallbackId::GuildDelete, ev);
}

void EventHandler::OnGuildBanAdd(const dpp::guild_ban_add_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_ban_add", m_client);
	ev->SetString("guild_id", event.banning_guild.id.str());
	ev->SetString("user_id", event.banned.id.str());
	ev->SetString("username", event.banned.username);

	DiscordUser* user = new DiscordUser(event.banned, m_client);
	Handle_t userHandle = Handles.CreateCallback(user, HandleId::DiscordUser);
	ev->SetHandle("user", userHandle);

	DispatchEvent(CallbackId::GuildBanAdd, ev);
}

void EventHandler::OnGuildBanRemove(const dpp::guild_ban_remove_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_ban_remove", m_client);
	ev->SetString("guild_id", event.unbanning_guild.id.str());
	ev->SetString("user_id", event.unbanned.id.str());
	ev->SetString("username", event.unbanned.username);

	DiscordUser* user = new DiscordUser(event.unbanned, m_client);
	Handle_t userHandle = Handles.CreateCallback(user, HandleId::DiscordUser);
	ev->SetHandle("user", userHandle);

	DispatchEvent(CallbackId::GuildBanRemove, ev);
}

void EventHandler::OnGuildEmojisUpdate(const dpp::guild_emojis_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_emojis_update", m_client);
	ev->SetString("guild_id", event.updating_guild.id.str());
	ev->SetInt("emoji_count", event.emojis.size());

	DiscordHandleArray* emojis = new DiscordHandleArray(true);
	for (const auto& emoji : event.emojis) {
		DiscordEmoji* emojiObj = new DiscordEmoji(emoji, event.updating_guild.id, m_client);
		Handle_t emojiHandle = Handles.CreateCallback(emojiObj, HandleId::DiscordEmoji);
		emojis->Add(emojiHandle);
	}
	Handle_t emojisHandle = Handles.CreateCallback(emojis, HandleId::DiscordHandleArray);
	ev->SetHandle("emojis", emojisHandle);

	DispatchEvent(CallbackId::GuildEmojisUpdate, ev);
}

void EventHandler::OnGuildStickersUpdate(const dpp::guild_stickers_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_stickers_update", m_client);
	ev->SetString("guild_id", event.updating_guild.id.str());
	ev->SetInt("sticker_count", event.stickers.size());

	DiscordHandleArray* stickers = new DiscordHandleArray(true);
	for (const auto& sticker : event.stickers) {
		DiscordSticker* stickerObj = new DiscordSticker(sticker, m_client);
		Handle_t stickerHandle = Handles.CreateCallback(stickerObj, HandleId::DiscordSticker);
		stickers->Add(stickerHandle);
	}
	Handle_t stickersHandle = Handles.CreateCallback(stickers, HandleId::DiscordHandleArray);
	ev->SetHandle("stickers", stickersHandle);

	DispatchEvent(CallbackId::GuildStickersUpdate, ev);
}

void EventHandler::OnGuildIntegrationsUpdate(const dpp::guild_integrations_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_integrations_update", m_client);
	ev->SetString("guild_id", event.updating_guild.id.str());
	DispatchEvent(CallbackId::GuildIntegrationsUpdate, ev);
}

void EventHandler::OnGuildMemberUpdate(const dpp::guild_member_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_member_update", m_client);
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("user_id", event.updated.user_id.str());
	ev->SetString("nickname", event.updated.get_nickname());

	DiscordGuildMember* member = new DiscordGuildMember(event.updated, m_client);
	Handle_t memberHandle = Handles.CreateCallback(member, HandleId::DiscordGuildMember);
	ev->SetHandle("member", memberHandle);

	DispatchEvent(CallbackId::GuildMemberUpdate, ev);
}

void EventHandler::OnGuildMembersChunk(const dpp::guild_members_chunk_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_members_chunk", m_client);
	ev->SetString("guild_id", event.adding.id.str());
	ev->SetInt("member_count", event.members.size());
	DispatchEvent(CallbackId::GuildMembersChunk, ev);
}

void EventHandler::OnGuildRoleCreate(const dpp::guild_role_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_role_create", m_client);
	DiscordRole* role = new DiscordRole(event.created, event.creating_guild.id, m_client);
	Handle_t roleHandle = Handles.CreateCallback(role, HandleId::DiscordRole);
	ev->SetHandle("role", roleHandle);
	ev->SetString("guild_id", event.creating_guild.id.str());
	ev->SetString("role_id", event.created.id.str());
	ev->SetString("name", event.created.name);
	DispatchEvent(CallbackId::GuildRoleCreate, ev);
}

void EventHandler::OnGuildRoleUpdate(const dpp::guild_role_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_role_update", m_client);
	DiscordRole* role = new DiscordRole(event.updated, event.updating_guild.id, m_client);
	Handle_t roleHandle = Handles.CreateCallback(role, HandleId::DiscordRole);
	ev->SetHandle("role", roleHandle);
	ev->SetString("guild_id", event.updating_guild.id.str());
	ev->SetString("role_id", event.updated.id.str());
	ev->SetString("name", event.updated.name);
	DispatchEvent(CallbackId::GuildRoleUpdate, ev);
}

void EventHandler::OnGuildRoleDelete(const dpp::guild_role_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_role_delete", m_client);
	ev->SetString("guild_id", event.deleting_guild.id.str());
	ev->SetString("role_id", event.deleted.id.str());
	DispatchEvent(CallbackId::GuildRoleDelete, ev);
}

void EventHandler::OnGuildScheduledEventCreate(const dpp::guild_scheduled_event_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_scheduled_event_create", m_client);
	ev->SetString("event_id", event.created.id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetString("name", event.created.name);
	ev->SetString("description", event.created.description);
	ev->SetString("channel_id", event.created.channel_id.str());
	ev->SetString("creator_id", event.created.creator_id.str());
	ev->SetString("scheduled_start_time", std::to_string(event.created.scheduled_start_time));
	ev->SetString("scheduled_end_time", std::to_string(event.created.scheduled_end_time));
	ev->SetInt("entity_type", static_cast<int>(event.created.entity_type));
	ev->SetInt("status", static_cast<int>(event.created.status));
	ev->SetInt("user_count", event.created.user_count);

	DiscordScheduledEvent* scheduledEvent = new DiscordScheduledEvent(event.created, m_client);
	Handle_t eventHandle = Handles.CreateCallback(scheduledEvent, HandleId::DiscordScheduledEvent);
	ev->SetHandle("scheduled_event", eventHandle);

	DispatchEvent(CallbackId::GuildScheduledEventCreate, ev);
}

void EventHandler::OnGuildScheduledEventUpdate(const dpp::guild_scheduled_event_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_scheduled_event_update", m_client);
	ev->SetString("event_id", event.updated.id.str());
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("name", event.updated.name);
	ev->SetString("description", event.updated.description);
	ev->SetString("channel_id", event.updated.channel_id.str());
	ev->SetString("scheduled_start_time", std::to_string(event.updated.scheduled_start_time));
	ev->SetString("scheduled_end_time", std::to_string(event.updated.scheduled_end_time));
	ev->SetInt("entity_type", static_cast<int>(event.updated.entity_type));
	ev->SetInt("status", static_cast<int>(event.updated.status));
	ev->SetInt("user_count", event.updated.user_count);

	DiscordScheduledEvent* scheduledEvent = new DiscordScheduledEvent(event.updated, m_client);
	Handle_t eventHandle = Handles.CreateCallback(scheduledEvent, HandleId::DiscordScheduledEvent);
	ev->SetHandle("scheduled_event", eventHandle);

	DispatchEvent(CallbackId::GuildScheduledEventUpdate, ev);
}

void EventHandler::OnGuildScheduledEventDelete(const dpp::guild_scheduled_event_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_scheduled_event_delete", m_client);
	ev->SetString("event_id", event.deleted.id.str());
	ev->SetString("guild_id", event.deleted.guild_id.str());

	DiscordScheduledEvent* scheduledEvent = new DiscordScheduledEvent(event.deleted, m_client);
	Handle_t eventHandle = Handles.CreateCallback(scheduledEvent, HandleId::DiscordScheduledEvent);
	ev->SetHandle("scheduled_event", eventHandle);

	DispatchEvent(CallbackId::GuildScheduledEventDelete, ev);
}

void EventHandler::OnGuildScheduledEventUserAdd(const dpp::guild_scheduled_event_user_add_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_scheduled_event_user_add", m_client);
	ev->SetString("event_id", event.event_id.str());
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("user_id", event.user_id.str());
	DispatchEvent(CallbackId::GuildScheduledEventUserAdd, ev);
}

void EventHandler::OnGuildScheduledEventUserRemove(const dpp::guild_scheduled_event_user_remove_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_scheduled_event_user_remove", m_client);
	ev->SetString("event_id", event.event_id.str());
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("user_id", event.user_id.str());
	DispatchEvent(CallbackId::GuildScheduledEventUserRemove, ev);
}

void EventHandler::OnGuildAuditLogEntryCreate(const dpp::guild_audit_log_entry_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_audit_log_entry_create", m_client);
	ev->SetString("entry_id", event.entry.id.str());
	ev->SetString("user_id", event.entry.user_id.str());
	ev->SetString("target_id", event.entry.target_id.str());
	ev->SetInt("action_type", static_cast<int>(event.entry.type));
	DispatchEvent(CallbackId::GuildAuditLogEntryCreate, ev);
}

void EventHandler::OnGuildJoinRequestDelete(const dpp::guild_join_request_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("guild_join_request_delete", m_client);
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("user_id", event.user_id.str());
	DispatchEvent(CallbackId::GuildJoinRequestDelete, ev);
}

void EventHandler::OnIntegrationCreate(const dpp::integration_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("integration_create", m_client);
	ev->SetString("integration_id", event.created_integration.id.str());
	ev->SetString("name", event.created_integration.name);
	ev->SetInt("type", static_cast<int>(event.created_integration.type));
	DispatchEvent(CallbackId::IntegrationCreate, ev);
}

void EventHandler::OnIntegrationUpdate(const dpp::integration_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("integration_update", m_client);
	ev->SetString("integration_id", event.updated_integration.id.str());
	ev->SetString("name", event.updated_integration.name);
	DispatchEvent(CallbackId::IntegrationUpdate, ev);
}

void EventHandler::OnIntegrationDelete(const dpp::integration_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("integration_delete", m_client);
	ev->SetString("integration_id", event.deleted_integration.id.str());
	DispatchEvent(CallbackId::IntegrationDelete, ev);
}

void EventHandler::OnInviteCreate(const dpp::invite_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("invite_create", m_client);
	ev->SetString("guild_id", event.created_invite.guild_id.str());
	ev->SetString("channel_id", event.created_invite.channel_id.str());
	ev->SetString("code", event.created_invite.code);
	ev->SetString("inviter_id", event.created_invite.inviter_id.str());
	ev->SetInt("max_uses", event.created_invite.max_uses);
	ev->SetInt("max_age", event.created_invite.max_age);
	ev->SetBool("temporary", event.created_invite.temporary);

	DiscordInvite* invite = new DiscordInvite(event.created_invite, m_client);
	Handle_t inviteHandle = Handles.CreateCallback(invite, HandleId::DiscordInvite);
	ev->SetHandle("invite", inviteHandle);

	DispatchEvent(CallbackId::InviteCreate, ev);
}

void EventHandler::OnInviteDelete(const dpp::invite_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("invite_delete", m_client);
	ev->SetString("guild_id", event.deleted_invite.guild_id.str());
	ev->SetString("channel_id", event.deleted_invite.channel_id.str());
	ev->SetString("code", event.deleted_invite.code);

	DiscordInvite* invite = new DiscordInvite(event.deleted_invite, m_client);
	Handle_t inviteHandle = Handles.CreateCallback(invite, HandleId::DiscordInvite);
	ev->SetHandle("invite", inviteHandle);

	DispatchEvent(CallbackId::InviteDelete, ev);
}

void EventHandler::OnInteractionCreate(const dpp::interaction_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("interaction_create", m_client);
	ev->SetString("interaction_id", event.command.id.str());
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());
	ev->SetInt("type", static_cast<int>(event.command.type));
	DispatchEvent(CallbackId::InteractionCreate, ev);
}

void EventHandler::OnUserContextMenu(const dpp::user_context_menu_t& event) {
	DiscordEvent* ev = new DiscordEvent("user_context_menu", m_client);
	ev->SetString("interaction_id", event.command.id.str());
	ev->SetString("command_name", event.command.get_command_name());
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());
	ev->SetString("target_user_id", event.get_user().id.str());
	DispatchEvent(CallbackId::UserContextMenu, ev);
}

void EventHandler::OnMessageContextMenu(const dpp::message_context_menu_t& event) {
	DiscordEvent* ev = new DiscordEvent("message_context_menu", m_client);
	ev->SetString("interaction_id", event.command.id.str());
	ev->SetString("command_name", event.command.get_command_name());
	ev->SetString("guild_id", event.command.guild_id.str());
	ev->SetString("channel_id", event.command.channel_id.str());
	ev->SetString("user_id", event.command.usr.id.str());
	ev->SetString("target_message_id", event.get_message().id.str());
	DispatchEvent(CallbackId::MessageContextMenu, ev);
}

void EventHandler::OnVoiceStateUpdate(const dpp::voice_state_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_state_update", m_client);
	ev->SetString("guild_id", event.state.guild_id.str());
	ev->SetString("channel_id", event.state.channel_id.str());
	ev->SetString("user_id", event.state.user_id.str());
	ev->SetString("session_id", event.state.session_id);
	ev->SetBool("deaf", event.state.is_deaf());
	ev->SetBool("mute", event.state.is_mute());
	ev->SetBool("self_deaf", event.state.is_self_deaf());
	ev->SetBool("self_mute", event.state.is_self_mute());
	ev->SetBool("suppress", event.state.is_suppressed());
	ev->SetBool("self_video", event.state.self_video());
	ev->SetBool("self_stream", event.state.self_stream());
	ev->SetString("request_to_speak_timestamp", std::to_string(event.state.request_to_speak));
	DispatchEvent(CallbackId::VoiceStateUpdate, ev);
}

void EventHandler::OnVoiceServerUpdate(const dpp::voice_server_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_server_update", m_client);
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("token", event.token);
	ev->SetString("endpoint", event.endpoint);
	DispatchEvent(CallbackId::VoiceServerUpdate, ev);
}

void EventHandler::OnVoiceReady(const dpp::voice_ready_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_ready", m_client);
	ev->SetString("voice_channel_id", event.voice_channel_id.str());
	DispatchEvent(CallbackId::VoiceReady, ev);
}

void EventHandler::OnVoiceReceive(const dpp::voice_receive_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_receive", m_client);
	ev->SetString("user_id", event.user_id.str());
	DispatchEvent(CallbackId::VoiceReceive, ev);
}

void EventHandler::OnVoiceTrackMarker(const dpp::voice_track_marker_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_track_marker", m_client);
	ev->SetString("track_meta", event.track_meta);
	DispatchEvent(CallbackId::VoiceTrackMarker, ev);
}

void EventHandler::OnVoiceClientDisconnect(const dpp::voice_client_disconnect_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_client_disconnect", m_client);
	ev->SetString("user_id", event.user_id.str());
	DispatchEvent(CallbackId::VoiceClientDisconnect, ev);
}

void EventHandler::OnVoiceClientSpeaking(const dpp::voice_client_speaking_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_client_speaking", m_client);
	ev->SetString("user_id", event.user_id.str());
	ev->SetInt("ssrc", event.ssrc);
	DispatchEvent(CallbackId::VoiceClientSpeaking, ev);
}

void EventHandler::OnVoiceBufferSend(const dpp::voice_buffer_send_t& event) {
	DiscordEvent* ev = new DiscordEvent("voice_buffer_send", m_client);
	ev->SetString("buffer_size", std::to_string(event.buffer_size));
	ev->SetInt("packets_left", event.packets_left);
	DispatchEvent(CallbackId::VoiceBufferSend, ev);
}

void EventHandler::OnStageInstanceCreate(const dpp::stage_instance_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("stage_instance_create", m_client);
	ev->SetString("stage_id", event.created.id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetString("channel_id", event.created.channel_id.str());
	ev->SetString("topic", event.created.topic);
	DispatchEvent(CallbackId::StageInstanceCreate, ev);
}

void EventHandler::OnStageInstanceUpdate(const dpp::stage_instance_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("stage_instance_update", m_client);
	ev->SetString("stage_id", event.updated.id.str());
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("topic", event.updated.topic);
	DispatchEvent(CallbackId::StageInstanceUpdate, ev);
}

void EventHandler::OnStageInstanceDelete(const dpp::stage_instance_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("stage_instance_delete", m_client);
	ev->SetString("stage_id", event.deleted.id.str());
	ev->SetString("guild_id", event.deleted.guild_id.str());
	ev->SetString("channel_id", event.deleted.channel_id.str());
	DispatchEvent(CallbackId::StageInstanceDelete, ev);
}

void EventHandler::OnPresenceUpdate(const dpp::presence_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("presence_update", m_client);
	ev->SetString("user_id", event.rich_presence.user_id.str());
	ev->SetString("guild_id", event.rich_presence.guild_id.str());
	ev->SetInt("status", static_cast<int>(event.rich_presence.status()));
	DispatchEvent(CallbackId::PresenceUpdate, ev);
}

void EventHandler::OnTypingStart(const dpp::typing_start_t& event) {
	DiscordEvent* ev = new DiscordEvent("typing_start", m_client);
	ev->SetString("user_id", event.user_id.str());
	ev->SetString("channel_id", event.typing_channel.id.str());
	ev->SetString("guild_id", event.typing_guild.id ? event.typing_guild.id.str() : "");
	ev->SetString("timestamp", std::to_string(event.timestamp));

	DiscordChannel* channel = new DiscordChannel(event.typing_channel, m_client);
	Handle_t channelHandle = Handles.CreateCallback(channel, HandleId::DiscordChannel);
	ev->SetHandle("channel", channelHandle);

	if (event.typing_guild.id) {
		DiscordGuild* guild = new DiscordGuild(event.typing_guild, m_client);
		Handle_t guildHandle = Handles.CreateCallback(guild, HandleId::DiscordGuild);
		ev->SetHandle("guild", guildHandle);
	}

	if (event.typing_user.id) {
		DiscordUser* user = new DiscordUser(event.typing_user, m_client);
		Handle_t userHandle = Handles.CreateCallback(user, HandleId::DiscordUser);
		ev->SetHandle("user", userHandle);
	}

	DispatchEvent(CallbackId::TypingStart, ev);
}

void EventHandler::OnUserUpdate(const dpp::user_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("user_update", m_client);
	ev->SetString("user_id", event.updated.id.str());
	ev->SetString("username", event.updated.username);
	ev->SetString("global_name", event.updated.global_name);

	DiscordUser* user = new DiscordUser(event.updated, m_client);
	Handle_t userHandle = Handles.CreateCallback(user, HandleId::DiscordUser);
	ev->SetHandle("user", userHandle);

	DispatchEvent(CallbackId::UserUpdate, ev);
}

void EventHandler::OnAutomodRuleCreate(const dpp::automod_rule_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("automod_rule_create", m_client);
	ev->SetString("rule_id", event.created.id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetString("name", event.created.name);
	ev->SetInt("event_type", static_cast<int>(event.created.event_type));
	ev->SetInt("trigger_type", static_cast<int>(event.created.trigger_type));
	DispatchEvent(CallbackId::AutomodRuleCreate, ev);
}

void EventHandler::OnAutomodRuleUpdate(const dpp::automod_rule_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("automod_rule_update", m_client);
	ev->SetString("rule_id", event.updated.id.str());
	ev->SetString("guild_id", event.updated.guild_id.str());
	ev->SetString("name", event.updated.name);
	DispatchEvent(CallbackId::AutomodRuleUpdate, ev);
}

void EventHandler::OnAutomodRuleDelete(const dpp::automod_rule_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("automod_rule_delete", m_client);
	ev->SetString("rule_id", event.deleted.id.str());
	ev->SetString("guild_id", event.deleted.guild_id.str());
	DispatchEvent(CallbackId::AutomodRuleDelete, ev);
}

void EventHandler::OnAutomodRuleExecute(const dpp::automod_rule_execute_t& event) {
	DiscordEvent* ev = new DiscordEvent("automod_rule_execute", m_client);
	ev->SetString("guild_id", event.guild_id.str());
	ev->SetString("rule_id", event.rule_id.str());
	ev->SetString("user_id", event.user_id.str());
	ev->SetString("channel_id", event.channel_id.str());
	ev->SetString("message_id", event.message_id.str());
	ev->SetString("content", event.content);
	ev->SetInt("action_type", static_cast<int>(event.action.type));
	ev->SetInt("rule_trigger_type", static_cast<int>(event.rule_trigger_type));
	ev->SetString("alert_system_message_id", event.alert_system_message_id.str());
	ev->SetString("matched_keyword", event.matched_keyword);
	ev->SetString("matched_content", event.matched_content);
	DispatchEvent(CallbackId::AutomodRuleExecute, ev);
}

void EventHandler::OnEntitlementCreate(const dpp::entitlement_create_t& event) {
	DiscordEvent* ev = new DiscordEvent("entitlement_create", m_client);
	ev->SetString("entitlement_id", event.created.id.str());
	ev->SetString("sku_id", event.created.sku_id.str());
	ev->SetString("user_id", event.created.user_id.str());
	ev->SetString("guild_id", event.created.guild_id.str());
	ev->SetInt("type", static_cast<int>(event.created.type));
	DispatchEvent(CallbackId::EntitlementCreate, ev);
}

void EventHandler::OnEntitlementUpdate(const dpp::entitlement_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("entitlement_update", m_client);
	ev->SetString("entitlement_id", event.updating_entitlement.id.str());
	ev->SetString("sku_id", event.updating_entitlement.sku_id.str());
	DispatchEvent(CallbackId::EntitlementUpdate, ev);
}

void EventHandler::OnEntitlementDelete(const dpp::entitlement_delete_t& event) {
	DiscordEvent* ev = new DiscordEvent("entitlement_delete", m_client);
	ev->SetString("entitlement_id", event.deleted.id.str());
	ev->SetString("sku_id", event.deleted.sku_id.str());
	DispatchEvent(CallbackId::EntitlementDelete, ev);
}

void EventHandler::OnWebhooksUpdate(const dpp::webhooks_update_t& event) {
	DiscordEvent* ev = new DiscordEvent("webhooks_update", m_client);
	ev->SetString("guild_id", event.webhook_guild.id.str());
	ev->SetString("channel_id", event.webhook_channel.id.str());
	DispatchEvent(CallbackId::WebhooksUpdate, ev);
}
