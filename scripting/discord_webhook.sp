#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;
char g_WebhookUrl[256];

public Plugin myinfo = {
	name = "Discord Webhook Example",
	author = "ProjectSky",
	description = "Demonstrates sending messages via webhooks",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_webhook", Command_Webhook, ADMFLAG_ROOT, "Send a webhook message");
	RegAdminCmd("sm_discord_webhook_set", Command_SetWebhook, ADMFLAG_ROOT, "Set webhook URL");
	RegAdminCmd("sm_discord_webhook_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_discord_webhook_list", Command_ListWebhooks, ADMFLAG_ROOT, "List webhooks in a channel");
	RegAdminCmd("sm_discord_webhook_multi", Command_MultiEmbed, ADMFLAG_ROOT, "Send multiple embeds via webhook");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_webhook_start <bot_token>");
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

Action Command_SetWebhook(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_webhook_set <webhook_url>");
		return Plugin_Handled;
	}

	GetCmdArg(1, g_WebhookUrl, sizeof(g_WebhookUrl));
	ReplyToCommand(client, "[Discord] Webhook URL set.");
	return Plugin_Handled;
}

Action Command_Webhook(int client, int args)
{
	if (g_Bot == null)
	{
		ReplyToCommand(client, "[Discord] Bot not initialized.");
		return Plugin_Handled;
	}

	if (g_WebhookUrl[0] == '\0')
	{
		ReplyToCommand(client, "[Discord] Webhook URL not set. Use sm_discord_webhook_set first.");
		return Plugin_Handled;
	}

	// Create webhook
	DiscordWebhook webhook = new DiscordWebhook(g_Bot, g_WebhookUrl);

	// Create embed
	DiscordEmbed embed = new DiscordEmbed();
	embed.SetTitle("Server Notification");
	embed.SetDescription("This message was sent via webhook!");
	embed.Color = 0x5865F2; // Discord blurple

	char mapName[64];
	GetCurrentMap(mapName, sizeof(mapName));
	embed.AddField("Current Map", mapName, true);

	char playerCount[32];
	FormatEx(playerCount, sizeof(playerCount), "%d/%d", GetClientCount(), MaxClients);
	embed.AddField("Players", playerCount, true);

	embed.Timestamp = GetTime();

	// Set webhook name and avatar
	webhook.SetName("Game Server");

	if (FileExists("addons/sourcemod/data/avatar.png")) {
		webhook.SetAvatarFromFile("addons/sourcemod/data/avatar.png");
	}

	// Send webhook message with embed
	webhook.ExecuteEmbed("Server update:", embed);

	delete embed;
	delete webhook;

	ReplyToCommand(client, "[Discord] Webhook message sent.");
	return Plugin_Handled;
}

Action Command_ListWebhooks(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_webhook_list <channel_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));

	// Fetch webhooks from channel - demonstrates DiscordHandleArray usage
	DiscordWebhook.GetChannelWebhooks(g_Bot, channelId, OnWebhooksReceived);

	ReplyToCommand(client, "[Discord] Fetching webhooks...");
	return Plugin_Handled;
}

void OnWebhooksReceived(Discord bot, DiscordResult result, any data)
{
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to get webhooks: %s", error);
		return;
	}

	// Get the webhooks array from result
	DiscordHandleArray webhooks = view_as<DiscordHandleArray>(result.GetHandle("items"));
	int count = webhooks.Length;

	PrintToServer("[Discord] Found %d webhooks:", count);

	// Iterate through the array
	for (int i = 0; i < count; i++)
	{
		DiscordWebhook webhook = view_as<DiscordWebhook>(webhooks.GetAt(i));

		char name[64], id[SNOWFLAKE_SIZE];
		webhook.GetName(name, sizeof(name));
		webhook.GetId(id, sizeof(id));

		PrintToServer("  [%d] %s (ID: %s)", i + 1, name, id);
	}
}

Action Command_MultiEmbed(int client, int args)
{
	if (g_Bot == null)
	{
		ReplyToCommand(client, "[Discord] Bot not initialized.");
		return Plugin_Handled;
	}

	if (g_WebhookUrl[0] == '\0')
	{
		ReplyToCommand(client, "[Discord] Webhook URL not set. Use sm_discord_webhook_set first.");
		return Plugin_Handled;
	}

	// Create webhook
	DiscordWebhook webhook = new DiscordWebhook(g_Bot, g_WebhookUrl);
	webhook.SetName("Server Status");

	// Create message with multiple embeds using ExecuteFromObject
	DiscordMessage msg = DiscordMessage.CreateWithContent(g_Bot, "Server Status Report:");

	// First embed - Server Info
	DiscordEmbed embed1 = new DiscordEmbed();
	embed1.SetTitle("Server Information");
	embed1.Color = 0x57F287; // Green

	char mapName[64];
	GetCurrentMap(mapName, sizeof(mapName));
	embed1.AddField("Map", mapName, true);

	char playerCount[32];
	FormatEx(playerCount, sizeof(playerCount), "%d/%d", GetClientCount(), MaxClients);
	embed1.AddField("Players", playerCount, true);
	msg.AddEmbed(embed1);

	// Second embed - Performance
	DiscordEmbed embed2 = new DiscordEmbed();
	embed2.SetTitle("Performance");
	embed2.Color = 0xFEE75C; // Yellow

	char tickrate[16];
	FormatEx(tickrate, sizeof(tickrate), "%.1f", 1.0 / GetTickInterval());
	embed2.AddField("Tickrate", tickrate, true);

	char uptime[32];
	FormatEx(uptime, sizeof(uptime), "%.1f hours", GetGameTime() / 3600.0);
	embed2.AddField("Uptime", uptime, true);
	msg.AddEmbed(embed2);

	// Third embed - Timestamp
	DiscordEmbed embed3 = new DiscordEmbed();
	embed3.SetTitle("Report Generated");
	embed3.Color = 0x5865F2; // Blurple
	embed3.Timestamp = GetTime();
	msg.AddEmbed(embed3);

	// Send message with multiple embeds
	webhook.ExecuteFromObject(msg);

	delete embed1;
	delete embed2;
	delete embed3;
	delete msg;
	delete webhook;

	ReplyToCommand(client, "[Discord] Multi-embed webhook message sent.");
	return Plugin_Handled;
}
