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

#include "dpp/dpp.h"

class DiscordClient;

class BaseOperations {
public:
	using Callback = std::function<void(const dpp::confirmation_callback_t&)>;

	BaseOperations(dpp::cluster* cluster, DiscordClient* client) : m_cluster(cluster), m_client(client) {}
	virtual ~BaseOperations() = default;

protected:
	dpp::cluster* m_cluster;
	DiscordClient* m_client;

	bool IsValid() const { return m_cluster && m_client; }
};
