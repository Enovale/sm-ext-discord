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

#include "core/operations/thread_operations.h"
#include "utils/discord_common.h"

void ThreadOperations::Get(dpp::snowflake thread_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->thread_get(thread_id, callback);
}

void ThreadOperations::Create(dpp::snowflake channel_id, const char* name, dpp::channel_type type, uint16_t auto_archive, bool invitable, uint16_t rate_limit, Callback callback) {
	if (!IsValid() || !name) return;
	m_cluster->thread_create(name, channel_id, auto_archive, type, invitable, rate_limit, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create thread"); });
}

void ThreadOperations::CreateWithMessage(dpp::snowflake channel_id, dpp::snowflake message_id, const char* name, uint16_t auto_archive, uint16_t rate_limit, Callback callback) {
	if (!IsValid() || !name) return;
	m_cluster->thread_create_with_message(name, channel_id, message_id, auto_archive, rate_limit, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create thread with message"); });
}

void ThreadOperations::CreateInForum(dpp::snowflake channel_id, const char* name, const char* message, const std::vector<dpp::snowflake>& tag_ids, dpp::auto_archive_duration_t auto_archive, uint16_t rate_limit, Callback callback) {
	if (!IsValid() || !name || !message) return;
	dpp::message msg(channel_id, message);
	m_cluster->thread_create_in_forum(name, channel_id, msg, auto_archive, rate_limit, tag_ids, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create forum thread"); });
}

void ThreadOperations::CreateInForumWithMessage(dpp::snowflake channel_id, const char* name, const dpp::message& message, const std::vector<dpp::snowflake>& tag_ids, dpp::auto_archive_duration_t auto_archive, uint16_t rate_limit, Callback callback) {
	if (!IsValid() || !name) return;
	m_cluster->thread_create_in_forum(name, channel_id, message, auto_archive, rate_limit, tag_ids, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create forum thread"); });
}

void ThreadOperations::GetActive(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->threads_get_active(guild_id, callback);
}

void ThreadOperations::Join(dpp::snowflake thread_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->current_user_join_thread(thread_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to join thread"); });
}

void ThreadOperations::Leave(dpp::snowflake thread_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->current_user_leave_thread(thread_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to leave thread"); });
}

void ThreadOperations::MemberAdd(dpp::snowflake thread_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->thread_member_add(thread_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to add thread member"); });
}

void ThreadOperations::MemberRemove(dpp::snowflake thread_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->thread_member_remove(thread_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to remove thread member"); });
}

void ThreadOperations::Archive(dpp::snowflake thread_id, bool archived, Callback callback) {
	if (!IsValid()) return;
	dpp::thread th;
	th.id = thread_id;
	th.metadata.archived = archived;
	m_cluster->channel_edit(th, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to archive thread"); });
}

void ThreadOperations::Lock(dpp::snowflake thread_id, bool locked, Callback callback) {
	if (!IsValid()) return;
	dpp::thread th;
	th.id = thread_id;
	th.metadata.locked = locked;
	m_cluster->channel_edit(th, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to lock thread"); });
}

void ThreadOperations::Modify(dpp::snowflake thread_id, const std::string& name, uint16_t auto_archive, uint16_t rate_limit, bool archived, bool locked, Callback callback) {
	if (!IsValid()) return;
	dpp::thread th;
	th.id = thread_id;
	if (!name.empty()) th.name = name;
	if (auto_archive > 0) th.metadata.auto_archive_duration = auto_archive;
	if (rate_limit > 0) th.rate_limit_per_user = rate_limit;
	th.metadata.archived = archived;
	th.metadata.locked = locked;
	m_cluster->channel_edit(th, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify thread"); });
}

void ThreadOperations::GetPublicArchived(dpp::snowflake channel_id, time_t before, uint16_t limit, Callback callback) {
	if (!IsValid()) return;
	m_cluster->threads_get_public_archived(channel_id, before, limit, callback);
}

void ThreadOperations::GetPrivateArchived(dpp::snowflake channel_id, time_t before, uint16_t limit, Callback callback) {
	if (!IsValid()) return;
	m_cluster->threads_get_private_archived(channel_id, before, limit, callback);
}

void ThreadOperations::GetMembers(dpp::snowflake thread_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->thread_members_get(thread_id, callback);
}
