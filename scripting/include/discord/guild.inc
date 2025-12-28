// Discord guild
methodmap DiscordGuild < Handle
{
  /**
   * Fetches guild information from Discord API asynchronously
   *
   * @param discord      Discord bot handle
   * @param guild_id     Guild ID to fetch
   * @param callback     Callback function to handle the response
   * @param data         Additional data to pass to callback
   * @note               This method first checks the cache for guild data. If found in cache,
   *                     the callback is called immediately. If not cached, an API request is made
   * @note               Result contains: "guild" (DiscordGuild handle)
   */
  public static native void FetchGuild(Discord discord, const char[] guild_id, DiscordResultCallback callback, any data = 0);

  /**
   * Finds a guild object from cache by ID
   *
   * @param discord     Discord client handle (some functions may require it)
   * @param guild_id    Guild ID to search for
   * @return            Handle to the DiscordGuild object if found, null if not found or cache unavailable
   *
   * @note              This performs a local lookup only - no API requests are made,
   *                    The cache is populated when the bot successfully connects to Discord
   *
   * @note              Cache behavior depends on bot state:
   *                    - If bot has never run: cache is empty (always returns null)
   *                    - If bot is currently online: cache is up-to-date
   *                    - If bot was online but disconnected: cache remains at last-known state
   *
   * @error             Invalid guild ID format
   */
  public static native DiscordGuild FindGuild(Discord discord, const char[] guild_id);

  /**
   * Gets the guild ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the guild name
   *
   * @param buffer       Buffer to store the name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Sets the guild name
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @param name         New guild name
   */
  public native void SetName(const char[] name);

  /**
   * Gets the guild description
   *
   * @param buffer       Buffer to store the description
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Sets the guild description
   *
   * @note               Must call Modify() after to sync changes to Discord
   *
   * @param description  New guild description
   */
  public native void SetDescription(const char[] description);

  /**
   * Gets the guild owner ID
   *
   * @param buffer       Buffer to store the owner ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetOwnerId(char[] buffer, int maxlength);

  /**
   * Gets the guild member count
   *
   * @return             Member count
   */
  property int MemberCount {
    public native get();
  }

  /**
   * Gets the guild verification level
   *
   * @return             Verification level (0-4)
   */
  property GuildVerificationLevel VerificationLevel {
    public native get();
  }

  /**
   * Gets the guild premium tier (boost level)
   *
   * @return             Premium tier (0-3)
   */
  property GuildPremiumTier PremiumTier {
    public native get();
  }

  /**
   * Checks if the guild is large (over 250 members)
   *
   * @return             True if large, false otherwise
   */
  property bool IsLarge {
    public native get();
  }

  /**
   * Checks if the guild is verified
   *
   * @return             True if verified, false otherwise
   */
  property bool IsVerified {
    public native get();
  }

  /**
   * Checks if the guild is partnered
   *
   * @return             True if partnered, false otherwise
   */
  property bool IsPartnered {
    public native get();
  }

  /**
   * Gets the guild icon URL
   *
   * @param buffer           Buffer to store the URL
   * @param maxlength        Maximum length of the buffer
   * @param size             Icon size (default: 0 for default size)
   * @param prefer_animated  Whether to prefer animated version if available
   */
  public native void GetIconUrl(char[] buffer, int maxlength, int size = 0, bool prefer_animated = true);

  /**
   * Gets the guild banner URL
   *
   * @param buffer           Buffer to store the URL
   * @param maxlength        Maximum length of the buffer
   * @param size             Banner size (default: 0 for default size)
   * @param prefer_animated  Whether to prefer animated version if available
   */
  public native void GetBannerUrl(char[] buffer, int maxlength, int size = 0, bool prefer_animated = true);

  /**
   * Gets the guild splash URL
   *
   * @param buffer           Buffer to store the URL
   * @param maxlength        Maximum length of the buffer
   * @param size             Splash size (default: 0 for default size)
   */
  public native void GetSplashUrl(char[] buffer, int maxlength, int size = 0);

  /**
   * Gets the guild discovery splash URL
   *
   * @param buffer           Buffer to store the URL
   * @param maxlength        Maximum length of the buffer
   * @param size             Splash size (default: 0 for default size)
   */
  public native void GetDiscoverySplashUrl(char[] buffer, int maxlength, int size = 0);

  /**
   * Gets the guild vanity URL code
   *
   * @param buffer           Buffer to store the vanity URL code
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetVanityUrlCode(char[] buffer, int maxlength);

  /**
   * Gets the maximum number of members for the guild
   */
  property int MaxMembers {
    public native get();
  }

  /**
   * Gets the maximum number of presences for the guild
   */
  property int MaxPresences {
    public native get();
  }

  /**
   * Gets the number of premium subscriptions (boosts)
   */
  property int PremiumSubscriptionCount {
    public native get();
  }

  /**
   * Gets the explicit content filter level
   */
  property GuildExplicitContentFilter ExplicitContentFilter {
    public native get();
  }

  /**
   * Gets the MFA level required for moderation actions
   */
  property GuildMfaLevel MfaLevel {
    public native get();
  }

  /**
   * Gets the NSFW level of the guild
   */
  property GuildNsfwLevel NsfwLevel {
    public native get();
  }

  /**
   * Gets the AFK timeout in seconds
   */
  property GuildAfkTimeout AfkTimeout {
    public native get();
  }

  /**
   * Gets the default message notification level
   */
  property GuildDefaultMessageNotifications DefaultMessageNotifications {
    public native get();
  }

  /**
   * Gets the shard ID this guild is on
   */
  property int ShardId {
    public native get();
  }

  /**
   * Gets the guild flags bitmask
   */
  property GuildFlags Flags {
    public native get();
  }

  /**
   * Gets the additional guild flags bitmask
   */
  property GuildFlagsExtra FlagsExtra {
    public native get();
  }

  /**
   * Gets the AFK channel ID
   *
   * @param buffer           Buffer to store the channel ID
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetAfkChannelId(char[] buffer, int maxlength);

  /**
   * Gets the system channel ID
   *
   * @param buffer           Buffer to store the channel ID
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetSystemChannelId(char[] buffer, int maxlength);

  /**
   * Gets the rules channel ID
   *
   * @param buffer           Buffer to store the channel ID
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetRulesChannelId(char[] buffer, int maxlength);

  /**
   * Gets the public updates channel ID
   *
   * @param buffer           Buffer to store the channel ID
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetPublicUpdatesChannelId(char[] buffer, int maxlength);

  /**
   * Gets the safety alerts channel ID
   *
   * @param buffer           Buffer to store the channel ID
   * @param maxlength        Maximum length of the buffer
   */
  public native void GetSafetyAlertsChannelId(char[] buffer, int maxlength);

  /**
   * Gets the number of roles in the guild
   */
  property int RoleCount {
    public native get();
  }

  /**
   * Gets the number of channels in the guild
   */
  property int ChannelCount {
    public native get();
  }

  /**
   * Gets the number of threads in the guild
   */
  property int ThreadCount {
    public native get();
  }

  /**
   * Gets the number of emojis in the guild
   */
  property int EmojiCount {
    public native get();
  }

  /**
   * Gets the number of members currently in voice channels
   */
  property int VoiceMemberCount {
    public native get();
  }

  /**
   * Gets base permissions for a user in this guild
   *
   * @param user_id      User ID to check permissions for
   * @param buffer       Buffer to store the permissions string
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetBasePermissions(const char[] user_id, char[] buffer, int maxlength);

  /**
   * Gets permissions for a user in a specific channel
   *
   * @param user_id      User ID to check permissions for
   * @param channel_id   Channel ID to check permissions in
   * @param buffer       Buffer to store the permissions string
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetPermissionsInChannel(const char[] user_id, const char[] channel_id, char[] buffer, int maxlength);

  /**
   * Checks if a user has a specific permission in the guild
   *
   * @param user_id      User ID to check
   * @param permission   Permission string to check
   * @return             True if user has permission, false otherwise
   */
  public native bool HasPermission(const char[] user_id, const char[] permission);

  /**
   * Checks if a user has a specific permission in a channel
   *
   * @param user_id      User ID to check
   * @param channel_id   Channel ID to check
   * @param permission   Permission string to check
   * @return             True if user has permission in channel, false otherwise
   */
  public native bool HasPermissionInChannel(const char[] user_id, const char[] channel_id, const char[] permission);

  /**
   * Checks if a user has a specific permission using enum
   *
   * @param user_id      User ID to check
   * @param permission   Permission from DiscordPermission enum
   * @return             True if user has permission, false otherwise
   */
  public bool HasPermissionEnum(const char[] user_id, DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    return this.HasPermission(user_id, permStr);
  }

  /**
   * Checks if a user has a specific permission in a channel using enum
   *
   * @param user_id      User ID to check
   * @param channel_id   Channel ID to check
   * @param permission   Permission from DiscordPermission enum
   * @return             True if user has permission in channel, false otherwise
   */
  public bool HasPermissionInChannelEnum(const char[] user_id, const char[] channel_id, DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    return this.HasPermissionInChannel(user_id, channel_id, permStr);
  }

  /**
   * Modifies the guild, syncing local changes to Discord
   */
  public native void Modify();

  /**
   * Gets all invites for this guild
   *
   * @param callback    Callback function to receive invites
   * @param data        User data to pass to callback
   * @note              Invites are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordInvite), "count" (int)
   */
  public native void GetInvites(DiscordResultCallback callback, any data = 0);

  /**
   * Gets all webhooks for this guild
   *
   * @param callback    Callback function to receive webhooks
   * @param data        User data to pass to callback
   * @note              Webhooks are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordWebhook), "count" (int)
   */
  public native void GetWebhooks(DiscordResultCallback callback, any data = 0);

  /**
   * Gets a guild member by user ID
   *
   * @param user_id     User ID of the member to fetch
   * @param callback    Callback function to receive the member
   * @param data        User data to pass to callback
   * @note              Member is returned asynchronously via callback
   * @note              Result contains: "member" (DiscordGuildMember handle)
   */
  public native void GetMember(const char[] user_id, DiscordResultCallback callback, any data = 0);

  /**
   * Gets multiple guild members
   *
   * @param callback    Callback function to receive the members
   * @param limit       Maximum number of members to return (1-1000, default 1000)
   * @param after       User ID to get members after (for pagination, empty for start)
   * @param data        User data to pass to callback
   * @note              Members are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordGuildMember), "count" (int)
   */
  public native void GetMembers(DiscordResultCallback callback, int limit = 1000, const char[] after = NULL_STRING, any data = 0);

  /**
   * Searches for guild members by username/nickname
   *
   * @param query       Search query (matches username or nickname)
   * @param callback    Callback function to receive the members
   * @param limit       Maximum number of members to return (1-1000, default 1)
   * @param data        User data to pass to callback
   * @note              Members are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordGuildMember), "count" (int)
   */
  public native void SearchMembers(const char[] query, DiscordResultCallback callback, int limit = 1, any data = 0);

  /**
   * Gets all active threads in this guild
   *
   * @param callback    Callback function to receive the threads
   * @param data        User data to pass to callback
   * @note              Threads are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordChannel), "count" (int)
   */
  public native void GetActiveThreads(DiscordResultCallback callback, any data = 0);

  /**
   * Gets the list of banned users in this guild
   *
   * @param callback    Callback function to receive the bans
   * @param before      Get bans before this user ID (optional, use "" for none)
   * @param after       Get bans after this user ID (optional, use "" for none)
   * @param limit       Maximum number of bans to return (1-1000, default 1000)
   * @param data        Optional data to pass to callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordUser), "count" (int)
   */
  public native void GetBans(DiscordResultCallback callback, const char[] before = NULL_STRING, const char[] after = NULL_STRING, int limit = 1000, any data = 0);

  /**
   * Creates a custom emoji in this guild
   *
   * @param name        Name of the emoji
   * @param image_path  Path to the image file
   * @param type        Image type (ImageType_PNG, ImageType_GIF, etc.)
   */
  public native void CreateEmoji(const char[] name, const char[] image_path, ImageType type = ImageType_PNG);

  /**
   * Creates a sticker in this guild
   *
   * @param name        Name of the sticker
   * @param description Description of the sticker
   * @param tags        Autocomplete/suggestion tags for the sticker (comma-separated)
   * @param file_path   Path to the sticker file
   * @param format      Sticker format type
   */
  public native void CreateSticker(const char[] name, const char[] description, const char[] tags, const char[] file_path, StickerFormat format = StickerFormat_PNG);

  /**
   * Creates a scheduled event in this guild
   *
   * @param name        Name of the event
   * @param description Description of the event
   * @param channel_id  Channel ID where the event will be hosted
   * @param start_time  Unix timestamp for event start
   * @param end_time    Unix timestamp for event end (0 for no end time)
   * @param type        Event entity type (voice, stage, etc.)
   */
  public native void CreateScheduledEvent(const char[] name, const char[] description, const char[] channel_id, int start_time, int end_time = 0, EventEntityType type = EventEntityType_Voice);

  /**
   * Creates an external scheduled event in this guild
   *
   * @param name        Name of the event
   * @param description Description of the event
   * @param location    Location string for the external event
   * @param start_time  Unix timestamp for event start
   * @param end_time    Unix timestamp for event end
   */
  public native void CreateExternalScheduledEvent(const char[] name, const char[] description, const char[] location, int start_time, int end_time);

  /**
   * Unbans a user from this guild
   *
   * @param user_id     User ID to unban
   */
  public native void UnbanUser(const char[] user_id);

  /**
   * Creates a new role in this guild
   *
   * @param name          Role name
   * @param color         Role color (0xRRGGBB format)
   * @param hoist         Whether the role should be displayed separately
   * @param mentionable   Whether the role should be mentionable
   * @param permissions   Role permissions as string (uint64)
   * @param callback      Callback function to receive the created role
   * @param data          User data to pass to callback
   * @note                Result contains: "role" (DiscordRole handle)
   */
  public native void CreateRole(const char[] name, int color = 0, bool hoist = false, bool mentionable = false, const char[] permissions = "0", DiscordResultCallback callback, any data = 0);

  /**
   * Creates a new role in this guild using a DiscordRole object
   *
   * @param role          DiscordRole object with configured settings
   * @param callback      Callback function to receive the created role
   * @param data          User data to pass to callback
   * @note                Use a DiscordRole object to configure all role properties before creating
   * @note                Result contains: "role" (DiscordRole handle)
   */
  public native void CreateRoleFromObject(DiscordRole role, DiscordResultCallback callback, any data = 0);

  /**
   * Bulk deletes all slash commands in this guild
   *
   * @note                This will remove all guild-specific slash commands
   */
  public native void BulkDeleteCommands();
}