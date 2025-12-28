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

#include "entities/discord_guild.h"
#include "entities/discord_guild_member.h"
#include "entities/discord_channel.h"
#include "entities/discord_role.h"
#include "entities/discord_ban.h"
#include "features/discord_invite.h"
#include "features/discord_webhook.h"
#include "utils/discord_common.h"
#include "core/discord_handle_array.h"
#include "core/discord_client.h"
#include "core/handle_manager.h"
#include "core/callback_helpers.h"
#include "core/task_manager.h"


bool DiscordGuild::HasPermission(dpp::snowflake user_id, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;
	uint64_t base_perms = GetBasePermissions(user_id);
	return (base_perms & perm) != 0;
}

bool DiscordGuild::HasPermissionInChannel(dpp::snowflake user_id, dpp::snowflake channel_id, const char* permission) const {
	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;
	uint64_t channel_perms = GetPermissionsInChannel(user_id, channel_id);
	return (channel_perms & perm) != 0;
}

void DiscordGuild::Modify() {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->GetCluster()->guild_edit(m_guild, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to modify guild");
	});
}

bool DiscordGuild::GetInvites(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->GetCluster()->guild_get_invites(m_guild.id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordInvite, dpp::invite_map>(client_handle, client, callback, data, cb, DiscordResultType::Invites);
	});
	return true;
}

bool DiscordGuild::GetWebhooks(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->GetCluster()->get_guild_webhooks(m_guild.id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordWebhook, dpp::webhook_map>(client_handle, client, callback, data, cb, DiscordResultType::Webhooks);
	});
	return true;
}
uint64_t DiscordGuild::GetBasePermissions(dpp::snowflake user_id) const {
	if (!m_client || !m_client->GetCluster()) return 0;

	dpp::user* user = dpp::find_user(user_id);
	if (!user) return 0;
	return m_guild.base_permissions(user);
}

uint64_t DiscordGuild::GetPermissionsInChannel(dpp::snowflake user_id, dpp::snowflake channel_id) const {
	dpp::channel* ch = dpp::find_channel(channel_id);
	if (!ch) return 0;

	dpp::user* user = dpp::find_user(user_id);
	if (!user) return 0;

	uint64_t base_perms = m_guild.base_permissions(user);
	return static_cast<uint64_t>(m_guild.permission_overwrites(base_perms, user, ch));
}

bool DiscordGuild::GetMember(dpp::snowflake user_id, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->GetCluster()->guild_get_member(m_guild.id, user_id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordGuildMember>(client_handle, client, callback, data, cb);
	});
	return true;
}

bool DiscordGuild::GetMembers(uint16_t limit, dpp::snowflake after, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetMembers(m_guild.id, limit, after, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordGuildMember, dpp::guild_member_map>(client_handle, client, callback, data, cb, DiscordResultType::Members);
	});
	return true;
}

bool DiscordGuild::SearchMembers(const char* query, uint16_t limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback || !query) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().SearchMembers(m_guild.id, query, limit, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordGuildMember, dpp::guild_member_map>(client_handle, client, callback, data, cb, DiscordResultType::Members);
	});
	return true;
}

bool DiscordGuild::GetActiveThreads(IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Threads().GetActive(m_guild.id, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushTask([client_handle, client, callback, data, cb]() {
			DiscordResult* result = new DiscordResult(DiscordResultType::Threads, client);
			if (cb.is_error()) {
				result->SetError(cb.get_error().human_readable);
			} else {
				result->SetSuccess(true);
				const auto& threads = std::get<dpp::active_threads>(cb.value);
				DiscordHandleArray* arr = new DiscordHandleArray();
				for (const auto& [_, thread_info] : threads) {
					DiscordChannel* ch = new DiscordChannel(thread_info.active_thread, client);
					Handle_t h = Handles.CreateCallback(ch, HandleId::DiscordChannel);
					arr->Add(h);
				}
				Handle_t arrHandle = Handles.CreateCallback(arr, HandleId::DiscordHandleArray);
				result->SetHandle("threads", arrHandle);
				result->SetInt("count", static_cast<int>(arr->GetLength()));
			}
			Handle_t resultHandle = Handles.CreateCallback(result, HandleId::DiscordResult);
			callback->PushCell(client_handle);
			callback->PushCell(resultHandle);
			callback->PushCell(data);
			callback->Execute(nullptr);
			Handles.FreeHandle(resultHandle);
		});
	});
	return true;
}

bool DiscordGuild::GetBans(dpp::snowflake before, dpp::snowflake after, uint16_t limit, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !callback) return false;
	Handle_t client_handle = m_client->GetHandle();
	m_client->Guilds().GetBans(m_guild.id, before, after, limit, [client = m_client, client_handle, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResultList<DiscordBan, dpp::ban_map>(client_handle, client, callback, data, cb, DiscordResultType::Bans);
	});
	return true;
}

void DiscordGuild::CreateEmoji(const char* name, const char* image_path, dpp::image_type type) {
	if (!m_client || !m_client->GetCluster() || !name || !image_path) return;
	dpp::emoji em;
	em.name = name;
	em.load_image(image_path, type);
	m_client->GetCluster()->guild_emoji_create(m_guild.id, em, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to create emoji");
	});
}

void DiscordGuild::CreateSticker(const char* name, const char* description, const char* tags, const char* file_path, dpp::sticker_format format) {
	if (!m_client || !m_client->GetCluster() || !name || !tags || !file_path) return;
	dpp::sticker st;
	st.guild_id = m_guild.id;
	st.name = name;
	if (description) st.description = description;
	st.tags = tags;
	st.format_type = format;
	st.set_filename(file_path);
	m_client->GetCluster()->guild_sticker_create(st, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to create sticker");
	});
}

void DiscordGuild::CreateScheduledEvent(const char* name, const char* description, dpp::snowflake channel_id, time_t start_time, time_t end_time, dpp::event_entity_type type) {
	if (!m_client || !m_client->GetCluster() || !name) return;
	dpp::scheduled_event ev;
	ev.guild_id = m_guild.id;
	ev.name = name;
	if (description) ev.description = description;
	ev.channel_id = channel_id;
	ev.scheduled_start_time = start_time;
	if (end_time > 0) ev.scheduled_end_time = end_time;
	ev.entity_type = type;
	m_client->GetCluster()->guild_event_create(ev, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to create scheduled event");
	});
}

void DiscordGuild::CreateExternalScheduledEvent(const char* name, const char* description, const char* location, time_t start_time, time_t end_time) {
	if (!m_client || !m_client->GetCluster() || !name || !location) return;
	dpp::scheduled_event ev;
	ev.guild_id = m_guild.id;
	ev.name = name;
	if (description) ev.description = description;
	ev.entity_metadata.location = location;
	ev.scheduled_start_time = start_time;
	ev.scheduled_end_time = end_time;
	ev.entity_type = dpp::eet_external;
	m_client->GetCluster()->guild_event_create(ev, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to create external scheduled event");
	});
}

void DiscordGuild::UnbanUser(dpp::snowflake user_id) {
	if (!m_client || !m_client->GetCluster()) return;
	m_client->GetCluster()->guild_ban_delete(m_guild.id, user_id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to unban user");
	});
}

void DiscordGuild::CreateRole(const char* name, uint32_t color, bool hoist, bool mentionable, uint64_t permissions, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !name || !callback) return;
	dpp::role role;
	role.guild_id = m_guild.id;
	role.name = name;
	role.colour = color;
	role.flags = (hoist ? dpp::r_hoist : 0) | (mentionable ? dpp::r_mentionable : 0);
	role.permissions = permissions;

	Handle_t client_handle = m_client->GetHandle();
	m_client->GetCluster()->role_create(role, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordRole>(client_handle, client, callback, data, cb);
	});
}

void DiscordGuild::CreateRoleFromObject(DiscordRole* role_obj, IPluginFunction* callback, cell_t data) {
	if (!m_client || !m_client->IsRunning() || !role_obj || !callback) return;
	dpp::role role = role_obj->GetDPPRole();
	role.guild_id = m_guild.id;
	role.id = 0;

	Handle_t client_handle = m_client->GetHandle();
	m_client->GetCluster()->role_create(role, [client_handle, client = m_client, callback, data](const dpp::confirmation_callback_t& cb) {
		PushResult<DiscordRole>(client_handle, client, callback, data, cb);
	});
}

void DiscordGuild::BulkDeleteCommands() {
	if (!m_client || !m_client->IsRunning()) return;
	m_client->GetCluster()->guild_bulk_command_create({}, m_guild.id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to bulk delete guild commands");
	});
}
