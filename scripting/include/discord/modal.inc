// Discord modal
methodmap DiscordModal < Handle
{
  /**
   * @brief Create a new modal
   *
   * @param custom_id   Custom ID for the modal
   * @param title       Modal title (max 45 characters)
   * @return            DiscordModal handle
   */
  public native DiscordModal(const char[] custom_id, const char[] title);

  /**
   * @brief Add a row (ActionRow with TextInput) to the modal
   *
   * @param row   DiscordComponent ActionRow containing a text input
   * @note        A modal can have maximum 5 rows
   */
  public native void AddRow(DiscordComponent row);

  /**
   * @brief Convenience method to add a text input (automatically creates ActionRow)
   *
   * This method simplifies modal creation by automatically wrapping text inputs in ActionRows.
   * Instead of manually creating both components, you can add inputs in a single call.
   *
   * @param custom_id       Custom ID for the text input
   * @param label           Label displayed above the input
   * @param style           Text input style (TextStyle_Short or TextStyle_Paragraph)
   * @param required        Whether the input is required
   * @param placeholder     Placeholder text (optional, default: "")
   * @param min_length      Minimum character length (0-4000, default: 0)
   * @param max_length      Maximum character length (1-4000, default: 4000)
   * @param default_value   Pre-filled default value (optional, default: "")
   * @return                True if added successfully, false if modal is full (5 rows max)
   *
   * @example
   * DiscordModal modal = new DiscordModal("survey", "User Survey");
   * modal.AddTextInput("name", "Your Name", TextStyle_Short, true, "Enter your name", 2, 50);
   * modal.AddTextInput("bio", "About You", TextStyle_Paragraph, false, "Tell us about yourself...", 10, 1000);
   */
  public native bool AddTextInput(const char[] custom_id, const char[] label,
                                   DiscordTextStyleType style, bool required,
                                   const char[] placeholder = NULL_STRING,
                                   int min_length = 0, int max_length = 4000,
                                   const char[] default_value = NULL_STRING);

  /**
   * @brief Clear all rows from the modal
   */
  public native void Clear();

  /**
   * @brief Get the custom ID of the modal
   *
   * @param buffer      Buffer to store the custom ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetCustomId(char[] buffer, int maxlength);

  /**
   * @brief Get the title of the modal
   *
   * @param buffer      Buffer to store the title
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetTitle(char[] buffer, int maxlength);

  /**
   * Whether the modal is full (has reached 5 rows)
   */
  property bool IsFull {
    public native get();
  }

  /**
   * Number of rows in the modal
   */
  property int RowCount {
    public native get();
  }
}