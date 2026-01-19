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
#include "utils/discord_forum.h"
#include "entities/discord_channel.h"

static cell_t forumtag_Create(IPluginContext* pContext, const cell_t* params)
{
	char* name;
	char* emoji;
	pContext->LocalToString(params[1], &name);
	pContext->LocalToString(params[2], &emoji);
	bool moderated = params[3];

	DiscordForumTag* pForumTag = new DiscordForumTag(name, emoji ? emoji : "", moderated);
	Handle_t handle = Handles.Create(pContext, pForumTag, HandleId::DiscordForumTag);
	if (!handle) return 0;
	return handle;
}

static cell_t forumtag_GetId(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	std::string tagId = tag->GetId();
	pContext->StringToLocal(params[2], params[3], tagId.c_str());
	return 1;
}

static cell_t forumtag_GetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	std::string tagName = tag->GetName();
	pContext->StringToLocal(params[2], params[3], tagName.c_str());
	return 1;
}

static cell_t forumtag_SetName(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);
	tag->SetName(name);
	return 1;
}

static cell_t forumtag_GetEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	std::string emoji = tag->GetEmoji();
	pContext->StringToLocal(params[2], params[3], emoji.c_str());
	return 1;
}

static cell_t forumtag_SetEmoji(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	char* emoji;
	pContext->LocalToString(params[2], &emoji);
	tag->SetEmoji(emoji);
	return 1;
}

static cell_t forumtag_GetIsModerated(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	return tag->IsModerated();
}

static cell_t forumtag_SetIsModerated(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	bool moderated = params[2] != 0;
	tag->SetModerated(moderated);
	return 1;
}

static cell_t forumtag_GetEmojiIsCustom(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	return tag->EmojiIsCustom();
}

static cell_t forumtag_ApplyToChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordForumTag* tag = Handles.GetPointer<DiscordForumTag>(pContext, params[1]);
	if (!tag) return 0;

	DiscordChannel* channel = Handles.GetPointer<DiscordChannel>(pContext, params[2]);
	if (!channel) return 0;

	channel->ApplyForumTag(tag);
	return 1;
}

extern const sp_nativeinfo_t forum_natives[] = {
	{"DiscordForumTag.DiscordForumTag", forumtag_Create},
	{"DiscordForumTag.GetId", forumtag_GetId},
	{"DiscordForumTag.GetName", forumtag_GetName},
	{"DiscordForumTag.SetName", forumtag_SetName},
	{"DiscordForumTag.GetEmoji", forumtag_GetEmoji},
	{"DiscordForumTag.SetEmoji", forumtag_SetEmoji},
	{"DiscordForumTag.IsModerated.get", forumtag_GetIsModerated},
	{"DiscordForumTag.IsModerated.set", forumtag_SetIsModerated},
	{"DiscordForumTag.EmojiIsCustom.get", forumtag_GetEmojiIsCustom},
	{"DiscordForumTag.ApplyToChannel", forumtag_ApplyToChannel},
	{nullptr, nullptr}
};
