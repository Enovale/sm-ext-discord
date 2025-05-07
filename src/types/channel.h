#ifndef _INCLUDE_CHANNEL_H
#define _INCLUDE_CHANNEL_H

#include "object_handler.h"
#include "dpp/dpp.h"

class DiscordChannel : public DiscordObject
{
private:
    dpp::channel m_channel;

public:
    DiscordChannel(const dpp::channel& chnl) : m_channel(chnl) {}

    const char* GetName() const { return m_channel.name.c_str(); }
};

inline DiscordObjectHandler<DiscordChannel> g_DiscordChannelHandler;

extern const sp_nativeinfo_t channel_natives[];

#endif //_INCLUDE_CHANNEL_H
