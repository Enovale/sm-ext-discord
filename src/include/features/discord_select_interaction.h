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

class DiscordSelectInteraction : public DiscordComponentInteractionBase<DiscordSelectInteraction, dpp::select_click_t>
{
	using Base = DiscordComponentInteractionBase<DiscordSelectInteraction, dpp::select_click_t>;

public:
	DiscordSelectInteraction(const dpp::select_click_t& event, DiscordClient* client)
		: Base(event, client) {}

	// Select-specific method
	const std::vector<std::string>& GetValues() const { return m_event.values; }
};
