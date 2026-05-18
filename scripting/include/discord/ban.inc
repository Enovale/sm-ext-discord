// Discord ban
methodmap DiscordBan < Handle
{
  /**
   * Gets the banned user's ID
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the ban reason
   *
   * @param buffer       Buffer to store the reason
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetReason(char[] buffer, int maxlength);
}
