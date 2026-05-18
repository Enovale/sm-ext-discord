#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Autocomplete Example",
	author = "ProjectSky",
	description = "Demonstrates creating slash commands with autocomplete options",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

// Sample data for autocomplete
char g_Maps[][] = {
	"de_dust2",
	"de_mirage",
	"de_inferno",
	"de_nuke",
	"de_overpass",
	"de_ancient",
	"de_anubis",
	"de_vertigo"
};

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_autocomplete_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_discord_autocomplete_register", Command_Register, ADMFLAG_ROOT, "Register autocomplete command");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_autocomplete_start <bot_token>");
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

	// Create a slash command with autocomplete option
	DiscordSlashCommand cmd = new DiscordSlashCommand(g_Bot);
	cmd.SetName("changemap");
	cmd.SetDescription("Change the server map");

	// Add autocomplete option (set autocomplete=true)
	cmd.AddOption("map", "Map name", Option_String, true, true);

	cmd.RegisterGlobally();
	delete cmd;

	// Set up callbacks
	g_Bot.RegisterEvent("slashcommand", OnSlashCommand);
	g_Bot.RegisterEvent("autocomplete", OnAutocomplete);

	ReplyToCommand(client, "[Discord] Autocomplete command registered.");
	return Plugin_Handled;
}

void OnAutocomplete(Discord bot, DiscordEvent event, any data)
{
	DiscordAutocompleteInteraction interaction = view_as<DiscordAutocompleteInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char cmdName[MAX_COMMAND_NAME];
	interaction.GetCommandName(cmdName, sizeof(cmdName));

	if (strcmp(cmdName, "changemap") != 0)
		return;

	// Get current input value for filtering
	char input[64];
	interaction.GetFocusedOptionValue(input, sizeof(input));

	// Add matching maps as choices
	for (int i = 0; i < sizeof(g_Maps); i++)
	{
		if (input[0] == '\0' || StrContains(g_Maps[i], input, false) != -1)
		{
			interaction.AddAutocompleteChoiceString(g_Maps[i], g_Maps[i]);
		}
	}

	interaction.CreateAutocompleteResponse();
}

void OnSlashCommand(Discord bot, DiscordEvent event, any data)
{
	DiscordInteraction interaction = view_as<DiscordInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char cmdName[MAX_COMMAND_NAME];
	interaction.GetCommandName(cmdName, sizeof(cmdName));

	if (strcmp(cmdName, "changemap") == 0)
	{
		char mapName[64];
		interaction.GetOptionValue("map", mapName, sizeof(mapName));

		if (!IsMapValid(mapName))
		{
			char errorResponse[256];
			FormatEx(errorResponse, sizeof(errorResponse), "Map **%s** is not valid", mapName);
			interaction.CreateResponse(errorResponse);
			return;
		}

		char response[256];
		FormatEx(response, sizeof(response), "Changing map to: **%s**", mapName);
		interaction.CreateResponse(response);

		// Actually change the map (delayed)
		DataPack pack = new DataPack();
		pack.WriteString(mapName);
		CreateTimer(3.0, Timer_ChangeMap, pack);
	}
}

Action Timer_ChangeMap(Handle timer, DataPack pack)
{
	char mapName[64];
	pack.Reset();
	pack.ReadString(mapName, sizeof(mapName));
	delete pack;

	ForceChangeLevel(mapName, "Discord command");

	return Plugin_Stop;
}
