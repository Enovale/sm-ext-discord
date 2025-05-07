#include "user.h"

static cell_t user_GetId(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    pContext->StringToLocal(params[2], params[3], user->GetId().c_str());
    return 1;
}

static cell_t user_GetUsername(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    pContext->StringToLocal(params[2], params[3], user->GetUsername());
    return 1;
}

static cell_t user_GetDiscriminator(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    return user->GetDiscriminator();
}

static cell_t user_GetGlobalName(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    pContext->StringToLocal(params[2], params[3], user->GetGlobalName());
    return 1;
}

static cell_t user_GetAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    pContext->StringToLocal(params[3], params[4], user->GetAvatarUrl(params[2] ? true : false).c_str());
    return 1;
}

static cell_t user_IsBot(IPluginContext* pContext, const cell_t* params)
{
    DiscordUser* user = g_DiscordUserHandler.ReadHandle(params[1]);
    if (!user) {
        return 0;
    }

    return user->IsBot() ? 1 : 0;
}

const sp_nativeinfo_t user_natives[] = {
    {"DiscordUser.GetId",    user_GetId},
    {"DiscordUser.GetUsername",    user_GetUsername},
    {"DiscordUser.GetDiscriminator",    user_GetDiscriminator},
    {"DiscordUser.GetGlobalName",    user_GetGlobalName},
    {"DiscordUser.GetAvatarUrl",       user_GetAvatarUrl},
    {"DiscordUser.IsBot",    user_IsBot}
};