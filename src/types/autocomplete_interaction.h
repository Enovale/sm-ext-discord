#ifndef _INCLUDE_AUTOCOMPLETE_INTERACTION_H
#define _INCLUDE_AUTOCOMPLETE_INTERACTION_H

#include "discord.h"
#include "object_handler.h"
#include "user.h"
#include "dpp/dpp.h"

class DiscordAutocompleteInteraction : public DiscordObject
{
public:
	dpp::interaction_response m_response;
	std::string m_commandName;
	dpp::interaction m_command;
	dpp::autocomplete_t m_autocomplete;

	DiscordAutocompleteInteraction(const dpp::autocomplete_t& autocomplete) :
		m_response(dpp::ir_autocomplete_reply),
		m_commandName(autocomplete.command.get_command_name()),
		m_command(autocomplete.command),
		m_autocomplete(autocomplete)
	{
	}

	const char* GetCommandName() const { return m_commandName.c_str(); }
	std::string GetGuildId() const { return std::to_string(m_command.guild_id); }
	std::string GetChannelId() const { return std::to_string(m_command.channel_id); }
	DiscordUser* GetUser() const { return new DiscordUser(m_command.usr); }
	std::string GetUserNickname() const { return m_command.member.get_nickname(); }

	dpp::command_option GetOption(const char* name) const {
		for (auto & opt : m_autocomplete.options) {
			if (opt.name == name) return opt;
		}

		throw std::runtime_error("Option not found");
	}

	std::string GetOptionValue(const char* name) const {
		return std::get<std::string>(GetOption(name).value);
	}

	int64_t GetOptionValueInt(const char* name) const {
		return std::get<int64_t>(GetOption(name).value);
	}

	double GetOptionValueDouble(const char* name) const {
		return std::get<double>(GetOption(name).value);
	}

	bool GetOptionValueBool(const char* name) const {
		return std::get<bool>(GetOption(name).value);
	}

	void AddAutocompleteOption(dpp::command_option_choice choice) {
		m_response.add_autocomplete_choice(choice);
	}
};

inline DiscordObjectHandler<DiscordAutocompleteInteraction> g_DiscordAutocompleteInteractionHandler;

extern const sp_nativeinfo_t autocomplete_natives[];

#endif //_INCLUDE_AUTOCOMPLETE_INTERACTION_H
