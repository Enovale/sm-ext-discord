#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Modal Example",
	author = "ProjectSky",
	description = "Demonstrates creating and handling modal dialogs",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_modal", Command_SendModal, ADMFLAG_ROOT, "Send a button that opens a modal");
	RegAdminCmd("sm_discord_modal_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_modal_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("button_click", OnButtonClick);
	g_Bot.RegisterEvent("form_submit", OnModalSubmit);
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

Action Command_SendModal(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_modal <channel_id>");
		return Plugin_Handled;
	}

	char channelId[SNOWFLAKE_SIZE];
	GetCmdArg(1, channelId, sizeof(channelId));

	DiscordChannel.FetchChannel(g_Bot, channelId, OnChannelReady);

	ReplyToCommand(client, "[Discord] Sending modal button...");
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

	// Create message with button that opens modal
	DiscordMessage msg = DiscordMessage.CreateWithContent(bot, "Click the button to submit feedback:");

	DiscordComponent row = new DiscordComponent();
	row.Type = ComponentType_ActionRow;

	DiscordComponent btn = new DiscordComponent();
	btn.Type = ComponentType_Button;
	btn.Style = ComponentStyle_Primary;
	btn.SetLabel("Submit Feedback");
	btn.SetCustomId("open_feedback_modal");

	row.AddComponent(btn);
	msg.AddComponent(row);

	char channelId[SNOWFLAKE_SIZE];
	channel.GetId(channelId, sizeof(channelId));
	msg.SetChannelId(channelId);
	msg.Send();

	delete btn;
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

	if (strcmp(customId, "open_feedback_modal") == 0)
	{
		// Create modal using DiscordModal (elegant method)
		DiscordModal modal = new DiscordModal("feedback_modal", "Submit Feedback");

		// Add text inputs using convenience method
		modal.AddTextInput("title", "Title", TextStyle_Short, true, "Enter a title...", 1, 100);
		modal.AddTextInput("description", "Description", TextStyle_Paragraph, true, "Describe your feedback...", 10, 1000);
		modal.AddTextInput("contact", "Contact (optional)", TextStyle_Short, false, "Your email or Discord tag");

		// Show modal to user
		interaction.ShowModalFromObject(modal);
		delete modal;
	}
}

void OnModalSubmit(Discord bot, DiscordEvent event, any data)
{
	DiscordModalInteraction interaction = view_as<DiscordModalInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char customId[64];
	interaction.GetCustomId(customId, sizeof(customId));

	if (strcmp(customId, "feedback_modal") == 0)
	{
		// Get submitted values
		char title[128];
		char description[1024];
		char contact[128];

		interaction.GetValue("title", title, sizeof(title));
		interaction.GetValue("description", description, sizeof(description));
		interaction.GetValue("contact", contact, sizeof(contact));

		// Get user info
		DiscordUser user = interaction.User;
		char username[MAX_DISCORD_NAME_LENGTH];
		user.GetUserName(username, sizeof(username));

		// Log feedback
		PrintToServer("[Discord] Feedback from %s:", username);
		PrintToServer("  Title: %s", title);
		PrintToServer("  Description: %s", description);
		PrintToServer("  Contact: %s", contact[0] ? contact : "Not provided");

		// Reply to user
		interaction.Reply("Thank you for your feedback!");
	}
}
