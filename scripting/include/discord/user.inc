// Discord user
methodmap DiscordUser < Handle
{
  /**
   * Creates a DiscordUser object from user ID
   *
   * @param discord     Discord client handle to bind to this user
   * @param user_id     User ID as string
   * @return            DiscordUser handle or null on failure
   * @note              This creates a minimal user object. Use FetchUser for complete info
   */
  public native DiscordUser(Discord discord, const char[] user_id);

  /**
   * Fetches full user information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param user_id     User ID as string
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
  public static native void FetchUser(Discord discord, const char[] user_id, DiscordResultCallback callback, any data = 0);

  /**
   * Finds a user object from cache by ID
   *
   * @param discord     Discord client handle (some functions may require it)
   * @param user_id     User ID to search for
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
   * @note              Caller is responsible for closing the returned handle
   * @error             Invalid user ID format
   */
  public static native DiscordUser FindUser(Discord discord, const char[] user_id);

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
   * @param buffer         Buffer to store the url
   * @param maxlength      Maximum length of the buffer
   * @param preferAnimated Whether to return the gif version of animated avatars if they are available or not
   */
  public native void GetAvatarUrl(char[] buffer, int maxlength, bool preferAnimated = true);

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
  public native void GetFormattedUserName(char[] buffer, int maxlength);

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
   * Checks if the user is a bot
   *
   * @return          True if user is bot, false otherwise
   */
  property bool IsBot {
    public native get();
  }
}
