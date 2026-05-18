#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Slash Command Example",
	author = "ProjectSky",
	description = "Demonstrates creating and handling slash commands",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_register", Command_Register, ADMFLAG_ROOT, "Register slash commands");
	RegAdminCmd("sm_discord_slash_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_slash_start <bot_token>");
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

Action Command_Register(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	// Create a slash command
	DiscordSlashCommand cmd = new DiscordSlashCommand(g_Bot);
	cmd.SetName("status");
	cmd.SetDescription("Get server status");

	// Add options
	cmd.AddOption("message", "Optional message", Option_String, false);
	cmd.AddOption("detailed", "Show detailed info", Option_Boolean, false);

	// Register globally
	cmd.RegisterGlobally();

	delete cmd;

	// Set up interaction callback
	g_Bot.RegisterEvent("slashcommand", OnSlashCommand);

	ReplyToCommand(client, "[Discord] Slash command registered.");
	return Plugin_Handled;
}

void OnSlashCommand(Discord bot, DiscordEvent event, any data)
{
	DiscordInteraction interaction = view_as<DiscordInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char cmdName[MAX_COMMAND_NAME];
	interaction.GetCommandName(cmdName, sizeof(cmdName));

	if (strcmp(cmdName, "status") == 0)
	{
		HandleStatusCommand(interaction);
	}
}

void HandleStatusCommand(DiscordInteraction interaction)
{
	// Get options
	char message[256];
	interaction.GetOptionValue("message", message, sizeof(message));

	bool detailed = interaction.GetOptionValueBool("detailed");

	// Build response
	char response[512];
	char mapName[64];
	GetCurrentMap(mapName, sizeof(mapName));

	if (detailed)
	{
		FormatEx(response, sizeof(response),
			"**Server Status**\n"
		...	"Players: %d/%d\n"
		...	"Map: %s\n"
		...	"Message: %s",
			GetClientCount(), MaxClients, mapName,
			message[0] ? message : "None");
	}
	else
	{
		FormatEx(response, sizeof(response), "Players: %d/%d | Map: %s", GetClientCount(), MaxClients, mapName);
	}

	// Reply to interaction
	interaction.CreateResponse(response);
}
