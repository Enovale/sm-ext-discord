// Discord user
methodmap DiscordUser < Handle
{
  /**
   * Creates a DiscordUser object from user ID
   *
   * @param discord     Discord client handle to bind to this user
   * @param user_id     User ID as string
   * @param guild_id    Guild ID as string (optional, for member info)
   * @return            DiscordUser handle or null on failure
   * @note              This creates a minimal user object. Use FetchUser for complete info
   */
  public native DiscordUser(Discord discord, const char[] user_id, const char[] guild_id = NULL_STRING);

  /**
   * Fetches full user information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param user_id     User ID as string
   * @param guild_id    Guild ID as string (optional, for member info)
   * @param callback    Function to call when user info is received
   * @param data        Optional data to pass to callback
   * @note              Available information: username, avatar, discriminator, public flags
   * @note              Limited information: MFA status and email verification are not available
   *                    to bots due to Discord privacy restrictions. Flags = 0 is normal for
   *                    regular users without special Discord badges
   * @note              This method first checks the cache for user data. If found in cache,
   *                    the callback is called immediately. If not cached, an API request is made
   * @note              Result contains: "user" (DiscordUser handle)
   */
  public static native void FetchUser(Discord discord, const char[] user_id, const char[] guild_id = NULL_STRING, DiscordResultCallback callback, any data = 0);

  /**
   * Finds a user object from cache by ID
   *
   * @param discord     Discord client handle (some functions may require it)
   * @param user_id     User ID to search for
   * @param guild_id    Optional guild ID for member data (null string for no guild context)
   * @return            Handle to the DiscordUser object if found, null if not found or cache unavailable
   *
   * @note              This performs a local lookup only - no API requests are made,
   *                    The cache is populated when the bot successfully connects to Discord
   *
   * @note              Cache behavior depends on bot state:
   *                    - If bot has never run: cache is empty (always returns null)
   *                    - If bot is currently online: cache is up-to-date
   *                    - If bot was online but disconnected: cache remains at last-known state
   *
   * @error             Invalid user ID format
   */
  public static native DiscordUser FindUser(Discord discord, const char[] user_id, const char[] guild_id = NULL_STRING);

  /**
   * Gets the ID of the user
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the Discord client handle associated with this user
   *
   * @return            Discord client handle, or null if not available
   */
  public native Discord GetClient();

  /**
   * Gets the guild ID of the user (if this user has member data)
   *
   * @param buffer      Buffer to store the guild ID
   * @param maxlength   Maximum length of the buffer
   * @return            True if guild ID was retrieved, false if no member data
   */
  public native bool GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the username of the user
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetUserName(char[] buffer, int maxlength);

  /**
   * Gets the discriminator of the user
   */
  property int Discriminator {
    public native get();
  }

  /**
   * Gets the display name of the user
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGlobalName(char[] buffer, int maxlength);

  /**
   * Gets the avatar url of the user
   *
   * @param preferAnimated Whether to return the gif version of animated avatars if they are available or not
   * @param buffer         Buffer to store the url
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetAvatarUrl(bool preferAnimated, char[] buffer, int maxlength);

  /**
   * Gets the default avatar url of the user
   *
   * @param buffer         Buffer to store the url
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetDefaultAvatarUrl(char[] buffer, int maxlength);

  /**
   * Gets the avatar decoration url of the user (if they have one)
   *
   * @param size           Size of the avatar decoration in pixels (16-4096, power of 2)
   * @param buffer         Buffer to store the url
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetAvatarDecorationUrl(int size, char[] buffer, int maxlength);

  /**
   * Gets the mention string for the user
   *
   * @param buffer         Buffer to store the mention
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetMention(char[] buffer, int maxlength);

  /**
   * Gets the URL for the user's profile
   *
   * @param buffer         Buffer to store the url
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Gets the formatted username (username#discriminator or just username)
   *
   * @param buffer         Buffer to store the formatted username
   * @param maxlength      Maximum length of the buffer
   */
  public native void GetFormattedUsername(char[] buffer, int maxlength);

  /**
   * Gets the user's flags as a bitmask
   * @note flags = 0 is normal for regular users who don't have special Discord badges
   *       (Staff, VerifiedBot, ActiveDeveloper, etc.). Only users with special roles
   *       or achievements will have non-zero flag values
   */
  property DiscordUserFlags Flags {
    public native get();
  }

  /**
   * Checks if the user has a specific flag
   *
   * @param flag          Flag to check from DiscordUserFlags enum
   * @return              True if user has the flag, false otherwise
   */
  public native bool HasFlag(DiscordUserFlags flag);

  /**
   * Checks if the user is a bot
   *
   * @return          True if user is bot, false otherwise
   */
  property bool IsBot {
    public native get();
  }

  /**
   * Checks if this user has guild member information available
   *
   * @return          True if guild member info is available, false otherwise
   */
  property bool HasGuildMember {
    public native get();
  }

  /**
   * Gets the nickname of the user in the guild
   *
   * @param buffer    Buffer to store the nickname
   * @param maxlength Maximum length of the buffer
   */
  public native void GetNickName(char[] buffer, int maxlength);

  /**
   * Gets the timestamp when the user joined the guild
   */
  property int JoinedAt {
    public native get();
  }

  /**
   * Checks if the user is pending verification
   *
   * @return          True if user is pending, false otherwise
   */
  property bool IsPending {
    public native get();
  }

  /**
   * Checks if the user has a specific permission
   *
   * @param permission  Permission to check as string (e.g., "8" for administrator)
   * @return            True if user has permission, false otherwise
   */
  public native bool HasPermission(const char[] permission);

  /**
   * Checks if the user has a specific permission (enum version)
   *
   * @param permission  Permission to check from DiscordPermission enum
   * @return            True if user has permission, false otherwise
   */
  public bool HasPermissionEnum(DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    return this.HasPermission(permStr);
  }

  /**
   * Gets all permissions for the user
   *
   * @param buffer    Buffer to store the permissions as string
   * @param maxlength Maximum length of the buffer
   */
  public native void GetPermissions(char[] buffer, int maxlength);

  /**
   * Checks if the user has a specific permission in a channel
   *
   * @param channel_id  Channel ID to check permissions in
   * @param permission  Permission to check as string
   * @return            True if user has permission in channel, false otherwise
   */
  public native bool HasPermissionInChannel(const char[] channel_id, const char[] permission);

  /**
   * Checks if the user has a specific permission in a channel (enum version)
   *
   * @param channel_id  Channel ID to check permissions in
   * @param permission  Permission to check from DiscordPermission enum
   * @return            True if user has permission in channel, false otherwise
   */
  public bool HasPermissionInChannelEnum(const char[] channel_id, DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    return this.HasPermissionInChannel(channel_id, permStr);
  }

  /**
   * Gets all permissions for the user in a specific channel
   *
   * @param channel_id Channel ID to check permissions in
   * @param buffer     Buffer to store the permissions as string
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetPermissionsInChannel(const char[] channel_id, char[] buffer, int maxlength);

  /**
   * Gets all roles of the user
   *
   * @param roles     Array to store role IDs
   * @param maxSize   Maximum number of roles to retrieve
   * @return          Number of roles retrieved
   */
  public native int GetRoles(char[][] roles, int maxSize);

  /**
   * Checks if the user has a specific role
   *
   * @param roleId    Role ID to check
   * @return          True if user has role, false otherwise
   */
  public native bool HasRole(const char[] roleId);

  /**
   * Checks if the user has any of the specified roles
   *
   * @param roleIds   Array of role IDs to check
   * @param maxSize   Maximum number of roles to check
   * @return          True if user has any of the roles, false otherwise
   */
  public native bool HasAnyRole(const char[][] roleIds, int maxSize);

  /**
   * Checks if the user has all of the specified roles
   *
   * @param roleIds   Array of role IDs to check
   * @param maxSize   Maximum number of roles to check
   * @return          True if user has all of the roles, false otherwise
   */
  public native bool HasAllRoles(const char[][] roleIds, int maxSize);

  /**
   * Gets the highest role of the user
   *
   * @param buffer    Buffer to store the role ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetHighestRole(char[] buffer, int maxlength);

  /**
   * Gets the highest role of the user as a DiscordRole handle
   *
   * @return          DiscordRole handle of the highest role, or null if no roles
   * @note            Caller is responsible for closing the returned handle
   */
  public DiscordRole GetHighestRoleHandle()
  {
    if (!this.HasGuildMember) {
      return null;
    }

    char roleId[SNOWFLAKE_SIZE];
    this.GetHighestRole(roleId, sizeof(roleId));

    if (strcmp(roleId, "0") == 0) {
      return null;
    }

    Discord discord = this.GetClient();
    if (discord == null) {
      return null;
    }

    char guildId[SNOWFLAKE_SIZE];
    if (!this.GetGuildId(guildId, sizeof(guildId))) {
      return null;
    }

    return DiscordRole.FindRole(discord, guildId, roleId);
  }

  /**
   * Checks if the user has a specific role (using DiscordRole handle)
   *
   * @param role      DiscordRole handle to check
   * @return          True if user has the role, false otherwise
   */
  public bool HasRoleByHandle(DiscordRole role)
  {
    if (role == null) {
      return false;
    }

    char roleId[SNOWFLAKE_SIZE];
    role.GetId(roleId, sizeof(roleId));
    return this.HasRole(roleId);
  }

  /**
   * Gets the name of a specific role by role ID
   *
   * @param roleId    Role ID to get name for
   * @param buffer    Buffer to store the role name
   * @param maxlength Maximum length of the buffer
   * @return          True if role found, false otherwise
   */
  public native bool GetRoleName(const char[] roleId, char[] buffer, int maxlength);

  /**
   * Gets the names of all roles the user has
   *
   * @param roleNames Array to store role names
   * @param maxSize   Maximum number of role names to retrieve
   * @return          Number of role names retrieved
   */
  public native int GetRoleNames(char[][] roleNames, int maxSize);

  /**
   * Sets the nickname of the user in the guild
   *
   * @param nickname  New nickname to set (empty string to remove)
   */
  public native void SetNickName(const char[] nickname);

  /**
   * Kicks the user from the guild
   */
  public native void KickFromGuild();

  /**
   * Bans the user from the guild
   *
   * @param reason        Ban reason (optional)
   * @param delete_days   Days of messages to delete (0-7)
   */
  public native void BanFromGuild(const char[] reason = NULL_STRING, int delete_days = 0);

  /**
   * Unbans the user from the guild
   */
  public native void UnbanFromGuild();

  /**
   * Sets a timeout for the user
   *
   * @param timeout_until Unix timestamp when timeout expires
   */
  public native void SetTimeout(int timeout_until);

  /**
   * Removes timeout from the user
   */
  public native void RemoveTimeout();
}