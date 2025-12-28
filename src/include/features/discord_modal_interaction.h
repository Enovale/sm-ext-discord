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

#include "features/discord_interaction_base.h"

class DiscordModalInteraction : public DiscordInteractionBase<DiscordModalInteraction, dpp::form_submit_t>
{
	using Base = DiscordInteractionBase<DiscordModalInteraction, dpp::form_submit_t>;

public:
	DiscordModalInteraction(const dpp::form_submit_t& event, DiscordClient* client)
		: Base(event, client) {}

	// Modal-specific methods
	const std::vector<dpp::component>& GetComponents() const { return m_event.components; }

	// Get text input value by custom_id
	std::string GetValue(const std::string& custom_id) const {
		for (const auto& component : m_event.components) {
			if (component.custom_id == custom_id) {
				if (std::holds_alternative<std::string>(component.value)) {
					return std::get<std::string>(component.value);
				}
			}
			for (const auto& nested : component.components) {
				if (nested.custom_id == custom_id) {
					if (std::holds_alternative<std::string>(nested.value)) {
						return std::get<std::string>(nested.value);
					}
				}
			}
		}
		return "";
	}
};
