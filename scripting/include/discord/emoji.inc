// Discord emoji
methodmap DiscordEmoji < Handle
{
  /**
   * Gets the emoji ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the emoji name
   *
   * @param buffer       Buffer to store the name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Gets the user ID who created this emoji
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the guild ID this emoji belongs to
   *
   * @param buffer       Buffer to store the guild ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Whether this emoji requires colons
   */
  property bool RequiresColons {
    public native get();
  }

  /**
   * Whether this emoji is managed (by an integration)
   */
  property bool Managed {
    public native get();
  }

  /**
   * Whether this emoji is animated
   */
  property bool Animated {
    public native get();
  }

  /**
   * Whether this emoji is available
   */
  property bool Available {
    public native get();
  }

  /**
   * Gets the number of roles that can use this emoji
   */
  property int RoleCount {
    public native get();
  }

  /**
   * Gets a role ID at the specified index
   *
   * @param index        Index of the role
   * @param buffer       Buffer to store the role ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetRoleId(int index, char[] buffer, int maxlength);

  /**
   * Gets all role IDs as an array
   *
   * @return             DiscordHandleArray containing all role IDs
   * @note               Caller is responsible for closing the returned handle
   */
  public native DiscordHandleArray GetRoles();

  /**
   * Gets the emoji mention format
   *
   * @param buffer       Buffer to store the mention
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetMention(char[] buffer, int maxlength);

  /**
   * Gets the emoji URL
   *
   * @param buffer       Buffer to store the URL
   * @param maxlength    Maximum length of the buffer
   * @param size         Size of the image (optional, 0 for default)
   */
  public native void GetUrl(char[] buffer, int maxlength, int size = 0);

  /**
   * Gets the emoji format string (for reactions)
   *
   * @param buffer       Buffer to store the format
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetFormat(char[] buffer, int maxlength);

  /**
   * Deletes this emoji from the guild
   *
   * @param guild_id  Guild ID where the emoji is located
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Delete(const char[] guild_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits this emoji's name
   *
   * @param guild_id  Guild ID where the emoji is located
   * @param name      New name for the emoji
   * @param callback  Optional callback to receive the updated emoji
   * @param data      Optional data to pass to callback
   */
  public native void Edit(const char[] guild_id, const char[] name, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);
}
