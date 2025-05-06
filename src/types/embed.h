#ifndef _INCLUDE_EMBED_H
#define _INCLUDE_EMBED_H

#include "extension.h"

static cell_t embed_CreateEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = new DiscordEmbed();

	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());
	Handle_t handle = g_DiscordEmbedHandler.CreateHandle(embed, &sec, &err);

	if (handle == BAD_HANDLE)
	{
		delete embed;
		return pContext->ThrowNativeError("Could not create Discord embed handle (error %d)", err);
	}

	return handle;
}

static cell_t embed_SetTitle(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* title;
	pContext->LocalToString(params[2], &title);

	embed->SetTitle(title);
	return 1;
}

static cell_t embed_SetDescription(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* desc;
	pContext->LocalToString(params[2], &desc);

	embed->SetDescription(desc);
	return 1;
}

static cell_t embed_SetColor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	embed->SetColor(params[2]);
	return 1;
}

static cell_t embed_SetUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetUrl(url);
	return 1;
}

static cell_t embed_SetAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	char* url = nullptr;
	if (params[3] != 0) {
		pContext->LocalToString(params[3], &url);
	}

	char* icon_url = nullptr;
	if (params[4] != 0) {
		pContext->LocalToString(params[4], &icon_url);
	}

	embed->SetAuthor(name, url, icon_url);
	return 1;
}

static cell_t embed_SetFooter(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* text;
	pContext->LocalToString(params[2], &text);

	char* icon_url = nullptr;
	if (params[3] != 0) {
		pContext->LocalToString(params[3], &icon_url);
	}

	embed->SetFooter(text, icon_url);
	return 1;
}

static cell_t embed_AddField(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	char* value;
	pContext->LocalToString(params[3], &value);

	bool inLine = params[4] ? true : false;

	embed->AddField(name, value, inLine);
	return 1;
}

static cell_t embed_SetThumbnail(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetThumbnail(url);
	return 1;
}

static cell_t embed_SetImage(IPluginContext* pContext, const cell_t* params)
{
	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[1]);
	if (!embed) {
		return 0;
	}

	char* url;
	pContext->LocalToString(params[2], &url);

	embed->SetImage(url);
	return 1;
}

#endif //_INCLUDE_EMBED_H
