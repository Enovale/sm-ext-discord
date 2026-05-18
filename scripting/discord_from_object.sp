#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord FromObject Example",
	author = "ProjectSky",
	description = "Demonstrates using FromObject methods for advanced message features (components, multiple embeds)",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_fromobj_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_discord_fromobj_register", Command_Register, ADMFLAG_ROOT, "Register test slash command");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_fromobj_start <bot_token>");
		return Plugin_Handled;
	}

	char token[128];
	GetCmdArg(1, token, sizeof(token));

	delete g_Bot;
	g_Bot = new Discord(token);
	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("slashcommand", OnSlashCommand);
	g_Bot.RegisterEvent("button_click", OnButtonClick);
	g_Bot.RegisterEvent("message_create", OnMessageCreate);
	g_Bot.Start();

	ReplyToCommand(client, "[Discord] Bot starting...");
	return Plugin_Handled;
}

void OnBotReady(Discord bot, DiscordEvent event, any data)
{
	char botName[64];
	bot.GetBotName(botName, sizeof(botName));
	PrintToServer("[Discord] Bot %s is ready!", botName);
}

Action Command_Register(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_fromobj_register <guild_id>");
		return Plugin_Handled;
	}

	char guildId[SNOWFLAKE_SIZE];
	GetCmdArg(1, guildId, sizeof(guildId));

	// Register test command
	DiscordSlashCommand cmd = new DiscordSlashCommand(g_Bot);
	cmd.SetName("testfromobj");
	cmd.SetDescription("Test FromObject methods with components");
	cmd.RegisterToGuild(guildId);
	delete cmd;

	ReplyToCommand(client, "[Discord] Registered /testfromobj command");
	return Plugin_Handled;
}

// Test DiscordInteraction.CreateResponseFromObject
void OnSlashCommand(Discord bot, DiscordEvent event, any data)
{
	DiscordInteraction interaction = view_as<DiscordInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char cmdName[64];
	interaction.GetCommandName(cmdName, sizeof(cmdName));

	if (strcmp(cmdName, "testfromobj") != 0)
		return;

	// Create message with components using CreateResponseFromObject
	DiscordMessage msg = DiscordMessage.CreateWithContent(bot, "Test FromObject - Click a button:");

	// Create embed
	DiscordEmbed embed = new DiscordEmbed();
	embed.SetTitle("FromObject Test");
	embed.SetDescription("This response was sent using CreateResponseFromObject");
	embed.Color = 0x00FF00;
	msg.AddEmbed(embed);

	// Create action row with buttons
	DiscordComponent row = new DiscordComponent();
	row.Type = ComponentType_ActionRow;

	DiscordComponent btnEdit = new DiscordComponent();
	btnEdit.Type = ComponentType_Button;
	btnEdit.Style = ComponentStyle_Primary;
	btnEdit.SetLabel("Edit Response");
	btnEdit.SetCustomId("fromobj_edit");

	DiscordComponent btnFollowUp = new DiscordComponent();
	btnFollowUp.Type = ComponentType_Button;
	btnFollowUp.Style = ComponentStyle_Secondary;
	btnFollowUp.SetLabel("Follow Up");
	btnFollowUp.SetCustomId("fromobj_followup");

	row.AddComponent(btnEdit);
	row.AddComponent(btnFollowUp);
	msg.AddComponent(row);

	// Send response using FromObject
	interaction.CreateResponseFromObject(msg);

	delete embed;
	delete btnEdit;
	delete btnFollowUp;
	delete row;
	delete msg;

	PrintToServer("[Discord] Sent CreateResponseFromObject");
}

void OnButtonClick(Discord bot, DiscordEvent event, any data)
{
	DiscordButtonInteraction interaction = view_as<DiscordButtonInteraction>(event.GetHandle("interaction"));
	if (interaction == null)
		return;

	char customId[64];
	interaction.GetCustomId(customId, sizeof(customId));

	if (strcmp(customId, "fromobj_edit") == 0)
	{
		// Test EditResponseFromObject - not available on button interaction
		// Instead demonstrate ReplyFromObject
		DiscordMessage reply = DiscordMessage.CreateWithContent(bot, "Button clicked! This uses ReplyFromObject:");

		DiscordEmbed embed = new DiscordEmbed();
		embed.SetTitle("Edit Button Clicked");
		embed.SetDescription("ReplyFromObject allows components in button responses");
		embed.Color = 0x0099FF;
		reply.AddEmbed(embed);

		interaction.ReplyFromObject(reply, true);

		delete embed;
		delete reply;

		PrintToServer("[Discord] Button: ReplyFromObject sent");
	}
	else if (strcmp(customId, "fromobj_followup") == 0)
	{
		// First acknowledge the interaction
		interaction.Reply("Processing...", true);

		// Then send a follow-up with components (if FollowUpFromObject was available on button)
		// For now just reply
		PrintToServer("[Discord] Button: Follow up clicked");
	}
	else
	{
		char button[32];
		FormatEx(button, sizeof(button), "Click %s button ok", customId);
		interaction.Reply(button, true);
	}
}

// Test DiscordMessage.ReplyFromObject
void OnMessageCreate(Discord bot, DiscordEvent event, any data)
{
	DiscordMessage message = view_as<DiscordMessage>(event.GetHandle("message"));
	if (message == null)
		return;

	// Ignore bot messages
	if (message.IsBot)
		return;

	char content[256];
	message.GetContent(content, sizeof(content));

	// Trigger on "!testreply"
	if (strcmp(content, "!testreply") != 0)
		return;

	// Create reply with components using ReplyFromObject
	DiscordMessage reply = DiscordMessage.CreateWithContent(bot, "Reply with components:");

	DiscordEmbed embed = new DiscordEmbed();
	embed.SetTitle("ReplyFromObject Test");
	embed.SetDescription("This reply was sent using ReplyFromObject method");
	embed.Color = 0xFF6600;
	reply.AddEmbed(embed);

	// Add buttons
	DiscordComponent row = new DiscordComponent();
	row.Type = ComponentType_ActionRow;

	DiscordComponent btn1 = new DiscordComponent();
	btn1.Type = ComponentType_Button;
	btn1.Style = ComponentStyle_Success;
	btn1.SetLabel("Option A");
	btn1.SetCustomId("reply_option_a");

	DiscordComponent btn2 = new DiscordComponent();
	btn2.Type = ComponentType_Button;
	btn2.Style = ComponentStyle_Danger;
	btn2.SetLabel("Option B");
	btn2.SetCustomId("reply_option_b");

	row.AddComponent(btn1);
	row.AddComponent(btn2);
	reply.AddComponent(row);

	// Send reply using ReplyFromObject
	message.ReplyFromObject(reply);

	delete embed;
	delete btn1;
	delete btn2;
	delete row;
	delete reply;

	PrintToServer("[Discord] Sent ReplyFromObject");
}
