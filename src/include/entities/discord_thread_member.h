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

#include "utils/discord_common.h"

class DiscordClient;

class DiscordThreadMember {
private:
	dpp::thread_member m_member;

public:
	DiscordThreadMember(const dpp::thread_member& member, DiscordClient* = nullptr) : m_member(member) {}

	std::string GetThreadId() const { return m_member.thread_id.str(); }
	std::string GetUserId() const { return m_member.user_id.str(); }
	int GetJoinedTimestamp() const { return static_cast<int>(m_member.joined); }
	uint32_t GetFlags() const { return m_member.flags; }

	const dpp::thread_member& GetDPPThreadMember() const { return m_member; }
};
