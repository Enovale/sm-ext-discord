// Discord guild member
methodmap DiscordGuildMember < Handle
{
  /**
   * Gets the user ID
   *
   * @param buffer       Buffer to store the user ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetUserId(char[] buffer, int maxlength);

  /**
   * Gets the guild ID
   *
   * @param buffer       Buffer to store the guild ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the member's nickname
   *
   * @param buffer       Buffer to store the nickname
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetNickname(char[] buffer, int maxlength);

  /**
   * Gets the member's guild avatar hash
   *
   * @param buffer       Buffer to store the avatar hash
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetAvatarHash(char[] buffer, int maxlength);

  /**
   * Gets the member's guild avatar URL
   *
   * @param buffer       Buffer to store the avatar URL
   * @param maxlength    Maximum length of the buffer
   * @param size         Size of the image (optional, 0 for default)
   */
  public native void GetAvatarUrl(char[] buffer, int maxlength, int size = 0);

  /**
   * Whether the member has an animated guild avatar
   */
  property bool HasAnimatedGuildAvatar {
    public native get();
  }

  /**
   * Gets the timestamp when the member joined the guild
   */
  property int JoinedAt {
    public native get();
  }

  /**
   * Gets the timestamp when the member started boosting
   */
  property int PremiumSince {
    public native get();
  }

  /**
   * Gets the timestamp when the timeout expires
   */
  property int CommunicationDisabledUntil {
    public native get();
  }

  /**
   * Whether the member is deafened
   */
  property bool Deaf {
    public native get();
  }

  /**
   * Whether the member is muted
   */
  property bool Muted {
    public native get();
  }

  /**
   * Whether the member is pending verification
   */
  property bool Pending {
    public native get();
  }

  /**
   * Whether the member is in timeout
   */
  property bool CommunicationDisabled {
    public native get();
  }

  /**
   * Whether the member has rejoined the guild
   */
  property bool HasRejoined {
    public native get();
  }

  /**
   * Whether the member is the guild owner
   */
  property bool IsGuildOwner {
    public native get();
  }

  /**
   * Whether the member has completed onboarding
   */
  property bool HasCompletedOnboarding {
    public native get();
  }

  /**
   * Whether the member has started onboarding
   */
  property bool HasStartedOnboarding {
    public native get();
  }

  /**
   * Whether the member bypasses verification
   */
  property bool HasBypassesVerification {
    public native get();
  }

  /**
   * Gets the number of roles the member has
   */
  property int RoleCount {
    public native get();
  }

  /**
   * Gets a role ID at the specified index
   *
   * @param index        Index of the role
   * @param buffer       Buffer to store the role ID
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetRoleId(int index, char[] buffer, int maxlength);

  /**
   * Checks if the member has a specific role
   *
   * @param role_id      Role ID to check
   * @return             True if the member has the role
   */
  public native bool HasRole(const char[] role_id);

  /**
   * Gets the member's permissions
   *
   * @param buffer       Buffer to store the permissions
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetPermissions(char[] buffer, int maxlength);

  /**
   * Checks if the member has a specific permission
   *
   * @param permission   Permission to check (as string)
   * @return             True if the member has the permission
   */
  public native bool HasPermission(const char[] permission);

  /**
   * Adds a role to the member
   *
   * @param role_id      Role ID to add
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void AddRole(const char[] role_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Removes a role from the member
   *
   * @param role_id      Role ID to remove
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void RemoveRole(const char[] role_id, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Sets the member's nickname
   *
   * @param nickname     New nickname (empty string to remove)
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void SetNickname(const char[] nickname, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Kicks the member from the guild
   *
   * @param reason       Reason for the kick (optional)
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void Kick(const char[] reason = NULL_STRING, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Bans the member from the guild
   *
   * @param delete_message_seconds  Seconds of messages to delete (0-604800)
   * @param reason                  Reason for the ban (optional)
   * @param callback                Optional callback to receive confirmation
   * @param data                    Optional data to pass to callback
   */
  public native void Ban(int delete_message_seconds = 0, const char[] reason = NULL_STRING, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Times out the member
   *
   * @param until        Unix timestamp when the timeout expires
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void Timeout(int until, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Removes the member's timeout
   *
   * @param callback     Optional callback to receive confirmation
   * @param data         Optional data to pass to callback
   */
  public native void RemoveTimeout(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);
}
