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

#include "natives/entity_natives_common.h"
#include "entities/discord_attachment.h"

static cell_t attachment_GetFilename(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	pContext->StringToLocal(params[2], params[3], att->GetFilename());
	return 1;
}

static cell_t attachment_GetContentType(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	pContext->StringToLocal(params[2], params[3], att->GetContentType());
	return 1;
}

static cell_t attachment_GetSize(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return static_cast<cell_t>(att->GetSize());
}

static cell_t attachment_GetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	pContext->StringToLocal(params[2], params[3], att->GetUrl());
	return 1;
}

static cell_t attachment_GetProxyUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	pContext->StringToLocal(params[2], params[3], att->GetProxyUrl());
	return 1;
}

static cell_t attachment_GetWidth(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return static_cast<cell_t>(att->GetWidth());
}

static cell_t attachment_GetHeight(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return static_cast<cell_t>(att->GetHeight());
}

static cell_t attachment_IsEphemeral(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return att->IsEphemeral();
}

static cell_t attachment_GetDurationSecs(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return sp_ftoc(static_cast<float>(att->GetDurationSecs()));
}

static cell_t attachment_GetWaveform(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	pContext->StringToLocal(params[2], params[3], att->GetWaveform());
	return 1;
}

static cell_t attachment_IsSpoiler(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return att->IsSpoiler();
}

static cell_t attachment_IsImage(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return att->IsImage();
}

static cell_t attachment_IsVideo(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return att->IsVideo();
}

static cell_t attachment_IsAudio(IPluginContext* pContext, const cell_t* params)
{
	DiscordAttachment* att = Handles.GetPointer<DiscordAttachment>(pContext, params[1]);
	if (!att) return 0;

	return att->IsAudio();
}

extern const sp_nativeinfo_t attachment_natives[] = {
	{"DiscordAttachment.GetId", EntityGetId<DiscordAttachment>},
	{"DiscordAttachment.GetFilename", attachment_GetFilename},
	{"DiscordAttachment.GetDescription", EntityGetDescription<DiscordAttachment>},
	{"DiscordAttachment.GetContentType", attachment_GetContentType},
	{"DiscordAttachment.Size.get", attachment_GetSize},
	{"DiscordAttachment.GetUrl", attachment_GetUrl},
	{"DiscordAttachment.GetProxyUrl", attachment_GetProxyUrl},
	{"DiscordAttachment.Width.get", attachment_GetWidth},
	{"DiscordAttachment.Height.get", attachment_GetHeight},
	{"DiscordAttachment.Ephemeral.get", attachment_IsEphemeral},
	{"DiscordAttachment.DurationSecs.get", attachment_GetDurationSecs},
	{"DiscordAttachment.GetWaveform", attachment_GetWaveform},
	{"DiscordAttachment.Flags.get", EntityGetFlags<DiscordAttachment>},
	{"DiscordAttachment.IsSpoiler.get", attachment_IsSpoiler},
	{"DiscordAttachment.IsImage.get", attachment_IsImage},
	{"DiscordAttachment.IsVideo.get", attachment_IsVideo},
	{"DiscordAttachment.IsAudio.get", attachment_IsAudio},
	{nullptr, nullptr}
};
