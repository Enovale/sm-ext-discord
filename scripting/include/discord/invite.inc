// Discord invite
methodmap DiscordInvite < Handle
{
  /**
   * Creates a new DiscordInvite object for configuring invite settings
   *
   * Use setters like MaxAge, MaxUses, etc. to configure the invite,
   * then pass it to DiscordChannel.CreateInviteFromObject() to create the actual invite.
   *
   * @param discord     Discord client handle
   * @return            DiscordInvite handle
   */
  public native DiscordInvite(Discord discord);

  /**
   * Gets the invite code
   *
   * @param buffer      Buffer to store the code
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetCode(char[] buffer, int maxlength);

  /**
   * Gets the guild ID this invite is for
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the channel ID this invite is for
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetChannelId(char[] buffer, int maxlength);

  /**
   * Gets the ID of the user who created this invite
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetInviterId(char[] buffer, int maxlength);

  /**
   * Gets the username of the user who created this invite
   *
   * @param buffer      Buffer to store the username
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetInviterUserName(char[] buffer, int maxlength);

  /**
   * Gets the avatar URL of the user who created this invite
   *
   * @param buffer      Buffer to store the URL
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetInviterAvatarUrl(char[] buffer, int maxlength);

  /**
   * Whether the inviter is a bot
   */
  property bool InviterIsBot {
    public native get();
  }

  /**
   * Gets the name of the guild this invite is for
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGuildName(char[] buffer, int maxlength);

  /**
   * Gets the name of the channel this invite is for
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetChannelName(char[] buffer, int maxlength);

  /**
   * Gets the channel type
   */
  property int ChannelType {
    public native get();
  }

  /**
   * Approximate number of online users
   */
  property int ApproximatePresenceCount {
    public native get();
  }

  /**
   * Approximate number of total users
   */
  property int ApproximateMemberCount {
    public native get();
  }

  /**
   * Number of times this invite has been used
   */
  property int Uses {
    public native get();
  }

  /**
   * Maximum number of uses (0 = unlimited, max 100)
   */
  property int MaxUses {
    public native get();
    public native set(int max_uses);
  }

  /**
   * Duration in seconds after which the invite expires (0 = never, max 604800)
   */
  property int MaxAge {
    public native get();
    public native set(int max_age);
  }

  /**
   * Target type for this voice channel invite (0 = none, 1 = stream, 2 = embedded application)
   */
  property int TargetType {
    public native get();
    public native set(int type);
  }

  /**
   * Whether this invite only grants temporary membership
   */
  property bool IsTemporary {
    public native get();
    public native set(bool temporary);
  }

  /**
   * Whether this invite is unique
   */
  property bool IsUnique {
    public native get();
    public native set(bool unique);
  }

  /**
   * Gets the timestamp at which the invite was created
   *
   * @param buffer      Buffer to store the Unix timestamp string
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetCreatedAt(char[] buffer, int maxlength);

  /**
   * Gets the timestamp at which the invite expires (0 if it doesn't expire)
   *
   * @param buffer      Buffer to store the Unix timestamp string
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetExpiresAt(char[] buffer, int maxlength);

  /**
   * Sets the target user ID for this voice channel stream invite
   *
   * @param user_id     User ID
   */
  public native void SetTargetUserId(const char[] user_id);

  /**
   * Creates the invite for the specified channel
   *
   * @param channel_id  Channel ID to create the invite for
   */
  public native void Create(const char[] channel_id);

  /**
   * Deletes this invite
   */
  public native void Delete();

  /**
   * Gets an invite by its code (static method)
   *
   * @param discord     Discord client instance
   * @param code        Invite code (e.g., "abc123")
   * @param callback    Callback function to receive the invite
   * @param data        Optional data to pass to callback
   * @note              Result contains: "invite" (DiscordInvite handle)
   */
  public static native void Get(Discord discord, const char[] code, DiscordResultCallback callback, any data = 0);
}
