#include "extension.h"

#define MAX_PROCESS 10

DiscordExtension g_DiscordExt;
SMEXT_LINK(&g_DiscordExt);

DiscordObjectHandler<DiscordClient> g_DiscordHandler;
DiscordObjectHandler<DiscordUser> g_DiscordUserHandler;
DiscordObjectHandler<DiscordMessage> g_DiscordMessageHandler;
DiscordObjectHandler<DiscordChannel> g_DiscordChannelHandler;
DiscordObjectHandler<DiscordWebhook> g_DiscordWebhookHandler;
DiscordObjectHandler<DiscordEmbed> g_DiscordEmbedHandler;
DiscordObjectHandler<DiscordInteraction> g_DiscordInteractionHandler;
DiscordObjectHandler<DiscordAutocompleteInteraction> g_DiscordAutocompleteInteractionHandler;

IForward* g_pForwardReady = nullptr;
IForward* g_pForwardMessage = nullptr;
IForward* g_pForwardError = nullptr;
IForward* g_pForwardSlashCommand = nullptr;
IForward* g_pForwardAutocomplete = nullptr;

ThreadSafeQueue<std::function<void()>> g_TaskQueue;

static void OnGameFrame(bool simulating) {
	std::function<void()> task;
	int count = 0;
	while (g_TaskQueue.TryPop(task) && count < MAX_PROCESS) {
		task();
		count++;
	}
}

bool DiscordExtension::SDK_OnLoad(char* error, size_t maxlen, bool late)
{
	sharesys->AddNatives(myself, discord_natives);
	sharesys->RegisterLibrary(myself, "discord");

	HandleAccess haDefaults;
	handlesys->InitAccessDefaults(nullptr, &haDefaults);
	haDefaults.access[HandleAccess_Delete] = 0;

	g_DiscordHandler.HandleType = handlesys->CreateType("Discord", &g_DiscordHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordUserHandler.HandleType = handlesys->CreateType("DiscordUser", &g_DiscordUserHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordMessageHandler.HandleType = handlesys->CreateType("DiscordMessage", &g_DiscordMessageHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordChannelHandler.HandleType = handlesys->CreateType("DiscordChannel", &g_DiscordChannelHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordWebhookHandler.HandleType = handlesys->CreateType("DiscordWebhook", &g_DiscordWebhookHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordEmbedHandler.HandleType = handlesys->CreateType("DiscordEmbed", &g_DiscordEmbedHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordInteractionHandler.HandleType = handlesys->CreateType("DiscordInteraction", &g_DiscordInteractionHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);
	g_DiscordAutocompleteInteractionHandler.HandleType = handlesys->CreateType("DiscordAutocompleteInteraction", &g_DiscordAutocompleteInteractionHandler, 0, nullptr, &haDefaults, myself->GetIdentity(), nullptr);

	g_pForwardReady = forwards->CreateForward("Discord_OnReady", ET_Ignore, 1, nullptr, Param_Cell);
	g_pForwardMessage = forwards->CreateForward("Discord_OnMessage", ET_Ignore, 2, nullptr, Param_Cell, Param_Cell);
	g_pForwardError = forwards->CreateForward("Discord_OnError", ET_Ignore, 2, nullptr, Param_Cell, Param_String);
	g_pForwardSlashCommand = forwards->CreateForward("Discord_OnSlashCommand", ET_Ignore, 2, nullptr, Param_Cell, Param_Cell);
	g_pForwardAutocomplete = forwards->CreateForward("Discord_OnAutocomplete", ET_Ignore, 5, nullptr, Param_Cell, Param_Cell, Param_Cell, Param_Cell, Param_String);

	smutils->AddGameFrameHook(&OnGameFrame);

	return true;
}

void DiscordExtension::SDK_OnUnload()
{
	forwards->ReleaseForward(g_pForwardReady);
	forwards->ReleaseForward(g_pForwardMessage);
	forwards->ReleaseForward(g_pForwardError);
	forwards->ReleaseForward(g_pForwardSlashCommand);
	forwards->ReleaseForward(g_pForwardAutocomplete);

	handlesys->RemoveType(g_DiscordHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordUserHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordMessageHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordChannelHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordWebhookHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordEmbedHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordInteractionHandler.HandleType, myself->GetIdentity());
	handlesys->RemoveType(g_DiscordAutocompleteInteractionHandler.HandleType, myself->GetIdentity());

	smutils->RemoveGameFrameHook(&OnGameFrame);
}

void DiscordHandler::OnHandleDestroy(HandleType_t type, void* object)
{
	DiscordClient* discord = (DiscordClient*)object;
	discord->Stop();
	delete discord;
}