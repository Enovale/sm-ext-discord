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

#include "discord_common.h"

class DiscordForumTag
{
private:
	dpp::forum_tag m_tag;

public:
	DiscordForumTag() {}
	DiscordForumTag(const char* name, const char* emoji = "", bool moderated = false) {
		m_tag.set_name(name);
		if (emoji && emoji[0] != '\0') {
			size_t len = strlen(emoji);
			if (std::all_of(emoji, emoji + len, ::isdigit)) {
				m_tag.emoji = dpp::snowflake(emoji);
			} else {
				m_tag.emoji = std::string(emoji);
			}
		}
		m_tag.moderated = moderated;
	}
	DiscordForumTag(const dpp::forum_tag& tag) : m_tag(tag) {}

	std::string GetId() const { return m_tag.id.str(); }
	std::string GetName() const { return m_tag.name; }
	void SetName(const char* name) { m_tag.set_name(name); }

	std::string GetEmoji() const {
		if (std::holds_alternative<dpp::snowflake>(m_tag.emoji)) {
			return std::get<dpp::snowflake>(m_tag.emoji).str();
		} else if (std::holds_alternative<std::string>(m_tag.emoji)) {
			return std::get<std::string>(m_tag.emoji);
		}
		return "";
	}

	void SetEmoji(const char* emoji) {
		if (emoji && emoji[0] != '\0') {
			size_t len = strlen(emoji);
			if (std::all_of(emoji, emoji + len, ::isdigit)) {
				m_tag.emoji = dpp::snowflake(emoji);
			} else {
				m_tag.emoji = std::string(emoji);
			}
		} else {
			m_tag.emoji = std::monostate{};
		}
	}

	bool IsModerated() const { return m_tag.moderated; }
	void SetModerated(bool moderated) { m_tag.moderated = moderated; }

	bool EmojiIsCustom() const {
		return std::holds_alternative<dpp::snowflake>(m_tag.emoji);
	}

	const dpp::forum_tag& GetTag() const { return m_tag; }
};
