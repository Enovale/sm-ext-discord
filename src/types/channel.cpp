#include "channel.h"

static cell_t channel_GetName(IPluginContext* pContext, const cell_t* params)
{
    DiscordChannel* channel = g_DiscordChannelHandler.ReadHandle(params[1]);
    if (!channel) {
        return 0;
    }

    pContext->StringToLocal(params[2], params[3], channel->GetName());
    return 1;
}

const sp_nativeinfo_t channel_natives[] = {
    {"DiscordChannel.GetName",       channel_GetName}
};