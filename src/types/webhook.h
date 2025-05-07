#ifndef _INCLUDE_WEBHOOK_H
#define _INCLUDE_WEBHOOK_H

#include "object_handler.h"
#include "user.h"
#include "dpp/dpp.h"

class DiscordWebhook : public DiscordObject
{
public:
	dpp::webhook m_webhook;
	DiscordWebhook(const dpp::webhook& wbhk) : m_webhook(wbhk) {}

	std::string GetId() const { return std::to_string(m_webhook.id); }

	DiscordUser* GetUser() const { return new DiscordUser(m_webhook.user_obj); }

	const char* GetName() const { return m_webhook.name.c_str(); }

	void SetName(const char* value) { m_webhook.name = value; }

	const char* GetAvatarUrl() const { return m_webhook.avatar_url.c_str(); }

	void SetAvatarUrl(const char* value) { m_webhook.avatar_url = value; }

	std::string GetAvatarData() const { return m_webhook.avatar.to_string(); }

	void SetAvatarData(const char* value) { m_webhook.avatar = dpp::utility::iconhash(value); }
};

inline DiscordObjectHandler<DiscordWebhook> g_DiscordWebhookHandler;

extern const sp_nativeinfo_t webhook_natives[];

#endif //_INCLUDE_WEBHOOK_H