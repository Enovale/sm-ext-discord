// Discord autocomplete interaction
methodmap DiscordAutocompleteInteraction < Handle
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
  public native void GetUserNickname(char[] buffer, int maxlength);

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
   * Gets an integer option value from the command
   *
   * @param name      Name of the option
   * @return          Integer value of the option, 0 if option doesn't exist
   * @note            WARNING: Discord integers are 64-bit. Values exceeding int32 range will be truncated.
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
   * Adds a choice to the autocomplete results that will be included in the response
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param type    The option type of the value to be passed
   * @param value   The actual value that will be used in the option when it is sent
   */
  public native void AddAutocompleteChoice(const char[] name, DiscordCommandOptionType type, any value);

  /**
   * Adds a string-type choice to the autocomplete results that will be included in the response
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param value   The actual value that will be used in the option when it is sent
   */
  public native void AddAutocompleteChoiceString(const char[] name, const char[] value);

  /**
   * Gets the name of the currently focused option (the one being typed)
   *
   * @param buffer    Buffer to store the option name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetFocusedOptionName(char[] buffer, int maxlength);

  /**
   * Gets the type of the currently focused option
   */
  property DiscordCommandOptionType FocusedOptionType {
    public native get();
  }

  /**
   * Gets the current value of the focused option as string
   *
   * @param buffer    Buffer to store the option value
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetFocusedOptionValue(char[] buffer, int maxlength);

  /**
   * Gets the current value of the focused option as integer
   */
  property int FocusedOptionValueInt {
    public native get();
  }

  /**
   * Gets the current value of the focused option as float
   */
  property float FocusedOptionValueFloat {
    public native get();
  }

  /**
   * Gets the current value of the focused option as boolean
   */
  property bool FocusedOptionValueBool {
    public native get();
  }

  /**
   * Creates and sends the autocomplete response
  */
  public native void CreateAutocompleteResponse();
}