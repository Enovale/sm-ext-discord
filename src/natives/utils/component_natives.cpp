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

#include "natives/natives_common.h"
#include "utils/discord_component.h"

static cell_t component_Constructor(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = new DiscordComponent();
	Handle_t handle = Handles.Create(pContext, component, HandleId::DiscordComponent);
	if (!handle) return 0;
	return handle;
}

static cell_t component_SetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	dpp::component_type type = static_cast<dpp::component_type>(params[2]);
	component->SetType(type);
	return 1;
}

static cell_t component_SetLabel(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* label;
	pContext->LocalToString(params[2], &label);
	component->SetLabel(label);
	return 1;
}

static cell_t component_SetCustomId(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* custom_id;
	pContext->LocalToString(params[2], &custom_id);
	component->SetCustomId(custom_id);
	return 1;
}

static cell_t component_SetStyle(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	dpp::component_style style = static_cast<dpp::component_style>(params[2]);
	component->SetStyle(style);
	return 1;
}

static cell_t component_SetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);
	component->SetUrl(url);
	return 1;
}

static cell_t component_SetDisabled(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	component->SetDisabled(params[2] != 0);
	return 1;
}

static cell_t component_SetPlaceholder(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* placeholder;
	pContext->LocalToString(params[2], &placeholder);
	component->SetPlaceholder(placeholder);
	return 1;
}

static cell_t component_SetMinValues(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	uint8_t min_values;
	if (!GetNativeUInt8(pContext, params[2], 25, "Minimum values", min_values)) return 0;
	component->SetMinValues(min_values);
	return 1;
}

static cell_t component_SetMaxValues(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	uint8_t max_values;
	if (!GetNativeUInt8(pContext, params[2], 25, "Maximum values", max_values)) return 0;
	component->SetMaxValues(max_values);
	return 1;
}

static cell_t component_SetEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	char* id_str;
	pContext->LocalToString(params[3], &id_str);

	dpp::snowflake id = 0;
	if (id_str[0] && !ParseSnowflake(pContext, id_str, id)) return 0;

	bool animated = params[4] != 0;

	component->SetEmoji(name, id, animated);
	return 1;
}

static cell_t component_AddOption(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char *label, *value, *description = nullptr, *emoji_name = nullptr;
	pContext->LocalToString(params[2], &label);
	pContext->LocalToString(params[3], &value);
	pContext->LocalToString(params[4], &description);
	bool is_default = params[5] != 0;
	pContext->LocalToString(params[6], &emoji_name);
	char* emoji_id_str;
	pContext->LocalToString(params[7], &emoji_id_str);

	dpp::snowflake emoji_id = 0;
	if (emoji_id_str[0] && !ParseSnowflake(pContext, emoji_id_str, emoji_id)) return 0;

	component->AddOption(label, value, description, is_default, emoji_name, emoji_id);
	return 1;
}

static cell_t component_AddComponent(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	DiscordComponent* child = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!child) return 0;

	component->AddComponent(child);
	return 1;
}

static cell_t component_AddComponentV2(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	DiscordComponent* child = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!child) return 0;

	component->AddComponentV2(child);
	return 1;
}

static cell_t component_SetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	component->SetContent(content);
	return 1;
}

static cell_t component_SetThumbnail(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);
	component->SetThumbnail(url);
	return 1;
}

static cell_t component_SetAccessory(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	DiscordComponent* accessory = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!accessory) return 0;

	component->SetAccessory(accessory);
	return 1;
}

static cell_t component_SetFile(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* attachment_url;
	pContext->LocalToString(params[2], &attachment_url);
	component->SetFile(attachment_url);
	return 1;
}

static cell_t component_AddMediaGalleryItem(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	DiscordComponent* item = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!item) return 0;
	component->AddMediaGalleryItem(item);
	return 1;
}

static cell_t component_SetDivider(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	component->SetDivider(params[2] != 0);
	return 1;
}

static cell_t component_SetSpacing(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	dpp::separator_spacing spacing = static_cast<dpp::separator_spacing>(params[2]);
	component->SetSpacing(spacing);
	return 1;
}

static cell_t component_SetAccent(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	uint32_t accent;
	if (!GetNativeUInt32(pContext, params[2], 0xFFFFFF, "Accent color", accent)) return 0;

	component->SetAccent(accent);
	return 1;
}

static cell_t component_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* description;
	pContext->LocalToString(params[2], &description);
	component->SetDescription(description);
	return 1;
}

static cell_t component_SetSpoiler(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	component->SetSpoiler(params[2] != 0);
	return 1;
}

static cell_t component_SetSkuId(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* sku_id_str;
	pContext->LocalToString(params[2], &sku_id_str);
	dpp::snowflake sku_id;
	if (!ParseSnowflake(pContext, sku_id_str, sku_id)) return 0;
	component->SetSkuId(sku_id);
	return 1;
}

static cell_t component_SetTextStyle(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	dpp::text_style_type text_style = static_cast<dpp::text_style_type>(params[2]);
	component->SetTextStyle(text_style);
	return 1;
}

static cell_t component_SetDefaultValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* value;
	pContext->LocalToString(params[2], &value);
	component->SetDefaultValue(value);
	return 1;
}

static cell_t component_SetRequired(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	component->SetRequired(params[2] != 0);
	return 1;
}

static cell_t component_SetMinLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	uint16_t min_length;
	if (!GetNativeUInt16(pContext, params[2], 4000, "Minimum length", min_length)) return 0;
	component->SetMinLength(min_length);
	return 1;
}

static cell_t component_SetMaxLength(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	uint16_t max_length;
	if (!GetNativeUInt16(pContext, params[2], 4000, "Maximum length", max_length)) return 0;
	component->SetMaxLength(max_length);
	return 1;
}

static cell_t component_AddChannelType(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	dpp::channel_type channel_type = static_cast<dpp::channel_type>(params[2]);
	component->AddChannelType(static_cast<uint8_t>(channel_type));
	return 1;
}

static cell_t component_AddDefaultValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[1]);
	if (!component) return 0;

	char* id_str;
	pContext->LocalToString(params[2], &id_str);
	dpp::snowflake id;
	if (!ParseSnowflake(pContext, id_str, id)) return 0;

	dpp::component_default_value_type type = static_cast<dpp::component_default_value_type>(params[3]);
	component->AddDefaultValue(id, type);
	return 1;
}

extern const sp_nativeinfo_t component_natives[] = {
	{"DiscordComponent.DiscordComponent", component_Constructor},

	// Properties (simple types: bool, int, enum)
	{"DiscordComponent.Type.set", component_SetType},
	{"DiscordComponent.Style.set", component_SetStyle},
	{"DiscordComponent.Disabled.set", component_SetDisabled},
	{"DiscordComponent.MinValues.set", component_SetMinValues},
	{"DiscordComponent.MaxValues.set", component_SetMaxValues},
	{"DiscordComponent.Divider.set", component_SetDivider},
	{"DiscordComponent.Spacing.set", component_SetSpacing},
	{"DiscordComponent.Accent.set", component_SetAccent},
	{"DiscordComponent.Spoiler.set", component_SetSpoiler},
	{"DiscordComponent.TextStyle.set", component_SetTextStyle},
	{"DiscordComponent.Required.set", component_SetRequired},
	{"DiscordComponent.MinLength.set", component_SetMinLength},
	{"DiscordComponent.MaxLength.set", component_SetMaxLength},

	// Set methods (strings and objects - cannot be properties)
	{"DiscordComponent.SetLabel", component_SetLabel},
	{"DiscordComponent.SetCustomId", component_SetCustomId},
	{"DiscordComponent.SetUrl", component_SetUrl},
	{"DiscordComponent.SetPlaceholder", component_SetPlaceholder},
	{"DiscordComponent.SetEmoji", component_SetEmoji},
	{"DiscordComponent.SetContent", component_SetContent},
	{"DiscordComponent.SetThumbnail", component_SetThumbnail},
	{"DiscordComponent.SetAccessory", component_SetAccessory},
	{"DiscordComponent.SetFile", component_SetFile},
	{"DiscordComponent.SetDescription", component_SetDescription},
	{"DiscordComponent.SetSkuId", component_SetSkuId},
	{"DiscordComponent.SetDefaultValue", component_SetDefaultValue},

	// Add methods
	{"DiscordComponent.AddOption", component_AddOption},
	{"DiscordComponent.AddComponent", component_AddComponent},
	{"DiscordComponent.AddComponentV2", component_AddComponentV2},
	{"DiscordComponent.AddMediaGalleryItem", component_AddMediaGalleryItem},
	{"DiscordComponent.AddChannelType", component_AddChannelType},
	{"DiscordComponent.AddDefaultValue", component_AddDefaultValue},

	{nullptr, nullptr}
};
