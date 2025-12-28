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

class DiscordReaction
{
private:
	dpp::reaction m_reaction;

public:
	DiscordReaction(const dpp::reaction& react) : m_reaction(react) {}

	// Basic information
	uint32_t GetCount() const { return m_reaction.count; }
	uint32_t GetCountBurst() const { return m_reaction.count_burst; }
	uint32_t GetCountNormal() const { return m_reaction.count_normal; }
	std::string GetEmojiId() const { return m_reaction.emoji_id.str(); }
	const char* GetEmojiName() const { return m_reaction.emoji_name.c_str(); }
	bool GetMe() const { return m_reaction.me; }
	bool GetMeBurst() const { return m_reaction.me_burst; }

	// Type checks
	bool IsCustomEmoji() const { return m_reaction.emoji_id != 0; }

	// Get emoji mention format
	std::string GetEmojiMention() const {
		if (m_reaction.emoji_id != 0) {
			return "<:" + m_reaction.emoji_name + ":" + m_reaction.emoji_id.str() + ">";
		}
		return m_reaction.emoji_name;
	}

	// Burst colors
	size_t GetBurstColorCount() const { return m_reaction.burst_colors.size(); }
	uint32_t GetBurstColor(size_t index) const {
		if (index >= m_reaction.burst_colors.size()) return 0;
		return m_reaction.burst_colors[index];
	}

	// Internal accessor
	const dpp::reaction& GetDPPReaction() const { return m_reaction; }
};
