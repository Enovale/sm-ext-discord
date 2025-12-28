// Discord event - Unified event object for all Discord events
methodmap DiscordEvent < Handle {
	/**
	 * Gets an integer value from the event
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Integer value
	 */
	public native int GetInt(const char[] key, int defaultValue = 0);

	/**
	 * Gets a float value from the event
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Float value
	 */
	public native float GetFloat(const char[] key, float defaultValue = 0.0);

	/**
	 * Gets a boolean value from the event
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Boolean value
	 */
	public native bool GetBool(const char[] key, bool defaultValue = false);

	/**
	 * Gets a string value from the event
	 *
	 * @param key         Key name
	 * @param buffer      Buffer to store the string
	 * @param maxlength   Maximum length of the buffer
	 * @return            True if successful
	 */
	public native bool GetString(const char[] key, char[] buffer, int maxlength);

	/**
	 * Gets a handle value from the event (e.g., DiscordMessage, DiscordInteraction)
	 *
	 * @param key       Key name
	 * @return          Handle value, or null if not found
	 * @note            The returned handle and its child handles (e.g., User, Channel, Message)
	 *                  are managed by the event and will be freed when the event is destroyed
	 * @note            Use AutoFreeHandles property to control automatic freeing of handles
	 */
	public native Handle GetHandle(const char[] key);

	/**
	 * Checks if a key exists in the event
	 *
	 * @param key       Key name
	 * @return          True if key exists
	 */
	public native bool HasKey(const char[] key);

	/**
	 * Gets the event name
	 *
	 * @param buffer      Buffer to store the name
	 * @param maxlength   Maximum length of the buffer
	 * @return            True if successful
	 */
	public native bool GetName(char[] buffer, int maxlength);

	/**
	 * Controls whether handles stored in this event are automatically freed
	 * when the event is destroyed.
	 *
	 * @note Set to false if you need to keep handles beyond the callback scope.
	 *       You are then responsible for manually closing those handles.
	 */
	property bool AutoFreeHandles {
		public native get();
		public native set(bool value);
	}
}
