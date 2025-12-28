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

class DiscordComponent
{
private:
	dpp::component m_component;

public:
	DiscordComponent() : m_component() {}
	DiscordComponent(const dpp::component& component) : m_component(component) {}
	~DiscordComponent() {}

	dpp::component& GetComponent() { return m_component; }
	const dpp::component& GetComponent() const { return m_component; }

	void SetType(uint8_t type) { m_component.set_type(static_cast<dpp::component_type>(type)); }
	void SetLabel(const char* label) { if (label) m_component.set_label(label); }
	void SetCustomId(const char* custom_id) { if (custom_id) m_component.set_id(custom_id); }
	void SetStyle(uint8_t style) { m_component.set_style(static_cast<dpp::component_style>(style)); }
	void SetUrl(const char* url) { if (url) m_component.set_url(url); }
	void SetDisabled(bool disabled) { m_component.set_disabled(disabled); }
	void SetPlaceholder(const char* placeholder) { if (placeholder) m_component.set_placeholder(placeholder); }
	void SetMinValues(uint32_t min_values) { m_component.set_min_values(min_values); }
	void SetMaxValues(uint32_t max_values) { m_component.set_max_values(max_values); }
	void SetEmoji(const char* name, dpp::snowflake id = 0, bool animated = false) {
		if (name) m_component.set_emoji(name, id, animated);
	}
	void AddOption(const char* label, const char* value, const char* description = nullptr, bool is_default = false, const char* emoji_name = nullptr, dpp::snowflake emoji_id = 0) {
		if (!label || !value) return;
		dpp::select_option opt(label, value, description ? description : "");
		opt.is_default = is_default;
		if (emoji_name) opt.set_emoji(emoji_name, emoji_id);
		m_component.add_select_option(opt);
	}
	void AddComponent(DiscordComponent* component) {
		if (component) m_component.add_component(component->GetComponent());
	}
	void AddComponentV2(DiscordComponent* component) {
		if (component) m_component.add_component_v2(component->GetComponent());
	}
	void SetContent(const char* content) { if (content) m_component.set_content(content); }
	void SetThumbnail(const char* url) { if (url) m_component.set_thumbnail(url); }
	void SetAccessory(DiscordComponent* accessory) {
		if (accessory) m_component.set_accessory(accessory->GetComponent());
	}
	void SetFile(const char* attachment_url) {
		if (attachment_url) m_component.set_file(attachment_url);
	}
	void AddMediaGalleryItem(DiscordComponent* item) {
		if (item) m_component.add_media_gallery_item(item->GetComponent());
	}
	void SetDivider(bool divider) { m_component.set_divider(divider); }
	void SetSpacing(uint8_t spacing) { m_component.set_spacing(static_cast<dpp::separator_spacing>(spacing)); }
	void SetAccent(uint32_t accent_color) { m_component.set_accent(accent_color); }
	void SetDescription(const char* description) { if (description) m_component.set_description(description); }
	void SetSpoiler(bool spoiler) { m_component.set_spoiler(spoiler); }
	void SetSkuId(dpp::snowflake sku_id) { m_component.set_sku_id(sku_id); }
	void SetTextStyle(uint8_t text_style) { m_component.set_text_style(static_cast<dpp::text_style_type>(text_style)); }
	void SetDefaultValue(const char* value) { if (value) m_component.set_default_value(value); }
	void SetRequired(bool required) { m_component.set_required(required); }
	void SetMinLength(uint32_t min_length) { m_component.set_min_length(min_length); }
	void SetMaxLength(uint32_t max_length) { m_component.set_max_length(max_length); }
	void AddChannelType(uint8_t channel_type) { m_component.add_channel_type(channel_type); }
	void AddDefaultValue(dpp::snowflake id, uint8_t type) { m_component.add_default_value(id, static_cast<dpp::component_default_value_type>(type)); }
};
