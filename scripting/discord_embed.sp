#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Embed Example",
	author = "ProjectSky",
	description = "Demonstrates creating and sending embeds",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_embed", Command_SendEmbed, ADMFLAG_ROOT, "Send an embed to Discord");
	RegAdminCmd("sm_discord_embed_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_embed_start <bot_token>");
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

Action Command_SendEmbed(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_embed <channel_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));

	// Create embed
	DiscordEmbed embed = new DiscordEmbed();
	embed.SetTitle("Server Status");
	embed.SetDescription("Current server information");
	embed.Color = 0x00FF00; // Green

	// Add fields
	char playerCount[32];
	FormatEx(playerCount, sizeof(playerCount), "%d / %d", GetClientCount(), MaxClients);
	embed.AddField("Players", playerCount, true);

	char mapName[64];
	GetCurrentMap(mapName, sizeof(mapName));
	embed.AddField("Map", mapName, true);

	// Set footer
	embed.SetFooter("Server Bot");

	// Set timestamp
	embed.Timestamp = GetTime();

	// Send to channel
	DiscordChannel.FetchChannel(g_Bot, channelId, OnChannelFetched, embed);

	ReplyToCommand(client, "[Discord] Sending embed...");
	return Plugin_Handled;
}

void OnChannelFetched(Discord bot, DiscordResult result, any data)
{
	DiscordEmbed embed = view_as<DiscordEmbed>(data);

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch channel: %s", error);
		delete embed;
		return;
	}

	DiscordChannel channel = view_as<DiscordChannel>(result.GetHandle("channel"));
	channel.SendMessageEmbed("", embed);
	delete embed;
}
