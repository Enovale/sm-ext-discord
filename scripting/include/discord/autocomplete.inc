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
  public native void GetUserNickName(char[] buffer, int maxlength);

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
   * @return          Integer value of the option, 0 if option doesn't exist or type mismatch
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
   * @return          True if option exists and is integer type, false otherwise
   */
  public native bool GetOptionValueInt64(const char[] name, char[] buffer, int maxlength);

  /**
   * Gets a float option value from the command
   *
   * @param name      Name of the option
   * @return          Float value of the option, 0.0 if option doesn't exist or type mismatch
   */
  public native float GetOptionValueFloat(const char[] name);

  /**
   * Gets a boolean option value from the command
   *
   * @param name      Name of the option
   * @return          Boolean value of the option, false if option doesn't exist or type mismatch
   */
  public native bool GetOptionValueBool(const char[] name);

  /**
   * Adds a string-type choice to the autocomplete results that will be included in the response
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param value   The actual value that will be used in the option when it is sent
   */
  public native void AddAutocompleteChoiceString(const char[] name, const char[] value);

  /**
   * Adds an integer-type choice to the autocomplete results
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param value   The integer value that will be used in the option when it is sent
   *                Use AddAutocompleteChoiceInt64() when the value exceeds int32 range.
   */
  public native void AddAutocompleteChoiceInt(const char[] name, int value);

  /**
   * Adds an integer-type choice to the autocomplete results using a 64-bit value string
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param value   The integer value string that will be used in the option when it is sent
   */
  public native void AddAutocompleteChoiceInt64(const char[] name, const char[] value);

  /**
   * Adds a float-type choice to the autocomplete results
   *
   * @param name    The name that will appear to the user for this autocomplete result
   * @param value   The float value that will be used in the option when it is sent
   */
  public native void AddAutocompleteChoiceFloat(const char[] name, float value);

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
   * @return          True if option exists and is string type, false otherwise
   */
  public native bool GetFocusedOptionValue(char[] buffer, int maxlength);

  /**
   * Gets the current value of the focused option as integer
   * @note WARNING: Discord integers are 64-bit. Values exceeding int32 range will cause an error.
   *                Use GetFocusedOptionValueInt64() for full 64-bit integers.
   */
  property int FocusedOptionValueInt {
    public native get();
  }

  /**
   * Gets the current value of the focused option as 64-bit integer
   *
   * @param buffer    Buffer to store the integer as string
   * @param maxlength Maximum length of the buffer
   * @return          True if option exists and is integer type, false otherwise
   */
  public native bool GetFocusedOptionValueInt64(char[] buffer, int maxlength);

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
