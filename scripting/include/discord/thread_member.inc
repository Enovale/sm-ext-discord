// Discord thread member
methodmap DiscordThreadMember < Handle
{
  /**
   * Gets the thread ID
   *
   * @param buffer       Buffer to store the thread ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetThreadId(char[] buffer, int maxlength);

  /**
   * Gets the user ID
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the timestamp when the user joined the thread
   */
  property int JoinedTimestamp {
    public native get();
  }

  /**
   * Gets the thread member flags
   * @note Any user-thread settings, currently only used for notifications
   */
  property int Flags {
    public native get();
  }
}
