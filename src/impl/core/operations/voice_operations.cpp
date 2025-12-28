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

#include "core/operations/voice_operations.h"
#include "utils/discord_common.h"

void VoiceOperations::MoveToChannel(dpp::snowflake guild_id, dpp::snowflake user_id, dpp::snowflake channel_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_member_move(channel_id, guild_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to move member to voice channel"); });
}

void VoiceOperations::Disconnect(dpp::snowflake guild_id, dpp::snowflake user_id, Callback callback) {
	if (!IsValid()) return;
	m_cluster->guild_member_move(0, guild_id, user_id, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to disconnect member from voice"); });
}

void VoiceOperations::SetMute(dpp::snowflake guild_id, dpp::snowflake user_id, bool mute, Callback callback) {
	if (!IsValid()) return;
	dpp::guild_member member;
	member.guild_id = guild_id;
	member.user_id = user_id;
	member.set_mute(mute);
	m_cluster->guild_edit_member(member, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to set member mute state"); });
}

void VoiceOperations::SetDeaf(dpp::snowflake guild_id, dpp::snowflake user_id, bool deaf, Callback callback) {
	if (!IsValid()) return;
	dpp::guild_member member;
	member.guild_id = guild_id;
	member.user_id = user_id;
	member.set_deaf(deaf);
	m_cluster->guild_edit_member(member, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to set member deaf state"); });
}
