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

#include "entities/discord_message.h"
#include "entities/discord_channel.h"
#include "entities/discord_user.h"
#include "utils/discord_common.h"
#include "utils/discord_component.h"
#include "utils/discord_embed.h"
#include "utils/discord_poll.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"

void DiscordMessage::AddComponent(const DiscordComponent* component) {
	if (component) m_message.add_component(component->GetComponent());
}

void DiscordMessage::Edit(const char* new_content, IPluginFunction* callback, cell_t data) {
	if (!m_client || !new_content) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().Edit(m_message.channel_id, m_message.id, new_content, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().Edit(m_message.channel_id, m_message.id, new_content);
	}
}

void DiscordMessage::EditEmbed(const char* new_content, const DiscordEmbed* embed, IPluginFunction* callback, cell_t data) {
	if (!m_client || !embed) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().EditEmbed(m_message.channel_id, m_message.id, new_content ? new_content : "", embed, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().EditEmbed(m_message.channel_id, m_message.id, new_content ? new_content : "", embed);
	}
}

void DiscordMessage::EditFromObject(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().EditMessage(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().EditMessage(this);
	}
}

void DiscordMessage::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().Delete(m_message.channel_id, m_message.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Messages().Delete(m_message.channel_id, m_message.id);
	}
}

void DiscordMessage::Pin(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().Pin(m_message.channel_id, m_message.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Pin);
		});
	} else {
		m_client->Messages().Pin(m_message.channel_id, m_message.id);
	}
}

void DiscordMessage::Unpin(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().Unpin(m_message.channel_id, m_message.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Unpin);
		});
	} else {
		m_client->Messages().Unpin(m_message.channel_id, m_message.id);
	}
}

void DiscordMessage::AddReaction(const char* emoji) {
	if (!m_client || !emoji) return;
	m_client->Messages().AddReaction(m_message.channel_id, m_message.id, emoji);
}

void DiscordMessage::RemoveReaction(const char* emoji) {
	if (!m_client || !emoji) return;
	m_client->Messages().RemoveReaction(m_message.channel_id, m_message.id, emoji);
}

void DiscordMessage::RemoveAllReactions() {
	if (!m_client) return;
	m_client->Messages().RemoveAllReactions(m_message.channel_id, m_message.id);
}

void DiscordMessage::Reply(const char* content, IPluginFunction* callback, cell_t data) {
	if (!m_client || !content) return;

	dpp::message reply_msg;
	reply_msg.set_content(content);
	reply_msg.set_channel_id(m_message.channel_id);
	reply_msg.message_reference.message_id = m_message.id;
	reply_msg.message_reference.channel_id = m_message.channel_id;
	reply_msg.message_reference.guild_id = m_message.guild_id;

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendDPPMessage(reply_msg, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendDPPMessage(reply_msg);
	}
}

void DiscordMessage::ReplyEmbed(const char* content, const DiscordEmbed* embed, IPluginFunction* callback, cell_t data) {
	if (!m_client || !embed) return;

	dpp::message reply_msg;
	if (content) {
		reply_msg.set_content(content);
	}
	reply_msg.set_channel_id(m_message.channel_id);
	reply_msg.message_reference.message_id = m_message.id;
	reply_msg.message_reference.channel_id = m_message.channel_id;
	reply_msg.message_reference.guild_id = m_message.guild_id;
	reply_msg.add_embed(embed->GetEmbed());

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendDPPMessage(reply_msg, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendDPPMessage(reply_msg);
	}
}

void DiscordMessage::ReplyFromObject(const DiscordMessage* reply_message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !reply_message) return;

	dpp::message reply_msg = reply_message->GetDPPMessage();
	reply_msg.set_channel_id(m_message.channel_id);
	reply_msg.message_reference.message_id = m_message.id;
	reply_msg.message_reference.channel_id = m_message.channel_id;
	reply_msg.message_reference.guild_id = m_message.guild_id;

	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendDPPMessage(reply_msg, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendDPPMessage(reply_msg);
	}
}

void DiscordMessage::Crosspost() {
	if (!m_client) return;
	m_client->Messages().Crosspost(m_message.channel_id, m_message.id);
}

void DiscordMessage::CreateThread(const char* name, int auto_archive_duration, IPluginFunction* callback, cell_t data) {
	if (!m_client || !name) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Threads().CreateWithMessage(m_message.channel_id, m_message.id, name, auto_archive_duration, 0, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordChannel, dpp::thread>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Threads().CreateWithMessage(m_message.channel_id, m_message.id, name, auto_archive_duration, 0);
	}
}

void DiscordMessage::Send(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (m_message.channel_id == 0) {
		Log.Error("Cannot send message: channel_id not set");
		return;
	}
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Messages().SendObject(this, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Messages().SendObject(this);
	}
}

void DiscordMessage::SetPoll(const DiscordPoll* poll) {
	if (poll) m_message.set_poll(poll->GetPoll());
}

DiscordPoll* DiscordMessage::GetPollInternal() const {
	if (!m_message.has_poll()) return nullptr;
	return new DiscordPoll(m_message.get_poll());
}

void DiscordMessage::EndPoll() {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->Messages().EndPoll(m_message);
}

bool DiscordMessage::GetPollAnswerVoters(uint32_t answer_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Messages().GetPollAnswerVoters(m_message, answer_id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordUser, dpp::user_map>(client_handle, client, callback, data, cb, DiscordResultType::Voters);
	});
	return true;
}
