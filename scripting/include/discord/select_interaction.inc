// Discord select interaction
methodmap DiscordSelectInteraction < Handle
{
  /**
   * Gets the custom ID of the select menu
   *
   * @param buffer      Buffer to store the custom ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetCustomId(char[] buffer, int maxlength);

  /**
   * Gets the component type
   */
  property ComponentType Type {
    public native get();
  }

  /**
   * Gets the number of selected values
   */
  property int ValueCount {
    public native get();
  }

  /**
   * Gets the user who used the select menu
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordUser User {
    public native get();
  }

  /**
   * Gets the channel where the interaction occurred
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordChannel Channel {
    public native get();
  }

  /**
   * Gets the message containing the select menu
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordMessage Message {
    public native get();
  }

  /**
   * Gets a selected value by index
   *
   * @param index       Value index (0 to ValueCount-1)
   * @param buffer      Buffer to store the value
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetValue(int index, char[] buffer, int maxlength);

  /**
   * Replies to the interaction
   *
   * @param content     Reply content
   * @param ephemeral   Whether the reply is only visible to the user
   */
  public native void Reply(const char[] content, bool ephemeral = false);

  /**
   * Replies to the interaction with an embed
   *
   * @param content     Reply content
   * @param embed       Embed to include in the reply
   * @param ephemeral   Whether the reply is only visible to the user
   */
  public native void ReplyEmbed(const char[] content, DiscordEmbed embed, bool ephemeral = false);

  /**
   * Replies to the interaction with a complete message
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

  /**
   * Updates the message containing the select menu
   *
   * @param content     New message content
   */
  public native void UpdateMessage(const char[] content);

  /**
   * Updates the message containing the select menu with an embed
   *
   * @param content     New message content
   * @param embed       Embed to include in the message
   */
  public native void UpdateMessageEmbed(const char[] content, DiscordEmbed embed);

  /**
   * Updates the message containing the select menu with a complete message
   *
   * @param message     DiscordMessage object to update with
   */
  public native void UpdateMessageFromObject(DiscordMessage message);

  /**
   * Defers the update (acknowledges without changes)
   */
  public native void DeferUpdate();

  /**
   * Shows a modal dialog to the user
   *
   * @param custom_id   Custom ID for the modal
   * @param title       Modal title
   * @param components  Array of DiscordComponent (ActionRow with TextInput)
   * @param count       Number of components
   * @note              Components must be ActionRows containing TextInput components
   */
  public native void ShowModal(const char[] custom_id, const char[] title, const DiscordComponent[] components, int count);

  /**
   * Shows a modal dialog to the user using a DiscordModal object (elegant method)
   *
   * @param modal       DiscordModal object containing modal configuration
   * @note              More elegant than ShowModal - no need to manually manage component arrays
   */
  public native void ShowModalFromObject(DiscordModal modal);
}