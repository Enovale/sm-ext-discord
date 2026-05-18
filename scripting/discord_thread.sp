#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Thread Example",
	author = "ProjectSky",
	description = "Demonstrates creating and managing threads",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_thread", Command_CreateThread, ADMFLAG_ROOT, "Create a thread from a message");
	RegAdminCmd("sm_discord_thread_new", Command_NewThread, ADMFLAG_ROOT, "Create a new thread in channel");
	RegAdminCmd("sm_discord_thread_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_thread_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("log", OnDiscordLog);
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

Action Command_CreateThread(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_thread <channel_id> <message_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE], messageId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));
	GetCmdArg(2, messageId, sizeof(messageId));

	// Fetch message to create thread from
	DiscordMessage.FetchMessage(g_Bot, messageId, channelId, OnMessageFetched);

	ReplyToCommand(client, "[Discord] Creating thread from message...");
	return Plugin_Handled;
}

void OnDiscordLog(Discord bot, DiscordEvent event, any data)
{
	int level = event.GetInt("level");
	char log[512];
	event.GetString("message", log, sizeof(log));
	PrintToServer("[Discord][%d] %s", level, log);
}

void OnMessageFetched(Discord bot, DiscordResult result, any data)
{
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch message: %s", error);
		return;
	}

	DiscordMessage message = view_as<DiscordMessage>(result.GetHandle("message"));

	// Create thread from message (no callback available)
	message.CreateThread("Discussion Thread", AutoArchive_1Day);

	PrintToServer("[Discord] Thread creation initiated");
}

Action Command_NewThread(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_thread_new <channel_id> <thread_name>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE], threadName[128];
	GetCmdArg(1, channelId, sizeof(channelId));
	GetCmdArg(2, threadName, sizeof(threadName));

	// Fetch channel first
	DataPack pack = new DataPack();
	pack.WriteString(threadName);

	DiscordChannel.FetchChannel(g_Bot, channelId, OnChannelForThread, pack);

	ReplyToCommand(client, "[Discord] Creating new thread...");
	return Plugin_Handled;
}

void OnChannelForThread(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char threadName[128];
	pack.ReadString(threadName, sizeof(threadName));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch channel: %s", error);
		return;
	}

	DiscordChannel channel = view_as<DiscordChannel>(result.GetHandle("channel"));

	// Create a new thread in the channel (no callback available)
	channel.CreateThread(threadName, ChannelType_PublicThread, AutoArchive_1Hour, false);

	PrintToServer("[Discord] Thread creation initiated: %s", threadName);
}
