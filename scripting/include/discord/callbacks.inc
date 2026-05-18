/**
 * Unified callback for all Discord events
 *
 * @param client    Discord client handle
 * @param event     DiscordEvent object containing event data
 * @param data      User data passed to RegisterEvent
 */
typedef DiscordEventCallback = function void (Discord client, DiscordEvent event, any data);

/**
 * Unified callback for all Discord API results
 *
 * @param client    Discord client handle
 * @param result    DiscordResult object containing result data
 * @param data      User data passed to the API call
 */
typedef DiscordResultCallback = function void (Discord client, DiscordResult result, any data);