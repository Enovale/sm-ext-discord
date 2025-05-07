#ifndef _INCLUDE_MESSAGE_H
#define _INCLUDE_MESSAGE_H

#include "object_handler.h"
#include "user.h"
#include "dpp/dpp.h"

class DiscordMessage : public DiscordObject
{
private:
	dpp::message m_message;

public:
	DiscordMessage(const dpp::message& msg) : m_message(msg) {}

	DiscordUser* GetAuthor() const { return new DiscordUser(m_message.author); }
	const char* GetContent() const { return m_message.content.c_str(); }
	std::string GetMessageId() const { return std::to_string(m_message.id); }
	std::string GetChannelId() const { return std::to_string(m_message.channel_id); }
	std::string GetGuildId() const { return std::to_string(m_message.guild_id); }
	std::string GetAuthorId() const { return std::to_string(m_message.author.id); }
	const char* GetAuthorName() const { return m_message.author.username.c_str(); }
	const char* GetAuthorDisplayName() const { return m_message.author.global_name.c_str(); }
	std::string GetAuthorNickname() const { return m_message.member.get_nickname(); }
	const uint16_t GetAuthorDiscriminator() const { return m_message.author.discriminator; }
	bool IsPinned() const { return m_message.pinned; }
	bool IsTTS() const { return m_message.tts; }
	bool IsMentionEveryone() const { return m_message.mention_everyone; }
	bool IsBot() const { return m_message.author.is_bot(); }
};

inline DiscordObjectHandler<DiscordMessage> g_DiscordMessageHandler;

extern const sp_nativeinfo_t message_natives[];

#endif //_INCLUDE_MESSAGE_H
