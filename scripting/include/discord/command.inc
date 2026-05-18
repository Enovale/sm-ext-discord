// Discord slash command
methodmap DiscordSlashCommand < Handle
{
  /**
   * Creates a new Discord slash command
   *
   * @param discord     Discord client handle to bind to this command
   */
  public native DiscordSlashCommand(Discord discord);

  /**
   * Creates a DiscordSlashCommand from an existing global command ID
   *
   * @param discord     Discord client handle to bind to this command
   * @param commandId   Existing command ID to load
   */
  public static native DiscordSlashCommand FromGlobalCommand(Discord discord, const char[] commandId);

  /**
   * Creates a DiscordSlashCommand from an existing guild command ID
   *
   * @param discord     Discord client handle to bind to this command
   * @param commandId   Existing command ID to load
   * @param guild_id    Guild ID where the command exists
   */
  public static native DiscordSlashCommand FromGuildCommand(Discord discord, const char[] commandId, const char[] guild_id);

  /**
   * Sets the name of the slash command
   *
   * @param name      Command name
   */
  public native void SetName(const char[] name);

  /**
   * Sets the description of the slash command
   *
   * @param description Command description
   */
  public native void SetDescription(const char[] description);

  /**
   * Sets the default permissions for the slash command
   *
   * @param permissions Permission bitmask as string
   */
  public native void SetDefaultPermissions(const char[] permissions);

  /**
   * Sets the default permissions for the slash command (enum version)
   *
   * @param permission Permission from DiscordPermission enum
   */
  public void SetDefaultPermissionsEnum(DiscordPermission permission)
  {
    char permStr[SNOWFLAKE_SIZE];
    DiscordPermissionToString(permission, permStr, sizeof(permStr));
    this.SetDefaultPermissions(permStr);
  }

  /**
   * Sets the default permissions for the slash command (multiple permissions)
   *
   * @param permissions Array of permissions from DiscordPermission enum
   * @param numPermissions Number of permissions in the array
   */
  public void SetDefaultPermissionsMultiple(const DiscordPermission[] permissions, int numPermissions)
  {
    char permStr[SNOWFLAKE_SIZE*4];
    DiscordCombinePermissions(permissions, numPermissions, permStr, sizeof(permStr));
    this.SetDefaultPermissions(permStr);
  }

  /**
   * Gets the name of the slash command
   *
   * @param buffer      Buffer to store the name
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Gets the description of the slash command
   *
   * @param buffer      Buffer to store the description
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Gets the default permissions of the slash command
   *
   * @param buffer      Buffer to store the permissions
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetDefaultPermissions(char[] buffer, int maxlength);

  /**
   * Adds an option to the slash command
   *
   * @param name         Option name
   * @param description  Option description
   * @param type         Option type from DiscordCommandOptionType enum
   * @param required     Whether the option is required
   * @param autocomplete Whether the option supports autocomplete
   * @note               Autocomplete is only valid for Option_String, Option_Integer, and Option_Number
   */
  public native void AddOption(const char[] name, const char[] description, DiscordCommandOptionType type, bool required = false, bool autocomplete = false);

  /**
   * Adds a string option that supports choices
   *
   * @param name        Option name
   * @param description Option description
   * @param required    Whether the option is required
   */
  public native void AddStringOptionWithChoices(const char[] name, const char[] description, bool required = false);

  /**
   * Adds an integer option that supports choices
   *
   * @param name        Option name
   * @param description Option description
   * @param required    Whether the option is required
   */
  public native void AddIntOptionWithChoices(const char[] name, const char[] description, bool required = false);

  /**
   * Adds a float option that supports choices
   *
   * @param name        Option name
   * @param description Option description
   * @param required    Whether the option is required
   */
  public native void AddFloatOptionWithChoices(const char[] name, const char[] description, bool required = false);

  /**
   * Adds a string choice to the last added choice option
   *
   * @param choice_name  Display name of the choice
   * @param choice_value Value of the choice
   */
  public native void AddStringChoice(const char[] choice_name, const char[] choice_value);

  /**
   * Adds an integer choice to the last added choice option
   *
   * @param choice_name  Display name of the choice
   * @param choice_value Integer value of the choice
   *                     Use AddInt64Choice() when the value exceeds int32 range.
   */
  public native void AddIntChoice(const char[] choice_name, int choice_value);

  /**
   * Adds an integer choice to the last added choice option using a 64-bit value string
   *
   * @param choice_name  Display name of the choice
   * @param choice_value Integer value string of the choice
   */
  public native void AddInt64Choice(const char[] choice_name, const char[] choice_value);

  /**
   * Adds a float choice to the last added choice option
   *
   * @param choice_name  Display name of the choice
   * @param choice_value Float value of the choice
   */
  public native void AddFloatChoice(const char[] choice_name, float choice_value);

  /**
   * Registers the slash command to a specific guild
   *
   * @param guild_id  Guild ID to register the command to
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void RegisterToGuild(const char[] guild_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Registers the slash command globally
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void RegisterGlobally(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Updates an existing slash command
   *
   * @param guild_id  Guild ID (empty string for global command)
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Update(const char[] guild_id = NULL_STRING, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Deletes a slash command
   *
   * @param guild_id  Guild ID (empty string for global command)
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Delete(const char[] guild_id = NULL_STRING, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Adds a permission override for the slash command
   * This overrides the default permissions for specific roles/users/channels
   *
   * @param target_id    ID of the role/user/channel
   * @param type         Permission type (role, user, or channel)
   * @param permission   Whether to allow (true) or deny (false) access
   */
  public native void AddPermissionOverride(const char[] target_id, DiscordCommandPermissionType type, bool permission);

  /**
   * Removes a permission override for the slash command
   *
   * @param target_id    ID of the role/user/channel
   * @param type         Permission type (role, user, or channel)
   */
  public native void RemovePermissionOverride(const char[] target_id, DiscordCommandPermissionType type);

  /**
   * Clears all permission overrides for the slash command
   * This removes all custom permission settings, reverting to default permissions
   */
  public native void ClearPermissionOverrides();

  /**
   * Number of permission overrides for the slash command
   */
  property int PermissionOverrideCount {
    public native get();
  }

  /**
   * Gets a permission override entry by index
   *
   * @param index       Permission override entry index
   * @param target_id   Buffer to store target ID
   * @param maxlength   Maximum length of target_id buffer
   * @param type        Variable to store permission type
   * @param permission  Variable to store permission value
   * @return            True if entry exists, false otherwise
   */
  public native bool GetPermissionOverride(int index, char[] target_id, int maxlength, DiscordCommandPermissionType &type, bool &permission);

  /**
   * Applies permission overrides to the slash command
   *
   * @param guild_id  Guild ID where the command exists
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void ApplyPermissionOverrides(const char[] guild_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Controls the context menu type of the slash command
   */
  property DiscordContextMenuType ContextMenuType {
    public native get();
    public native set(DiscordContextMenuType type);
  }

  /**
   * Controls whether the command is age-restricted (NSFW)
   */
  property bool IsNSFW {
    public native get();
    public native set(bool nsfw);
  }

  /**
   * Controls whether the command can be used in DMs
   */
  property bool DMPermission {
    public native get();
    public native set(bool dm_permission);
  }

  /**
   * Adds a localization for the command name and description
   *
   * @param language     Language code (e.g., "en-US", "zh-CN")
   * @param name         Localized command name
   * @param description  Optional localized command description
   */
  public native void AddLocalization(const char[] language, const char[] name, const char[] description = NULL_STRING);

  /**
   * Sets interaction contexts where the command can be used
   *
   * @param contexts      Array of interaction context types
   * @param numContexts   Number of contexts in the array
   */
  public native void SetInteractionContexts(const DiscordInteractionContextType[] contexts, int numContexts);

  /**
   * Sets integration types for the command
   *
   * @param types     Array of integration types
   * @param numTypes  Number of types in the array
   */
  public native void SetIntegrationTypes(const DiscordIntegrationType[] types, int numTypes);

  /**
   * Sets minimum value for numeric options (co_number/co_integer)
   *
   * @param min_value  Minimum value
   * @note             Applies to the last added option
   */
  public native void SetOptionMinValue(float min_value);

  /**
   * Sets maximum value for numeric options (co_number/co_integer)
   *
   * @param max_value  Maximum value
   * @note             Applies to the last added option
   */
  public native void SetOptionMaxValue(float max_value);

  /**
   * Sets minimum length for string options (co_string)
   *
   * @param min_length  Minimum string length (0-6000)
   * @note              Applies to the last added option
   */
  public native void SetOptionMinLength(int min_length);

  /**
   * Sets maximum length for string options (co_string)
   *
   * @param max_length  Maximum string length (1-6000)
   * @note              Applies to the last added option
   */
  public native void SetOptionMaxLength(int max_length);

  /**
   * Adds a channel type filter for channel options (co_channel)
   *
   * @param channel_type  Channel type from DiscordChannelType enum
   * @note                Applies to the last added option
   */
  public native void AddOptionChannelType(DiscordChannelType channel_type);

  /**
   * Gets the mention string for this slash command
   *
   * @param buffer     Buffer to store the mention string
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetMention(char[] buffer, int maxlength);

  /**
   * Gets the command ID (if created from existing command)
   *
   * @param buffer     Buffer to store the command ID
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetCommandId(char[] buffer, int maxlength);

  /**
   * Gets the guild ID (if this is a guild command)
   *
   * @param buffer     Buffer to store the guild ID
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Bulk deletes all global slash commands
   *
   * @param discord    Discord client handle
   * @param callback   Optional callback to receive confirmation
   * @param data       Optional data to pass to callback
   * @note             This will remove all global slash commands for the bot
   */
  public static native void BulkDeleteGlobalCommands(Discord discord, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);
}
