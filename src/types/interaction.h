#ifndef _INCLUDE_INTERACTION_H
#define _INCLUDE_INTERACTION_H

#include "embed.h"
#include "object_handler.h"
#include "user.h"
#include "dpp/dpp.h"

class DiscordInteraction : public DiscordObject
{
private:
	dpp::slashcommand_t m_interaction;
	std::string m_commandName;

public:
	DiscordInteraction(const dpp::slashcommand_t& interaction) :
		m_interaction(interaction),
		m_commandName(interaction.command.get_command_name())
	{
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return std::to_string(m_interaction.command.guild_id); }
	std::string GetChannelId() const { return std::to_string(m_interaction.command.channel_id); }
	DiscordUser* GetUser() const { return new DiscordUser(m_interaction.command.usr); }
	std::string GetUserId() const { return std::to_string(m_interaction.command.usr.id); }
	const char* GetUserName() const { return m_interaction.command.usr.username.c_str(); }
	std::string GetUserNickname() const { return m_interaction.command.member.get_nickname(); }

	bool GetOptionValue(const char* name, std::string& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<std::string>(param);
		return true;
	}

	bool GetOptionValueInt(const char* name, int64_t& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<int64_t>(param);
		return true;
	}

	bool GetOptionValueDouble(const char* name, double& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<double>(param);
		return true;
	}

	bool GetOptionValueBool(const char* name, bool& value) const {
		auto param = m_interaction.get_parameter(name);
		if (param.index() == 0) return false;
		value = std::get<bool>(param);
		return true;
	}

	void CreateResponse(const char* content) const {
		m_interaction.reply(dpp::message(content));
	}

	void CreateResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}

	void DeferReply(bool ephemeral = false) const {
		m_interaction.thinking(ephemeral);
	}

	void EditResponse(const char* content) const {
		m_interaction.edit_response(dpp::message(content));
	}

	void EditResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.add_embed(embed->GetEmbed());
		m_interaction.edit_response(msg);
	}

	void CreateEphemeralResponse(const char* content) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		m_interaction.reply(msg);
	}

	void CreateEphemeralResponseEmbed(const char* content, const DiscordEmbed* embed) const {
		dpp::message msg(content);
		msg.set_flags(dpp::m_ephemeral);
		msg.add_embed(embed->GetEmbed());
		m_interaction.reply(msg);
	}
};

inline DiscordObjectHandler<DiscordInteraction> g_DiscordInteractionHandler;

extern const sp_nativeinfo_t interaction_natives[];

#endif //_INCLUDE_INTERACTION_H
