#if defined _discord_included_
  #endinput
#endif
#define _discord_included_

#define MAX_DISCORD_MESSAGE_LENGTH       2001
#define MAX_DISCORD_NITRO_MESSAGE_LENGTH 4001
#define SNOWFLAKE_SIZE                   20
#define MAX_DISCORD_NAME_LENGTH          33
#define MAX_DISCORD_PRESENCE_LENGTH      129
#define MAX_DISCORD_CHANNEL_NAME_LENGTH  101
#define MAX_COMMAND_NAME                 33

// Limits found here: https://discordjs.guide/legacy/popular-topics/embeds#embed-limits
#define DISCORD_TITLE_LENGTH  257
#define DISCORD_DESC_LENGTH   4097
#define DISCORD_FIELD_LENGTH  1025
#define DISCORD_FOOTER_LENGTH 2049
#define DISCORD_EMBED_LENGTH  6001

// Include all sub-files
#include <discord/enums>
#include <discord/helpers>
#include <discord/event>
#include <discord/result>
#include <discord/handle_array>
#include <discord/callbacks>
#include <discord/http>
#include <discord/client>
#include <discord/user>
#include <discord/guild>
#include <discord/command>
#include <discord/message>
#include <discord/channel>
#include <discord/forum>
#include <discord/webhook>
#include <discord/poll>
#include <discord/embed>
#include <discord/interaction>
#include <discord/autocomplete>
#include <discord/role>
#include <discord/invite>
#include <discord/component>
#include <discord/modal>
#include <discord/button_interaction>
#include <discord/select_interaction>
#include <discord/modal_interaction>
#include <discord/guild_member>
#include <discord/emoji>
#include <discord/sticker>
#include <discord/scheduled_event>
#include <discord/attachment>
#include <discord/reaction>
#include <discord/ban>
#include <discord/thread_member>

public Extension __ext_discord = {
  name = "discord",
  file = "discord.ext",
#if defined AUTOLOAD_EXTENSIONS
  autoload = 1,
#else
  autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
  required = 1,
#else
  required = 0,
#endif
};
