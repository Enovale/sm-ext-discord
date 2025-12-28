#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <discord>

Discord g_Bot;

public Plugin myinfo = {
	name = "Discord Poll Example",
	author = "ProjectSky",
	description = "Demonstrates Discord poll creation and handling",
	version = "1.0.0",
	url = "https://github.com/ProjectSky/sm-ext-discord"
}

public void OnPluginStart()
{
	RegAdminCmd("sm_discord_poll_start", Command_Start, ADMFLAG_ROOT, "Start the Discord bot");
	RegAdminCmd("sm_discord_poll", Command_Poll, ADMFLAG_ROOT, "Create a Discord poll");
}

Action Command_Start(int client, int args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_poll_start <bot_token>");
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
	g_Bot = new Discord(token);

	g_Bot.RegisterEvent("ready", OnBotReady);
	g_Bot.RegisterEvent("message_create", OnMessageReceived);
	g_Bot.RegisterEvent("message_poll_vote_add", OnPollVoteAdd);
	g_Bot.RegisterEvent("message_poll_vote_remove", OnPollVoteRemove);

	g_Bot.Start();

	ReplyToCommand(client, "[Discord] Bot starting...");
	return Plugin_Handled;
}

Action Command_Poll(int client, int args)
{
	if (g_Bot == null || !g_Bot.IsRunning)
	{
		ReplyToCommand(client, "[Discord] Bot is not running.");
		return Plugin_Handled;
	}

	if (args < 1)
	{
		ReplyToCommand(client, "Usage: sm_discord_poll <channel_id>");
		return Plugin_Handled;
	}

	char channelId[32];
	GetCmdArg(1, channelId, sizeof(channelId));

	// Create a poll
	DiscordPoll poll = new DiscordPoll();
	poll.SetQuestion("What's your favorite game mode?");
	poll.Duration = 24; // 24 hours
	poll.MultiSelect = false;

	// Add answers (with optional emoji)
	poll.AddAnswer("Deathmatch");
	poll.AddAnswer("Team Deathmatch");
	poll.AddAnswer("Capture the Flag", "🚩");
	poll.AddAnswer("Survival", "💀");

	// Create message with poll
	DiscordMessage msg = DiscordMessage.CreateWithChannel(g_Bot, channelId, "Vote for your favorite game mode!");
	msg.SetPoll(poll);
	msg.Send();

	delete poll;
	delete msg;

	ReplyToCommand(client, "[Discord] Poll created!");
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

void OnMessageReceived(Discord bot, DiscordEvent event, any data)
{
	DiscordMessage message = view_as<DiscordMessage>(event.GetHandle("message"));
	if (message == null)
		return;

	// Check if message has a poll
	if (!message.HasPoll)
		return;

	DiscordPoll poll = message.Poll;
	if (poll == null)
		return;

	char question[256];
	poll.GetQuestion(question, sizeof(question));
	PrintToServer("[Discord] Poll: %s", question);

	int answerCount = poll.AnswerCount;
	PrintToServer("[Discord] Answers: %d", answerCount);

	// Display results if available
	if (poll.HasResults)
	{
		for (int i = 0; i < answerCount; i++)
		{
			int answerId = poll.GetAnswerId(i);
			char answerText[128];
			poll.GetAnswerText(i, answerText, sizeof(answerText));
			int votes = poll.GetVoteCount(answerId);

			PrintToServer("[Discord]   %s: %d votes", answerText, votes);
		}
	}

	delete poll;
}

void OnPollVoteAdd(Discord bot, DiscordEvent event, any data)
{
	char userId[SNOWFLAKE_SIZE], messageId[SNOWFLAKE_SIZE], channelId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));
	event.GetString("message_id", messageId, sizeof(messageId));
	event.GetString("channel_id", channelId, sizeof(channelId));
	int answerId = event.GetInt("answer_id");

	PrintToServer("[Discord] User %s voted for answer %d on poll %s", userId, answerId, messageId);
}

void OnPollVoteRemove(Discord bot, DiscordEvent event, any data)
{
	char userId[SNOWFLAKE_SIZE], messageId[SNOWFLAKE_SIZE];
	event.GetString("user_id", userId, sizeof(userId));
	event.GetString("message_id", messageId, sizeof(messageId));
	int answerId = event.GetInt("answer_id");

	PrintToServer("[Discord] User %s removed vote for answer %d on poll %s", userId, answerId, messageId);
}
