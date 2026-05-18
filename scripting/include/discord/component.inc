// Discord component
methodmap DiscordComponent < Handle
{
  /**
   * Creates a new Discord component
   */
  public native DiscordComponent();

  /**
   * Component type
   */
  property DiscordComponentType Type {
    public native set(DiscordComponentType type);
  }

  /**
   * Button style
   */
  property DiscordComponentStyle Style {
    public native set(DiscordComponentStyle style);
  }

  /**
   * Whether the component is disabled
   */
  property bool Disabled {
    public native set(bool disabled);
  }

  /**
   * Minimum number of items that must be chosen for select menus (0-25)
   */
  property int MinValues {
    public native set(int min_values);
  }

  /**
   * Maximum number of items that can be chosen for select menus (1-25)
   */
  property int MaxValues {
    public native set(int max_values);
  }

  /**
   * Whether to show a divider (V2 components)
   */
  property bool Divider {
    public native set(bool divider);
  }

  /**
   * Separator spacing (V2 components)
   */
  property DiscordSeparatorSpacing Spacing {
    public native set(DiscordSeparatorSpacing spacing);
  }

  /**
   * Accent color (RGB value, V2 components)
   */
  property int Accent {
    public native set(int accent_color);
  }

  /**
   * Whether content is marked as spoiler (V2 components)
   */
  property bool Spoiler {
    public native set(bool spoiler);
  }

  /**
   * Text input style
   */
  property DiscordTextStyleType TextStyle {
    public native set(DiscordTextStyleType text_style);
  }

  /**
   * Whether the text input is required
   */
  property bool Required {
    public native set(bool required);
  }

  /**
   * Minimum length for text inputs (0-4000)
   */
  property int MinLength {
    public native set(int min_length);
  }

  /**
   * Maximum length for text inputs (1-4000)
   */
  property int MaxLength {
    public native set(int max_length);
  }

  /**
   * Sets the button label
   *
   * @param label   Button label text
   */
  public native void SetLabel(const char[] label);

  /**
   * Sets the custom ID for the component
   *
   * @param custom_id   Custom identifier
   */
  public native void SetCustomId(const char[] custom_id);

  /**
   * Sets the URL for link buttons
   *
   * @param url   Button URL
   */
  public native void SetUrl(const char[] url);

  /**
   * Sets the placeholder text for select menus
   *
   * @param placeholder   Placeholder text
   */
  public native void SetPlaceholder(const char[] placeholder);

  /**
   * Sets the emoji for the component
   *
   * @param name       Emoji name
   * @param id         Emoji ID (for custom emojis)
   * @param animated   Whether the emoji is animated
   */
  public native void SetEmoji(const char[] name, const char[] id = "0", bool animated = false);

  /**
   * Adds an option to a select menu
   *
   * @param label         Option label
   * @param value         Option value
   * @param description   Option description
   * @param is_default    Whether this option is selected by default
   * @param emoji_name    Emoji name for the option
   * @param emoji_id      Emoji ID for custom emojis
   */
  public native void AddOption(const char[] label, const char[] value, const char[] description = NULL_STRING, bool is_default = false, const char[] emoji_name = NULL_STRING, const char[] emoji_id = "0");

  /**
   * Adds a sub-component to an action row
   *
   * @param component   Component to add
   */
  public native void AddComponent(DiscordComponent component);

  /**
   * Adds a V2 sub-component
   *
   * @param component   Component to add
   */
  public native void AddComponentV2(DiscordComponent component);

  /**
   * Sets the content text for V2 components
   *
   * @param content   Content text
   */
  public native void SetContent(const char[] content);

  /**
   * Sets the thumbnail URL for V2 components
   *
   * @param url   Thumbnail URL
   */
  public native void SetThumbnail(const char[] url);

  /**
   * Sets an accessory component for V2 components
   *
   * @param accessory   Accessory component
   */
  public native void SetAccessory(DiscordComponent accessory);

  /**
   * Sets the file attachment URL for V2 file components
   *
   * @param attachment_url   Attachment URL
   */
  public native void SetFile(const char[] attachment_url);

  /**
   * Adds a media item to a media gallery component
   *
   * @param item   Media component to add
   */
  public native void AddMediaGalleryItem(DiscordComponent item);

  /**
   * Sets the description text
   *
   * @param description   Description text
   */
  public native void SetDescription(const char[] description);

  /**
   * Sets the SKU ID for premium components
   *
   * @param sku_id   SKU identifier
   */
  public native void SetSkuId(const char[] sku_id);

  /**
   * Sets the default value for text inputs
   *
   * @param value   Default value
   */
  public native void SetDefaultValue(const char[] value);

  /**
   * Adds a channel type filter for channel select menus
   *
   * @param channel_type   Channel type to include
   */
  public native void AddChannelType(DiscordChannelType channel_type);

  /**
   * Adds a default value for auto-populated select menus
   *
   * @param id     Entity ID (user/role/channel)
   * @param type   Entity type
   */
  public native void AddDefaultValue(const char[] id, DiscordComponentDefaultValueType type);
}
