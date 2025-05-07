#ifndef _INCLUDE_USER_H
#define _INCLUDE_USER_H

#include "object_handler.h"
#include "dpp/dpp.h"

class DiscordUser : public DiscordObject
{
private:
    dpp::user m_user;

public:
    DiscordUser(const dpp::user& user) : m_user(user) {}

    std::string GetId() const { return std::to_string(m_user.id); }

    const char* GetUsername() const { return m_user.username.c_str(); }

    const uint16_t GetDiscriminator() const { return m_user.discriminator; }

    const char* GetGlobalName() const { return m_user.global_name.c_str(); }

    std::string GetAvatarUrl(bool prefer_animated_avatars) const { return m_user.get_avatar_url(0, dpp::i_png, prefer_animated_avatars); }

    bool IsBot() const { return m_user.is_bot(); }
};

inline DiscordObjectHandler<DiscordUser> g_DiscordUserHandler;

extern const sp_nativeinfo_t user_natives[];

#endif //_INCLUDE_USER_H
