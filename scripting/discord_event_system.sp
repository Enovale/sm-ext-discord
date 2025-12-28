#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;
bool g_bGuildEventsEnabled = true;

public Plugin myinfo = {
	name = "Discord Event System Example",
	author = "ProjectSky",
	description = "Demonstrates the unified event and callback system",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_event_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_event_stop", Command_Stop, ADMFLAG_ROOT, "Stop the Discord bot");
	RegAdminCmd("sm_event_toggle_guild", Command_ToggleGuild, ADMFLAG_ROOT, "Toggle guild events");
	RegAdminCmd("sm_event_fetch_user", Command_FetchUser, ADMFLAG_ROOT, "Fetch user info");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_event_start <bot_token>");
		return Plugin_Handled;
	}

	if (g_Bot != null && g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is already running.");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token, DISCORD_INTENTS_ALL);

	// Register connection events
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("resumed", OnBotResumed);
	g_Bot.RegisterEvent("shutdown", OnBotShutdown);

	// Register message events
	g_Bot.RegisterEvent("message_create", OnMessageCreate);
	g_Bot.RegisterEvent("message_update", OnMessageUpdate);
	g_Bot.RegisterEvent("message_delete", OnMessageDelete);

	// Register guild events (can be toggled)
	RegisterGuildEvents();

	// Register interaction events
	g_Bot.RegisterEvent("slashcommand", OnSlashCommand);
	g_Bot.RegisterEvent("button_click", OnButtonClick);

	g_Bot.Start();
	ReplyToCommand(client, "[Discord] Bot starting with event system...");
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

// Demonstrates dynamic event registration/unregistration
Action Command_ToggleGuild(int client, int args)
{
	if (g_Bot == null)
	{
		ReplyToCommand(client, "[Discord] Bot is not initialized.");
		return Plugin_Handled;
	}

	g_bGuildEventsEnabled = !g_bGuildEventsEnabled;

	if (g_bGuildEventsEnabled)
	{
		RegisterGuildEvents();
		ReplyToCommand(client, "[Discord] Guild events enabled.");
	}
	else
	{
		UnregisterGuildEvents();
		ReplyToCommand(client, "[Discord] Guild events disabled.");
	}

	return Plugin_Handled;
}

// Demonstrates DiscordResult callback pattern
Action Command_FetchUser(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_event_fetch_user <user_id>");
		return Plugin_Handled;
	}

	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	char userId[SNOWFLAKE_SIZE];
	GetCmdArg(1, userId, sizeof(userId));

	// FetchUser uses DiscordResult callback
	DiscordUser.FetchUser(g_Bot, userId, "", OnUserFetched, client);
	ReplyToCommand(client, "[Discord] Fetching user %s...", userId);
	return Plugin_Handled;
}

void RegisterGuildEvents()
{
	g_Bot.RegisterEvent("guild_create", OnGuildCreate);
	g_Bot.RegisterEvent("guild_member_add", OnGuildMemberAdd);
	g_Bot.RegisterEvent("guild_member_remove", OnGuildMemberRemove);
	g_Bot.RegisterEvent("guild_role_create", OnGuildRoleCreate);
}

void UnregisterGuildEvents()
{
	g_Bot.UnregisterEvent("guild_create");
	g_Bot.UnregisterEvent("guild_member_add");
	g_Bot.UnregisterEvent("guild_member_remove");
	g_Bot.UnregisterEvent("guild_role_create");
}

// ============================================================================
// Connection Events
// ============================================================================

void OnBotReady(Discord bot, DiscordEvent event, any data)
{
	char botName[64];
	bot.GetBotName(botName, sizeof(botName));

	char session_id[64];
	event.GetString("session_id", session_id, sizeof(session_id));
	int guild_count = event.GetInt("guild_count");

	PrintToServer("[Discord] Bot %s is ready! Session: %s, Guilds: %d", botName, session_id, guild_count);
}

void OnBotResumed(Discord bot, DiscordEvent event, any data)
{
	char sessionId[64];
	event.GetString("session_id", sessionId, sizeof(sessionId));
	int shardId = event.GetInt("shard_id");

	PrintToServer("[Discord] Bot resumed! Session: %s, Shard: %d", sessionId, shardId);
}

void OnBotShutdown(Discord bot, DiscordEvent event, any data)
{
	PrintToServer("[Discord] Bot is shutting down.");
}

// ============================================================================
// Message Events
// ============================================================================
void OnMessageCreate(Discord bot, DiscordEvent event, any data)
{
	// Get message handle from event
	DiscordMessage message = view_as<DiscordMessage>(event.GetHandle("message"));
	if (message == null)
		return;

	// Skip bot messages
	if (message.IsBot)
		return;

	// Get message data
	char content[MAX_DISCORD_MESSAGE_LENGTH];
	message.GetContent(content, sizeof(content));

	char channelId[SNOWFLAKE_SIZE];
	event.GetString("channel_id", channelId, sizeof(channelId));

	char authorId[SNOWFLAKE_SIZE];
	event.GetString("author_id", authorId, sizeof(authorId));

	PrintToServer("[Discord] Message in %s from %s: %s", channelId, authorId, content);

	// Example: Keep handle beyond callback scope
	if (strcmp(content, "!save") == 0)
	{
		// Disable auto-free to keep the handle
		event.AutoFreeHandles = false;
		// Now you're responsible for closing the message handle later
		// delete message; // Call this when done
	}
}

void OnMessageUpdate(Discord bot, DiscordEvent event, any data)
{
	char messageId[SNOWFLAKE_SIZE];
	event.GetString("message_id", messageId, sizeof(messageId));

	char channelId[SNOWFLAKE_SIZE];
	event.GetString("channel_id", channelId, sizeof(channelId));

	PrintToServer("[Discord] Message %s edited in channel %s", messageId, channelId);
}

void OnMessageDelete(Discord bot, DiscordEvent event, any data)
{
	char messageId[SNOWFLAKE_SIZE];
	event.GetString("message_id", messageId, sizeof(messageId));

	char guildId[SNOWFLAKE_SIZE];
	event.GetString("guild_id", guildId, sizeof(guildId));

	PrintToServer("[Discord] Message %s deleted in guild %s", messageId, guildId);
}

// ============================================================================
// Guild Events
// ============================================================================

void OnGuildCreate(Discord bot, DiscordEvent event, any data)
{
	char guildId[SNOWFLAKE_SIZE];
	event.GetString("guild_id", guildId, sizeof(guildId));

	char name[MAX_DISCORD_NAME_LENGTH];
	event.GetString("name", name, sizeof(name));

	int memberCount = event.GetInt("member_count");

	PrintToServer("[Discord] Joined guild: %s (%s) with %d members", name, guildId, memberCount);

	// Access guild handle if needed
	DiscordGuild guild = view_as<DiscordGuild>(event.GetHandle("guild"));
	if (guild != null)
	{
		int roleCount = guild.RoleCount;
		int channelCount = guild.ChannelCount;
		PrintToServer("[Discord] Guild has %d roles and %d channels", roleCount, channelCount);
	}
}

void OnGuildMemberAdd(Discord bot, DiscordEvent event, any data)
{
	char userId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));

	char username[MAX_DISCORD_NAME_LENGTH];
	event.GetString("username", username, sizeof(username));

	char guildId[SNOWFLAKE_SIZE];
	event.GetString("guild_id", guildId, sizeof(guildId));

	PrintToServer("[Discord] %s (%s) joined guild %s", username, userId, guildId);
}

void OnGuildMemberRemove(Discord bot, DiscordEvent event, any data)
{
	char userId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));

	char username[MAX_DISCORD_NAME_LENGTH];
	event.GetString("username", username, sizeof(username));

	char guildId[SNOWFLAKE_SIZE];
	event.GetString("guild_id", guildId, sizeof(guildId));

	PrintToServer("[Discord] %s (%s) left guild %s", username, userId, guildId);
}

void OnGuildRoleCreate(Discord bot, DiscordEvent event, any data)
{
	char roleId[SNOWFLAKE_SIZE];
	event.GetString("role_id", roleId, sizeof(roleId));

	char name[MAX_DISCORD_NAME_LENGTH];
	event.GetString("name", name, sizeof(name));

	// Access role handle
	DiscordRole role = view_as<DiscordRole>(event.GetHandle("role"));
	if (role != null)
	{
		int color = role.Color;
		bool hoisted = role.Hoist;
		PrintToServer("[Discord] Role created: %s (color: %d, hoisted: %s)",
			name, color, hoisted ? "yes" : "no");
	}
}

// ============================================================================
// Interaction Events
// ============================================================================

void OnSlashCommand(Discord bot, DiscordEvent event, any data)
{
	DiscordInteraction interaction = view_as<DiscordInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char commandName[64];
	event.GetString("command_name", commandName, sizeof(commandName));

	char userId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));

	PrintToServer("[Discord] Slash command /%s used by %s", commandName, userId);

	// Reply to the interaction
	interaction.CreateResponse("Command received!");
}

void OnButtonClick(Discord bot, DiscordEvent event, any data)
{
	DiscordButtonInteraction interaction = view_as<DiscordButtonInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char customId[64];
	event.GetString("custom_id", customId, sizeof(customId));

	char userId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));

	PrintToServer("[Discord] Button %s clicked by %s", customId, userId);

	interaction.Reply("Button clicked!");
}

// ============================================================================
// DiscordResult Callback Example
// ============================================================================

void OnUserFetched(Discord bot, DiscordResult result, any data)
{
	int client = data;

	// Check if the request was successful
	if (!result.IsSuccess)
	{
		char error[256];
		result.GetError(error, sizeof(error));
		PrintToServer("[Discord] Failed to fetch user: %s", error);
		return;
	}

	// Get the user handle from result
	DiscordUser user = view_as<DiscordUser>(result.GetHandle("user"));
	if (user == null)
		return;

	char username[MAX_DISCORD_NAME_LENGTH];
	user.GetUserName(username, sizeof(username));

	char globalName[MAX_DISCORD_NAME_LENGTH];
	user.GetGlobalName(globalName, sizeof(globalName));

	char avatarUrl[256];
	user.GetAvatarUrl(true, avatarUrl, sizeof(avatarUrl));

	bool isBot = user.IsBot;

	PrintToServer("[Discord] User: %s (%s), Bot: %s, Avatar: %s",
		username, globalName, isBot ? "yes" : "no", avatarUrl);

	if (client > 0 && IsClientInGame(client))
	{
		PrintToChat(client, "[Discord] User: %s (%s)", username, globalName);
	}
}
