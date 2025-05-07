#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include "discord.h"
#include "object_handler.h"
#include "queue.h"
#include "smsdk_ext.h"
#include "dpp/dpp.h"

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

extern DiscordExtension g_DiscordExt;
extern ThreadSafeQueue<std::function<void()>> g_TaskQueue;

extern IForward* g_pForwardReady;
extern IForward* g_pForwardMessage;
extern IForward* g_pForwardError;
extern IForward* g_pForwardSlashCommand;
extern IForward* g_pForwardAutocomplete;

extern const sp_nativeinfo_t discord_natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
