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

#include "utils/discord_component.h"

/**
 * @brief Enhanced modal builder for creating Discord modals with convenience methods
 */
class DiscordModal
{
private:
	std::string m_custom_id;
	std::string m_title;
	std::vector<dpp::component> m_rows;

public:
	DiscordModal(const char* custom_id, const char* title)
		: m_custom_id(custom_id ? custom_id : ""), m_title(title ? title : "") {}

	~DiscordModal() {}

	/**
	 * @brief Add a pre-built row component to the modal
	 * @param row ActionRow component containing text inputs
	 */
	void AddRow(const DiscordComponent* row) {
		if (row && m_rows.size() < 5) {
			m_rows.push_back(row->GetComponent());
		}
	}

	/**
	 * @brief Convenience method to add a text input (automatically creates ActionRow)
	 * @param custom_id Custom ID for the text input
	 * @param label Label displayed above the input
	 * @param style Text input style (short or paragraph)
	 * @param required Whether the input is required
	 * @param placeholder Placeholder text (optional)
	 * @param min_length Minimum character length (0-4000)
	 * @param max_length Maximum character length (1-4000)
	 * @param default_value Pre-filled default value (optional)
	 * @return true if added successfully, false if modal is full
	 */
	bool AddTextInput(const char* custom_id, const char* label, int style,
					  bool required, const char* placeholder = nullptr,
					  int min_length = 0, int max_length = 4000,
					  const char* default_value = nullptr) {
		if (m_rows.size() >= 5) {
			return false;
		}

		dpp::component text_input;
		text_input.set_type(dpp::cot_text);
		text_input.set_text_style(static_cast<dpp::text_style_type>(style));
		text_input.set_label(label ? label : "");
		text_input.set_id(custom_id ? custom_id : "");

		if (placeholder && placeholder[0]) {
			text_input.set_placeholder(placeholder);
		}

		if (default_value && default_value[0]) {
			text_input.set_default_value(default_value);
		}

		text_input.set_required(required);
		text_input.set_min_length(min_length);
		text_input.set_max_length(max_length);

		// Create ActionRow and add text input to it
		dpp::component action_row;
		action_row.set_type(dpp::cot_action_row);
		action_row.add_component(text_input);

		m_rows.push_back(action_row);
		return true;
	}

	/**
	 * @brief Check if the modal is full (has 5 rows)
	 * @return true if modal has reached maximum capacity
	 */
	bool IsFull() const {
		return m_rows.size() >= 5;
	}

	/**
	 * @brief Clear all rows from the modal
	 */
	void Clear() {
		m_rows.clear();
	}

	/**
	 * @brief Get a row component by index
	 * @param index Row index (0-4)
	 * @return Pointer to component if exists, nullptr otherwise
	 */
	const dpp::component* GetRow(size_t index) const {
		if (index < m_rows.size()) {
			return &m_rows[index];
		}
		return nullptr;
	}

	const char* GetCustomId() const { return m_custom_id.c_str(); }
	const char* GetTitle() const { return m_title.c_str(); }
	const std::vector<dpp::component>& GetRows() const { return m_rows; }
	size_t GetRowCount() const { return m_rows.size(); }
};
