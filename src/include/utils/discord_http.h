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
#include <map>
#include <string>
#include <vector>

// HTTP Classes
class HttpHeaders
{
private:
	using HeaderMap = std::map<std::string, std::string>;

	HeaderMap m_headers;
	mutable std::vector<HeaderMap::const_iterator> m_headerIndex;
	mutable bool m_headerIndexDirty = true;

	void InvalidateHeaderIndex() {
		m_headerIndexDirty = true;
	}

	void EnsureHeaderIndex() const {
		if (!m_headerIndexDirty) return;

		m_headerIndex.clear();
		m_headerIndex.reserve(m_headers.size());
		for (auto it = m_headers.cbegin(); it != m_headers.cend(); ++it) {
			m_headerIndex.push_back(it);
		}
		m_headerIndexDirty = false;
	}

public:
	HttpHeaders() {}

	void SetHeader(const char* name, const char* value) {
		auto result = m_headers.insert_or_assign(name, value);
		if (result.second) InvalidateHeaderIndex();
	}

	bool GetHeader(const char* name, std::string& value) const {
		auto it = m_headers.find(name);
		if (it != m_headers.end()) {
			value = it->second;
			return true;
		}
		return false;
	}

	bool RemoveHeader(const char* name) {
		size_t erased = m_headers.erase(name);
		if (erased) InvalidateHeaderIndex();
		return erased > 0;
	}

	void ClearHeaders() {
		if (!m_headers.empty()) InvalidateHeaderIndex();
		m_headers.clear();
	}

	size_t Count() const {
		return m_headers.size();
	}

	bool GetHeaderByIndex(size_t index, std::string& name, std::string& value) const {
		if (index >= m_headers.size()) return false;
		EnsureHeaderIndex();

		auto it = m_headerIndex[index];
		name = it->first;
		value = it->second;
		return true;
	}

	// Convert to DPP headers format
	dpp::http_headers ToDppHeaders() const {
		dpp::http_headers headers;
		for (const auto& [key, value] : m_headers) {
			headers.emplace(key, value);
		}
		return headers;
	}
};

class HttpCompletion
{
private:
	dpp::http_request_completion_t m_completion;
	std::string m_protocol;
	mutable CachedHandle<HttpHeaders> m_responseHeadersHandle;

public:
	HttpCompletion(const dpp::http_request_completion_t& completion, const std::string& protocol = "1.1")
		: m_completion(completion), m_protocol(protocol) {}

	Handle_t GetResponseHeadersHandle() const {
		return m_responseHeadersHandle.GetOrCreate([this]() { return CreateResponseHeaders(); });
	}

	int GetStatus() const {
		return static_cast<int>(m_completion.status);
	}

	const std::string& GetBody() const {
		return m_completion.body;
	}

	size_t GetBodyLength() const {
		return m_completion.body.length();
	}

	// Create HttpHeaders object from response headers
	HttpHeaders* CreateResponseHeaders() const {
		HttpHeaders* headers = new HttpHeaders();
		for (const auto& [key, value] : m_completion.headers) {
			headers->SetHeader(key.c_str(), value.c_str());
		}
		return headers;
	}

	const std::string& GetProtocol() const {
		return m_protocol;
	}

	bool IsSuccess() const {
		return m_completion.error == dpp::http_error::h_success && m_completion.status >= 200 && m_completion.status < 300;
	}
};
