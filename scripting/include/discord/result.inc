// Discord result - Unified result object for all Discord API callbacks
methodmap DiscordResult < Handle {
	/**
	 * Whether the API call was successful
	 */
	property bool IsSuccess {
		public native get();
	}

	/**
	 * Gets the error message if the call failed
	 *
	 * @param buffer      Buffer to store the error message
	 * @param maxlength   Maximum length of the buffer
	 */
	public native void GetError(char[] buffer, int maxlength);

	/**
	 * Gets the result type (see DiscordResultType enum)
	 */
	property DiscordResultType Type {
		public native get();
	}

	/**
	 * Gets a integer value from the result
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Integer value
	 */
	public native int GetInt(const char[] key, int defaultValue = 0);

	/**
	 * Gets a float value from the result
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Float value
	 */
	public native float GetFloat(const char[] key, float defaultValue = 0.0);

	/**
	 * Gets a boolean value from the result
	 *
	 * @param key           Key name
	 * @param defaultValue  Default value if key not found
	 * @return              Boolean value
	 */
	public native bool GetBool(const char[] key, bool defaultValue = false);

	/**
	 * Gets a string value from the result
	 *
	 * @param key         Key name
	 * @param buffer      Buffer to store the string
	 * @param maxlength   Maximum length of the buffer
	 */
	public native void GetString(const char[] key, char[] buffer, int maxlength);

	/**
	 * Gets a handle value from the result (e.g., DiscordChannel, DiscordUser)
	 *
	 * @param key       Key name
	 * @return          Handle value, or null if not found
	 * @note            The returned handle is managed by the result and will be freed when the result is destroyed
	 * @note            Use AutoFreeHandles property to control automatic freeing of handles
	 */
	public native Handle GetHandle(const char[] key);

	/**
	 * Checks if a key exists in the result
	 *
	 * @param key       Key name
	 * @return          True if key exists
	 */
	public native bool HasKey(const char[] key);

	/**
	 * Controls whether handles stored in this result are automatically freed when the result is destroyed
	 *
	 * @note Set to false if you need to keep handles beyond the callback scope.
	 *       You are then responsible for manually closing those handles
	 */
	property bool AutoFreeHandles {
		public native get();
		public native set(bool value);
	}
}
