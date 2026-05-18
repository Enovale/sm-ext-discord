/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2026 ProjectSky
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
#include "core/async_callback.h"

static constexpr std::streamoff kMaxWebhookAvatarBytes = 10 * 1024 * 1024;

void DiscordWebhook::Modify(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Modify(m_webhook, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordWebhook>(cb);
		});
	} else {
		m_client->Webhooks().Modify(m_webhook);
	}
}

void DiscordWebhook::Delete(IPluginFunction* callback, cell_t data) {
	if (!m_client) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Delete(m_webhook.id, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Confirm(cb, DiscordResultType::Delete);
		});
	} else {
		m_client->Webhooks().Delete(m_webhook.id);
	}
}

void DiscordWebhook::Execute(const char* message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !message) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().Execute(m_webhook, message, m_thread_id, m_thread_name, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
		});
	} else {
		m_client->Webhooks().Execute(m_webhook, message, m_thread_id, m_thread_name);
	}
}

void DiscordWebhook::ExecuteEmbed(const char* message, const DiscordEmbed* embed, IPluginFunction* callback, cell_t data) {
	if (!m_client || !embed) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().ExecuteEmbed(m_webhook, message ? message : "", embed, m_thread_id, m_thread_name, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
		});
	} else {
		m_client->Webhooks().ExecuteEmbed(m_webhook, message ? message : "", embed, m_thread_id, m_thread_name);
	}
}

void DiscordWebhook::ExecuteMessage(const DiscordMessage* message, IPluginFunction* callback, cell_t data) {
	if (!m_client || !message) return;
	if (callback) {
		Handle_t client_handle = m_client->GetHandle();
		m_client->Webhooks().ExecuteMessage(m_webhook, message, m_thread_id, m_thread_name, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& cb) {
			callback.Result<DiscordMessage>(cb);
		});
	} else {
		m_client->Webhooks().ExecuteMessage(m_webhook, message, m_thread_id, m_thread_name);
	}
}

bool DiscordWebhook::SetAvatarFromFile(const char* filepath, dpp::image_type type) {
	if (!filepath) return false;

	char fullpath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullpath, sizeof(fullpath), "%s", filepath);

	std::string buffer;
	std::string error;
	if (!ReadBinaryFile(fullpath, kMaxWebhookAvatarBytes, "webhook avatar file", buffer, error)) {
		Log.Error("SetAvatar: %s", error.c_str());
		return false;
	}

	m_webhook.load_image(buffer, type);
	return true;
}
