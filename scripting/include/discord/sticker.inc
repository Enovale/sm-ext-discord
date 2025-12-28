// Discord sticker
methodmap DiscordSticker < Handle
{
  /**
   * Gets the sticker ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the sticker pack ID
   *
   * @param buffer       Buffer to store the pack ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetPackId(char[] buffer, int maxlength);

  /**
   * Gets the sticker name
   *
   * @param buffer       Buffer to store the name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Gets the sticker description
   *
   * @param buffer       Buffer to store the description
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Gets the sticker tags
   *
   * @param buffer       Buffer to store the tags
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetTags(char[] buffer, int maxlength);

  /**
   * Gets the guild ID this sticker belongs to
   *
   * @param buffer       Buffer to store the guild ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the sticker type
   */
  property int Type {
    public native get();
  }

  /**
   * Gets the sticker format type
   */
  property int FormatType {
    public native get();
  }

  /**
   * Whether this sticker is available
   */
  property bool Available {
    public native get();
  }

  /**
   * Gets the sticker sort value
   */
  property int SortValue {
    public native get();
  }

  /**
   * Gets the user ID who uploaded this sticker
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the username who uploaded this sticker
   *
   * @param buffer       Buffer to store the username
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUsername(char[] buffer, int maxlength);

  /**
   * Gets the sticker URL
   *
   * @param buffer       Buffer to store the URL
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Deletes this sticker from the guild
   */
  public native void Delete();

  /**
   * Edits this sticker
   *
   * @param name         New name for the sticker
   * @param description  New description for the sticker
   * @param tags         New tags for the sticker
   */
  public native void Edit(const char[] name, const char[] description, const char[] tags);
}
