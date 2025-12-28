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

#include "core/handle_manager.h"
#include "utils/discord_embed.h"

static cell_t embed_CreateEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = new DiscordEmbed();

	return Handles.Create(pContext, embed, HandleId::DiscordEmbed);
}

static cell_t embed_SetTitle(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* title;
	pContext->LocalToString(params[2], &title);

	embed->SetTitle(title);
	return 1;
}

static cell_t embed_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* desc;
	pContext->LocalToString(params[2], &desc);

	embed->SetDescription(desc);
	return 1;
}

static cell_t embed_SetColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	embed->SetColor(params[2]);
	return 1;
}

static cell_t embed_SetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetUrl(url);
	return 1;
}

static cell_t embed_SetAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* url;
	pContext->LocalToString(params[3], &url);

	char* icon_url;
	pContext->LocalToString(params[4], &icon_url);

	embed->SetAuthor(name, url, icon_url);
	return 1;
}

static cell_t embed_SetFooter(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* text;
	pContext->LocalToString(params[2], &text);

	char* icon_url;
	pContext->LocalToString(params[3], &icon_url);

	embed->SetFooter(text, icon_url);
	return 1;
}

static cell_t embed_AddField(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	char* value;
	pContext->LocalToString(params[3], &value);

	bool inLine = params[4];

	embed->AddField(name, value, inLine);
	return 1;
}

static cell_t embed_SetThumbnail(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetThumbnail(url);
	return 1;
}

static cell_t embed_SetImage(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetImage(url);
	return 1;
}

static cell_t embed_SetVideo(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* url;
	pContext->LocalToString(params[2], &url);
	embed->SetVideo(url);
	return 1;
}

static cell_t embed_SetProvider(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	char* name;
	char* url = nullptr;
	pContext->LocalToString(params[2], &name);

	if (params[0] >= 3) {
		pContext->LocalToString(params[3], &url);
	}

	embed->SetProvider(name, url ? url : "");
	return 1;
}

static cell_t embed_SetTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	embed->SetTimestamp(params[2]);
	return 1;
}

static cell_t embed_GetTitle(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string title = embed->GetTitle();
	pContext->StringToLocal(params[2], params[3], title.c_str());
	return 1;
}

static cell_t embed_GetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string description = embed->GetDescription();
	pContext->StringToLocal(params[2], params[3], description.c_str());
	return 1;
}

static cell_t embed_GetColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return static_cast<cell_t>(embed->GetColor());
}

static cell_t embed_GetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetAuthorName(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string author_name = embed->GetAuthorName();
	pContext->StringToLocal(params[2], params[3], author_name.c_str());
	return 1;
}

static cell_t embed_GetAuthorUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string author_url = embed->GetAuthorUrl();
	pContext->StringToLocal(params[2], params[3], author_url.c_str());
	return 1;
}

static cell_t embed_GetAuthorIconUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string author_icon_url = embed->GetAuthorIconUrl();
	pContext->StringToLocal(params[2], params[3], author_icon_url.c_str());
	return 1;
}

static cell_t embed_GetFooterText(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string footer_text = embed->GetFooterText();
	pContext->StringToLocal(params[2], params[3], footer_text.c_str());
	return 1;
}

static cell_t embed_GetFooterIconUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string footer_icon_url = embed->GetFooterIconUrl();
	pContext->StringToLocal(params[2], params[3], footer_icon_url.c_str());
	return 1;
}

static cell_t embed_GetThumbnailUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string thumbnail_url = embed->GetThumbnailUrl();
	pContext->StringToLocal(params[2], params[3], thumbnail_url.c_str());
	return 1;
}

static cell_t embed_GetImageUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string image_url = embed->GetImageUrl();
	pContext->StringToLocal(params[2], params[3], image_url.c_str());
	return 1;
}

static cell_t embed_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string type = embed->GetType();
	pContext->StringToLocal(params[2], params[3], type.c_str());
	return 1;
}

static cell_t embed_GetTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return static_cast<cell_t>(embed->GetTimestamp());
}

static cell_t embed_GetAuthorProxyIconUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetAuthorProxyIconUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetFooterProxyUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetFooterProxyUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetThumbnailProxyUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetThumbnailProxyUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetThumbnailWidth(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetThumbnailWidth();
}

static cell_t embed_GetThumbnailHeight(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetThumbnailHeight();
}

static cell_t embed_GetImageProxyUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetImageProxyUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetImageWidth(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetImageWidth();
}

static cell_t embed_GetImageHeight(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetImageHeight();
}

static cell_t embed_GetVideoUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetVideoUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetVideoProxyUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetVideoProxyUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_GetVideoWidth(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetVideoWidth();
}

static cell_t embed_GetVideoHeight(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->GetVideoHeight();
}

static cell_t embed_GetProviderName(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string name = embed->GetProviderName();
	pContext->StringToLocal(params[2], params[3], name.c_str());
	return 1;
}

static cell_t embed_GetProviderUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	std::string url = embed->GetProviderUrl();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t embed_RemoveField(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->RemoveField(params[2]);
}

static cell_t embed_ClearFields(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	embed->ClearFields();
	return 1;
}

static cell_t embed_HasThumbnail(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasThumbnail();
}

static cell_t embed_HasImage(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasImage();
}

static cell_t embed_HasVideo(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasVideo();
}

static cell_t embed_HasProvider(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasProvider();
}

static cell_t embed_HasAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasAuthor();
}

static cell_t embed_HasFooter(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasFooter();
}

static cell_t embed_HasTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return embed->HasTimestamp();
}

static cell_t embed_GetFieldCount(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	return static_cast<cell_t>(embed->GetFieldCount());
}

static cell_t embed_GetFieldName(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	size_t index = static_cast<size_t>(params[2]);
	std::string field_name = embed->GetFieldName(index);
	pContext->StringToLocal(params[3], params[4], field_name.c_str());
	return !field_name.empty();
}

static cell_t embed_GetFieldValue(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	size_t index = static_cast<size_t>(params[2]);
	std::string field_value = embed->GetFieldValue(index);
	pContext->StringToLocal(params[3], params[4], field_value.c_str());
	return !field_value.empty();
}

static cell_t embed_GetFieldInline(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[1]);
	if (!embed) return 0;

	size_t index = static_cast<size_t>(params[2]);
	return embed->GetFieldInline(index);
}

extern const sp_nativeinfo_t embed_natives[] = {
	{"DiscordEmbed.DiscordEmbed", embed_CreateEmbed},
	{"DiscordEmbed.SetTitle", embed_SetTitle},
	{"DiscordEmbed.SetDescription", embed_SetDescription},
	{"DiscordEmbed.Color.set", embed_SetColor},
	{"DiscordEmbed.SetUrl", embed_SetUrl},
	{"DiscordEmbed.SetAuthor", embed_SetAuthor},
	{"DiscordEmbed.SetFooter", embed_SetFooter},
	{"DiscordEmbed.AddField", embed_AddField},
	{"DiscordEmbed.SetThumbnail", embed_SetThumbnail},
	{"DiscordEmbed.SetImage", embed_SetImage},
	{"DiscordEmbed.SetVideo", embed_SetVideo},
	{"DiscordEmbed.SetProvider", embed_SetProvider},
	{"DiscordEmbed.GetTitle", embed_GetTitle},
	{"DiscordEmbed.GetDescription", embed_GetDescription},
	{"DiscordEmbed.Color.get", embed_GetColor},
	{"DiscordEmbed.GetUrl", embed_GetUrl},
	{"DiscordEmbed.GetAuthorName", embed_GetAuthorName},
	{"DiscordEmbed.GetAuthorUrl", embed_GetAuthorUrl},
	{"DiscordEmbed.GetAuthorIconUrl", embed_GetAuthorIconUrl},
	{"DiscordEmbed.GetAuthorProxyIconUrl", embed_GetAuthorProxyIconUrl},
	{"DiscordEmbed.GetFooterText", embed_GetFooterText},
	{"DiscordEmbed.GetFooterIconUrl", embed_GetFooterIconUrl},
	{"DiscordEmbed.GetFooterProxyUrl", embed_GetFooterProxyUrl},
	{"DiscordEmbed.GetThumbnailUrl", embed_GetThumbnailUrl},
	{"DiscordEmbed.GetThumbnailProxyUrl", embed_GetThumbnailProxyUrl},
	{"DiscordEmbed.ThumbnailWidth.get", embed_GetThumbnailWidth},
	{"DiscordEmbed.ThumbnailHeight.get", embed_GetThumbnailHeight},
	{"DiscordEmbed.GetImageUrl", embed_GetImageUrl},
	{"DiscordEmbed.GetImageProxyUrl", embed_GetImageProxyUrl},
	{"DiscordEmbed.ImageWidth.get", embed_GetImageWidth},
	{"DiscordEmbed.ImageHeight.get", embed_GetImageHeight},
	{"DiscordEmbed.GetVideoUrl", embed_GetVideoUrl},
	{"DiscordEmbed.GetVideoProxyUrl", embed_GetVideoProxyUrl},
	{"DiscordEmbed.VideoWidth.get", embed_GetVideoWidth},
	{"DiscordEmbed.VideoHeight.get", embed_GetVideoHeight},
	{"DiscordEmbed.GetProviderName", embed_GetProviderName},
	{"DiscordEmbed.GetProviderUrl", embed_GetProviderUrl},
	{"DiscordEmbed.GetType", embed_GetType},
	{"DiscordEmbed.Timestamp.get", embed_GetTimestamp},
	{"DiscordEmbed.Timestamp.set", embed_SetTimestamp},
	{"DiscordEmbed.FieldCount.get", embed_GetFieldCount},
	{"DiscordEmbed.GetFieldName", embed_GetFieldName},
	{"DiscordEmbed.GetFieldValue", embed_GetFieldValue},
	{"DiscordEmbed.GetFieldInline", embed_GetFieldInline},
	{"DiscordEmbed.RemoveField", embed_RemoveField},
	{"DiscordEmbed.ClearFields", embed_ClearFields},
	{"DiscordEmbed.HasThumbnail.get", embed_HasThumbnail},
	{"DiscordEmbed.HasImage.get", embed_HasImage},
	{"DiscordEmbed.HasVideo.get", embed_HasVideo},
	{"DiscordEmbed.HasProvider.get", embed_HasProvider},
	{"DiscordEmbed.HasAuthor.get", embed_HasAuthor},
	{"DiscordEmbed.HasFooter.get", embed_HasFooter},
	{"DiscordEmbed.HasTimestamp.get", embed_HasTimestamp},
	{nullptr, nullptr}
};
