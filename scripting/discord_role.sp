#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;
char g_GuildId[SNOWFLAKE_SIZE];

public Plugin myinfo = {
	name = "Discord Role Example",
	author = "ProjectSky",
	description = "Demonstrates role management",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_role_add", Command_AddRole, ADMFLAG_ROOT, "Add role to member");
	RegAdminCmd("sm_discord_role_remove", Command_RemoveRole, ADMFLAG_ROOT, "Remove role from member");
	RegAdminCmd("sm_discord_setguild", Command_SetGuild, ADMFLAG_ROOT, "Set guild ID");
	RegAdminCmd("sm_discord_role_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_role_start <bot_token>");
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

Action Command_SetGuild(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_setguild <guild_id>");
		return Plugin_Handled;
	}

	GetCmdArg(1, g_GuildId, sizeof(g_GuildId));
	ReplyToCommand(client, "[Discord] Guild ID set.");
	return Plugin_Handled;
}

Action Command_AddRole(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_role_add <user_id> <role_id>");
		return Plugin_Handled;
	}

	if (g_GuildId[0] == '\0')
	{
		ReplyToCommand(client, "[Discord] Guild ID not set. Use sm_discord_setguild first.");
		return Plugin_Handled;
	}

	char userId[SNOWFLAKE_SIZE], roleId[SNOWFLAKE_SIZE];
	GetCmdArg(1, userId, sizeof(userId));
	GetCmdArg(2, roleId, sizeof(roleId));

	DataPack pack = new DataPack();
	pack.WriteString(roleId);

	// Get guild and fetch member
	DiscordGuild guild = DiscordGuild.FindGuild(g_Bot, g_GuildId);
	if (guild == null)
	{
		delete pack;
		ReplyToCommand(client, "[Discord] Guild not found in cache.");
		return Plugin_Handled;
	}

	guild.GetMember(userId, OnMemberForAddRole, pack);
	delete guild;

	ReplyToCommand(client, "[Discord] Adding role...");
	return Plugin_Handled;
}

void OnMemberForAddRole(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char roleId[SNOWFLAKE_SIZE];
	pack.ReadString(roleId, sizeof(roleId));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch member: %s", error);
		return;
	}

	DiscordGuildMember member = view_as<DiscordGuildMember>(result.GetHandle("member"));

	// Add role to member
	member.AddRole(roleId, OnRoleAdded);

	char username[MAX_DISCORD_NAME_LENGTH];
	member.GetUserName(username, sizeof(username));
	PrintToServer("[Discord] Adding role %s to %s...", roleId, username);
}

void OnRoleAdded(Discord bot, DiscordResult result, any data)
{
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to add role: %s", error);
		return;
	}

	PrintToServer("[Discord] Role added successfully");
}

Action Command_RemoveRole(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 2)
	{
		ReplyToCommand(client, "Usage: sm_discord_role_remove <user_id> <role_id>");
		return Plugin_Handled;
	}

	if (g_GuildId[0] == '\0')
	{
		ReplyToCommand(client, "[Discord] Guild ID not set. Use sm_discord_setguild first.");
		return Plugin_Handled;
	}

	char userId[SNOWFLAKE_SIZE], roleId[SNOWFLAKE_SIZE];
	GetCmdArg(1, userId, sizeof(userId));
	GetCmdArg(2, roleId, sizeof(roleId));

	DataPack pack = new DataPack();
	pack.WriteString(roleId);

	// Get guild and fetch member
	DiscordGuild guild = DiscordGuild.FindGuild(g_Bot, g_GuildId);
	if (guild == null)
	{
		delete pack;
		ReplyToCommand(client, "[Discord] Guild not found in cache.");
		return Plugin_Handled;
	}

	guild.GetMember(userId, OnMemberForRemoveRole, pack);
	delete guild;

	ReplyToCommand(client, "[Discord] Removing role...");
	return Plugin_Handled;
}

void OnMemberForRemoveRole(Discord bot, DiscordResult result, any data)
{
	DataPack pack = view_as<DataPack>(data);
	pack.Reset();

	char roleId[SNOWFLAKE_SIZE];
	pack.ReadString(roleId, sizeof(roleId));
	delete pack;

	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch member: %s", error);
		return;
	}

	DiscordGuildMember member = view_as<DiscordGuildMember>(result.GetHandle("member"));

	// Remove role from member
	member.RemoveRole(roleId, OnRoleRemoved);

	char username[MAX_DISCORD_NAME_LENGTH];
	member.GetUserName(username, sizeof(username));
	PrintToServer("[Discord] Removing role %s from %s...", roleId, username);
}

void OnRoleRemoved(Discord bot, DiscordResult result, any data)
{
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to remove role: %s", error);
		return;
	}

	PrintToServer("[Discord] Role removed successfully");
}
