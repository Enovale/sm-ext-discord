#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Select Menu Example",
	author = "ProjectSky",
	description = "Demonstrates creating and handling select menus",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_select", Command_SendSelect, ADMFLAG_ROOT, "Send a message with select menu");
	RegAdminCmd("sm_discord_select_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_select_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("select_click", OnSelectMenu);
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

Action Command_SendSelect(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_select <channel_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));

	// Fetch channel and send message
	DiscordChannel.FetchChannel(g_Bot, channelId, OnChannelReady);

	ReplyToCommand(client, "[Discord] Sending select menu...");
	return Plugin_Handled;
}

void OnChannelReady(Discord bot, DiscordResult result, any data)
{
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch channel: %s", error);
		return;
	}

	DiscordChannel channel = view_as<DiscordChannel>(result.GetHandle("channel"));

	// Create message with select menu
	DiscordMessage msg = DiscordMessage.CreateWithContent(bot, "Select your favorite game:");

	// Create action row
	DiscordComponent row = new DiscordComponent();
	row.Type = ComponentType_ActionRow;

	// Create string select menu
	DiscordComponent select = new DiscordComponent();
	select.Type = ComponentType_SelectMenu;
	select.SetCustomId("game_select");
	select.SetPlaceholder("Choose a game...");
	select.MinValues = 1;
	select.MaxValues = 1;

	// Add options
	select.AddOption("Counter-Strike 2", "cs2", "Tactical FPS");
	select.AddOption("Team Fortress 2", "tf2", "Class-based shooter");
	select.AddOption("Left 4 Dead 2", "l4d2", "Zombie survival");

	// Add select to row
	row.AddComponent(select);

	// Add row to message
	msg.AddComponent(row);

	// Send message
	char channelId[SNOWFLAKE_SIZE];
	channel.GetId(channelId, sizeof(channelId));
	msg.SetChannelId(channelId);
	msg.Send();

	delete select;
	delete row;
	delete msg;
}

void OnSelectMenu(Discord bot, DiscordEvent event, any data)
{
	DiscordSelectInteraction interaction = view_as<DiscordSelectInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char customId[64];
	interaction.GetCustomId(customId, sizeof(customId));

	if (strcmp(customId, "game_select") == 0)
	{
		char value[64];
		interaction.GetValue(0, value, sizeof(value));

		char response[128];
		FormatEx(response, sizeof(response), "You selected: %s", value);
		interaction.Reply(response);

		DiscordUser user = interaction.User;
		char username[MAX_DISCORD_NAME_LENGTH];
		user.GetUserName(username, sizeof(username));
		PrintToServer("[Discord] %s selected: %s", username, value);
	}
}
