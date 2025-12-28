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

#include "core/operations/http_operations.h"
#include "utils/discord_common.h"

void HttpOperations::Request(const std::string& url, dpp::http_method method, std::function<void(const dpp::http_request_completion_t&)> callback,
	const std::string& body, const std::string& mime_type, const dpp::http_headers& headers) {
	if (!IsValid()) return;
	m_cluster->request(url, method, callback, body, mime_type, headers, "1.1");
}
