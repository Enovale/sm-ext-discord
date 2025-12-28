#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Forum Example",
	author = "ProjectSky",
	description = "Demonstrates creating forum posts",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_forum", Command_CreatePost, ADMFLAG_ROOT, "Create a forum post");
	RegAdminCmd("sm_discord_forum_embed", Command_CreatePostEmbed, ADMFLAG_ROOT, "Create a forum post with embed");
	RegAdminCmd("sm_discord_forum_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_forum_start <bot_token>");
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

Action Command_CreatePost(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_forum <forum_channel_id> <title>");
		return Plugin_Handled;
	}

	char forumId[SNOWFLAKE_SIZE], title[128];
	GetCmdArg(1, forumId, sizeof(forumId));
	GetCmdArg(2, title, sizeof(title));

	DataPack pack = new DataPack();
	pack.WriteString(title);

	DiscordChannel.FetchChannel(g_Bot, forumId, OnForumFetched, pack);

	ReplyToCommand(client, "[Discord] Creating forum post...");
	return Plugin_Handled;
}

void OnForumFetched(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char title[128];
	pack.ReadString(title, sizeof(title));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch forum channel: %s", error);
		return;
	}

	DiscordChannel forum = view_as<DiscordChannel>(result.GetHandle("channel"));

	// Create forum post with message content
	char content[256];
	FormatEx(content, sizeof(content), "This is a new forum post created from the game server.");

	forum.CreateForumThread(title, content);

	PrintToServer("[Discord] Forum post creation initiated: %s", title);
}

Action Command_CreatePostEmbed(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_forum_embed <forum_channel_id> <title>");
		return Plugin_Handled;
	}

	char forumId[SNOWFLAKE_SIZE], title[128];
	GetCmdArg(1, forumId, sizeof(forumId));
	GetCmdArg(2, title, sizeof(title));

	DataPack pack = new DataPack();
	pack.WriteString(title);

	DiscordChannel.FetchChannel(g_Bot, forumId, OnForumFetchedEmbed, pack);

	ReplyToCommand(client, "[Discord] Creating forum post with embed...");
	return Plugin_Handled;
}

void OnForumFetchedEmbed(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char title[128];
	pack.ReadString(title, sizeof(title));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch forum channel: %s", error);
		return;
	}

	DiscordChannel forum = view_as<DiscordChannel>(result.GetHandle("channel"));

	// Create embed for forum post
	DiscordEmbed embed = new DiscordEmbed();
	embed.SetTitle("Server Report");
	embed.SetDescription("Automated server status report");
	embed.Color = 0x5865F2;

	char mapName[64], playerCount[32];
	GetCurrentMap(mapName, sizeof(mapName));
	FormatEx(playerCount, sizeof(playerCount), "%d/%d", GetClientCount(), MaxClients);

	embed.AddField("Map", mapName, true);
	embed.AddField("Players", playerCount, true);
	embed.Timestamp = GetTime();

	forum.CreateForumThreadEmbed(title, "", embed);
	delete embed;

	PrintToServer("[Discord] Forum post with embed creation initiated: %s", title);
}
