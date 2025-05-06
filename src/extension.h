#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include "queue.h"
#include "dpp/dpp.h"
#include "discord.h"

class DiscordExtension : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char* error, size_t maxlength, bool late);
	virtual void SDK_OnUnload();
};

class DiscordHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void* object);
};

template <class T = DiscordObject> class DiscordObjectHandler : public IHandleTypeDispatch
{
public:
	HandleType_t HandleType;

	Handle_t CreateHandle(T* object, const HandleSecurity* sec, HandleError* err)
	{
		Handle_t handle = handlesys->CreateHandleEx(HandleType, object, sec, nullptr, err);

		return handle;
	}

	T* ReadHandle(Handle_t handle, const HandleSecurity* sec, HandleError* err)
	{
		T* obj;
		if ((*err = handlesys->ReadHandle(handle, HandleType, sec, (void**)&obj)) != HandleError_None)
		{
			return nullptr;
		}

		return obj;
	}

	T* ReadHandle(Handle_t handle)
	{
		HandleError err;
		HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
		return ReadHandle(handle, &sec, &err);
	}

	void OnHandleDestroy(HandleType_t type, void* object)
	{
		T* obj = (T*)object;
		delete obj;
	}
};

extern DiscordExtension g_DiscordExt;
extern ThreadSafeQueue<std::function<void()>> g_TaskQueue;

extern IForward* g_pForwardReady;
extern IForward* g_pForwardMessage;
extern IForward* g_pForwardError;
extern IForward* g_pForwardSlashCommand;
extern IForward* g_pForwardAutocomplete;

extern DiscordObjectHandler<DiscordClient> g_DiscordHandler;
extern DiscordObjectHandler<DiscordUser> g_DiscordUserHandler;
extern DiscordObjectHandler<DiscordMessage> g_DiscordMessageHandler;
extern DiscordObjectHandler<DiscordChannel> g_DiscordChannelHandler;
extern DiscordObjectHandler<DiscordWebhook> g_DiscordWebhookHandler;
extern DiscordObjectHandler<DiscordEmbed> g_DiscordEmbedHandler;
extern DiscordObjectHandler<DiscordInteraction> g_DiscordInteractionHandler;
extern DiscordObjectHandler<DiscordAutocompleteInteraction> g_DiscordAutocompleteInteractionHandler;

extern const sp_nativeinfo_t discord_natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
