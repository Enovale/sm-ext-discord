// Discord enums
enum HttpMethod {
  HTTP_GET = 0,
  HTTP_POST = 1,
  HTTP_PUT = 2,
  HTTP_PATCH = 3,
  HTTP_DELETE = 4
};

// Log levels enum
enum Loglevel {
  Loglevel_Trace = 0,
  Loglevel_Debug = 1,
  Loglevel_Info = 2,
  Loglevel_Warning = 3,
  Loglevel_Error = 4,
  Loglevel_Critical = 5
};

// Discord permissions enum
enum DiscordPermission
{
  Permission_CreateInstantInvite = 0,               // Allows creation of instant invites
  Permission_KickMembers = 1,                       // Allows kicking members from the guild
  Permission_BanMembers = 2,                        // Allows banning members from the guild
  Permission_Administrator = 3,                     // Administrator permissions (all permissions)
  Permission_ManageChannels = 4,                    // Allows management and editing of channels
  Permission_ManageGuild = 5,                       // Allows management and editing of the guild
  Permission_AddReactions = 6,                      // Allows adding reactions to messages
  Permission_ViewAuditLog = 7,                      // Allows viewing audit logs
  Permission_PrioritySpeaker = 8,                   // Allows using priority speaker in voice channels
  Permission_Stream = 9,                            // Allows the user to go live
  Permission_ViewChannel = 10,                      // Allows viewing channels (read messages/join voice)
  Permission_SendMessages = 11,                     // Allows sending messages in channels
  Permission_SendTTSMessages = 12,                  // Allows sending text-to-speech messages
  Permission_ManageMessages = 13,                   // Allows deleting other users' messages
  Permission_EmbedLinks = 14,                       // Allows links to be auto-embedded
  Permission_AttachFiles = 15,                      // Allows uploading files and images
  Permission_ReadMessageHistory = 16,               // Allows reading message history
  Permission_MentionEveryone = 17,                  // Allows using @everyone and @here mentions
  Permission_UseExternalEmojis = 18,                // Allows using emojis from other servers
  Permission_ViewGuildInsights = 19,                // Allows viewing guild insights
  Permission_Connect = 20,                          // Allows joining voice channels
  Permission_Speak = 21,                            // Allows speaking in voice channels
  Permission_MuteMembers = 22,                      // Allows muting members in voice channels
  Permission_DeafenMembers = 23,                    // Allows deafening members in voice channels
  Permission_MoveMembers = 24,                      // Allows moving members between voice channels
  Permission_UseVAD = 25,                           // Allows using voice activity detection
  Permission_ChangeNickname = 26,                   // Allows changing own nickname
  Permission_ManageNicknames = 27,                  // Allows managing others' nicknames
  Permission_ManageRoles = 28,                      // Allows managing roles
  Permission_ManageWebhooks = 29,                   // Allows managing webhooks
  Permission_ManageEmojis = 30,                     // Allows managing emojis and stickers
  Permission_UseSlashCommands = 31,                 // Allows using slash commands
  Permission_RequestToSpeak = 32,                   // Allows requesting to speak in stage channels
  Permission_ManageEvents = 33,                     // Allows managing scheduled events
  Permission_ManageThreads = 34,                    // Allows managing threads
  Permission_CreatePublicThreads = 35,              // Allows creating public threads
  Permission_CreatePrivateThreads = 36,             // Allows creating private threads
  Permission_UseExternalStickers = 37,              // Allows using stickers from other servers
  Permission_SendMessagesInThreads = 38,            // Allows sending messages in threads
  Permission_UseEmbeddedActivities = 39,            // Allows using embedded activities
  Permission_ModerateMembers = 40,                  // Allows timing out members
  Permission_ViewCreatorMonetizationAnalytics = 41, // Allows viewing monetization analytics
  Permission_UseSoundboard = 42,                    // Allows using soundboard
  Permission_CreateGuildExpressions = 43,           // Allows creating guild expressions
  Permission_CreateEvents = 44,                     // Allows creating events
  Permission_UseExternalSounds = 45,                // Allows using sounds from other servers
  Permission_SendVoiceMessages = 46,                // Allows sending voice messages
  Permission_UseClydeAI = 47,                       // Allows using Clyde AI
  // Bit 48 is reserved/unused
  Permission_SendPolls = 49,                        // Allows sending polls
  Permission_UseExternalApps = 50,                  // Allows user-installed apps to send public responses
  Permission_PinMessages = 51                       // Allows pinning and unpinning messages
};

// Discord webhook types enum
enum DiscordWebhookType
{
  WebhookType_Incoming = 1,        // Incoming webhook (created by user in Discord)
  WebhookType_ChannelFollower = 2, // Channel follower webhook (follows another channel)
  WebhookType_Application = 3      // Application webhook (created by application)
};

// Discord user flags enum
enum DiscordUserFlags
{
  UserFlag_Bot = (1 << 0),                  // User is a bot
  UserFlag_System = (1 << 1),               // User is a system user (Clyde!)
  UserFlag_MfaEnabled = (1 << 2),           // User has multi-factor authentication enabled
  UserFlag_Verified = (1 << 3),             // User is verified (verified email address)
  UserFlag_NitroFull = (1 << 4),            // User has full nitro
  UserFlag_NitroClassic = (1 << 5),         // User has nitro classic
  UserFlag_DiscordEmployee = (1 << 6),      // User is discord staff
  UserFlag_PartneredOwner = (1 << 7),       // User owns a partnered server
  UserFlag_HypesquadEvents = (1 << 8),      // User is a member of hypesquad events
  UserFlag_BugHunter1 = (1 << 9),           // User has BugHunter level 1
  UserFlag_HouseBravery = (1 << 10),        // User is a member of House Bravery
  UserFlag_HouseBrilliance = (1 << 11),     // User is a member of House Brilliance
  UserFlag_HouseBalance = (1 << 12),        // User is a member of House Balance
  UserFlag_EarlySupporter = (1 << 13),      // User is an early supporter
  UserFlag_TeamUser = (1 << 14),            // User is a team user
  UserFlag_BugHunter2 = (1 << 15),          // User is has Bug Hunter level 2
  UserFlag_VerifiedBot = (1 << 16),         // User is a verified bot
  UserFlag_VerifiedBotDev = (1 << 17),      // User has the Early Verified Bot Developer badge
  UserFlag_AnimatedIcon = (1 << 18),        // User's icon is animated
  UserFlag_CertifiedModerator = (1 << 19),  // User is a certified moderator
  UserFlag_BotHttpInteractions = (1 << 20), // User is a bot using HTTP interactions
  UserFlag_NitroBasic = (1 << 21),          // User has nitro basic
  UserFlag_ActiveDeveloper = (1 << 22),     // User has the active developer badge
  UserFlag_AnimatedBanner = (1 << 23)       // User's banner is animated
};

// Discord channel types enum
enum DiscordChannelType
{
  ChannelType_Text = 0,                // Text channel
  ChannelType_DM = 1,                  // Direct message
  ChannelType_Voice = 2,               // Voice channel
  ChannelType_GroupDM = 3,             // Group DM (deprecated)
  ChannelType_Category = 4,            // Category channel
  ChannelType_Announcement = 5,        // Announcement channel
  ChannelType_Store = 6,               // Store channel (deprecated)
  ChannelType_AnnouncementThread = 10, // Announcement thread
  ChannelType_PublicThread = 11,       // Public thread
  ChannelType_PrivateThread = 12,      // Private thread
  ChannelType_Stage = 13,              // Stage channel
  ChannelType_Directory = 14,          // Directory channel
  ChannelType_Forum = 15,              // Forum channel
  ChannelType_Media = 16               // Media channel
};

// Discord slash command option types
enum DiscordCommandOptionType
{
  Option_SubCommand = 1,      // Sub-command option (Not implemented)
  Option_SubCommandGroup = 2, // Sub-command group option (Not implemented)
  Option_String = 3,          // String option
  Option_Integer = 4,         // Integer option
  Option_Boolean = 5,         // Boolean option
  Option_User = 6,            // User option
  Option_Channel = 7,         // Channel option
  Option_Role = 8,            // Role option
  Option_Mentionable = 9,     // Mentionable option (user or role)
  Option_Number = 10,         // Number option (double)
  Option_Attachment = 11      // Attachment option (Not implemented)
};

// Discord slash command context menu types
enum DiscordContextMenuType
{
  ContextMenu_None = 0,      // Undefined context menu type
  ContextMenu_ChatInput = 1, // Default slash commands (/ping, /pong, etc)
  ContextMenu_User = 2,      // User context menu command
  ContextMenu_Message = 3    // Message context menu command
};

// Discord interaction context types
enum DiscordInteractionContextType
{
  InteractionContext_Guild = 0,         // Can be used within servers
  InteractionContext_BotDM = 1,         // Can be used within DMs with bot
  InteractionContext_PrivateChannel = 2 // Can be used within Group DMs and other DMs
};

// Discord application integration types
enum DiscordIntegrationType
{
  IntegrationType_GuildInstall = 0, // App installed to a guild
  IntegrationType_UserInstall = 1   // App installed to a user
};

// Discord command permission types
enum DiscordCommandPermissionType
{
  CommandPermission_Role = 1,    // Permission for a role
  CommandPermission_User = 2,    // Permission for a user
  CommandPermission_Channel = 3  // Permission for a channel
};

// Discord presence status enum
enum DiscordPresenceStatus
{
  Presence_Offline = 0,   // Status is offline
  Presence_Online = 1,    // Status is online
  Presence_DND = 2,       // Status is busy (Do Not Disturb)
  Presence_Idle = 3,      // Status is idle (away)
  Presence_Invisible = 4  // Status is invisible (not shown as online)
};

// Discord activity types enum
enum DiscordActivityType
{
  Activity_Game = 0,      // Playing a game
  Activity_Streaming = 1, // Streaming a game
  Activity_Listening = 2, // Listening to music
  Activity_Watching = 3,  // Watching something
  Activity_Custom = 4,    // Custom activity (not a game)
  Activity_Competing = 5  // Competing in a game
};

// Discord image types enum
enum DiscordImageType
{
  Image_PNG = 0,   // PNG image format
  Image_JPG = 1,   // JPEG image format
  Image_GIF = 2,   // GIF image format
  Image_WEBP = 3   // WebP image format
};

// Image type aliases for guild methods
enum ImageType
{
  ImageType_PNG = 0,
  ImageType_JPG = 1,
  ImageType_GIF = 2,
  ImageType_WEBP = 3
};

// Sticker format types
enum StickerFormat
{
  StickerFormat_PNG = 1,    // PNG sticker
  StickerFormat_APNG = 2,   // Animated PNG sticker
  StickerFormat_Lottie = 3, // Lottie sticker
  StickerFormat_GIF = 4     // GIF sticker
};

// Scheduled event entity types
enum EventEntityType
{
  EventEntityType_StageInstance = 1, // Stage instance event
  EventEntityType_Voice = 2,         // Voice channel event
  EventEntityType_External = 3       // External event (location-based)
};

// Message flags bitmask
enum MessageFlags {
  MessageFlag_Crossposted = (1 << 0),            // This message has been published to subscribed channels (via Channel Following)
  MessageFlag_IsCrosspost = (1 << 1),            // This message originated from a message in another channel (via Channel Following)
  MessageFlag_SuppressEmbeds = (1 << 2),         // Do not include any embeds when serializing this message
  MessageFlag_SourceMessageDeleted = (1 << 3),   // The source message for this crosspost has been deleted (via Channel Following)
  MessageFlag_Urgent = (1 << 4),                 // This message came from the urgent message system
  MessageFlag_HasThread = (1 << 5),              // his message has an associated thread, with the same id as the message
  MessageFlag_Ephemeral = (1 << 6),              // This message is only visible to the user who invoked the Interaction
  MessageFlag_Loading = (1 << 7),                // This message is an Interaction Response and the bot is "thinking"
  MessageFlag_ThreadMentionFailed = (1 << 8),    // This message failed to mention some roles and add their members to the thread
  MessageFlag_SuppressNotifications = (1 << 12), // This message will not trigger push and desktop notifications
  MessageFlag_IsVoiceMessage = (1 << 13),        // This message is a voice message
  MessageFlag_HasSnapshot = (1 << 14),           // Contains forwarded snapshot
  MessageFlag_UsingComponentsV2 = (1 << 15)      // Message components vector contains v2 components
};

// Channel flags bitmask
enum ChannelFlags {
  ChannelFlag_NSFW = (1 << 4),                     // NSFW Gated Channel
  ChannelFlag_VideoQuality720p = (1 << 5),         // Video quality forced to 720p
  ChannelFlag_LockPermissions = (1 << 6),          // Lock permissions (only used when updating channel positions)
  ChannelFlag_PinnedThread = (1 << 7),             // Thread is pinned to the top of its parent forum or media channel
  ChannelFlag_RequireTag = (1 << 8),               // Whether a tag is required to be specified when creating a thread in a forum or media channel
  ChannelFlag_HideMediaDownloadOptions = (1 << 12) // When set hides the embedded media download options. Available only for media channels
};

// Types for sort posts in a forum channel
enum ForumSortOrder {
  ForumSort_LatestActivity = 0, // Sort forum posts by activity (default)
  ForumSort_CreationDate = 1    // Sort forum posts by creation time (from most recent to oldest)
};

// Types of forum layout views that indicates how the threads in a forum channel will be displayed for users by default
enum ForumLayoutType {
  ForumLayout_NotSet = 0,     // No default has been set for the forum channel
  ForumLayout_ListView = 1,   // Display posts as a list
  ForumLayout_GalleryView = 2 // Display posts as a collection of tiles
};

// Auto archive duration of threads
enum AutoArchiveDuration {
  AutoArchive_1Hour = 1,  // Auto archive duration of 1 hour (60 minutes)
  AutoArchive_1Day = 2,   // Auto archive duration of 1 day (1440 minutes)
  AutoArchive_3Days = 3,  // Auto archive duration of 3 days (4320 minutes)
  AutoArchive_1Week = 4   // Auto archive duration of 1 week (10080 minutes)
};

// Channel permission overwrite types
enum OverwriteType {
  Overwrite_Role = 0,  // Role overwrite
  Overwrite_Member = 1 // Member overwrite
};

// Message types enum
enum MessageType {
  MessageType_Default = 0,                                  // Default message type
  MessageType_RecipientAdd = 1,                             // Add recipient
  MessageType_RecipientRemove = 2,                          // Remove recipient
  MessageType_Call = 3,                                     // Call
  MessageType_ChannelNameChange = 4,                        // Channel name change
  MessageType_ChannelIconChange = 5,                        // Channel icon change
  MessageType_ChannelPinnedMessage = 6,                     // Message pinned
  MessageType_GuildMemberJoin = 7,                          // Member joined
  MessageType_UserPremiumGuildSubscription = 8,             // Boost
  MessageType_UserPremiumGuildSubscriptionTier1 = 9,        // Boost level 1
  MessageType_UserPremiumGuildSubscriptionTier2 = 10,       // Boost level 2
  MessageType_UserPremiumGuildSubscriptionTier3 = 11,       // Boost level 3
  MessageType_ChannelFollowAdd = 12,                        // Follow channel
  MessageType_GuildDiscoveryDisqualified = 14,              // Disqualified from discovery
  MessageType_GuildDiscoveryRequalified = 15,               // Re-qualified for discovery
  MessageType_GuildDiscoveryGracePeriodInitialWarning = 16, // Discovery grace period warning 1
  MessageType_GuildDiscoveryGracePeriodFinalWarning  = 17,  // Discovery grace period warning 2
  MessageType_ThreadCreated = 18,                           // Thread Created
  MessageType_Reply = 19,                                   // Reply
  MessageType_ApplicationCommand = 20,                      // Application command
  MessageType_ThreadStarterMessage = 21,                    // Thread starter message
  MessageType_GuildInviteReminder = 22,                     // Invite reminder
  MessageType_ContextMenuCommand = 23,                      // Context Menu Command
  MessageType_AutoModerationAction = 24,                    // Auto moderation action
  MessageType_RoleSubscriptionPurchase = 25,                // Role subscription purchase
  MessageType_InteractionPremiumUpsell = 26,                // Interaction premium upsell
  MessageType_StageStart = 27,                              // Stage start
  MessageType_StageEnd = 28,                                // Stage end
  MessageType_StageSpeaker = 29,                            // Stage speaker
  MessageType_StageTopic = 31,                              // Stage topic
  MessageType_ApplicationPremiumSubscription = 32           // Guild application premium subscription
};

// Guild flags enum
enum GuildFlags {
  GuildFlag_Large = (1 << 0),                    // Large guild
  GuildFlag_Unavailable = (1 << 1),              // Unavailable due to outage
  GuildFlag_WidgetEnabled = (1 << 2),            // Widget enabled
  GuildFlag_InviteSplash = (1 << 3),             // Can have invite splash
  GuildFlag_VipRegions = (1 << 4),               // Can have VIP regions
  GuildFlag_VanityUrl = (1 << 5),                // Can have vanity URL
  GuildFlag_Verified = (1 << 6),                 // Verified server
  GuildFlag_Partnered = (1 << 7),                // Partnered server
  GuildFlag_Community = (1 << 8),                // Community features enabled
  GuildFlag_RoleSubscriptionEnabled = (1 << 9),  // Role subscriptions enabled
  GuildFlag_News = (1 << 10),                    // Has news channels
  GuildFlag_Discoverable = (1 << 11),            // Discoverable in discovery
  GuildFlag_Featureable = (1 << 12),             // Featureable
  GuildFlag_AnimatedIcon = (1 << 13),            // Can have animated icon
  GuildFlag_Banner = (1 << 14),                  // Can have banner
  GuildFlag_WelcomeScreenEnabled = (1 << 15),    // Has welcome screen
  GuildFlag_MemberVerificationGate = (1 << 16),  // Has member verification
  GuildFlag_PreviewEnabled = (1 << 17),          // Has preview enabled
  GuildFlag_NoJoinNotifications = (1 << 18),     // Join notifications off
  GuildFlag_NoBoostNotifications = (1 << 19),    // Boost notifications off
  GuildFlag_HasAnimatedIcon = (1 << 20),         // Has animated icon
  GuildFlag_HasAnimatedBanner = (1 << 21),       // Has animated banner
  GuildFlag_NoSetupTips = (1 << 22),             // Setup tips off
  GuildFlag_NoStickerGreeting = (1 << 23),       // Sticker greeting off
  GuildFlag_MonetizationEnabled = (1 << 24),     // Monetization enabled
  GuildFlag_MoreStickers = (1 << 25),            // More sticker slots
  GuildFlag_CreatorStorePageEnabled = (1 << 26), // Creator store page enabled
  GuildFlag_RoleIcons = (1 << 27),               // Can set role icons
  GuildFlag_SevenDayThreadArchive = (1 << 28),   // Seven day thread archive (deprecated)
  GuildFlag_ThreeDayThreadArchive = (1 << 29),   // Three day thread archive (deprecated)
  GuildFlag_TicketedEvents = (1 << 30),          // Ticketed events enabled
  GuildFlag_ChannelBanners = (1 << 31)           // Channel banners (deprecated)
};

// Additional guild flags enum
enum GuildFlagsExtra {
  GuildFlagExtra_PremiumProgressBarEnabled = (1 << 0),             // Premium progress bar enabled
  GuildFlagExtra_AnimatedBanner = (1 << 1),                        // Can have animated banner
  GuildFlagExtra_AutoModeration = (1 << 2),                        // Auto moderation enabled
  GuildFlagExtra_InvitesDisabled = (1 << 3),                       // Invites disabled
  GuildFlagExtra_DeveloperSupportServer = (1 << 4),                // Developer support server
  GuildFlagExtra_NoRoleSubscriptionNotifications = (1 << 5),       // Role subscription notifications off
  GuildFlagExtra_NoRoleSubscriptionNotificationReplies = (1 << 6), // Role subscription notification replies off
  GuildFlagExtra_RoleSubscriptionsAvailableForPurchase = (1 << 7), // Role subscriptions available for purchase
  GuildFlagExtra_RaidAlertsDisabled = (1 << 8)                     // Raid alerts disabled
};

// Guild verification levels
enum GuildVerificationLevel {
  GuildVerification_None = 0,    // Unrestricted
  GuildVerification_Low = 1,     // Must have verified email
  GuildVerification_Medium = 2,  // Must be registered for 5+ minutes
  GuildVerification_High = 3,    // Must be member for 10+ minutes
  GuildVerification_VeryHigh = 4 // Must have verified phone
};

// Guild explicit content filter levels
enum GuildExplicitContentFilter {
  GuildExplicitContent_Disabled = 0,            // No scanning
  GuildExplicitContent_MembersWithoutRoles = 1, // Scan members without roles
  GuildExplicitContent_AllMembers = 2           // Scan all members
};

// Guild MFA levels
enum GuildMfaLevel {
  GuildMfa_None = 0,    // MFA not required
  GuildMfa_Elevated = 1 // MFA required for moderation actions
};

// Guild NSFW levels
enum GuildNsfwLevel {
  GuildNsfw_Default = 0,      // Default setting
  GuildNsfw_Explicit = 1,     // Explicit content may be present
  GuildNsfw_Safe = 2,         // Safe for work content only
  GuildNsfw_AgeRestricted = 3 // Age restricted (18+)
};

// Guild premium tiers (boost levels)
enum GuildPremiumTier {
  GuildPremium_None = 0,  // No boosts
  GuildPremium_Tier1 = 1, // Tier 1 perks
  GuildPremium_Tier2 = 2, // Tier 2 perks
  GuildPremium_Tier3 = 3  // Tier 3 perks
};

// Guild default message notification levels
enum GuildDefaultMessageNotifications {
  GuildNotifications_AllMessages = 0, // All messages
  GuildNotifications_OnlyMentions = 1 // Only @mentions
};

// Guild AFK timeout values
enum GuildAfkTimeout {
  GuildAfk_Off = 0,  // Disabled
  GuildAfk_60 = 1,   // 1 minute (60 seconds)
  GuildAfk_300 = 2,  // 5 minutes (300 seconds)
  GuildAfk_900 = 3,  // 15 minutes (900 seconds)
  GuildAfk_1800 = 4, // 30 minutes (1800 seconds)
  GuildAfk_3600 = 5  // 1 hour (3600 seconds)
};

// Component types enum
enum ComponentType {
  ComponentType_ActionRow = 1,              // Container for other components
  ComponentType_Button = 2,                 // Clickable button
  ComponentType_SelectMenu = 3,             // String select menu (dropdown)
  ComponentType_Text = 4,                   // Text input field (modals only)
  ComponentType_UserSelectMenu = 5,         // User select menu
  ComponentType_RoleSelectMenu = 6,         // Role select menu
  ComponentType_MentionableSelectMenu = 7,  // User and role select menu
  ComponentType_ChannelSelectMenu = 8,      // Channel select menu
  ComponentType_Section = 9,                // Section container
  ComponentType_TextDisplay = 10,           // Text display element
  ComponentType_Thumbnail = 11,             // Thumbnail image
  ComponentType_MediaGallery = 12,          // Media gallery container
  ComponentType_File = 13,                  // File attachment
  ComponentType_Separator = 14,             // Visual separator
  ComponentType_ContentInventoryEntry = 16, // Content inventory entry
  ComponentType_Container = 17,             // Generic container
  ComponentType_Label = 18,                 // Label with description
  ComponentType_FileUpload = 19             // File upload component
};

// Component button styles enum
enum ComponentStyle {
  ComponentStyle_Primary = 1,   // Blurple button
  ComponentStyle_Secondary = 2, // Grey button
  ComponentStyle_Success = 3,   // Green button
  ComponentStyle_Danger = 4,    // Red button
  ComponentStyle_Link = 5,      // URL button (no interaction event)
  ComponentStyle_Premium = 6    // Premium/SKU button
};

// Text input styles enum
enum TextStyleType {
  TextStyle_Short = 1,    // Single-line input
  TextStyle_Paragraph = 2 // Multi-line input
};

// Separator spacing enum
enum SeparatorSpacing {
  SeparatorSpacing_Small = 1, // Small spacing
  SeparatorSpacing_Large = 2  // Large spacing
};

// Component default value types enum
enum ComponentDefaultValueType {
  ComponentDefaultValueType_User = 0,    // Default value is a user
  ComponentDefaultValueType_Role = 1,    // Default value is a role
  ComponentDefaultValueType_Channel = 2  // Default value is a channel
};

// Discord result types - identifies the type of result returned from API callbacks
enum DiscordResultType {
  // Single objects
  ResultType_Message = 0,
  ResultType_Channel = 1,
  ResultType_Guild = 2,
  ResultType_User = 3,
  ResultType_Role = 4,
  ResultType_Webhook = 5,
  ResultType_Invite = 6,
  ResultType_GuildMember = 7,
  ResultType_Emoji = 8,
  ResultType_Sticker = 9,
  ResultType_ScheduledEvent = 10,
  ResultType_Poll = 11,
  ResultType_Attachment = 12,
  ResultType_Reaction = 13,
  ResultType_Ban = 14,
  ResultType_ThreadMember = 15,

  // Lists
  ResultType_Messages = 16,
  ResultType_Channels = 17,
  ResultType_Users = 18,
  ResultType_Roles = 19,
  ResultType_Webhooks = 20,
  ResultType_Invites = 21,
  ResultType_Members = 22,
  ResultType_Emojis = 23,
  ResultType_Stickers = 24,
  ResultType_ScheduledEvents = 25,
  ResultType_Bans = 26,
  ResultType_Threads = 27,
  ResultType_Voters = 28,
  ResultType_ThreadMembers = 29,

  // Operations (no object returned)
  ResultType_Confirmation = 30,
  ResultType_Delete = 31,
  ResultType_Pin = 32,
  ResultType_Unpin = 33,
  ResultType_Kick = 34,
  ResultType_BanOp = 35,
  ResultType_AddRole = 36,
  ResultType_RemoveRole = 37,
  ResultType_Timeout = 38,
  ResultType_Edit = 39,
  ResultType_Create = 40,
  ResultType_Send = 41,
  ResultType_Http = 42
};

// Allowed mentions flags - controls what mentions are parsed in a message
enum AllowedMentions {
  AllowedMentions_Users = (1 << 0),       // Parse user mentions
  AllowedMentions_Everyone = (1 << 1),    // Parse @everyone and @here mentions
  AllowedMentions_Roles = (1 << 2),       // Parse role mentions
  AllowedMentions_RepliedUser = (1 << 3)  // Mention the replied user
};

/**
 * Discord intents - controls what events the bot receives
 * Use bitwise OR to combine multiple intents
 * Reference: https://discord.com/developers/docs/topics/gateway#list-of-intents
 */
enum DiscordIntents {
  Intent_Guilds = (1 << 0),                       // Guild create/update/delete, role create/update/delete, channel create/update/delete
  Intent_GuildMembers = (1 << 1),                 // Guild member add/update/remove (PRIVILEGED)
  Intent_GuildBans = (1 << 2),                    // Guild ban add/remove
  Intent_GuildEmojis = (1 << 3),                  // Guild emoji/sticker create/update/delete
  Intent_GuildIntegrations = (1 << 4),            // Guild integration create/update/delete
  Intent_GuildWebhooks = (1 << 5),                // Guild webhook create/update/delete
  Intent_GuildInvites = (1 << 6),                 // Guild invite create/delete
  Intent_GuildVoiceStates = (1 << 7),             // Guild voice state update
  Intent_GuildPresences = (1 << 8),               // Guild presence update (PRIVILEGED)
  Intent_GuildMessages = (1 << 9),                // Guild message create/update/delete
  Intent_GuildMessageReactions = (1 << 10),       // Guild message reaction add/remove/clear
  Intent_GuildMessageTyping = (1 << 11),          // Guild typing start
  Intent_DirectMessages = (1 << 12),              // DM message create/update/delete
  Intent_DirectMessageReactions = (1 << 13),      // DM message reaction add/remove/clear
  Intent_DirectMessageTyping = (1 << 14),         // DM typing start
  Intent_MessageContent = (1 << 15),              // Message content access (PRIVILEGED)
  Intent_GuildScheduledEvents = (1 << 16),        // Guild scheduled event create/update/delete
  Intent_AutoModerationConfiguration = (1 << 20), // Auto moderation rule create/update/delete
  Intent_AutoModerationExecution = (1 << 21),     // Auto moderation action execution
  Intent_GuildMessagePolls = (1 << 24),           // Guild message poll vote add/remove
  Intent_DirectMessagePolls = (1 << 25)           // DM message poll vote add/remove
};

// Pre-defined intent combinations
#define DISCORD_INTENTS_DEFAULT                                               \
  (Intent_Guilds | Intent_GuildBans | Intent_GuildEmojis |                    \
  Intent_GuildIntegrations | Intent_GuildWebhooks | Intent_GuildInvites |     \
  Intent_GuildVoiceStates | Intent_GuildMessages |                            \
  Intent_GuildMessageReactions | Intent_GuildMessageTyping |                  \
  Intent_DirectMessages | Intent_DirectMessageTyping |                        \
  Intent_DirectMessageReactions | Intent_GuildScheduledEvents |               \
  Intent_AutoModerationConfiguration | Intent_AutoModerationExecution |       \
  Intent_GuildMessagePolls | Intent_DirectMessagePolls)
#define DISCORD_INTENTS_PRIVILEGED (Intent_GuildMembers | Intent_GuildPresences | Intent_MessageContent)
#define DISCORD_INTENTS_ALL (DISCORD_INTENTS_DEFAULT | DISCORD_INTENTS_PRIVILEGED)
#define DISCORD_INTENTS_UNVERIFIED_DEFAULT (DISCORD_INTENTS_DEFAULT | Intent_MessageContent)