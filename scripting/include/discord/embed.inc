// Discord embed
methodmap DiscordEmbed < Handle
{
  /**
   * Creates a new Discord Embed
   */
  public native DiscordEmbed();

  /**
   * Sets the embed title
   *
   * @param title     Title text
   */
  public native void SetTitle(const char[] title);

  /**
   * Sets the embed description
   *
   * @param desc      Description text
   */
  public native void SetDescription(const char[] desc);

  /**
   * Sets the embed URL
   *
   * @param url       URL link
   */
  public native void SetUrl(const char[] url);

  /**
   * Sets the embed author
   *
   * @param name      Author name
   * @param url       Optional author URL
   * @param icon_url  Optional author icon URL
   */
  public native void SetAuthor(const char[] name, const char[] url = NULL_STRING, const char[] icon_url = NULL_STRING);

  /**
   * Sets the embed footer
   *
   * @param text      Footer text
   * @param icon_url  Optional Footer icon URL
   */
  public native void SetFooter(const char[] text, const char[] icon_url = NULL_STRING);

  /**
   * Adds a field to the embed
   *
   * @param name      Field name
   * @param value     Field value
   * @param inLine    Whether to display inline
   */
  public native void AddField(const char[] name, const char[] value, bool inLine = false);

  /**
   * Sets the embed thumbnail
   *
   * @param url       Image URL
   */
  public native void SetThumbnail(const char[] url);

  /**
   * Sets the embed image
   *
   * @param url       Image URL
   */
  public native void SetImage(const char[] url);

  /**
   * Sets the embed video URL
   *
   * @param url       Video URL
   */
  public native void SetVideo(const char[] url);

  /**
   * Sets the embed provider information
   *
   * @param name      Provider name
   * @param url       Optional provider URL
   */
  public native void SetProvider(const char[] name, const char[] url = NULL_STRING);

  /**
   * Gets the embed title
   *
   * @param buffer    Buffer to store the title
   * @param maxlength Maximum length of the buffer
   */
  public native void GetTitle(char[] buffer, int maxlength);

  /**
   * Gets the embed description
   *
   * @param buffer    Buffer to store the description
   * @param maxlength Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Controls the embed color
   */
  property int Color {
    public native get();
    public native set(int color);
  }

 /**
   * Gets the embed timestamp as a Unix timestamp string
   *
   * @param buffer      Buffer to store the timestamp
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetTimestamp(char[] buffer, int maxlength);

  /**
   * Sets the embed timestamp from a Unix timestamp string
   *
   * @param timestamp   Unix timestamp string
   */
  public native void SetTimestamp(const char[] timestamp);

  /**
   * Gets the embed type
   *
   * @param buffer    Buffer to store the type
   * @param maxlength Maximum length of the buffer
   */
  public native void GetType(char[] buffer, int maxlength);

  /**
   * Gets the embed URL
   *
   * @param buffer    Buffer to store the URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed author name
   *
   * @param buffer    Buffer to store the author name
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAuthorName(char[] buffer, int maxlength);

  /**
   * Gets the embed author URL
   *
   * @param buffer    Buffer to store the author URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAuthorUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed author icon URL
   *
   * @param buffer    Buffer to store the author icon URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAuthorIconUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed author proxy icon URL
   *
   * @param buffer    Buffer to store the author proxy icon URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetAuthorProxyIconUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed footer text
   *
   * @param buffer    Buffer to store the footer text
   * @param maxlength Maximum length of the buffer
   */
  public native void GetFooterText(char[] buffer, int maxlength);

  /**
   * Gets the embed footer icon URL
   *
   * @param buffer    Buffer to store the footer icon URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetFooterIconUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed footer proxy URL
   *
   * @param buffer    Buffer to store the footer proxy URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetFooterProxyUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed thumbnail URL
   *
   * @param buffer    Buffer to store the thumbnail URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetThumbnailUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed thumbnail proxy URL
   *
   * @param buffer    Buffer to store the thumbnail proxy URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetThumbnailProxyUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed thumbnail width
   */
  property int ThumbnailWidth {
    public native get();
  }

  /**
   * Gets the embed thumbnail height
   */
  property int ThumbnailHeight {
    public native get();
  }

  /**
   * Gets the embed image URL
   *
   * @param buffer    Buffer to store the image URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetImageUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed image proxy URL
   *
   * @param buffer    Buffer to store the image proxy URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetImageProxyUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed image width
   */
  property int ImageWidth {
    public native get();
  }

  /**
   * Gets the embed image height
   */
  property int ImageHeight {
    public native get();
  }

  /**
   * Gets the embed video URL
   *
   * @param buffer    Buffer to store the video URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetVideoUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed video proxy URL
   *
   * @param buffer    Buffer to store the video proxy URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetVideoProxyUrl(char[] buffer, int maxlength);

  /**
   * Gets the embed video width
   */
  property int VideoWidth {
    public native get();
  }

  /**
   * Gets the embed video height
   */
  property int VideoHeight {
    public native get();
  }

  /**
   * Gets the embed provider name
   *
   * @param buffer    Buffer to store the provider name
   * @param maxlength Maximum length of the buffer
   */
  public native void GetProviderName(char[] buffer, int maxlength);

  /**
   * Gets the embed provider URL
   *
   * @param buffer    Buffer to store the provider URL
   * @param maxlength Maximum length of the buffer
   */
  public native void GetProviderUrl(char[] buffer, int maxlength);

  /**
   * Number of fields in the embed
   */
  property int FieldCount {
    public native get();
  }

  /**
   * Gets the name of a field by index
   *
   * @param index     Field index (0-based)
   * @param buffer    Buffer to store the field name
   * @param maxlength Maximum length of the buffer
   * @return          True if field exists, false otherwise
   */
  public native bool GetFieldName(int index, char[] buffer, int maxlength);

  /**
   * Gets the value of a field by index
   *
   * @param index     Field index (0-based)
   * @param buffer    Buffer to store the field value
   * @param maxlength Maximum length of the buffer
   * @return          True if field exists, false otherwise
   */
  public native bool GetFieldValue(int index, char[] buffer, int maxlength);

  /**
   * Gets whether a field is inline by index
   *
   * @param index     Field index (0-based)
   * @return          True if field is inline, false otherwise
   */
  public native bool GetFieldInline(int index);

  /**
   * Removes a field by index
   *
   * @param index     Field index (0-based)
   * @return          True if field was removed, false otherwise
   */
  public native bool RemoveField(int index);

  /**
   * Clears all fields from the embed
   */
  public native void ClearFields();

  /**
   * Checks if the embed has a thumbnail set
   */
  property bool HasThumbnail {
    public native get();
  }

  /**
   * Checks if the embed has an image set
   */
  property bool HasImage {
    public native get();
  }

  /**
   * Checks if the embed has a video set
   */
  property bool HasVideo {
    public native get();
  }

  /**
   * Checks if the embed has provider information set
   */
  property bool HasProvider {
    public native get();
  }

  /**
   * Checks if the embed has author information set
   */
  property bool HasAuthor {
    public native get();
  }

  /**
   * Checks if the embed has footer information set
   */
  property bool HasFooter {
    public native get();
  }

  /**
   * Checks if the embed has a timestamp set
   */
  property bool HasTimestamp {
    public native get();
  }
}
