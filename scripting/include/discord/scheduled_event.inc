// Discord scheduled event
methodmap DiscordScheduledEvent < Handle
{
  /**
   * Gets the event ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the guild ID
   *
   * @param buffer       Buffer to store the guild ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the channel ID
   *
   * @param buffer       Buffer to store the channel ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetChannelId(char[] buffer, int maxlength);

  /**
   * Gets the creator ID
   *
   * @param buffer       Buffer to store the creator ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetCreatorId(char[] buffer, int maxlength);

  /**
   * Gets the event name
   *
   * @param buffer       Buffer to store the name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetName(char[] buffer, int maxlength);

  /**
   * Sets the event name
   *
   * @param name         New name for the event
   */
  public native void SetName(const char[] name);

  /**
   * Gets the event description
   *
   * @param buffer       Buffer to store the description
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Sets the event description
   *
   * @param description  New description for the event
   */
  public native void SetDescription(const char[] description);

  /**
   * Gets or sets the scheduled start time
   */
  property int ScheduledStartTime {
    public native get();
    public native set(int value);
  }

  /**
   * Gets or sets the scheduled end time
   */
  property int ScheduledEndTime {
    public native get();
    public native set(int value);
  }

  /**
   * Gets the privacy level
   */
  property int PrivacyLevel {
    public native get();
  }

  /**
   * Gets the event status
   */
  property int Status {
    public native get();
  }

  /**
   * Gets the entity type
   */
  property int EntityType {
    public native get();
  }

  /**
   * Gets the entity ID
   *
   * @param buffer       Buffer to store the entity ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetEntityId(char[] buffer, int maxlength);

  /**
   * Gets the event location
   *
   * @param buffer       Buffer to store the location
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetLocation(char[] buffer, int maxlength);

  /**
   * Sets the event location
   *
   * @param location     New location for the event
   */
  public native void SetLocation(const char[] location);

  /**
   * Gets the number of users subscribed to the event
   */
  property int UserCount {
    public native get();
  }

  /**
   * Gets the creator's user ID
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetCreatorUserId(char[] buffer, int maxlength);

  /**
   * Gets the creator's username
   *
   * @param buffer       Buffer to store the username
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetCreatorUserName(char[] buffer, int maxlength);

  /**
   * Sets the channel ID
   *
   * @param channel_id   New channel ID for the event
   */
  public native void SetChannelId(const char[] channel_id);

  /**
   * Deletes this scheduled event
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Delete(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits this scheduled event (applies all changes)
   *
   * @param callback  Optional callback to receive the updated event
   * @param data      Optional data to pass to callback
   */
  public native void Edit(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Starts this scheduled event
   *
   * @param callback  Optional callback to receive the updated event
   * @param data      Optional data to pass to callback
   */
  public native void Start(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Ends this scheduled event
   *
   * @param callback  Optional callback to receive the updated event
   * @param data      Optional data to pass to callback
   */
  public native void End(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Cancels this scheduled event
   *
   * @param callback  Optional callback to receive the updated event
   * @param data      Optional data to pass to callback
   */
  public native void Cancel(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);
}
