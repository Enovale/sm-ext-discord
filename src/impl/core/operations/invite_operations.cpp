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

#include "core/operations/invite_operations.h"
#include "utils/discord_common.h"
#include "features/discord_invite.h"

void InviteOperations::Create(dpp::snowflake channel_id, uint32_t max_age, uint32_t max_uses, bool temporary, bool unique, Callback callback) {
	if (!IsValid()) return;
	dpp::invite inv;
	inv.max_age = max_age;
	inv.max_uses = max_uses;
	inv.temporary = temporary;
	inv.unique = unique;
	dpp::channel ch;
	ch.id = channel_id;
	m_cluster->channel_invite_create(ch, inv, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create invite"); });
}

void InviteOperations::CreateFromObject(dpp::snowflake channel_id, const DiscordInvite* invite, Callback callback) {
	if (!IsValid() || !invite) return;
	dpp::channel ch;
	ch.id = channel_id;
	m_cluster->channel_invite_create(ch, invite->GetDPPInvite(), callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create invite from object"); });
}

void InviteOperations::CreateWithChannel(const dpp::channel& channel, const dpp::invite& invite, Callback callback) {
	if (!IsValid()) return;
	m_cluster->channel_invite_create(channel, invite, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to create invite"); });
}

void InviteOperations::Delete(const char* code, const char* reason, Callback callback) {
	if (!IsValid() || !code) return;
	m_cluster->invite_delete(code, callback ? callback : [](const dpp::confirmation_callback_t& cb) { Log.DppError(cb, "Failed to delete invite"); });
}

void InviteOperations::Get(const char* code, Callback callback) {
	if (!IsValid() || !code) return;
	m_cluster->invite_get(code, callback);
}
