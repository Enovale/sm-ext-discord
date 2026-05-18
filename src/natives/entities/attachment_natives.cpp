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

extern const sp_nativeinfo_t attachment_natives[] = {
	{"DiscordAttachment.GetId", EntityGetId<DiscordAttachment>},
	{"DiscordAttachment.GetFilename", attachment_GetFilename},
	{"DiscordAttachment.GetDescription", EntityGetDescription<DiscordAttachment>},
	{"DiscordAttachment.GetContentType", attachment_GetContentType},
	{"DiscordAttachment.Size.get", EntityGetInt<DiscordAttachment, uint32_t, &DiscordAttachment::GetSize>},
	{"DiscordAttachment.GetUrl", attachment_GetUrl},
	{"DiscordAttachment.GetProxyUrl", attachment_GetProxyUrl},
	{"DiscordAttachment.Width.get", EntityGetInt<DiscordAttachment, uint32_t, &DiscordAttachment::GetWidth>},
	{"DiscordAttachment.Height.get", EntityGetInt<DiscordAttachment, uint32_t, &DiscordAttachment::GetHeight>},
	{"DiscordAttachment.Ephemeral.get", EntityGetBool<DiscordAttachment, &DiscordAttachment::IsEphemeral>},
	{"DiscordAttachment.DurationSecs.get", attachment_GetDurationSecs},
	{"DiscordAttachment.GetWaveform", attachment_GetWaveform},
	{"DiscordAttachment.Flags.get", EntityGetFlags<DiscordAttachment>},
	{"DiscordAttachment.IsSpoiler.get", EntityGetBool<DiscordAttachment, &DiscordAttachment::IsSpoiler>},
	{"DiscordAttachment.IsImage.get", EntityGetBool<DiscordAttachment, &DiscordAttachment::IsImage>},
	{"DiscordAttachment.IsVideo.get", EntityGetBool<DiscordAttachment, &DiscordAttachment::IsVideo>},
	{"DiscordAttachment.IsAudio.get", EntityGetBool<DiscordAttachment, &DiscordAttachment::IsAudio>},
	{nullptr, nullptr}
};
