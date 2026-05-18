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

class DiscordEmbed
{
private:
	dpp::embed m_embed;

public:
	DiscordEmbed() {}

	void SetTitle(const char* title) { m_embed.set_title(title); }
	void SetDescription(const char* desc) { m_embed.set_description(desc); }
	void SetColor(uint32_t color) { m_embed.set_color(color); }
	void SetUrl(const char* url) { m_embed.set_url(url); }
	void SetAuthor(const char* name, const char* url = nullptr, const char* icon_url = nullptr) {
		m_embed.set_author(name, url ? url : "", icon_url ? icon_url : "");
	}
	void SetFooter(const char* text, const char* icon_url = nullptr) {
		m_embed.set_footer(text, icon_url ? icon_url : "");
	}
	void AddField(const char* name, const char* value, bool inLine = false) {
		m_embed.add_field(name, value, inLine);
	}
	void SetThumbnail(const char* url) { m_embed.set_thumbnail(url); }
	void SetImage(const char* url) { m_embed.set_image(url); }
	void SetTimestamp(time_t timestamp) { m_embed.set_timestamp(timestamp); }
	void SetVideo(const char* url) { m_embed.set_video(url); }
	void SetProvider(const char* name, const char* url = nullptr) {
		m_embed.set_provider(name, url ? url : "");
	}

	// Get methods
	const std::string& GetTitle() const { return m_embed.title; }
	const std::string& GetDescription() const { return m_embed.description; }
	uint32_t GetColor() const { return m_embed.color.value_or(0); }
	time_t GetTimestamp() const { return m_embed.timestamp; }
	const std::string& GetType() const { return m_embed.type; }
	const std::string& GetUrl() const { return m_embed.url; }
	std::string GetAuthorName() const { return m_embed.author.has_value() ? m_embed.author->name : ""; }
	std::string GetAuthorUrl() const { return m_embed.author.has_value() ? m_embed.author->url : ""; }
	std::string GetAuthorIconUrl() const { return m_embed.author.has_value() ? m_embed.author->icon_url : ""; }
	std::string GetAuthorProxyIconUrl() const { return m_embed.author.has_value() ? m_embed.author->proxy_icon_url : ""; }
	std::string GetFooterText() const { return m_embed.footer.has_value() ? m_embed.footer->text : ""; }
	std::string GetFooterIconUrl() const { return m_embed.footer.has_value() ? m_embed.footer->icon_url : ""; }
	std::string GetFooterProxyUrl() const { return m_embed.footer.has_value() ? m_embed.footer->proxy_url : ""; }
	std::string GetThumbnailUrl() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->url : ""; }
	std::string GetThumbnailProxyUrl() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->proxy_url : ""; }
	uint32_t GetThumbnailWidth() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->width : 0; }
	uint32_t GetThumbnailHeight() const { return m_embed.thumbnail.has_value() ? m_embed.thumbnail->height : 0; }
	std::string GetImageUrl() const { return m_embed.image.has_value() ? m_embed.image->url : ""; }
	std::string GetImageProxyUrl() const { return m_embed.image.has_value() ? m_embed.image->proxy_url : ""; }
	uint32_t GetImageWidth() const { return m_embed.image.has_value() ? m_embed.image->width : 0; }
	uint32_t GetImageHeight() const { return m_embed.image.has_value() ? m_embed.image->height : 0; }
	std::string GetVideoUrl() const { return m_embed.video.has_value() ? m_embed.video->url : ""; }
	std::string GetVideoProxyUrl() const { return m_embed.video.has_value() ? m_embed.video->proxy_url : ""; }
	uint32_t GetVideoWidth() const { return m_embed.video.has_value() ? m_embed.video->width : 0; }
	uint32_t GetVideoHeight() const { return m_embed.video.has_value() ? m_embed.video->height : 0; }
	std::string GetProviderName() const { return m_embed.provider.has_value() ? m_embed.provider->name : ""; }
	std::string GetProviderUrl() const { return m_embed.provider.has_value() ? m_embed.provider->url : ""; }
	size_t GetFieldCount() const { return m_embed.fields.size(); }
	std::string GetFieldName(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].name : ""; }
	std::string GetFieldValue(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].value : ""; }
	bool GetFieldInline(size_t index) const { return index < m_embed.fields.size() ? m_embed.fields[index].is_inline : false; }

	// Field management
	bool RemoveField(size_t index) {
		if (index < m_embed.fields.size()) {
			m_embed.fields.erase(m_embed.fields.begin() + index);
			return true;
		}
		return false;
	}
	void ClearFields() { m_embed.fields.clear(); }

	// Utility properties
	bool HasThumbnail() const { return m_embed.thumbnail.has_value(); }
	bool HasImage() const { return m_embed.image.has_value(); }
	bool HasVideo() const { return m_embed.video.has_value(); }
	bool HasProvider() const { return m_embed.provider.has_value(); }
	bool HasAuthor() const { return m_embed.author.has_value(); }
	bool HasFooter() const { return m_embed.footer.has_value(); }
	bool HasTimestamp() const { return m_embed.timestamp != 0; }

	const dpp::embed& GetEmbed() const { return m_embed; }
};
