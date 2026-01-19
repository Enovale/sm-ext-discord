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

#include "handle_manager.h"
#include "discord_result.h"
#include "dpp/dpp.h"

// Handle traits - maps wrapper types to their metadata
template<typename T, typename DppT = void>
struct HandleTraits;

// Single object traits
template<> struct HandleTraits<DiscordMessage> {
	using DppType = dpp::message;
	static constexpr DiscordResultType type = DiscordResultType::Message;
	static constexpr const char* key = "message";
};

template<> struct HandleTraits<DiscordChannel> {
	using DppType = dpp::channel;
	static constexpr DiscordResultType type = DiscordResultType::Channel;
	static constexpr const char* key = "channel";
};

template<> struct HandleTraits<DiscordChannel, dpp::channel> : HandleTraits<DiscordChannel> {};
template<> struct HandleTraits<DiscordChannel, dpp::thread> {
	using DppType = dpp::thread;
	static constexpr DiscordResultType type = DiscordResultType::Channel;
	static constexpr const char* key = "thread";
};

template<> struct HandleTraits<DiscordGuild> {
	using DppType = dpp::guild;
	static constexpr DiscordResultType type = DiscordResultType::Guild;
	static constexpr const char* key = "guild";
};

template<> struct HandleTraits<DiscordUser> {
	using DppType = dpp::user;
	static constexpr DiscordResultType type = DiscordResultType::User;
	static constexpr const char* key = "user";
};

template<> struct HandleTraits<DiscordUser, dpp::user> : HandleTraits<DiscordUser> {};
template<> struct HandleTraits<DiscordUser, dpp::user_identified> {
	using DppType = dpp::user_identified;
	static constexpr DiscordResultType type = DiscordResultType::User;
	static constexpr const char* key = "user";
};

template<> struct HandleTraits<DiscordGuildMember> {
	using DppType = dpp::guild_member;
	static constexpr DiscordResultType type = DiscordResultType::GuildMember;
	static constexpr const char* key = "member";
};

template<> struct HandleTraits<DiscordRole> {
	using DppType = dpp::role;
	static constexpr DiscordResultType type = DiscordResultType::Role;
	static constexpr const char* key = "role";
};

template<> struct HandleTraits<DiscordWebhook> {
	using DppType = dpp::webhook;
	static constexpr DiscordResultType type = DiscordResultType::Webhook;
	static constexpr const char* key = "webhook";
};

template<> struct HandleTraits<DiscordInvite> {
	using DppType = dpp::invite;
	static constexpr DiscordResultType type = DiscordResultType::Invite;
	static constexpr const char* key = "invite";
};

template<> struct HandleTraits<DiscordEmoji> {
	using DppType = dpp::emoji;
	static constexpr DiscordResultType type = DiscordResultType::Emoji;
	static constexpr const char* key = "emoji";
};

template<> struct HandleTraits<DiscordSticker> {
	using DppType = dpp::sticker;
	static constexpr DiscordResultType type = DiscordResultType::Sticker;
	static constexpr const char* key = "sticker";
};

template<> struct HandleTraits<DiscordScheduledEvent> {
	using DppType = dpp::scheduled_event;
	static constexpr DiscordResultType type = DiscordResultType::ScheduledEvent;
	static constexpr const char* key = "event";
};

template<> struct HandleTraits<DiscordPoll> {
	using DppType = dpp::poll;
	static constexpr DiscordResultType type = DiscordResultType::Poll;
	static constexpr const char* key = "poll";
};

template<> struct HandleTraits<DiscordAttachment> {
	using DppType = dpp::attachment;
	static constexpr DiscordResultType type = DiscordResultType::Attachment;
	static constexpr const char* key = "attachment";
};

template<> struct HandleTraits<DiscordReaction> {
	using DppType = dpp::reaction;
	static constexpr DiscordResultType type = DiscordResultType::Reaction;
	static constexpr const char* key = "reaction";
};

template<> struct HandleTraits<DiscordBan> {
	using DppType = dpp::ban;
	static constexpr DiscordResultType type = DiscordResultType::Ban;
	static constexpr const char* key = "ban";
};

template<> struct HandleTraits<DiscordThreadMember> {
	using DppType = dpp::thread_member;
	static constexpr DiscordResultType type = DiscordResultType::ThreadMember;
	static constexpr const char* key = "thread_member";
};

template<> struct HandleTraits<DiscordSlashCommand> {
	using DppType = dpp::slashcommand;
	static constexpr DiscordResultType type = DiscordResultType::SlashCommand;
	static constexpr const char* key = "command";
};

// Explicit DppType versions for types that need them
template<> struct HandleTraits<DiscordMessage, dpp::message> : HandleTraits<DiscordMessage> {};
template<> struct HandleTraits<DiscordGuild, dpp::guild> : HandleTraits<DiscordGuild> {};
template<> struct HandleTraits<DiscordGuildMember, dpp::guild_member> : HandleTraits<DiscordGuildMember> {};
template<> struct HandleTraits<DiscordRole, dpp::role> : HandleTraits<DiscordRole> {};
template<> struct HandleTraits<DiscordWebhook, dpp::webhook> : HandleTraits<DiscordWebhook> {};
template<> struct HandleTraits<DiscordInvite, dpp::invite> : HandleTraits<DiscordInvite> {};
template<> struct HandleTraits<DiscordEmoji, dpp::emoji> : HandleTraits<DiscordEmoji> {};
template<> struct HandleTraits<DiscordSticker, dpp::sticker> : HandleTraits<DiscordSticker> {};
template<> struct HandleTraits<DiscordScheduledEvent, dpp::scheduled_event> : HandleTraits<DiscordScheduledEvent> {};
template<> struct HandleTraits<DiscordPoll, dpp::poll> : HandleTraits<DiscordPoll> {};
template<> struct HandleTraits<DiscordAttachment, dpp::attachment> : HandleTraits<DiscordAttachment> {};
template<> struct HandleTraits<DiscordReaction, dpp::reaction> : HandleTraits<DiscordReaction> {};
template<> struct HandleTraits<DiscordBan, dpp::ban> : HandleTraits<DiscordBan> {};
template<> struct HandleTraits<DiscordThreadMember, dpp::thread_member> : HandleTraits<DiscordThreadMember> {};
template<> struct HandleTraits<DiscordSlashCommand, dpp::slashcommand> : HandleTraits<DiscordSlashCommand> {};
