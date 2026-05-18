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

#pragma once

#include "discord_common.h"

class DiscordPoll
{
private:
	dpp::poll m_poll;

public:
	DiscordPoll() {}
	DiscordPoll(const dpp::poll& poll) : m_poll(poll) {}

	// Builder methods
	void SetQuestion(const char* text) { if (text) m_poll.set_question(text); }
	void SetDuration(uint32_t hours) { m_poll.set_duration(hours); }
	void SetMultiSelect(bool allow) { m_poll.set_allow_multiselect(allow); }
	void AddAnswer(const char* text) { if (text) m_poll.add_answer(text); }
	void AddAnswerWithEmoji(const char* text, const char* emoji) {
		if (text) m_poll.add_answer(text, emoji ? emoji : "");
	}

	// Getters
	const char* GetQuestion() const { return m_poll.get_question_text().c_str(); }
	double GetExpiry() const { return m_poll.expiry; }
	bool IsMultiSelect() const { return m_poll.allow_multiselect; }
	bool IsFinalized() const { return m_poll.results.has_value() && m_poll.results->is_finalized; }

	// Answer access
	size_t GetAnswerCount() const { return m_poll.answers.size(); }
	uint32_t GetAnswerId(size_t index) const {
		if (index >= m_poll.answers.size()) return 0;
		auto it = m_poll.answers.begin();
		std::advance(it, index);
		return it->second.id;
	}
	const char* GetAnswerText(size_t index) const {
		if (index >= m_poll.answers.size()) return "";
		auto it = m_poll.answers.begin();
		std::advance(it, index);
		return it->second.media.text.c_str();
	}

	// Results access
	bool HasResults() const { return m_poll.results.has_value(); }
	uint32_t GetVoteCount(uint32_t answer_id) const {
		auto count = m_poll.get_vote_count(answer_id);
		return count.value_or(0);
	}
	uint32_t GetTotalVoteCount() const {
		if (!m_poll.results.has_value()) return 0;
		uint32_t total = 0;
		for (const auto& [id, count] : m_poll.results->answer_counts) {
			total += count.count;
		}
		return total;
	}
	bool DidVote(uint32_t answer_id) const {
		if (!m_poll.results.has_value()) return false;
		auto it = m_poll.results->answer_counts.find(answer_id);
		return it != m_poll.results->answer_counts.end() ? it->second.me_voted : false;
	}

	const dpp::poll& GetPoll() const { return m_poll; }
};
