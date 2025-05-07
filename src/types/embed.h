#ifndef _INCLUDE_EMBED_H
#define _INCLUDE_EMBED_H

#include "object_handler.h"
#include "dpp/dpp.h"

class DiscordEmbed : public DiscordObject
{
private:
    dpp::embed m_embed;

public:
    DiscordEmbed() {}

    void SetTitle(const char* title) { m_embed.set_title(title); }
    void SetDescription(const char* desc) { m_embed.set_description(desc); }
    void SetColor(int color) { m_embed.set_color(color); }
    void SetUrl(const char* url) { m_embed.set_url(url); }
    void SetAuthor(const char* name, const char* url = nullptr, const char* icon_url = nullptr) {
        m_embed.set_author(name, url ? url : "", icon_url ? icon_url : "");
    }
    void SetFooter(const char* text, const char* icon_url = nullptr) {
        m_embed.set_footer(text, icon_url ? icon_url : "");
    }
    void AddField(const char* name, const char* value, bool inLine = false) {
        m_embed.add_field(name, value, inLine);
    }
    void SetThumbnail(const char* url) { m_embed.set_thumbnail(url); }
    void SetImage(const char* url) { m_embed.set_image(url); }

    const dpp::embed& GetEmbed() const { return m_embed; }
};

inline DiscordObjectHandler<DiscordEmbed> g_DiscordEmbedHandler;

extern const sp_nativeinfo_t embed_natives[];

#endif //_INCLUDE_EMBED_H
