// Discord channel
methodmap DiscordChannel < Handle
{
  /**
   * Creates a new Discord Channel handle from channel ID
   *
   * @param discord     Discord client handle to bind to this channel
   * @param channel_id  Channel ID
   * @return            New Discord channel handle, or null on failure
   * @note              This creates a minimal channel object. Use FetchChannel for complete info
   */
  public native DiscordChannel(Discord discord, const char[] channel_id);

  /**
   * Fetches full channel information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param channel_id  Channel ID as string
   * @param callback    Function to call when channel info is received
   * @param data        Optional data to pass to callback
   * @note              Available information: name, type, topic, permissions, parent category, etc.
   * @note              This method first checks the cache for channel data. If found in cache,
   *                    the callback is called immediately. If not cached, an API request is made
   * @note              Result contains: "channel" (DiscordChannel handle)
   */
  public static native void FetchChannel(Discord discord, const char[] channel_id, DiscordResultCallback callback, any data = 0);

  /**
   * Finds a channel object from the Discord client's memory cache by ID
   *
   * @param discord     Discord client handle (some functions may require it)
   * @param channel_id  Channel ID to search for
   * @return            Handle to the DiscordChannel object if found, null if not found or cache unavailable
   *
   * @note              This performs a local lookup only - no API requests are made,
   *                    The cache is populated when the bot successfully connects to Discord
   *
   * @note              Cache behavior depends on bot state:
   *                    - If bot has never run: cache is empty (always returns null)
   *                    - If bot is currently online: cache is up-to-date
   *                    - If bot was online but disconnected: cache remains at last-known state
   *
   * @error             Invalid channel ID format
   */
  public static native DiscordChannel FindChannel(Discord discord, const char[] channel_id);

  /**
   * Gets the name of the channel
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Gets the ID of the channel
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the guild (server) ID of the channel
   *
   * @param buffer      Buffer to store the guild ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the parent ID of the channel (category for channels, channel for threads)
   *
   * @param buffer      Buffer to store the parent ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetParentId(char[] buffer, int maxlength);

  /**
   * Gets the topic of the channel
   *
   * @param buffer      Buffer to store the topic
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetTopic(char[] buffer, int maxlength);

  /**
   * Gets the type of the channel
   *
   * @return           Channel type from DiscordChannelType enum
   */
  property DiscordChannelType Type {
    public native get();
  }

  /**
   * Gets or sets the position of the channel
   */
  property int Position {
    public native get();
    public native set(int position);
  }

  /**
   * Gets or sets whether the channel is NSFW
   */
  property bool IsNSFW {
    public native get();
    public native set(bool nsfw);
  }

  /**
   * Checks if the channel is a text channel
   *
   * @return           True if text channel, false otherwise
   */
  property bool IsTextChannel {
    public native get();
  }

  /**
   * Checks if the channel is a voice channel
   *
   * @return           True if voice channel, false otherwise
   */
  property bool IsVoiceChannel {
    public native get();
  }

  /**
   * Checks if the channel is a category
   *
   * @return           True if category, false otherwise
   */
  property bool IsCategory {
    public native get();
  }

  /**
   * Checks if the channel is a thread
   *
   * @return           True if thread, false otherwise
   */
  property bool IsThread {
    public native get();
  }

  /**
   * Checks if the channel is a forum
   *
   * @return           True if forum, false otherwise
   */
  property bool IsForum {
    public native get();
  }

  /**
   * Checks if the channel is a news/announcement channel
   *
   * @return           True if news channel, false otherwise
   */
  property bool IsNewsChannel {
    public native get();
  }

  /**
   * Checks if the channel is a stage channel
   *
   * @return           True if stage channel, false otherwise
   */
  property bool IsStageChannel {
    public native get();
  }

  /**
   * Gets or sets the bitrate of the voice channel (voice channels only)
   */
  property int Bitrate {
    public native get();
    public native set(int bitrate);
  }

  /**
   * Gets or sets the user limit of the voice channel (voice channels only)
   */
  property int UserLimit {
    public native get();
    public native set(int limit);
  }

  /**
   * Gets or sets the rate limit per user (slowmode) in seconds
   */
  property int RateLimitPerUser {
    public native get();
    public native set(int seconds);
  }

  /**
   * Gets the mention string for this channel
   *
   * @param buffer      Buffer to store the mention string
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetMention(char[] buffer, int maxlength);

  /**
   * Gets the URL for this channel
   *
   * @param buffer      Buffer to store the URL
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Sets the name of the channel
   *
   * @param name      New channel name
   */
  public native void SetName(const char[] name);

  /**
   * Gets the channel flags
   */
  property ChannelFlags Flags {
    public native get();
  }

  /**
   * Gets the owner ID (for group DMs or threads)
   *
   * @param buffer      Buffer to store the owner ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetOwnerId(char[] buffer, int maxlength);

  /**
   * Gets the last message ID
   *
   * @param buffer      Buffer to store the last message ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetLastMessageId(char[] buffer, int maxlength);

  /**
   * Gets the last pin timestamp
   */
  property int LastPinTimestamp {
    public native get();
  }

  /**
   * Gets the default thread rate limit per user
   */
  property int DefaultThreadRateLimitPerUser {
    public native get();
  }

  /**
   * Gets the default auto archive duration for threads
   */
  property AutoArchiveDuration DefaultAutoArchiveDuration {
    public native get();
  }

  /**
   * Gets the default forum sort order
   */
  property ForumSortOrder DefaultSortOrder {
    public native get();
  }

  /**
   * Gets the forum layout type
   */
  property ForumLayoutType ForumLayout {
    public native get();
  }

  /**
   * Gets the RTC region for voice channels
   */
  public native void GetRTCRegion(char[] buffer, int maxlength);

  /**
   * Checks if the channel is a DM
   *
   * @return           True if DM, false otherwise
   */
  property bool IsDM {
    public native get();
  }

  /**
   * Checks if the channel is a group DM
   *
   * @return           True if group DM, false otherwise
   */
  property bool IsGroupDM {
    public native get();
  }

  /**
   * Checks if the channel is a media channel
   *
   * @return           True if media channel, false otherwise
   */
  property bool IsMediaChannel {
    public native get();
  }

  /**
   * Checks if video quality is 720p
   *
   * @return           True if 720p, false otherwise
   */
  property bool IsVideo720p {
    public native get();
  }

  /**
   * Checks if video quality is auto
   *
   * @return           True if auto, false otherwise
   */
  property bool IsVideoAuto {
    public native get();
  }

  /**
   * Checks if thread is pinned in forum
   *
   * @return           True if pinned, false otherwise
   */
  property bool IsPinnedThread {
    public native get();
  }

  /**
   * Checks if tag is required for forum threads
   *
   * @return           True if required, false otherwise
   */
  property bool IsTagRequired {
    public native get();
  }

  /**
   * Checks if download options are hidden in media channel
   *
   * @return           True if hidden, false otherwise
   */
  property bool IsDownloadOptionsHidden {
    public native get();
  }

  /**
   * Checks if permissions are locked (inherit from category)
   *
   * @return           True if locked, false otherwise
   */
  property bool IsLockedPermissions {
    public native get();
  }

  /**
   * Sets the topic of the channel
   */
  public native void SetTopic(const char[] topic);

  /**
   * Sets the parent ID (category for channels)
   *
   * @param parent_id   Parent ID to set
   * @return            True on success, false on failure
   */
  public native bool SetParentId(const char[] parent_id);

  /**
   * Sets the RTC region for voice channels
   *
   * @param region      RTC region to set
   * @return            True on success, false on failure
   */
  public native bool SetRTCRegion(const char[] region);

  /**
   * Adds permission overwrite for a user or role
   *
   * @param target_id   Target ID to add permission overwrite for
   * @param type        Type of the target (user or role)
   * @param allowed     Allowed permissions as string
   * @param denied      Denied permissions as string
   */
  public native void AddPermissionOverwrite(const char[] target_id, OverwriteType type, const char[] allowed, const char[] denied);

  /**
   * Sets permission overwrite for a user or role (replaces existing)
   *
   * @param target_id   Target ID to set permission overwrite for
   * @param type        Type of the target (user or role)
   * @param allowed     Allowed permissions as string
   * @param denied      Denied permissions as string
   */
  public native void SetPermissionOverwrite(const char[] target_id, OverwriteType type, const char[] allowed, const char[] denied);

  /**
   * Removes permission overwrite for a user or role
   *
   * @param target_id   Target ID to remove permission overwrite for
   * @param type        Type of the target (user or role)
   */
  public native void RemovePermissionOverwrite(const char[] target_id, OverwriteType type);

  /**
   * Gets user permissions in this channel
   *
   * @param user_id   User ID to get permissions for
   * @param buffer    Buffer to store the permissions
   * @param maxlength Maximum length of the buffer
   * @note            if user not found, buffer will be set to "-1"
   */
  public native void GetUserPermissions(const char[] user_id, char[] buffer, int maxlength);

  /**
   * Deletes the channel
   *
   * @param callback      Optional callback to receive confirmation
   * @param data          Optional data to pass to callback
   */
  public native void Delete(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates an invite for the channel
   *
   * @param max_age     Max age of the invite in seconds (0-604800)
   * @param max_uses    Max uses of the invite (0-100)
   * @param temporary   Whether the invite is temporary
   * @param unique      Whether the invite is unique
   * @param callback    Optional callback to receive the created invite
   * @param data        Optional data to pass to callback
   */
  public native void CreateInvite(int max_age = 86400, int max_uses = 0, bool temporary = false, bool unique = false, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates an invite for this channel using a DiscordInvite object
   *
   * @param invite      DiscordInvite object with configured settings
   * @param callback    Optional callback to receive the created invite
   * @param data        Optional data to pass to callback
   * @note              Use DiscordInvite setters to configure max_age, max_uses, etc.
   */
  public native void CreateInviteFromObject(DiscordInvite invite, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets all invites for this channel
   *
   * @param callback    Callback function to receive invites
   * @param data        User data to pass to callback
   * @note              Invites are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordInvite), "count" (int)
   */
  public native void GetInvites(DiscordResultCallback callback, any data = 0);

  /**
   * Gets all webhooks for this channel
   *
   * @param callback    Callback function to receive webhooks
   * @param data        User data to pass to callback
   * @note              Webhooks are returned asynchronously via callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordWebhook), "count" (int)
   */
  public native void GetWebhooks(DiscordResultCallback callback, any data = 0);

  /**
   * Sends a message to the channel
   *
   * @param content   Message content to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void SendMessage(const char[] content, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sends a message with embed to the channel
   *
   * @param content   Message content to send
   * @param embed     Embed object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void SendMessageEmbed(const char[] content, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sends a DiscordMessage object to this channel
   *
   * @param message   DiscordMessage object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            This will set the channel ID in the message object to this channel
   */
  public native void SendMessageFromObject(DiscordMessage message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets the icon URL for group DM channels
   */
  public native void GetIconUrl(char[] buffer, int maxlength, int size = 0);

  /**
   * Gets the number of permission overwrites
   */
  property int PermissionOverwriteCount {
    public native get();
  }

  /**
   * Gets permission overwrite target ID by index
   *
   * @param index     Index of the permission overwrite
   * @param buffer    Buffer to store the target ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetPermissionOverwriteTargetId(int index, char[] buffer, int maxlength);

  /**
   * Gets permission overwrite type by index
   *
   * @param index     Index of the permission overwrite
   * @return          Type of the permission overwrite
   */
  public native OverwriteType GetPermissionOverwriteType(int index);

  /**
   * Gets the number of available tags (forum channels)
   */
  property int AvailableTagCount {
    public native get();
  }

  /**
   * Gets available tag name by index
   *
   * @param index     Index of the available tag
   * @param buffer    Buffer to store the tag name
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAvailableTagName(int index, char[] buffer, int maxlength);

  /**
   * Gets available tag ID by index
   *
   * @param index     Index of the available tag
   * @param buffer    Buffer to store the tag ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAvailableTagId(int index, char[] buffer, int maxlength);

  /**
   * Gets available tag emoji by index (returns emoji ID for custom emojis, unicode for standard emojis)
   *
   * @param index     Index of the available tag
   * @param buffer    Buffer to store the tag emoji
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAvailableTagEmoji(int index, char[] buffer, int maxlength);

  /**
   * Checks if available tag is moderated by index (requires MANAGE_THREADS permission to add/remove)
   *
   * @param index     Index of the available tag
   * @return          True if the tag is moderated, false otherwise
   */
  public native bool GetAvailableTagModerated(int index);

  /**
   * Checks if available tag emoji is custom (has ID) vs unicode
   *
   * @param index     Index of the available tag
   * @return          True if the tag emoji is custom, false otherwise
   */
  public native bool GetAvailableTagEmojiIsCustom(int index);

  /**
   * Creates a new forum tag
   *
   * @param name         Tag name (max 20 characters)
   * @param emoji        Tag emoji (custom emoji ID or unicode character, optional)
   * @param moderated    Whether tag requires MANAGE_THREADS permission to add/remove
   */
  public native void CreateForumTag(const char[] name, const char[] emoji = NULL_STRING, bool moderated = false);

  /**
   * Edits an existing forum tag
   *
   * @param tag_id       Tag ID to edit
   * @param name         New tag name (max 20 characters)
   * @param emoji        New tag emoji (custom emoji ID or unicode character, optional)
   * @param moderated    Whether tag requires MANAGE_THREADS permission to add/remove
   */
  public native void EditForumTag(const char[] tag_id, const char[] name, const char[] emoji = NULL_STRING, bool moderated = false);

  /**
   * Deletes a forum tag
   *
   * @param tag_id       Tag ID to delete
   */
  public native void DeleteForumTag(const char[] tag_id);

  /**
   * Creates a new thread in this forum channel
   *
   * @param name              Thread name
   * @param message           Initial message content
   * @param tag_ids           Array of tag IDs to apply to the thread
   * @param tag_count         Number of tags in the array
   * @param auto_archive      Auto archive duration (AutoArchive_1Hour, AutoArchive_1Day, AutoArchive_3Days, AutoArchive_1Week)
   * @param rate_limit        Rate limit per user in seconds (0-21600)
   * @param callback          Optional callback to receive the created thread
   * @param data              Optional data to pass to callback
   */
  public native void CreateForumThread(const char[] name, const char[] message, const char[][] tag_ids = {}, int tag_count = 0, AutoArchiveDuration auto_archive = AutoArchive_1Day, int rate_limit = 0, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a new thread in this forum channel with embed
   *
   * @param name              Thread name
   * @param message           Initial message content
   * @param embed             Embed to include in initial message
   * @param tag_ids           Array of tag IDs to apply to the thread
   * @param tag_count         Number of tags in the array
   * @param auto_archive      Auto archive duration (AutoArchive_1Hour, AutoArchive_1Day, AutoArchive_3Days, AutoArchive_1Week)
   * @param rate_limit        Rate limit per user in seconds (0-21600)
   * @param callback          Optional callback to receive the created thread
   * @param data              Optional data to pass to callback
   */
  public native void CreateForumThreadEmbed(const char[] name, const char[] message, DiscordEmbed embed, const char[][] tag_ids = {}, int tag_count = 0, AutoArchiveDuration auto_archive = AutoArchive_1Day, int rate_limit = 0, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sets the parent category of the channel
   *
   * @param parent_id Parent category ID
   */
  public native void SetParent(const char[] parent_id);

  /**
   * Creates a new thread in this channel (not forum)
   *
   * @param name              Thread name
   * @param type              Thread type (11 = public, 12 = private)
   * @param auto_archive      Auto archive duration (AutoArchive_1Hour, AutoArchive_1Day, AutoArchive_3Days, AutoArchive_1Week)
   * @param invitable         Whether non-moderators can add other non-moderators to the thread (private threads only)
   * @param rate_limit        Rate limit per user in seconds (0-21600)
   * @param callback          Optional callback to receive the created thread
   * @param data              Optional data to pass to callback
   */
  public native void CreateThread(const char[] name, DiscordChannelType type = ChannelType_PublicThread, AutoArchiveDuration auto_archive = AutoArchive_1Day, bool invitable = true, int rate_limit = 0, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Adds a user to this thread
   *
   * @param user_id           User ID to add
   * @param callback          Optional callback to receive confirmation
   * @param data              Optional data to pass to callback
   */
  public native void ThreadMemberAdd(const char[] user_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Removes a user from this thread
   *
   * @param user_id           User ID to remove
   * @param callback          Optional callback to receive confirmation
   * @param data              Optional data to pass to callback
   */
  public native void ThreadMemberRemove(const char[] user_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sets the archived state of this thread
   *
   * @param archived          True to archive, false to unarchive
   * @param callback          Optional callback to receive confirmation
   * @param data              Optional data to pass to callback
   */
  public native void SetArchived(bool archived, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sets the locked state of this thread (prevents non-moderators from unarchiving)
   *
   * @param locked            True to lock, false to unlock
   * @param callback          Optional callback to receive confirmation
   * @param data              Optional data to pass to callback
   */
  public native void SetLocked(bool locked, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets archived threads from this channel
   *
   * @param is_private        True to get private archived threads, false for public
   * @param before            Get threads archived before this timestamp (0 for no filter)
   * @param limit             Maximum number of threads to return (0 for default)
   * @param callback          Callback function to receive the threads
   * @param data              Optional data to pass to callback
   * @note                    Result contains: "items" (DiscordHandleArray of DiscordChannel), "count" (int)
   */
  public native void GetArchivedThreads(bool is_private, int before = 0, int limit = 0, DiscordResultCallback callback, any data = 0);

  /**
   * Gets all members of this thread
   *
   * @param callback          Callback function to receive the thread members
   * @param data              Optional data to pass to callback
   * @note                    Result contains: "items" (DiscordHandleArray of DiscordThreadMember), "count" (int)
   */
  public native void GetThreadMembers(DiscordResultCallback callback, any data = 0);
}