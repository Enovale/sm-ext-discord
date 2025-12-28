// Discord poll
methodmap DiscordPoll < Handle
{
  /**
   * Creates a new Discord Poll
   */
  public native DiscordPoll();

  /**
   * Sets the poll question
   *
   * @param text       Question text
   */
  public native void SetQuestion(const char[] text);

  /**
   * Poll duration in hours (write-only, max 168 hours / 7 days)
   */
  property int Duration {
    public native set(int hours);
  }

  /**
   * Adds an answer option to the poll
   *
   * @param text       Answer text
   * @param emoji      Optional emoji (unicode or custom emoji string)
   */
  public native void AddAnswer(const char[] text, const char[] emoji = NULL_STRING);

  /**
   * Gets the poll question text
   *
   * @param buffer     Buffer to store the question
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetQuestion(char[] buffer, int maxlength);

  /**
   * Gets the poll expiry time (hours when creating, timestamp when received)
   */
  property float Expiry {
    public native get();
  }

  /**
   * Whether users can select multiple answers
   */
  property bool MultiSelect {
    public native get();
    public native set(bool allow);
  }

  /**
   * Whether the poll has finalized (voting ended)
   */
  property bool Finalized {
    public native get();
  }

  /**
   * Gets the number of answer options
   */
  property int AnswerCount {
    public native get();
  }

  /**
   * Gets the answer ID at the specified index
   *
   * @param index      Answer index (0-based)
   * @return           Answer ID
   */
  public native int GetAnswerId(int index);

  /**
   * Gets the answer text at the specified index
   *
   * @param index      Answer index (0-based)
   * @param buffer     Buffer to store the text
   * @param maxlength  Maximum length of the buffer
   */
  public native void GetAnswerText(int index, char[] buffer, int maxlength);

  /**
   * Whether the poll has results data
   */
  property bool HasResults {
    public native get();
  }

  /**
   * Gets the vote count for a specific answer
   *
   * @param answer_id  Answer ID
   * @return           Number of votes
   */
  public native int GetVoteCount(int answer_id);

  /**
   * Gets the total vote count across all answers
   */
  property int TotalVoteCount {
    public native get();
  }

  /**
   * Checks if the bot voted for a specific answer
   *
   * @param answer_id  Answer ID
   * @return           True if voted
   */
  public native bool DidVote(int answer_id);
}