// Discord webhook
methodmap DiscordWebhook < Handle
{
  /**
   * Creates a new Discord Webhook
   *
   * @param discord     Discord client handle
   * @param webhookUrl  Webhook Url
   * @return            New Discord webhook handle, or null on failure
   */
  public native DiscordWebhook(Discord discord, const char[] webhookUrl);

  /**
   * Creates a new Discord Webhook from ID and Token
   *
   * @param discord       Discord client handle
   * @param webhook_id    Webhook ID
   * @param webhook_token Webhook Token
   * @return              New Discord webhook handle, or null on failure
   * @note                This creates a minimal webhook object. Use GetWebhook for complete info
   */
  public static native DiscordWebhook CreateFromIdToken(Discord discord, const char[] webhook_id, const char[] webhook_token);

  /**
   * Fetches full webhook information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param webhook_id  Webhook ID
   * @param callback    Function to call when webhook info is received
   * @param data        Optional data to pass to callback
   * @note              This method first checks the cache for webhook data. If found in cache,
   *                    the callback is called immediately. If not cached, an API request is made
   * @note              Result contains: "webhook" (DiscordWebhook handle)
   */
  public static native void FetchWebhook(Discord discord, const char[] webhook_id, DiscordResultCallback callback, any data = 0);

  /**
   * Gets the ID of the webhook
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the user that created the webhook
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordUser User {
    public native get();
  }

  /**
   * Gets the name of the webhook
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Sets the name of the webhook
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @param name    New string to set the name to
   */
  public native void SetName(const char[] name);

  /**
   * Gets the avatar_url of the webhook
   *
   * @param buffer      Buffer to store the url
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetAvatarUrl(char[] buffer, int maxlength);

  /**
   * Sets the webhook avatar from a file
   *
   * @note               Must call Modify() after to sync changes to Discord
   * @note               Mutually exclusive with SetAvatarFromUrl - use one or the other, not both
   *
   * @param filepath     Path to the image file (relative to game directory)
   * @param type         Image type (Image_PNG, Image_JPG, Image_GIF, Image_WEBP)
   * @return             True on success, false on failure
   */
  public native bool SetAvatarFromFile(const char[] filepath, DiscordImageType type = Image_PNG);

  /**
   * Sets the webhook avatar from a URL
   *
   * @note               Must call Modify() after to sync changes to Discord
   * @note               Mutually exclusive with SetAvatarFromFile - use one or the other, not both
   *
   * @param url          URL of the avatar image
   */
  public native void SetAvatarFromUrl(const char[] url);

  /**
   * Gets the webhook type
   */
  property DiscordWebhookType Type {
    public native get();
  }

  /**
   * Gets the guild ID of the webhook
   *
   * @param buffer      Buffer to store the guild ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the channel ID of the webhook
   *
   * @param buffer      Buffer to store the channel ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetChannelId(char[] buffer, int maxlength);

  /**
   * Gets the token of the webhook
   *
   * @param buffer      Buffer to store the token
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetToken(char[] buffer, int maxlength);

  /**
   * Gets the application ID of the webhook
   *
   * @param buffer      Buffer to store the application ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetApplicationId(char[] buffer, int maxlength);

  /**
   * Gets the source guild ID of the webhook (for channel follower webhooks)
   *
   * @param buffer      Buffer to store the source guild ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetSourceGuildId(char[] buffer, int maxlength);

  /**
   * Gets the source channel ID of the webhook (for channel follower webhooks)
   *
   * @param buffer      Buffer to store the source channel ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetSourceChannelId(char[] buffer, int maxlength);

  /**
   * Gets the URL of the webhook (read-only)
   *
   * @param buffer      Buffer to store the URL
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Gets the image data of the webhook (read-only)
   *
   * @param buffer      Buffer to store the image data
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetImageData(char[] buffer, int maxlength);

  /**
   * Sets the thread ID for forum channel webhooks
   *
   * @param thread_id   Thread ID to post to (existing forum thread)
   * @note              ONLY for forum/media channels
   * @note              Using on normal channels will cause error 10003
   * @note              Call this before Execute/ExecuteEmbed/ExecuteFromObject
   */
  public native void SetThreadId(const char[] thread_id);

  /**
   * Sets the thread name for forum channel webhooks
   *
   * @param thread_name Thread name to create (new forum post title)
   * @note              ONLY for forum/media channels
   * @note              Using on normal channels will cause an error 220003
   * @note              Call this before Execute/ExecuteEmbed/ExecuteFromObject
   */
  public native void SetThreadName(const char[] thread_name);

  /**
   * Modifies this webhook
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Modify(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Deletes this webhook
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Delete(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Executes this webhook with a message
   *
   * @param message   Message content to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            For advanced options (allowed mentions, embeds, components), use ExecuteFromObject
   */
  public native void Execute(const char[] message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Executes this webhook with a message and embed
   *
   * @param message   Message content to send
   * @param embed     Embed object to include
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            For advanced options (allowed mentions, components), use ExecuteFromObject
   */
  public native void ExecuteEmbed(const char[] message, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Executes this webhook with a DiscordMessage object
   *
   * @param message   DiscordMessage object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void ExecuteFromObject(DiscordMessage message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a webhook for a channel
   *
   * @param discord     Discord client handle
   * @param channel_id  Target channel ID
   * @param name        Name of the webhook to be created
   * @param callback    Method to run on success
   * @param data        Arbitrary value to pass to the callback
   * @note              Result contains: "webhook" (DiscordWebhook handle)
   */
  public static native void CreateWebhook(Discord discord, const char[] channel_id, const char[] name, DiscordResultCallback callback, any data = 0);

  /**
   * Gets all webhooks assigned to a channel
   *
   * @param discord     Discord client handle
   * @param channel_id  Target channel ID
   * @param callback    Method to run on success
   * @param data        Arbitrary value to pass to the callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordWebhook), "count" (int)
   */
  public static native void GetChannelWebhooks(Discord discord, const char[] channel_id, DiscordResultCallback callback, any data = 0);

  /**
   * Gets all webhooks assigned to a guild
   *
   * @param discord     Discord client handle
   * @param guild_id    Target guild ID
   * @param callback    Method to run on success
   * @param data        Arbitrary value to pass to the callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordWebhook), "count" (int)
   */
  public static native void GetGuildWebhooks(Discord discord, const char[] guild_id, DiscordResultCallback callback, any data = 0);
}