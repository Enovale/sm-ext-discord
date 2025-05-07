#ifndef _INCLUDE_DISCORD_H_
#define _INCLUDE_DISCORD_H_

#include "object_handler.h"
#include "smsdk_ext.h"
#include "types/embed.h"

class DiscordClient : public DiscordObject
{
private:
	std::unique_ptr<dpp::cluster> m_cluster;
	bool m_isRunning;
	Handle_t m_discord_handle;
	std::unique_ptr<std::thread> m_thread;

	std::string m_botId;
	std::string m_botName;
	std::string m_botDiscriminator;
	std::string m_botAvatarUrl;

	void RunBot();
	void SetupEventHandlers();

public:
	DiscordClient(const char* token);
	~DiscordClient();

	bool Initialize();
	void Start();
	void Stop();
	bool IsRunning() const { return m_isRunning; }
	void SetHandle(Handle_t handle) { m_discord_handle = handle; }
	bool SetPresence(dpp::presence presence);
	bool CreateWebhook(dpp::webhook wh, IForward *callback_forward, cell_t data);
	bool ExecuteWebhook(dpp::webhook wh, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool SendMessage(dpp::snowflake channel_id, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool SendMessageEmbed(dpp::snowflake channel_id, const char* message, const DiscordEmbed* embed, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles);
	bool GetChannel(dpp::snowflake channel_id, IForward *callback_forward, cell_t data);
	bool GetChannelWebhooks(dpp::snowflake channel_id, IForward *callback_forward, cell_t data);
    bool RegisterSlashCommand(dpp::snowflake guild_id, const char* name, const char* description, const char* default_permissions);
	bool RegisterGlobalSlashCommand(const char* name, const char* description, const char* default_permissions);
	bool RegisterSlashCommandWithOptions(dpp::snowflake guild_id, const char* name, const char* description, const char* default_permisssions, const std::vector<dpp::command_option>& options);
	bool RegisterGlobalSlashCommandWithOptions(const char* name, const char* description, const char* default_permissions, const std::vector<dpp::command_option>& options);
	void CreateAutocompleteResponse(dpp::snowflake id, const std::string &token, const dpp::interaction_response &response);
	bool EditMessage(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content);
	bool EditMessageEmbed(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, const DiscordEmbed* embed);
	bool DeleteMessage(dpp::snowflake channel_id, dpp::snowflake message_id);
	bool DeleteGuildCommand(dpp::snowflake guild_id, dpp::snowflake command_id);
	bool DeleteGlobalCommand(dpp::snowflake command_id);
	bool BulkDeleteGuildCommands(dpp::snowflake guild_id);
	bool BulkDeleteGlobalCommands();

	const char* GetBotId() const { return m_botId.c_str(); }
	const char* GetBotName() const { return m_botName.c_str(); }
	const char* GetBotDiscriminator() const { return m_botDiscriminator.c_str(); }
	const char* GetBotAvatarUrl() const { return m_botAvatarUrl.c_str(); }

	void UpdateBotInfo() {
		if (m_cluster) {
			m_botId = std::to_string(m_cluster->me.id);
			m_botName = m_cluster->me.username;
			m_botDiscriminator = std::to_string(m_cluster->me.discriminator);
			m_botAvatarUrl = m_cluster->me.get_avatar_url();
		}
	}
};

inline DiscordObjectHandler<DiscordClient> g_DiscordHandler;

#endif // _INCLUDE_DISCORD_H_ 