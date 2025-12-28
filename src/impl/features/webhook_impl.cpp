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

#include "features/discord_webhook.h"
#include "entities/discord_message.h"
#include "utils/discord_common.h"
#include "utils/discord_embed.h"
#include "core/discord_client.h"
#include "core/callback_helpers.h"
#include <fstream>

void DiscordWebhook::Modify(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Modify(m_webhook, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordWebhook>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Webhooks().Modify(m_webhook);
	}
}

void DiscordWebhook::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Delete(m_webhook.id, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushConfirm(client_handle, client, callback, data, cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Webhooks().Delete(m_webhook.id);
	}
}

void DiscordWebhook::Execute(const char* message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !message) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Execute(m_webhook, message, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Webhooks().Execute(m_webhook, message);
	}
}

void DiscordWebhook::ExecuteEmbed(const char* message, const DiscordEmbed* embed, IPluginFunction* callback, cell_t data) {
	if (!m_client || !embed) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().ExecuteEmbed(m_webhook, message ? message : "", embed, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Webhooks().ExecuteEmbed(m_webhook, message ? message : "", embed);
	}
}

void DiscordWebhook::ExecuteMessage(const DiscordMessage* message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !message) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().ExecuteMessage(m_webhook, message, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
			PushResult<DiscordMessage>(client_handle, client, callback, data, cb);
		});
	} else {
		m_client->Webhooks().ExecuteMessage(m_webhook, message);
	}
}

bool DiscordWebhook::SetAvatar(const char* filepath, dpp::image_type type) {
	if (!filepath) return false;

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", filepath);

	std::ifstream file(fullpath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		Log.Error("SetAvatar: Failed to open file: %s", fullpath);
		return false;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) {
		Log.Error("SetAvatar: Failed to read file: %s", fullpath);
		return false;
	}

	m_webhook.load_image(buffer, type);
	return true;
}
