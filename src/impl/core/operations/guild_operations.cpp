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

#include "core/operations/guild_operations.h"
#include "entities/discord_guild.h"
#include "utils/discord_common.h"

void GuildOperations::Get(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get(guild_id, callback);
}

void GuildOperations::ModifyFromObject(const DiscordGuild* guild_obj, Callback callback) {
	if (!IsValid() || !guild_obj) return;
	m_cluster->guild_edit(guild_obj->GetDPPGuild(), callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify guild"); });
}

void GuildOperations::GetMembers(dpp::snowflake guild_id, uint16_t limit, dpp::snowflake after, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_members(guild_id, limit, after, callback);
}

void GuildOperations::SearchMembers(dpp::snowflake guild_id, const char* query, uint16_t limit, Callback callback) {
	if (!IsValid() || !query) return;
	m_cluster->guild_search_members(guild_id, query, limit, callback);
}

void GuildOperations::GetBans(dpp::snowflake guild_id, dpp::snowflake before, dpp::snowflake after, uint16_t limit, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_bans(guild_id, before, after, limit, callback);
}

void GuildOperations::GetInvites(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_get_invites(guild_id, callback);
}

void GuildOperations::GetWebhooks(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->get_guild_webhooks(guild_id, callback);
}

void GuildOperations::BulkDeleteCommands(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_bulk_command_create({}, guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to bulk delete guild commands"); });
}

void GuildOperations::Leave(dpp::snowflake guild_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_delete(guild_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to leave guild"); });
}

void GuildOperations::ModifyCurrentMember(dpp::snowflake guild_id, const char* nickname, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_current_member_edit(guild_id, nickname ? nickname : "", "", dpp::i_png, "", dpp::i_png, "", callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to modify current member"); });
}

void GuildOperations::GetPruneCount(dpp::snowflake guild_id, uint16_t days, Callback callback) {
	if (!IsValid()) return;
	dpp::prune pruneinfo;
	pruneinfo.days = days;
	m_cluster->guild_get_prune_counts(guild_id, pruneinfo, callback);
}

void GuildOperations::BeginPrune(dpp::snowflake guild_id, uint16_t days, Callback callback) {
	if (!IsValid()) return;
	dpp::prune pruneinfo;
	pruneinfo.days = days;
	m_cluster->guild_begin_prune(guild_id, pruneinfo, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to begin prune"); });
}
