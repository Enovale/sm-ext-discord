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

class DiscordAttachment
{
private:
	dpp::attachment m_attachment;

public:
	DiscordAttachment(const dpp::attachment& att) : m_attachment(att) {}

	// Basic information
	std::string GetId() const { return m_attachment.id.str(); }
	const char* GetFilename() const { return m_attachment.filename.c_str(); }
	const char* GetDescription() const { return m_attachment.description.c_str(); }
	const char* GetContentType() const { return m_attachment.content_type.c_str(); }
	uint32_t GetSize() const { return m_attachment.size; }
	const char* GetUrl() const { return m_attachment.url.c_str(); }
	const char* GetProxyUrl() const { return m_attachment.proxy_url.c_str(); }
	uint32_t GetWidth() const { return m_attachment.width; }
	uint32_t GetHeight() const { return m_attachment.height; }
	bool IsEphemeral() const { return m_attachment.ephemeral; }
	double GetDurationSecs() const { return m_attachment.duration_secs; }
	const char* GetWaveform() const { return m_attachment.waveform.c_str(); }
	uint8_t GetFlags() const { return m_attachment.flags; }

	// Type checks
	bool IsSpoiler() const { return m_attachment.filename.length() > 8 && m_attachment.filename.substr(0, 8) == "SPOILER_"; }
	bool IsImage() const {
		const std::string& ct = m_attachment.content_type;
		return ct.find("image/") == 0;
	}
	bool IsVideo() const {
		const std::string& ct = m_attachment.content_type;
		return ct.find("video/") == 0;
	}
	bool IsAudio() const {
		const std::string& ct = m_attachment.content_type;
		return ct.find("audio/") == 0;
	}

	// Internal accessor
	const dpp::attachment& GetDPPAttachment() const { return m_attachment; }
};
