// Discord role
methodmap DiscordRole < Handle
{
  /**
   * Creates a DiscordRole object
   *
   * @param discord     Discord client handle
   * @param guild_id    Guild ID as string (optional)
   * @return            DiscordRole handle or null on failure
   * @note              Call with no parameters to create an empty role template for CreateRoleFromObject
   */
  public native DiscordRole(Discord discord, const char[] guild_id = NULL_STRING);

  /**
   * Fetches full role information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param guild_id    Guild ID as string
   * @param role_id     Role ID as string
   * @param callback    Function to call when role info is received
   * @param data        Optional data to pass to callback
   * @note              This method first checks the cache for role data. If found in cache,
   *                    the callback is called immediately. If not cached, an API request is made
   * @note              Result contains: "role" (DiscordRole handle)
   */
  public static native void FetchRole(Discord discord, const char[] guild_id, const char[] role_id, DiscordResultCallback callback, any data = 0);

  /**
   * Finds a role object from cache by ID
   *
   * @param discord     Discord client handle
   * @param guild_id    Guild ID to search in
   * @param role_id     Role ID to search for
   * @return            Handle to the DiscordRole object if found, null if not found or cache unavailable
   *
   * @note              This performs a local lookup only - no API requests are made,
   *                    The cache is populated when the bot successfully connects to Discord
   *
   * @note              Cache behavior depends on bot state:
   *                    - If bot has never run: cache is empty (always returns null)
   *                    - If bot is currently online: cache is up-to-date
   *                    - If bot was online but disconnected: cache remains at last-known state
   *
   * @error             Invalid guild ID or role ID format
   */
  public static native DiscordRole FindRole(Discord discord, const char[] guild_id, const char[] role_id);

  /**
   * Gets the role ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the role name
   *
   * @param buffer       Buffer to store the name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Sets the role name
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @param name          New role name
   */
  public native void SetName(const char[] name);

  /**
   * Gets or sets the role color as an integer
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @return             Color as integer (decimal format)
   */
  property int Color {
    public native get();
    public native set(int color);
  }

  /**
   * Gets or sets whether role is hoisted (displayed separately in the member list)
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @return             True if hoisted, false otherwise
   */
  property bool Hoist {
    public native get();
    public native set(bool value);
  }

  /**
   * Gets the role icon hash
   *
   * @param buffer       Buffer to store the icon hash
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetIconHash(char[] buffer, int maxlength);

  /**
   * Sets the role icon from a file
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @param filepath     Path to the image file (relative to game directory)
   * @param type         Image type (Image_PNG, Image_JPG, Image_GIF, Image_WEBP)
   * @return             True on success, false on failure
   */
  public native bool SetIcon(const char[] filepath, DiscordImageType type = Image_PNG);

  /**
   * Gets the role unicode emoji
   *
   * @param buffer       Buffer to store the unicode emoji
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUnicodeEmoji(char[] buffer, int maxlength);

  /**
   * Gets the role position
   *
   * @return             Position of the role
   */
  property int Position {
    public native get();
  }

  /**
   * Gets the role permissions as string
   *
   * @param buffer       Buffer to store the permissions
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetPermissions(char[] buffer, int maxlength);

  /**
   * Checks if the role is managed by an integration
   *
   * @return             True if managed, false otherwise
   */
  property bool Managed {
    public native get();
  }

  /**
   * Gets or sets whether role is mentionable
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @return             True if mentionable, false otherwise
   */
  property bool Mentionable {
    public native get();
    public native set(bool value);
  }

  /**
   * Checks if the role has tags
   *
   * @return             True if has tags, false otherwise
   */
  property bool HasTags {
    public native get();
  }

  /**
   * Checks if the role is the premium subscriber role
   *
   * @return             True if premium subscriber role, false otherwise
   */
  property bool IsPremiumSubscriberRole {
    public native get();
  }

  /**
   * Checks if the role is available for purchase
   *
   * @return             True if available for purchase, false otherwise
   */
  property bool IsAvailableForPurchase {
    public native get();
  }

  /**
   * Checks if the role is a guild's linked role
   *
   * @return             True if linked role, false otherwise
   */
  property bool IsGuildConnections {
    public native get();
  }

  /**
   * Gets the bot ID that manages this role (if managed by a bot)
   *
   * @param buffer       Buffer to store the bot ID
   * @param maxlength    Maximum length of the buffer
   * @return             True if role has bot ID, false otherwise
   */
  public native bool GetBotId(char[] buffer, int maxlength);

  /**
   * Gets the integration ID that manages this role (if managed by integration)
   *
   * @param buffer       Buffer to store the integration ID
   * @param maxlength    Maximum length of the buffer
   * @return             True if role has integration ID, false otherwise
   */
  public native bool GetIntegrationId(char[] buffer, int maxlength);

  /**
   * Gets the subscription listing ID (if available for purchase)
   *
   * @param buffer       Buffer to store the subscription listing ID
   * @param maxlength    Maximum length of the buffer
   * @return             True if role has subscription listing ID, false otherwise
   */
  public native bool GetSubscriptionListingId(char[] buffer, int maxlength);

  /**
   * Checks if the role has a specific permission
   *
   * @param permission   Permission to check as string
   * @return             True if role has permission, false otherwise
   */
  public native bool HasPermission(const char[] permission);

  /**
   * Checks if the role has a specific permission (enum version)
   *
   * @param permission   Permission to check from DiscordPermission enum
   * @return             True if role has permission, false otherwise
   */
  public bool HasPermissionEnum(DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    return this.HasPermission(permStr);
  }

  /**
   * Gets the mention string for this role
   *
   * @param buffer       Buffer to store the mention string
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetMention(char[] buffer, int maxlength);

  /**
   * Modifies the role, syncing local changes to Discord
   */
  public native void Modify();

  /**
   * Deletes the role
   */
  public native void Delete();

  /**
   * Adds the role to a user
   *
   * @param user_id      User ID to add the role to
   */
  public native void AddToUser(const char[] user_id);

  /**
   * Removes the role from a user
   *
   * @param user_id      User ID to remove the role from
   */
  public native void RemoveFromUser(const char[] user_id);
}