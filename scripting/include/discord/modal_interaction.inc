// Discord modal interaction
methodmap DiscordModalInteraction < Handle
{
  /**
   * Gets the custom ID of the modal
   *
   * @param buffer      Buffer to store the custom ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetCustomId(char[] buffer, int maxlength);

  /**
   * Gets a text input value by its custom ID
   *
   * @param custom_id   Custom ID of the text input component
   * @param buffer      Buffer to store the value
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetValue(const char[] custom_id, char[] buffer, int maxlength);

  /**
   * Gets the user who submitted the modal
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordUser User {
    public native get();
  }

  /**
   * Gets the channel where the modal was submitted
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordChannel Channel {
    public native get();
  }

  /**
   * Replies to the modal submission
   *
   * @param content     Reply content
   * @param ephemeral   Whether the reply is only visible to the user
   */
  public native void Reply(const char[] content, bool ephemeral = false);

  /**
   * Replies to the modal submission with an embed
   *
   * @param content     Reply content
   * @param embed       Embed to include in the reply
   * @param ephemeral   Whether the reply is only visible to the user
   */
  public native void ReplyEmbed(const char[] content, DiscordEmbed embed, bool ephemeral = false);

  /**
   * Replies to the modal submission with a complete message
   *
   * @param message     DiscordMessage object to send
   * @param ephemeral   Whether the reply is only visible to the user
   */
  public native void ReplyFromObject(DiscordMessage message, bool ephemeral = false);

  /**
   * Defers the reply (shows "thinking" state)
   *
   * @param ephemeral   Whether the deferred reply is ephemeral
   */
  public native void DeferReply(bool ephemeral = false);
}
