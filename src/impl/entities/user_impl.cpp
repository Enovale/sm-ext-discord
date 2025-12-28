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

#include "entities/discord_user.h"
#include "utils/discord_common.h"
#include "core/discord_client.h"
#include <unordered_set>

bool DiscordUser::HasPermission(const char* permission) const {
	if (!m_has_member) return false;

	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	if (!g) return false;

	uint64_t base_perms = g->base_permissions(&m_user);
	dpp::permission perms(base_perms);
	return perms.has(perm);
}

uint64_t DiscordUser::GetPermissions() const {
	if (!m_has_member) return 0;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	if (!g) return 0;

	uint64_t base_perms = g->base_permissions(&m_user);
	return base_perms;
}

bool DiscordUser::HasPermissionInChannel(dpp::snowflake channel_id, const char* permission) const {
	if (!m_has_member) return false;

	uint64_t perm;
	if (!ParseUInt64(permission, perm)) return false;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	dpp::channel* c = dpp::find_channel(channel_id);
	if (!g || !c) return false;

	dpp::permission perms = g->permission_overwrites(m_member, *c);
	return perms.has(perm);
}

uint64_t DiscordUser::GetPermissionsInChannel(dpp::snowflake channel_id) const {
	if (!m_has_member) return 0;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	dpp::channel* c = dpp::find_channel(channel_id);
	if (!g || !c) return 0;

	dpp::permission perms = g->permission_overwrites(m_member, *c);
	return static_cast<uint64_t>(perms);
}

std::vector<dpp::snowflake> DiscordUser::GetRoles() const {
	if (!m_has_member) return std::vector<dpp::snowflake>();
	return m_member.get_roles();
}

bool DiscordUser::HasRole(dpp::snowflake role_id) const {
	if (!m_has_member) return false;

	std::vector<dpp::snowflake> roles = m_member.get_roles();
	for (const auto& role : roles) {
		if (role == role_id) return true;
	}
	return false;
}

bool DiscordUser::HasAnyRole(const std::vector<dpp::snowflake>& role_ids) const {
	if (!m_has_member) return false;

	std::vector<dpp::snowflake> user_roles = m_member.get_roles();
	std::unordered_set<dpp::snowflake> user_role_set(user_roles.begin(), user_roles.end());

	for (const auto& target_role : role_ids) {
		if (user_role_set.count(target_role)) {
			return true;
		}
	}
	return false;
}

bool DiscordUser::HasAllRoles(const std::vector<dpp::snowflake>& role_ids) const {
	if (!m_has_member) return false;

	std::vector<dpp::snowflake> user_roles = m_member.get_roles();
	std::unordered_set<dpp::snowflake> user_role_set(user_roles.begin(), user_roles.end());

	for (const auto& target_role : role_ids) {
		if (!user_role_set.count(target_role)) {
			return false;
		}
	}
	return true;
}

dpp::snowflake DiscordUser::GetHighestRole() const {
	if (!m_has_member) return 0;

	dpp::guild* g = dpp::find_guild(m_member.guild_id);
	if (!g) return 0;

	std::vector<dpp::snowflake> user_roles = m_member.get_roles();
	if (user_roles.empty()) return 0;

	dpp::snowflake highest_role = 0;
	int highest_position = -1;

	for (const auto& role_id : user_roles) {
		dpp::role* role = dpp::find_role(role_id);
		if (role && role->position > highest_position) {
			highest_position = role->position;
			highest_role = role_id;
		}
	}

	return highest_role;
}

std::string DiscordUser::GetRoleName(dpp::snowflake role_id) const {
	if (!m_has_member) return "";

	dpp::role* role = dpp::find_role(role_id);
	if (!role) return "";

	return role->name;
}

std::vector<std::string> DiscordUser::GetRoleNames() const {
	std::vector<std::string> role_names;
	if (!m_has_member) return role_names;

	std::vector<dpp::snowflake> user_roles = m_member.get_roles();
	for (const auto& role_id : user_roles) {
		dpp::role* role = dpp::find_role(role_id);
		if (role) {
			role_names.push_back(role->name);
		}
	}

	return role_names;
}

void DiscordUser::SetNickName(const char* nickname) {
	if (!m_client || !m_has_member) return;

	m_client->Members().Modify(m_member.guild_id, m_user.id, nickname ? nickname : "");
}

void DiscordUser::KickFromGuild() {
	if (!m_client || !m_has_member) return;

	m_client->Members().Kick(m_member.guild_id, m_user.id);
}

void DiscordUser::BanFromGuild(const char* reason, int delete_message_days) {
	if (!m_client || !m_has_member) return;

	m_client->Members().Ban(m_member.guild_id, m_user.id, reason, delete_message_days);
}

void DiscordUser::UnbanFromGuild() {
	if (!m_client || !m_has_member || !m_client->GetCluster()) return;

	m_client->GetCluster()->guild_ban_delete(m_member.guild_id, m_user.id, [](const dpp::confirmation_callback_t& cb) {
		Log.DppError(cb, "Failed to unban user");
	});
}

void DiscordUser::SetTimeout(time_t timeout_until) {
	if (!m_client || !m_has_member) return;

	m_client->Members().Timeout(m_member.guild_id, m_user.id, timeout_until);
}

void DiscordUser::RemoveTimeout() {
	if (!m_client || !m_has_member) return;

	m_client->Members().RemoveTimeout(m_member.guild_id, m_user.id);
}
