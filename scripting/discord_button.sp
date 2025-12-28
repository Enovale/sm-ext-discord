#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Button Example",
	author = "ProjectSky",
	description = "Demonstrates creating buttons and handling button interactions",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_button", Command_SendButton, ADMFLAG_ROOT, "Send a message with buttons");
	RegAdminCmd("sm_discord_button_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_button_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("button_click", OnButtonClick);
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

Action Command_SendButton(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_button <channel_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));

	// Fetch channel and send message
	DiscordChannel.FetchChannel(g_Bot, channelId, OnChannelReady);

	ReplyToCommand(client, "[Discord] Sending button message...");
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

	// Create message with buttons
	DiscordMessage msg = DiscordMessage.CreateWithContent(bot, "Choose an action:");

	// Create action row
	DiscordComponent row = new DiscordComponent();
	row.Type = ComponentType_ActionRow;

	// Create buttons
	DiscordComponent btnConfirm = new DiscordComponent();
	btnConfirm.Type = ComponentType_Button;
	btnConfirm.Style = ComponentStyle_Success;
	btnConfirm.SetLabel("Confirm");
	btnConfirm.SetCustomId("btn_confirm");

	DiscordComponent btnCancel = new DiscordComponent();
	btnCancel.Type = ComponentType_Button;
	btnCancel.Style = ComponentStyle_Danger;
	btnCancel.SetLabel("Cancel");
	btnCancel.SetCustomId("btn_cancel");

	DiscordComponent btnLink = new DiscordComponent();
	btnLink.Type = ComponentType_Button;
	btnLink.Style = ComponentStyle_Link;
	btnLink.SetLabel("Website");
	btnLink.SetUrl("https://example.com");

	// Add buttons to row
	row.AddComponent(btnConfirm);
	row.AddComponent(btnCancel);
	row.AddComponent(btnLink);

	// Add row to message
	msg.AddComponent(row);

	// Send message
	char channelId[SNOWFLAKE_SIZE];
	channel.GetId(channelId, sizeof(channelId));
	msg.SetChannelId(channelId);
	msg.Send();

	delete btnConfirm;
	delete btnCancel;
	delete btnLink;
	delete row;
	delete msg;
}

void OnButtonClick(Discord bot, DiscordEvent event, any data)
{
	DiscordButtonInteraction interaction = view_as<DiscordButtonInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char customId[64];
	interaction.GetCustomId(customId, sizeof(customId));

	DiscordUser user = interaction.User;
	char username[MAX_DISCORD_NAME_LENGTH];
	user.GetUserName(username, sizeof(username));

	if (strcmp(customId, "btn_confirm") == 0)
	{
		interaction.Reply("Confirmed!");
		PrintToServer("[Discord] %s clicked Confirm", username);
	}
	else if (strcmp(customId, "btn_cancel") == 0)
	{
		interaction.Reply("Cancelled!");
		PrintToServer("[Discord] %s clicked Cancel", username);
	}
}
