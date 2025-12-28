// Discord forum tag
methodmap DiscordForumTag < Handle
{
  /**
   * Creates a new Discord Forum Tag object
   *
   * @param name         Tag name (max 20 characters)
   * @param emoji        Tag emoji (custom emoji ID or unicode character, optional)
   * @param moderated    Whether tag requires MANAGE_THREADS permission to add/remove
   * @return             New DiscordForumTag handle, or null on failure
   */
  public native DiscordForumTag(const char[] name, const char[] emoji = NULL_STRING, bool moderated = false);

  /**
   * Gets the ID of the forum tag
   *
   * @param buffer      Buffer to store the tag ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the name of the forum tag
   *
   * @param buffer      Buffer to store the tag name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Sets the name of the forum tag
   *
   * @param name        New tag name (max 20 characters)
   */
  public native void SetName(const char[] name);

  /**
   * Gets the emoji of the forum tag
   *
   * @param buffer      Buffer to store the emoji (ID for custom, unicode for standard)
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetEmoji(char[] buffer, int maxlength);

  /**
   * Sets the emoji of the forum tag
   *
   * @param emoji       Tag emoji (custom emoji ID or unicode character)
   */
  public native void SetEmoji(const char[] emoji);

  /**
   * Checks if the forum tag is moderated
   */
  property bool IsModerated {
    public native get();
    public native set(bool moderated);
  }

  /**
   * Checks if the tag emoji is a custom emoji (has ID) vs unicode
   */
  property bool EmojiIsCustom {
    public native get();
  }

  /**
   * Applies this tag to a channel (used when creating/editing forum channels)
   *
   * @param channel     Forum channel to apply the tag to
   */
  public native void ApplyToChannel(DiscordChannel channel);
}