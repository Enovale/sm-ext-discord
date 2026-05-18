#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Basic Example",
	author = "ProjectSky",
	description = "Demonstrates basic bot setup and message handling",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_discord_stop", Command_Stop, ADMFLAG_ROOT, "Stop the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_start <bot_token>");
		return Plugin_Handled;
	}

	if (g_Bot != null && g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is already running.");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	// Create bot with default intents + message content
	delete g_Bot;
	g_Bot = new Discord(token);

	// Set up event callbacks using RegisterEvent
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("message_create", OnMessageReceived);

	// Start the bot
	g_Bot.Start();

	ReplyToCommand(client, "[Discord] Bot starting...");
	return Plugin_Handled;
}

Action Command_Stop(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	g_Bot.Stop();
	delete g_Bot;

	ReplyToCommand(client, "[Discord] Bot stopped.");
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

void OnMessageReceived(Discord bot, DiscordEvent event, any data)
{
	DiscordMessage message = view_as<DiscordMessage>(event.GetHandle("message"));
	if (message == null)
		return;

	// Ignore bot messages
	if (message.IsBot)
		return;

	char content[MAX_DISCORD_MESSAGE_LENGTH];
	message.GetContent(content, sizeof(content));

	char authorName[MAX_DISCORD_NAME_LENGTH];
	DiscordUser author = message.Author;
	author.GetUserName(authorName, sizeof(authorName));

	PrintToServer("[Discord] %s: %s", authorName, content);

	// Simple ping-pong response
	if (strcmp(content, "!ping") == 0)
	{
		message.Reply("Pong!");
	}
}
