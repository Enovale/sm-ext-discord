// Discord client
methodmap Discord < Handle {
  /**
   * Creates a new Discord bot client
   *
   * @param token     Discord bot token
   * @param intents   Optional Discord intents bitmask to control what events the bot receives (default: DISCORD_INTENTS_DEFAULT | Intent_MessageContent)
   * @return          New Discord client handle, or null on failure
   * @note            If no intents specified, the bot will use all available intents for backward compatibility.
   *                  For better performance and compliance, specify only the intents you need
   */
  public native Discord(const char[] token, DiscordIntents intents = DISCORD_INTENTS_DEFAULT | Intent_MessageContent);

  /**
   * Starts the Discord bot
   *
   * @note            This method performs an async start operation
   *                  You must wait for the BOT to be fully ready before executing certain actions
   */
  public native void Start();

  /**
   * Stops the Discord bot
   */
  public native void Stop();

  /**
   * Checks if the bot is running
   *
   * @return          True if running, false if stopped
   */
  property bool IsRunning {
    public native get();
  }

  /**
   * Gets the uptime of the Discord bot in seconds
   *
   * @return             Bot uptime in seconds since connection
   */
  property int Uptime {
    public native get();
  }

  /**
   * Gets the bot's user ID
   *
   * @param buffer    Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   * @return          True if successful, false otherwise
   */
  public native bool GetBotId(char[] buffer, int maxlength);

  /**
   * Gets the bot's username
   *
   * @param buffer    Buffer to store the username
   * @param maxlength    Maximum length of the buffer
   * @return          True if successful, false otherwise
   */
  public native bool GetBotName(char[] buffer, int maxlength);

  /**
   * Gets the bot's discriminator
   *
   * @return          The bot's discriminator
   */
  property int BotDiscriminator {
    public native get();
  }

  /**
   * Gets the bot's avatar URL
   *
   * @param buffer    Buffer to store the URL
   * @param maxlength    Maximum length of the buffer
   * @return          True if successful, false otherwise
   */
  public native bool GetBotAvatarUrl(char[] buffer, int maxlength);

  /**
   * Sets the bot's presence and activity
   *
   * @param status       The online presence status of the bot
   * @param activity     The type of activity for the bot
   * @param statusText   The text of the status to set
   */
  public native void SetPresence(DiscordPresenceStatus status, DiscordActivityType activity, const char[] statusText);


  /**
   * Sends a message to a specified channel
   *
   * @param channel_id    Target channel ID (numeric string)
   * @param message       Message content to send
   * @param callback      Optional callback to receive the created message
   * @param data          Optional data to pass to callback
   * @note                For advanced options (allowed mentions, embeds, components), use SendMessageFromObject
   */
  public native void SendMessage(const char[] channel_id, const char[] message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sends a message with embed to a specified channel
   *
   * @param channel_id    Target channel ID
   * @param message       Message content to send
   * @param embed         Embed object to send
   * @param callback      Optional callback to receive the created message
   * @param data          Optional data to pass to callback
   * @note                For advanced options (allowed mentions, components), use SendMessageFromObject
   */
  public native void SendMessageEmbed(const char[] channel_id, const char[] message, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sends a DiscordMessage object
   *
   * @param message   DiscordMessage object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            The message must have a channel ID set
   */
  public native void SendMessageFromObject(DiscordMessage message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sends a DiscordMessage object to a specific channel
   *
   * @param channel_id Target channel ID
   * @param message   DiscordMessage object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            This will override the channel ID in the message object
   */
  public native void SendMessageFromObjectToChannel(const char[] channel_id, DiscordMessage message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits an existing message
   *
   * @param channel_id    Channel ID
   * @param message_id    Message ID
   * @param content       New message content
   * @param callback      Optional callback to receive the edited message
   * @param data          Optional data to pass to callback
   */
  public native void EditMessage(const char[] channel_id, const char[] message_id, const char[] content, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits an existing message with embed
   *
   * @param channel_id    Channel ID
   * @param message_id    Message ID
   * @param content       New message content
   * @param embed         Embed object
   * @param callback      Optional callback to receive the edited message
   * @param data          Optional data to pass to callback
   */
  public native void EditMessageEmbed(const char[] channel_id, const char[] message_id, const char[] content, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Deletes a message
   *
   * @param channel_id    Channel ID
   * @param message_id    Message ID
   * @param callback      Optional callback to receive confirmation
   * @param data          Optional data to pass to callback
   */
  public native void DeleteMessage(const char[] channel_id, const char[] message_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Deletes multiple messages in a single request (bulk delete)
   *
   * @param channel_id    Channel ID where messages are located
   * @param message_ids   Array of message IDs to delete
   * @param count         Number of message IDs in the array
   * @param callback      Optional callback to receive confirmation
   * @param data          Optional data to pass to callback
   * @note                Requires between 2 and 100 message IDs
   * @note                Messages older than 2 weeks cannot be bulk deleted
   * @note                This is more efficient than deleting messages individually
   */
  public native void DeleteMessageBulk(const char[] channel_id, const char[][] message_ids, int count, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets multiple messages from a channel (message history)
   *
   * @param channel_id    Channel ID to get messages from
   * @param callback      Callback function to receive the messages
   * @param limit         Maximum number of messages to return (1-100, default 50)
   * @param around        Optional get messages around this message ID
   * @param before        Optional get messages before this message ID
   * @param after         Optional get messages after this message ID
   * @param data          Optional data to pass to callback
   * @note                Result contains: "items" (DiscordHandleArray of DiscordMessage), "count" (int)
   */
  public native void GetMessages(const char[] channel_id, DiscordResultCallback callback, int limit = 50, const char[] around = NULL_STRING, const char[] before = NULL_STRING, const char[] after = NULL_STRING, any data = 0);

  /**
   * Creates a DM channel with a user
   *
   * @param user_id       User ID to create DM channel with
   * @param callback      Callback function to receive the DM channel
   * @param data          Optional data to pass to callback
   * @note                Result contains: "channel" (DiscordChannel handle)
   */
  public native void CreateDMChannel(const char[] user_id, DiscordResultCallback callback, any data = 0);

  /**
   * Fetches the current bot user from Discord API
   *
   * @param callback      Callback function to receive the user
   * @param data          Optional data to pass to callback
   * @note                Result contains: "user" (DiscordUser handle)
   */
  public native void FetchCurrentUser(DiscordResultCallback callback, any data = 0);

  /**
   * Fetches a thread from Discord API
   *
   * @param thread_id     Thread ID to fetch
   * @param callback      Callback function to receive the thread
   * @param data          Optional data to pass to callback
   * @note                Result contains: "thread" (DiscordChannel handle)
   */
  public native void FetchThread(const char[] thread_id, DiscordResultCallback callback, any data = 0);

  /**
   * Sends a typing indicator to a channel
   *
   * @param channel_id    Channel ID to send typing indicator to
   * @note                Typing indicator lasts for 10 seconds or until a message is sent
   */
  public native void SendTyping(const char[] channel_id);

  /**
   * Gets the total number of cached users across all guilds
   *
   * @return              Total cached user count
   */
  property int CachedUserCount {
    public native get();
  }

  /**
   * Gets the total number of cached guilds
   *
   * @return              Total cached guild count
   */
  property int CachedGuildCount {
    public native get();
  }

  /**
   * Gets the total number of cached channels across all guilds
   *
   * @return              Total cached channel count
   */
  property int CachedChannelCount {
    public native get();
  }

  /**
   * Gets the total number of cached roles across all guilds
   *
   * @return              Total cached role count
   */
  property int CachedRoleCount {
    public native get();
  }

  /**
   * Gets the total number of cached emojis across all guilds
   *
   * @return              Total cached emoji count
   */
  property int CachedEmojiCount {
    public native get();
  }

  /**
   * Registers an event callback
   *
   * @param event       Event name
   * @param callback    Callback function to register
   * @param data        Optional user data to pass to callback
   * @return            True if successful, false if event name is invalid
   *
   */
  public native bool RegisterEvent(const char[] event, DiscordEventCallback callback, any data = 0);

  /**
   * Unregisters an event callback
   *
   * @param event       Event name
   * @return            True if successful, false if event name is invalid
   */
  public native bool UnregisterEvent(const char[] event);

  /**
   * Makes an HTTP request with full header support and complete response data
   *
   * @param url           Target URL
   * @param method        HTTP method to use
   * @param callback      Callback function to call when complete
   * @param body          Request body (for POST/PUT/PATCH)
   * @param content_type  Content-Type header (default: application/json)
   * @param headers       Optional custom headers to send
   * @param data          User data to pass to callback
   * @note                Use result.GetHandle("completion") to get HttpCompletion object
   */
  public native void HttpRequest(const char[] url, HttpMethod method = HTTP_GET, DiscordResultCallback callback, const char[] body = NULL_STRING, const char[] content_type = "application/json", HttpHeaders headers = null, any data = 0);

  /**
   * Gets all global slash commands for this bot
   *
   * @param callback    Callback to receive the commands list
   * @param data        Optional data to pass to callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordSlashCommand), "count" (int)
   */
  public native void GetGlobalCommands(DiscordResultCallback callback, any data = 0);

  /**
   * Gets a specific global slash command by ID
   *
   * @param command_id  Command ID to fetch
   * @param callback    Callback to receive the command
   * @param data        Optional data to pass to callback
   * @note              Result contains: "command" (DiscordSlashCommand handle)
   */
  public native void GetGlobalCommand(const char[] command_id, DiscordResultCallback callback, any data = 0);
}