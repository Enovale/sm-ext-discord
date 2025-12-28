// Discord attachment
methodmap DiscordAttachment < Handle
{
  /**
   * Gets the attachment ID
   *
   * @param buffer       Buffer to store the ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetId(char[] buffer, int maxlength);

  /**
   * Gets the attachment filename
   *
   * @param buffer       Buffer to store the filename
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetFilename(char[] buffer, int maxlength);

  /**
   * Gets the attachment description
   *
   * @param buffer       Buffer to store the description
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetDescription(char[] buffer, int maxlength);

  /**
   * Gets the attachment content type (MIME type)
   *
   * @param buffer       Buffer to store the content type
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetContentType(char[] buffer, int maxlength);

  /**
   * Gets the attachment size in bytes
   */
  property int Size {
    public native get();
  }

  /**
   * Gets the attachment URL
   *
   * @param buffer       Buffer to store the URL
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Gets the attachment proxy URL
   *
   * @param buffer       Buffer to store the proxy URL
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetProxyUrl(char[] buffer, int maxlength);

  /**
   * Gets the attachment width (for images/videos)
   */
  property int Width {
    public native get();
  }

  /**
   * Gets the attachment height (for images/videos)
   */
  property int Height {
    public native get();
  }

  /**
   * Whether this attachment is ephemeral
   */
  property bool Ephemeral {
    public native get();
  }

  /**
   * Gets the duration in seconds (for audio/voice messages)
   */
  property float DurationSecs {
    public native get();
  }

  /**
   * Gets the waveform data (for voice messages)
   *
   * @param buffer       Buffer to store the waveform
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetWaveform(char[] buffer, int maxlength);

  /**
   * Gets the attachment flags
   */
  property int Flags {
    public native get();
  }

  /**
   * Whether this attachment is a spoiler
   */
  property bool IsSpoiler {
    public native get();
  }

  /**
   * Whether this attachment is an image
   */
  property bool IsImage {
    public native get();
  }

  /**
   * Whether this attachment is a video
   */
  property bool IsVideo {
    public native get();
  }

  /**
   * Whether this attachment is audio
   */
  property bool IsAudio {
    public native get();
  }
}
