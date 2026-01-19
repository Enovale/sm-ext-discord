// Discord handle array - Container for multiple handles or strings returned from API calls
methodmap DiscordHandleArray < Handle {
	/**
	 * Gets the number of elements in the array
	 */
	property int Length {
		public native get();
	}

	/**
	 * Gets a handle at the specified index
	 *
	 * @param index     Index of the handle (0-based)
	 * @return          Handle at the index, or null if out of bounds
	 * @note            Automatically managed - no need to manually free this handle
	 */
	public native Handle GetAt(int index);

	/**
	 * Gets a string at the specified index
	 *
	 * @param index        Index of the string (0-based)
	 * @param buffer       Buffer to store the string
	 * @param maxlength    Maximum length of the buffer
	 * @return             True on success, false if out of bounds
	 */
	public native bool GetStringAt(int index, char[] buffer, int maxlength);
}
