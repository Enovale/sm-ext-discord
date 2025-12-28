// Discord reaction
methodmap DiscordReaction < Handle
{
  /**
   * Gets the total reaction count (including super reacts)
   */
  property int Count {
    public native get();
  }

  /**
   * Gets the count of super reactions
   */
  property int CountBurst {
    public native get();
  }

  /**
   * Gets the count of normal reactions
   */
  property int CountNormal {
    public native get();
  }

  /**
   * Gets the emoji ID (for custom emojis)
   *
   * @param buffer       Buffer to store the emoji ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetEmojiId(char[] buffer, int maxlength);

  /**
   * Gets the emoji name
   *
   * @param buffer       Buffer to store the emoji name
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetEmojiName(char[] buffer, int maxlength);

  /**
   * Whether the bot reacted using this emoji
   */
  property bool Me {
    public native get();
  }

  /**
   * Whether the bot super-reacted using this emoji
   */
  property bool MeBurst {
    public native get();
  }

  /**
   * Whether this is a custom emoji
   */
  property bool IsCustomEmoji {
    public native get();
  }

  /**
   * Gets the emoji mention format
   *
   * @param buffer       Buffer to store the mention
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetEmojiMention(char[] buffer, int maxlength);

  /**
   * Gets the number of burst colors
   */
  property int BurstColorCount {
    public native get();
  }

  /**
   * Gets a burst color at the specified index
   *
   * @param index        Index of the burst color
   * @return             Color as integer
   */
  public native int GetBurstColor(int index);
}
