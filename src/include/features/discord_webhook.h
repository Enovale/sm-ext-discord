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

#include "utils/discord_embed.h"
#include "entities/discord_user.h"
#include "core/handle_manager.h"

class DiscordWebhook
{
private:
	dpp::webhook m_webhook;
	DiscordClient* m_client;
	mutable CachedHandle<DiscordUser> m_userHandle;
	dpp::snowflake m_thread_id = 0;
	std::string m_thread_name;

public:
	// Constructors
	DiscordWebhook(const dpp::webhook& wbhk, DiscordClient* client) : m_webhook(wbhk), m_client(client) {}
	DiscordWebhook(const std::string& webhook_url, DiscordClient* client = nullptr) : m_webhook(webhook_url), m_client(client) {}
	DiscordWebhook(const char* webhook_url, DiscordClient* client = nullptr) : m_webhook(std::string(webhook_url)), m_client(client) {}
	DiscordWebhook(dpp::snowflake webhook_id, const std::string& webhook_token, DiscordClient* client = nullptr)
		: m_client(client) {
		m_webhook.id = webhook_id;
		m_webhook.token = webhook_token;
	}

	// Basic properties
	std::string GetId() const { return m_webhook.id.str(); }

	Handle_t GetUserHandle() const {
		return m_userHandle.GetOrCreate([this]() { return GetUser(); });
	}

	DiscordUser* GetUser() const {
		if (!m_client) return nullptr;
		return new DiscordUser(m_webhook.user_obj, m_client);
	}
	const char* GetName() const { return m_webhook.name.c_str(); }
	void SetName(const char* value) { m_webhook.name = value; }
	const char* GetAvatarUrl() const { return m_webhook.avatar_url.c_str(); }

	// Avatar methods (mutually exclusive - use either SetAvatarFromFile OR SetAvatarFromUrl, not both)
	bool SetAvatarFromFile(const char* filepath, dpp::image_type type);
	void SetAvatarFromUrl(const char* url) { m_webhook.avatar_url = url ? url : ""; }

	// Additional webhook properties (read-only)
	uint8_t GetType() const { return m_webhook.type; }
	std::string GetGuildId() const { return m_webhook.guild_id.str(); }
	std::string GetChannelId() const { return m_webhook.channel_id.str(); }
	const char* GetToken() const { return m_webhook.token.c_str(); }
	std::string GetApplicationId() const { return m_webhook.application_id.str(); }
	std::string GetSourceGuildId() const { return m_webhook.source_guild.id.str(); }
	std::string GetSourceChannelId() const { return m_webhook.source_channel.id.str(); }
	const char* GetUrl() const { return m_webhook.url.c_str(); }
	const std::string& GetImageData() const { return m_webhook.image_data; }

	// Webhook management methods
	void Modify(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Delete(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Execute(const char* message, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ExecuteEmbed(const char* message, const class DiscordEmbed* embed, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ExecuteMessage(const class DiscordMessage* message, IPluginFunction* callback = nullptr, cell_t data = 0);

	// Forum thread support
	void SetThreadId(dpp::snowflake thread_id) { m_thread_id = thread_id; }
	void SetThreadName(const char* thread_name) { m_thread_name = thread_name ? thread_name : ""; }
	dpp::snowflake GetThreadId() const { return m_thread_id; }
	const std::string& GetThreadName() const { return m_thread_name; }

	// Internal accessor
	const dpp::webhook& GetDPPWebhook() const { return m_webhook; }
};
