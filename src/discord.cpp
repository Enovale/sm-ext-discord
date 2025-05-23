#include "extension.h"
#include "types/webhook.h"
#include "types/channel.h"
#include "types/embed.h"
#include "types/message.h"
#include "types/user.h"
#include "types/interaction.h"
#include "types/autocomplete_interaction.h"

// Discord Client Implementation
DiscordClient::DiscordClient(const char* token) : m_isRunning(false), m_discord_handle(0)
{
	m_cluster = std::make_unique<dpp::cluster>(token, dpp::i_default_intents | dpp::i_message_content);
}

DiscordClient::~DiscordClient()
{
	Stop();
}

bool DiscordClient::Initialize()
{
	try {
		SetupEventHandlers();
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to initialize Discord bot: %s", e.what());
		return false;
	}
}

void DiscordClient::RunBot()
{
	try {
		m_cluster->start(false);
	}
	catch (const std::exception& e) {
		g_TaskQueue.Push([this, error = std::string(e.what())]() {
			smutils->LogError(myself, "Failed to run Discord bot: %s", error.c_str());
			});
	}
}

void DiscordClient::Start()
{
	if (!m_cluster || m_isRunning) {
		return;
	}

	m_isRunning = true;
	m_thread = std::make_unique<std::thread>(&DiscordClient::RunBot, this);
	smutils->LogMessage(myself, "Discord bot started successfully");
}

void DiscordClient::Stop()
{
	if (!m_cluster || !m_isRunning) {
		return;
	}

	m_isRunning = false;

	try {
		if (m_cluster) {
			m_cluster->shutdown();
		}

		if (m_thread && m_thread->joinable()) {
			m_thread->join();
		}

		m_thread.reset();

		m_cluster.reset();

		smutils->LogMessage(myself, "Discord bot stopped successfully");
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Error during Discord bot shutdown: %s", e.what());
	}
}

bool DiscordClient::SetPresence(dpp::presence presence)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->set_presence(presence);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to set presence: %s", e.what());
		return false;
	}
}

void AddAllowedMentionsToMessage(dpp::message* msg, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles)
{
	msg->set_allowed_mentions(allowed_mentions_mask & 1, allowed_mentions_mask & 2, allowed_mentions_mask & 4, allowed_mentions_mask & 8, users, roles);
}

bool DiscordClient::ExecuteWebhook(dpp::webhook wh, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles)
{
	if (!m_isRunning) {
		return false;
	}

	dpp::message message_obj(message);
	AddAllowedMentionsToMessage(&message_obj, allowed_mentions_mask, users, roles);

	try {
		m_cluster->execute_webhook(wh, message_obj);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to execute webhook: %s", e.what());
		return false;
	}
}

bool DiscordClient::SendMessage(dpp::snowflake channel_id, const char* message, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles)
{
	if (!m_isRunning) {
		return false;
	}

	dpp::message message_obj(channel_id, message);
	AddAllowedMentionsToMessage(&message_obj, allowed_mentions_mask, users, roles);

	try {
		m_cluster->message_create(message_obj);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to send message: %s", e.what());
		return false;
	}
}

bool DiscordClient::SendMessageEmbed(dpp::snowflake channel_id, const char* message, const DiscordEmbed* embed, int allowed_mentions_mask, std::vector<dpp::snowflake> users, std::vector<dpp::snowflake> roles)
{
	if (!m_isRunning) {
		return false;
	}

	dpp::message message_obj(channel_id, message);
	AddAllowedMentionsToMessage(&message_obj, allowed_mentions_mask, users, roles);

	try {
		message_obj.add_embed(embed->GetEmbed());
		m_cluster->message_create(message_obj);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to send message with embed: %s", e.what());
		return false;
	}
}

bool DiscordClient::GetChannelWebhooks(dpp::snowflake channel_id, IForward *callback_forward, cell_t data)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->get_channel_webhooks(channel_id, [this, forward = callback_forward, value = data](const dpp::confirmation_callback_t& callback)
		{
			if (callback.is_error())
			{
				smutils->LogError(myself, "Failed to get channel webhooks: %s", callback.get_error().message.c_str());
				forwards->ReleaseForward(forward);
				return;
			}
			auto webhook_map = callback.get<dpp::webhook_map>();

			g_TaskQueue.Push([this, &forward, webhooks = webhook_map, value = value]() {
				if (forward && forward->GetFunctionCount() == 0)
				{
					return;
				}

				int webhook_count = webhooks.size();
				std::unique_ptr<cell_t[]> handles = std::make_unique<cell_t[]>(webhook_count);

				HandleError err;
				HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
				int i = 0;
				for (auto pair : webhooks)
				{
					DiscordWebhook* wbk = new DiscordWebhook(pair.second);
					Handle_t webhookHandle = g_DiscordWebhookHandler.CreateHandle(wbk, &sec, &err);
					if (webhookHandle == BAD_HANDLE)
					{
						smutils->LogError(myself, "Could not create webhook handle (error %d)", err);
						continue;
					}
					handles[i++] = webhookHandle;
				}

				forward->PushCell(m_discord_handle);
				forward->PushArray(handles.get(), webhook_count);
				forward->PushCell(webhook_count);
				forward->PushCell(value);
				forward->Execute(nullptr);

				for (i = 0; i < webhook_count; i++)
				{
					handlesys->FreeHandle(handles[i], &sec);
				}

				forwards->ReleaseForward(forward);
			});
		});
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to get channel webhooks: %s", e.what());
		return false;
	}
}

bool DiscordClient::CreateWebhook(dpp::webhook wh, IForward *callback_forward, cell_t data)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->create_webhook(wh, [this, forward = callback_forward, value = data](const dpp::confirmation_callback_t& callback)
		{
			if (callback.is_error())
			{
				smutils->LogError(myself, "Failed to create webhook: %s", callback.get_error().message.c_str());
				forwards->ReleaseForward(forward);
				return;
			}
			auto webhook = callback.get<dpp::webhook>();

			g_TaskQueue.Push([this, &forward, webhook = new DiscordWebhook(webhook), value = value]() {
				if (forward && forward->GetFunctionCount() == 0)
				{
					return;
				}

				HandleError err;
				HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
				Handle_t webhookHandle = g_DiscordWebhookHandler.CreateHandle(webhook, &sec, &err);
				if (webhookHandle == BAD_HANDLE)
				{
					smutils->LogError(myself, "Could not create webhook handle (error %d)", err);
					return;
				}

				forward->PushCell(m_discord_handle);
				forward->PushCell(webhookHandle);
				forward->PushCell(value);
				forward->Execute(nullptr);

				handlesys->FreeHandle(webhookHandle, &sec);

				forwards->ReleaseForward(forward);
            });
		});
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to get create webhook: %s", e.what());
		return false;
	}
}

bool DiscordClient::GetChannel(dpp::snowflake channel_id, IForward *callback_forward, cell_t data)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->channel_get(channel_id, [this, forward = callback_forward, value = data](const dpp::confirmation_callback_t& callback)
		{
			if (callback.is_error())
			{
				smutils->LogError(myself, "Failed to get channel: %s", callback.get_error().message.c_str());
				forwards->ReleaseForward(forward);
				return;
			}
			auto channel = callback.get<dpp::channel>();

			g_TaskQueue.Push([this, &forward, channel = new DiscordChannel(channel), value = value]() {
				if (forward && forward->GetFunctionCount() == 0)
				{
					return;
				}

				HandleError err;
				HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
				Handle_t channelHandle = g_DiscordChannelHandler.CreateHandle(channel, &sec, &err);

				forward->PushCell(m_discord_handle);
				forward->PushCell(channelHandle);
				forward->PushCell(value);
				forward->Execute(nullptr);

				handlesys->FreeHandle(channelHandle, &sec);

				forwards->ReleaseForward(forward);
            });
		});
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to get channel: %s", e.what());
		return false;
	}
}

bool DiscordClient::DeleteGuildCommand(dpp::snowflake guild_id, dpp::snowflake command_id)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->guild_command_delete(command_id, guild_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to delete guild command: %s", e.what());
		return false;
	}
}

bool DiscordClient::DeleteGlobalCommand(dpp::snowflake command_id)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->global_command_delete(command_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to delete global command: %s", e.what());
		return false;
	}
}

bool DiscordClient::BulkDeleteGuildCommands(dpp::snowflake guild_id)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->guild_bulk_command_delete(guild_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to bulk delete guild commands: %s", e.what());
		return false;
	}
}

bool DiscordClient::BulkDeleteGlobalCommands()
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->global_bulk_command_delete();
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to bulk delete global commands: %s", e.what());
		return false;
	}
}

void DiscordClient::SetupEventHandlers()
{
	if (!m_cluster) {
		return;
	}

	m_cluster->on_ready([this](const dpp::ready_t& event) {
		UpdateBotInfo();
		g_TaskQueue.Push([this]() {
			if (g_pForwardReady && g_pForwardReady->GetFunctionCount()) {
				g_pForwardReady->PushCell(m_discord_handle);
				g_pForwardReady->Execute(nullptr);
			}
			});
		});

	m_cluster->on_message_create([this](const dpp::message_create_t& event) {
		g_TaskQueue.Push([this, msg = event.msg]() {
			if (g_pForwardMessage && g_pForwardMessage->GetFunctionCount()) {
				DiscordMessage* message = new DiscordMessage(msg);
				HandleError err;
				HandleSecurity sec;
				sec.pOwner = myself->GetIdentity();
				sec.pIdentity = myself->GetIdentity();

				Handle_t messageHandle = g_DiscordMessageHandler.CreateHandle(message, &sec, &err);

				if (messageHandle != BAD_HANDLE) {
					g_pForwardMessage->PushCell(m_discord_handle);
					g_pForwardMessage->PushCell(messageHandle);
					g_pForwardMessage->Execute(nullptr);

					handlesys->FreeHandle(messageHandle, &sec);
				}
			}
			});
		});

	m_cluster->on_log([this](const dpp::log_t& event) {
		g_TaskQueue.Push([this, message = event.message]() {
			if (g_pForwardError && g_pForwardError->GetFunctionCount()) {
				g_pForwardError->PushCell(m_discord_handle);
				g_pForwardError->PushString(message.c_str());
				g_pForwardError->Execute(nullptr);
			}
			});
		});

	m_cluster->on_slashcommand([this](const dpp::slashcommand_t& event) {
		g_TaskQueue.Push([this, event]() {
			if (g_pForwardSlashCommand && g_pForwardSlashCommand->GetFunctionCount()) {
				DiscordInteraction* interaction = new DiscordInteraction(event);

				HandleError err;
				HandleSecurity sec;
				sec.pOwner = myself->GetIdentity();
				sec.pIdentity = myself->GetIdentity();

				Handle_t interactionHandle = g_DiscordInteractionHandler.CreateHandle(interaction, &sec, &err);

				if (interactionHandle != BAD_HANDLE) {
					g_pForwardSlashCommand->PushCell(m_discord_handle);
					g_pForwardSlashCommand->PushCell(interactionHandle);
					g_pForwardSlashCommand->Execute(nullptr);

					handlesys->FreeHandle(interactionHandle, &sec);
				}
			}
			});
		});

	m_cluster->on_autocomplete([this](const dpp::autocomplete_t& event) {
		g_TaskQueue.Push([this, event]() {
			if (g_pForwardAutocomplete && g_pForwardAutocomplete->GetFunctionCount()) {
				DiscordAutocompleteInteraction* interaction = new DiscordAutocompleteInteraction(event);

				HandleError err;
				HandleSecurity sec;
				sec.pOwner = myself->GetIdentity();
				sec.pIdentity = myself->GetIdentity();

				Handle_t interactionHandle = g_DiscordAutocompleteInteractionHandler.CreateHandle(interaction, &sec, &err);

				if (interactionHandle != BAD_HANDLE) {
					std::string str;
					for (auto & opt : event.options) {
						dpp::command_option_type type = opt.type;

						g_pForwardAutocomplete->PushCell(m_discord_handle);
						g_pForwardAutocomplete->PushCell(interactionHandle);
						g_pForwardAutocomplete->PushCell(opt.focused ? 1 : 0);
						g_pForwardAutocomplete->PushCell(type);
						g_pForwardAutocomplete->PushString(opt.name.c_str());
						g_pForwardAutocomplete->Execute(nullptr);
					}
	        	}

				handlesys->FreeHandle(interactionHandle, &sec);
			}
		});
	});
}

// Natives Implementation
static cell_t discord_CreateClient(IPluginContext* pContext, const cell_t* params)
{
	char* token;
	pContext->LocalToString(params[1], &token);

	DiscordClient* pDiscordClient = new DiscordClient(token);

	if (!pDiscordClient->Initialize())
	{
		delete pDiscordClient;
		return BAD_HANDLE;
	}

	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());
	Handle_t handle = g_DiscordHandler.CreateHandle(pDiscordClient, &sec, &err);

	if (handle == BAD_HANDLE)
	{
		delete pDiscordClient;
		pContext->ReportError("Could not create Discord handle (error %d)", err);
		return BAD_HANDLE;
	}

	pDiscordClient->SetHandle(handle);
	return handle;
}

static cell_t discord_Start(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	discord->Start();
	return 1;
}

static cell_t discord_Stop(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	discord->Stop();
	return 1;
}

static cell_t discord_GetBotId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	const char* botId = discord->GetBotId();
	if (!botId) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], botId);
	return 1;
}

static cell_t discord_GetBotName(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	const char* botName = discord->GetBotName();
	if (!botName) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], botName);
	return 1;
}

static cell_t discord_GetBotDiscriminator(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	const char* discriminator = discord->GetBotDiscriminator();
	if (!discriminator) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], discriminator);
	return 1;
}

static cell_t discord_GetBotAvatarUrl(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	const char* avatarUrl = discord->GetBotAvatarUrl();
	if (!avatarUrl) {
		return 0;
	}

	pContext->StringToLocal(params[2], params[3], avatarUrl);
	return 1;
}

static cell_t discord_SetPresence(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* status_text;
	pContext->LocalToString(params[4], &status_text);

	try {
        dpp::presence presence(static_cast<dpp::presence_status>(params[2]), static_cast<dpp::activity_type>(params[3]), status_text);
		return discord->SetPresence(presence);
	}
	catch (const std::exception& e) {
		pContext->ReportError("Unable to create presence object: %s", e.what());
		return 0;
	}
}

static cell_t discord_GetChannelWebhooks(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	try {
		dpp::snowflake channelFlake = std::stoull(channelId);

		IPluginFunction *callback = pContext->GetFunctionById(params[3]);

		IChangeableForward *forward = forwards->CreateForwardEx(nullptr, ET_Ignore, 4, nullptr, Param_Cell, Param_Array, Param_Cell, Param_Any);
		if (forward == nullptr || !forward->AddFunction(callback))
		{
			return pContext->ThrowNativeError("Could not create forward.");
		}

		cell_t data = params[4];
		return discord->GetChannelWebhooks(channelFlake, forward, data);
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid channel ID format: %s", channelId);
		return 0;
	}
}

static cell_t discord_CreateWebhook(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* name;
	pContext->LocalToString(params[3], &name);

	try {
		dpp::snowflake channelFlake = std::stoull(channelId);
		dpp::webhook webhook;
		webhook.name = name;
		webhook.channel_id = channelFlake;

		IPluginFunction *callback = pContext->GetFunctionById(params[4]);

		IChangeableForward *forward = forwards->CreateForwardEx(nullptr, ET_Ignore, 3, nullptr, Param_Cell, Param_Cell, Param_Any);
		if (forward == nullptr || !forward->AddFunction(callback))
		{
			return pContext->ThrowNativeError("Could not create forward.");
		}

		cell_t data = params[5];
		return discord->CreateWebhook(webhook, forward, data);
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid channel ID format: %s", channelId);
		return 0;
	}
}

static cell_t discord_ExecuteWebhook(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	DiscordWebhook* webhook = g_DiscordWebhookHandler.ReadHandle(params[2]);

	char* message;
	pContext->LocalToString(params[3], &message);

	cell_t* users_array;
	cell_t* roles_array;

	pContext->LocalToPhysAddr(params[5], &users_array);
	pContext->LocalToPhysAddr(params[7], &roles_array);

	std::vector<dpp::snowflake> users(params[6]);
	std::vector<dpp::snowflake> roles(params[8]);

	for (int i = 0; i < users.size(); i++) {
		char* str;

		pContext->LocalToString(users_array[i], &str);
		try {
			users[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	for (int i = 0; i < roles.size(); i++) {
		char* str;

		pContext->LocalToString(roles_array[i], &str);
		try {
			roles[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	try {
		return discord->ExecuteWebhook(webhook->m_webhook, message, params[4], users, roles) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Failed to execute webhook: %s", e.what());
		return 0;
	}
}

static cell_t discord_SendMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* message;
	pContext->LocalToString(params[3], &message);

	cell_t* users_array;
	cell_t* roles_array;

	pContext->LocalToPhysAddr(params[5], &users_array);
	pContext->LocalToPhysAddr(params[7], &roles_array);

	std::vector<dpp::snowflake> users(params[6]);
	std::vector<dpp::snowflake> roles(params[8]);

	for (int i = 0; i < users.size(); i++) {
		char* str;

		pContext->LocalToString(users_array[i], &str);
		try {
			users[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	for (int i = 0; i < roles.size(); i++) {
		char* str;

		pContext->LocalToString(roles_array[i], &str);
		try {
			roles[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	try {
		dpp::snowflake channel = std::stoull(channelId);
		return discord->SendMessage(channel, message, params[4], users, roles) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid channel ID format: %s", channelId);
		return 0;
	}
}

static cell_t discord_SendMessageEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* message;
	pContext->LocalToString(params[3], &message);

	cell_t* users_array;
	cell_t* roles_array;

	pContext->LocalToPhysAddr(params[6], &users_array);
	pContext->LocalToPhysAddr(params[8], &roles_array);

	std::vector<dpp::snowflake> users(params[7]);
	std::vector<dpp::snowflake> roles(params[9]);

	for (int i = 0; i < users.size(); i++) {
		char* str;

		pContext->LocalToString(users_array[i], &str);
		try {
			users[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	for (int i = 0; i < roles.size(); i++) {
		char* str;

		pContext->LocalToString(roles_array[i], &str);
		try {
			roles[i] = std::stoull(str);
		}
		catch (const std::exception& e) {
			continue; // Stub
		}
	}

	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[4]);

	try {
		dpp::snowflake channel = std::stoull(channelId);
		return discord->SendMessageEmbed(channel, message, embed, params[5], users, roles) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid channel ID format: %s", channelId);
		return 0;
	}
}

static cell_t discord_GetChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	try {
		dpp::snowflake channelFlake = std::stoull(channelId);

		IPluginFunction *callback = pContext->GetFunctionById(params[3]);

		IChangeableForward *forward = forwards->CreateForwardEx(nullptr, ET_Ignore, 3, nullptr, Param_Cell, Param_Cell, Param_Any);
		if (forward == nullptr || !forward->AddFunction(callback))
		{
			return pContext->ThrowNativeError("Could not create forward.");
		}

		cell_t data = params[4];
		return discord->GetChannel(channelFlake, forward, data);
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid channel ID format: %s", channelId);
		return 0;
	}
}

static cell_t discord_IsRunning(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	return discord->IsRunning() ? 1 : 0;
}

bool DiscordClient::RegisterSlashCommand(dpp::snowflake guild_id, const char* name, const char* description, const char* default_permissions)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::slashcommand command;
		command.set_name(name)
			.set_description(description)
			.set_application_id(m_cluster->me.id);

		if ((default_permissions != NULL) && (default_permissions[0] != '\0')) {
			command.set_default_permissions(std::stoull(default_permissions));
		}

		m_cluster->guild_command_create(command, guild_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to register slash command: %s", e.what());
		return false;
	}
}

bool DiscordClient::RegisterGlobalSlashCommand(const char* name, const char* description, const char* default_permissions)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::slashcommand command;
		command.set_name(name)
			.set_description(description)
			.set_application_id(m_cluster->me.id);

		if ((default_permissions != NULL) && (default_permissions[0] != '\0')) {
			command.set_default_permissions(std::stoull(default_permissions));
		}

		m_cluster->global_command_create(command);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to register global slash command: %s", e.what());
		return false;
	}
}

static cell_t discord_RegisterSlashCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	char* name;
	pContext->LocalToString(params[3], &name);

	char* description;
	pContext->LocalToString(params[4], &description);

	char* permissions;
	pContext->LocalToString(params[5], &permissions);

	try {
		dpp::snowflake guild = std::stoull(guildId);
		return discord->RegisterSlashCommand(guild, name, description, permissions) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid guild ID format: %s", guildId);
		return 0;
	}
}

static cell_t discord_RegisterGlobalSlashCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	char* permissions;
	pContext->LocalToString(params[4], &permissions);

	return discord->RegisterGlobalSlashCommand(name, description, permissions) ? 1 : 0;
}

void DiscordClient::CreateAutocompleteResponse(dpp::snowflake id, const std::string &token, const dpp::interaction_response &response)
{
	m_cluster->interaction_response_create(id, token, response);
}

bool DiscordClient::EditMessage(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::message msg;
		msg.id = message_id;
		msg.channel_id = channel_id;
		msg.content = content;
		m_cluster->message_edit(msg);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to edit message: %s", e.what());
		return false;
	}
}

bool DiscordClient::EditMessageEmbed(dpp::snowflake channel_id, dpp::snowflake message_id, const char* content, const DiscordEmbed* embed)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::message msg;
		msg.id = message_id;
		msg.channel_id = channel_id;
		msg.content = content;
		msg.add_embed(embed->GetEmbed());
		m_cluster->message_edit(msg);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to edit message with embed: %s", e.what());
		return false;
	}
}

bool DiscordClient::DeleteMessage(dpp::snowflake channel_id, dpp::snowflake message_id)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		m_cluster->message_delete(message_id, channel_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to delete message: %s", e.what());
		return false;
	}
}

static cell_t discord_EditMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	char* content;
	pContext->LocalToString(params[4], &content);

	try {
		dpp::snowflake channel = std::stoull(channelId);
		dpp::snowflake message = std::stoull(messageId);
		return discord->EditMessage(channel, message, content) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid ID format");
		return 0;
	}
}

static cell_t discord_EditMessageEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	char* content;
	pContext->LocalToString(params[4], &content);

	DiscordEmbed* embed = g_DiscordEmbedHandler.ReadHandle(params[5]);

	try {
		dpp::snowflake channel = std::stoull(channelId);
		dpp::snowflake message = std::stoull(messageId);
		return discord->EditMessageEmbed(channel, message, content, embed) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid ID format");
		return 0;
	}
}

static cell_t discord_DeleteMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* messageId;
	pContext->LocalToString(params[3], &messageId);

	try {
		dpp::snowflake channel = std::stoull(channelId);
		dpp::snowflake message = std::stoull(messageId);
		return discord->DeleteMessage(channel, message) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid ID format");
		return 0;
	}
}

bool DiscordClient::RegisterSlashCommandWithOptions(dpp::snowflake guild_id, const char* name, const char* description, const char* default_permissions,
	const std::vector<dpp::command_option>& options)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::slashcommand command;
		command.set_name(name)
			.set_description(description)
			.set_application_id(m_cluster->me.id);

		if ((default_permissions != NULL) && (default_permissions[0] != '\0')) {
			command.set_default_permissions(std::stoull(default_permissions));
		}

		command.options = options;
		m_cluster->guild_command_create(command, guild_id);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to register slash command with options: %s", e.what());
		return false;
	}
}

bool DiscordClient::RegisterGlobalSlashCommandWithOptions(const char* name, const char* description, const char* default_permissions,
	const std::vector<dpp::command_option>& options)
{
	if (!m_isRunning) {
		return false;
	}

	try {
		dpp::slashcommand command;
		command.set_name(name)
			.set_description(description)
			.set_application_id(m_cluster->me.id);

		if ((default_permissions != NULL) && (default_permissions[0] != '\0')) {
			command.set_default_permissions(std::stoull(default_permissions));
		}

		command.options = options;
		m_cluster->global_command_create(command);
		return true;
	}
	catch (const std::exception& e) {
		smutils->LogError(myself, "Failed to register global slash command with options: %s", e.what());
		return false;
	}
}

static cell_t discord_RegisterSlashCommandWithOptions(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	char* name;
	pContext->LocalToString(params[3], &name);

	char* description;
	pContext->LocalToString(params[4], &description);

	char* permissions;
	pContext->LocalToString(params[5], &permissions);

	cell_t* option_names_array;
	cell_t* option_descriptions_array;
	cell_t* option_types_array;
	cell_t* option_required_array;
	cell_t* option_autocomplete_array;

	pContext->LocalToPhysAddr(params[6], &option_names_array);
	pContext->LocalToPhysAddr(params[7], &option_descriptions_array);
	pContext->LocalToPhysAddr(params[8], &option_types_array);
	pContext->LocalToPhysAddr(params[9], &option_required_array);
	pContext->LocalToPhysAddr(params[10], &option_autocomplete_array);

	cell_t optionsSize = params[11];

	std::vector<dpp::command_option> options;

	for (cell_t i = 0; i < optionsSize; i++) {
		char* option_name;
		char* option_description;

		pContext->LocalToString(option_names_array[i], &option_name);
		pContext->LocalToString(option_descriptions_array[i], &option_description);

		dpp::command_option cmd_option(
			static_cast<dpp::command_option_type>(option_types_array[i]),
			option_name,
			option_description,
			option_required_array[i]
		);
		cmd_option.set_auto_complete(option_autocomplete_array[i]);

		options.push_back(cmd_option);
	}

	try {
		dpp::snowflake guild = std::stoull(guildId);
		return discord->RegisterSlashCommandWithOptions(guild, name, description, permissions, options) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid guild ID format: %s", guildId);
		return 0;
	}
}

static cell_t discord_RegisterGlobalSlashCommandWithOptions(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* name;
	pContext->LocalToString(params[2], &name);

	char* description;
	pContext->LocalToString(params[3], &description);

	char* permissions;
	pContext->LocalToString(params[4], &permissions);

	cell_t* option_names_array;
	cell_t* option_descriptions_array;
	cell_t* option_types_array;
	cell_t* option_required_array;
	cell_t* option_autocomplete_array;

	pContext->LocalToPhysAddr(params[5], &option_names_array);
	pContext->LocalToPhysAddr(params[6], &option_descriptions_array);
	pContext->LocalToPhysAddr(params[7], &option_types_array);
	pContext->LocalToPhysAddr(params[8], &option_required_array);
	pContext->LocalToPhysAddr(params[9], &option_autocomplete_array);

	cell_t optionsSize = params[10];

	std::vector<dpp::command_option> options;

	for (cell_t i = 0; i < optionsSize; i++) {
		char* option_name;
		char* option_description;

		pContext->LocalToString(option_names_array[i], &option_name);
		pContext->LocalToString(option_descriptions_array[i], &option_description);

		dpp::command_option cmd_option(
			static_cast<dpp::command_option_type>(option_types_array[i]),
			option_name,
			option_description,
			option_required_array[i]
		);
		cmd_option.set_auto_complete(option_autocomplete_array[i]);

		options.push_back(cmd_option);
	}

	return discord->RegisterGlobalSlashCommandWithOptions(name, description, permissions, options) ? 1 : 0;
}

static cell_t discord_DeleteGuildCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	char* commandId;
	pContext->LocalToString(params[3], &commandId);

	try {
		dpp::snowflake guild = std::stoull(guildId);
		dpp::snowflake command = std::stoull(commandId);
		return discord->DeleteGuildCommand(guild, command) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid ID format");
		return 0;
	}
}

static cell_t discord_DeleteGlobalCommand(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* commandId;
	pContext->LocalToString(params[2], &commandId);

	try {
		dpp::snowflake command = std::stoull(commandId);
		return discord->BulkDeleteGuildCommands(command) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid command ID format");
		return 0;
	}
}

static cell_t discord_BulkDeleteGuildCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	char* guildId;
	pContext->LocalToString(params[2], &guildId);

	try {
		dpp::snowflake guild = std::stoull(guildId);
		return discord->BulkDeleteGuildCommands(guild) ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Invalid guild ID format");
		return 0;
	}
}

static cell_t discord_BulkDeleteGlobalCommands(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = g_DiscordHandler.ReadHandle(params[1]);
	if (!discord) {
		return 0;
	}

	try {
		return discord->BulkDeleteGlobalCommands() ? 1 : 0;
	}
	catch (const std::exception& e) {
		pContext->ReportError("Unable to bulk delete global commands: %s", e.what());
		return 0;
	}
}

const sp_nativeinfo_t discord_natives[] = {
	// Discord
	{"Discord.Discord",          discord_CreateClient},
	{"Discord.Start",            discord_Start},
	{"Discord.Stop",             discord_Stop},
	{"Discord.GetBotId",           discord_GetBotId},
	{"Discord.GetBotName",         discord_GetBotName},
	{"Discord.GetBotDiscriminator", discord_GetBotDiscriminator},
	{"Discord.GetBotAvatarUrl",    discord_GetBotAvatarUrl},
	{"Discord.SetPresence",      discord_SetPresence},
	{"Discord.CreateWebhook",      discord_CreateWebhook},
	{"Discord.GetChannelWebhooks",      discord_GetChannelWebhooks},
	{"Discord.ExecuteWebhook",      discord_ExecuteWebhook},
	{"Discord.SendMessage",      discord_SendMessage},
	{"Discord.SendMessageEmbed", discord_SendMessageEmbed},
	{"Discord.GetChannel", discord_GetChannel},
	{"Discord.IsRunning",        discord_IsRunning},
	{"Discord.RegisterSlashCommand", discord_RegisterSlashCommand},
	{"Discord.RegisterGlobalSlashCommand", discord_RegisterGlobalSlashCommand},
	{"Discord.EditMessage", discord_EditMessage},
	{"Discord.EditMessageEmbed", discord_EditMessageEmbed},
	{"Discord.DeleteMessage", discord_DeleteMessage},
	{"Discord.RegisterSlashCommandWithOptions", discord_RegisterSlashCommandWithOptions},
  	{"Discord.RegisterGlobalSlashCommandWithOptions", discord_RegisterGlobalSlashCommandWithOptions},
	{"Discord.DeleteGuildCommand", discord_DeleteGuildCommand},
  	{"Discord.DeleteGlobalCommand", discord_DeleteGlobalCommand},
  	{"Discord.BulkDeleteGuildCommands", discord_BulkDeleteGuildCommands},
  	{"Discord.BulkDeleteGlobalCommands", discord_BulkDeleteGlobalCommands},
	{nullptr, nullptr}
};