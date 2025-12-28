#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Reaction Example",
	author = "ProjectSky",
	description = "Demonstrates adding reactions to messages",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_react", Command_React, ADMFLAG_ROOT, "Add reaction to a message");
	RegAdminCmd("sm_discord_react_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_react_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.Start();

	ReplyToCommand(client, "[Discord] Bot starting...");
	return Plugin_Handled;
}

void OnBotReady(Discord bot, DiscordEvent event, any data)
{
	char botName[64];
	bot.GetBotName(botName, sizeof(botName));

	char session_id[64];
	event.GetString("session_id", session_id, sizeof(session_id));
	int guild_count = event.GetInt("guild_count");

	PrintToServer("[Discord] Bot %s is ready! Session: %s, Guilds: %d", botName, session_id, guild_count);
}

Action Command_React(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 3)
	{
		ReplyToCommand(client, "Usage: sm_discord_react <channel_id> <message_id> <emoji>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE], messageId[SNOWFLAKE_SIZE], emoji[64];
	GetCmdArg(1, channelId, sizeof(channelId));
	GetCmdArg(2, messageId, sizeof(messageId));
	GetCmdArg(3, emoji, sizeof(emoji));

	DataPack pack = new DataPack();
	pack.WriteString(emoji);

	DiscordMessage.FetchMessage(g_Bot, messageId, channelId, OnMessageForReact, pack);

	ReplyToCommand(client, "[Discord] Adding reaction...");
	return Plugin_Handled;
}

void OnMessageForReact(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char emoji[64];
	pack.ReadString(emoji, sizeof(emoji));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch message: %s", error);
		return;
	}

	DiscordMessage message = view_as<DiscordMessage>(result.GetHandle("message"));

	// Add reaction (no callback available)
	message.AddReaction(emoji);

	PrintToServer("[Discord] Reaction '%s' added", emoji);
}
