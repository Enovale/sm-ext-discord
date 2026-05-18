// Discord interaction
methodmap DiscordInteraction < Handle
{
  /**
   * Gets the name of the slash command
   *
   * @param buffer    Buffer to store the command name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetCommandName(char[] buffer, int maxlength);

  /**
   * Gets the guild (server) ID where the command was used
   *
   * @param buffer    Buffer to store the guild ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the channel ID where the command was used
   *
   * @param buffer    Buffer to store the channel ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetChannelId(char[] buffer, int maxlength);

  /**
   * Gets the user who used the command
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordUser User {
    public native get();
  }

  /**
   * Gets the nickname of the user who used the command relative to the guild it was used in
   *
   * @param buffer    Buffer to store the nickname
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserNickName(char[] buffer, int maxlength);

  /**
   * Gets the ID of the user who used the command
   *
   * @param buffer    Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the username of the user who used the command
   *
   * @param buffer    Buffer to store the username
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserName(char[] buffer, int maxlength);

  /**
   * Gets a string option value from the command
   *
   * @param name      Name of the option
   * @param buffer    Buffer to store the option value
   * @param maxlength    Maximum length of the buffer
   * @return          True if option exists and is string type, false otherwise
   */
  public native bool GetOptionValue(const char[] name, char[] buffer, int maxlength);

  /**
   * Gets a integer option value from the command
   *
   * @param name      Name of the option
   * @return          Integer value of the option, 0 if option doesn't exist
   * @note            WARNING: Discord integers are 64-bit. Values exceeding int32 range will cause a native error.
   *                  Use GetOptionValueInt64() for full 64-bit integers.
   */
  public native int GetOptionValueInt(const char[] name);

  /**
   * Gets a 64-bit integer option value from the command as a string
   *
   * @param name      Name of the option
   * @param buffer    Buffer to store the integer as string
   * @param maxlength Maximum length of the buffer
   * @return          True if option exists, false otherwise
   */
  public native bool GetOptionValueInt64(const char[] name, char[] buffer, int maxlength);

  /**
   * Gets a float option value from the command
   *
   * @param name      Name of the option
   * @return          Float value of the option, 0.0 if option doesn't exist
   */
  public native float GetOptionValueFloat(const char[] name);

  /**
   * Gets a boolean option value from the command
   *
   * @param name      Name of the option
   * @return          Boolean value of the option, false if option doesn't exist
   */
  public native bool GetOptionValueBool(const char[] name);

  /**
   * Creates an immediate response to the interaction
   *
   * @param content   Content of the response
   */
  public native void CreateResponse(const char[] content);

  /**
   * Creates an immediate response with embed to the interaction
   *
   * @param content   Content of the response
   * @param embed     Embed object to include in the response
   */
  public native void CreateResponseEmbed(const char[] content, DiscordEmbed embed);

  /**
   * Defers the response, showing a "thinking" state
   *
   * @param ephemeral Whether the response should be ephemeral (only visible to the command user)
   */
  public native void DeferReply(bool ephemeral = false);

  /**
   * Edits a previously sent response
   *
   * @param content   New content for the response
   */
  public native void EditResponse(const char[] content);

  /**
   * Creates an ephemeral response (only visible to the command user)
   *
   * @param content   Content of the response
   */
  public native void CreateEphemeralResponse(const char[] content);

  /**
   * Creates an ephemeral response with embed (only visible to the command user)
   *
   * @param content   Content of the response
   * @param embed     Embed object to include in the response
   */
  public native void CreateEphemeralResponseEmbed(const char[] content, DiscordEmbed embed);

  /**
   * Sends a follow-up message to the interaction
   *
   * @param content   Content of the follow-up message
   * @note            Can only be used after DeferReply or CreateResponse
   */
  public native void FollowUp(const char[] content);

  /**
   * Sends a follow-up message with embed to the interaction
   *
   * @param content   Content of the follow-up message (can be empty)
   * @param embed     Embed object to include
   * @note            Can only be used after DeferReply or CreateResponse
   */
  public native void FollowUpEmbed(const char[] content, DiscordEmbed embed);

  /**
   * Sends an ephemeral follow-up message to the interaction
   *
   * @param content   Content of the follow-up message
   * @note            Can only be used after DeferReply or CreateResponse
   */
  public native void FollowUpEphemeral(const char[] content);

  /**
   * Edits a follow-up message
   *
   * @param message_id  ID of the follow-up message to edit
   * @param content     New content for the message
   */
  public native void EditFollowUp(const char[] message_id, const char[] content);

  /**
   * Edits a follow-up message with embed
   *
   * @param message_id  ID of the follow-up message to edit
   * @param content     New content for the message
   * @param embed       Embed object to include (can be null)
   */
  public native void EditFollowUpEmbed(const char[] message_id, const char[] content, DiscordEmbed embed);

  /**
   * Creates an immediate response using a message object
   *
   * @param message     DiscordMessage object containing the response
   * @note              Use this method when you need to include components, multiple embeds, or other advanced features
   */
  public native void CreateResponseFromObject(DiscordMessage message);

  /**
   * Edits a previously sent response using a message object
   *
   * @param message     DiscordMessage object containing the new response
   * @note              Use this method when you need to include components, multiple embeds, or other advanced features
   */
  public native void EditResponseFromObject(DiscordMessage message);

  /**
   * Sends a follow-up message using a message object
   *
   * @param message     DiscordMessage object containing the follow-up
   * @note              Can only be used after DeferReply or CreateResponse
   * @note              Use this method when you need to include components, multiple embeds, or other advanced features
   */
  public native void FollowUpFromObject(DiscordMessage message);

  /**
   * Edits a follow-up message using a message object
   *
   * @param message_id  ID of the follow-up message to edit
   * @param message     DiscordMessage object containing the new content
   * @note              Use this method when you need to include components, multiple embeds, or other advanced features
   */
  public native void EditFollowUpFromObject(const char[] message_id, DiscordMessage message);
}
