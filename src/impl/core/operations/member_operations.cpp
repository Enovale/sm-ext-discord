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

#include "core/operations/member_operations.h"
#include "utils/discord_common.h"

void MemberOperations::Get(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_member(guild_id, user_id, callback);
}

void MemberOperations::GetUser(dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->user_get(user_id, callback);
}

void MemberOperations::Modify(dpp::snowflake guild_id, dpp::snowflake user_id, const std::string& nickname, Callback callback) {
	if (!IsValid()) return;
	dpp::guild_member member; member.guild_id = guild_id; member.user_id = user_id;
	if (!nickname.empty()) member.set_nickname(nickname);
	m_cluster->guild_edit_member(member, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify member"); });
}

void MemberOperations::AddRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_member_add_role(guild_id, user_id, role_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to add member role"); });
}

void MemberOperations::RemoveRole(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake role_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_member_remove_role(guild_id, user_id, role_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to remove member role"); });
}

void MemberOperations::Kick(dpp::snowflake guild_id, dpp::snowflake user_id, const char* reason, Callback callback) {
	if (!IsValid()) return;
	if (reason && reason[0] != '\0') m_cluster->set_audit_reason(reason);
	m_cluster->guild_member_kick(guild_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to kick member"); });
}

void MemberOperations::Ban(dpp::snowflake guild_id, dpp::snowflake user_id, const char* reason, uint32_t delete_message_seconds, Callback callback) {
	if (!IsValid()) return;
	if (reason && reason[0] != '\0') m_cluster->set_audit_reason(reason);
	m_cluster->guild_ban_add(guild_id, user_id, delete_message_seconds, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to ban member"); });
}

void MemberOperations::Unban(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_ban_delete(guild_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to unban member"); });
}

void MemberOperations::Timeout(dpp::snowflake guild_id, dpp::snowflake user_id, time_t timeout_until, Callback callback) {
	if (!IsValid()) return;
	dpp::guild_member member; member.guild_id = guild_id; member.user_id = user_id;
	member.communication_disabled_until = timeout_until;
	m_cluster->guild_edit_member(member, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to timeout member"); });
}

void MemberOperations::RemoveTimeout(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	dpp::guild_member member; member.guild_id = guild_id; member.user_id = user_id;
	member.communication_disabled_until = 0;
	m_cluster->guild_edit_member(member, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to remove timeout"); });
}
