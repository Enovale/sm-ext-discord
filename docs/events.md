# Discord Events Reference

All events use the unified callback signature:
```sourcepawn
typedef DiscordEventCallback = function void (Discord client, DiscordEvent event, any data);
```

Use `event.GetString()`, `event.GetInt()`, `event.GetBool()`, `event.GetHandle()` to access event data.

---

## Connection Events

### ready
Bot successfully connected to Discord.

| Key | Type | Description |
|-----|------|-------------|
| guild_ids | Handle (DiscordHandleArray) | Array of guild ID strings |
| session_id | string | WebSocket session ID |
| shard_id | int | Shard ID |
| guild_count | int | Number of guilds |

### resumed
Bot resumed after disconnect.

| Key | Type | Description |
|-----|------|-------------|
| session_id | string | WebSocket session ID |
| shard_id | int | Shard ID |

---

## Message Events

### message_create
New message received.

| Key | Type | Description |
|-----|------|-------------|
| message | Handle (DiscordMessage) | Message object |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| author_id | string | Author user ID |
| content | string | Message content |
| content_length | int | Message content length (⚠️not including any null-termination) |

### message_update
Message was edited.

| Key | Type | Description |
|-----|------|-------------|
| message | Handle (DiscordMessage) | Updated message object |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |

### message_delete
Message was deleted.

| Key | Type | Description |
|-----|------|-------------|
| message_id | string | Deleted message ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |

### message_delete_bulk
Multiple messages deleted at once.

| Key | Type | Description |
|-----|------|-------------|
| message_ids | Handle (DiscordHandleArray) | Array of deleted message ID strings |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |

### message_reaction_add
Reaction added to message.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Guild object (if in guild) |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID (empty if DM) |
| user_id | string | User who reacted |
| message_author_id | string | Message author ID |
| emoji | string | Emoji (name or name:id for custom) |

### message_reaction_remove
Reaction removed from message.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Guild object (if in guild) |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| user_id | string | User who removed reaction |
| emoji | string | Emoji removed |

### message_reaction_remove_all
All reactions removed from message.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Guild object |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |

### message_reaction_remove_emoji
All reactions of specific emoji removed.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Guild object |
| message_id | string | Message snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| emoji | string | Emoji removed |

### message_poll_vote_add
User voted on a poll.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | Voter user ID |
| message_id | string | Poll message ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| answer_id | int | Answer option ID |

### message_poll_vote_remove
User removed poll vote.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | User ID |
| message_id | string | Poll message ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| answer_id | int | Answer option ID |

---

## Channel Events

### channel_create
Channel created.

| Key | Type | Description |
|-----|------|-------------|
| channel | Handle (DiscordChannel) | Channel object |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Channel name |
| type | int | Channel type |

### channel_update
Channel updated.

| Key | Type | Description |
|-----|------|-------------|
| channel | Handle (DiscordChannel) | Updated channel object |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Channel name |

### channel_delete
Channel deleted.

| Key | Type | Description |
|-----|------|-------------|
| channel | Handle (DiscordChannel) | Deleted channel object |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |

### channel_pins_update
Channel pins updated.

| Key | Type | Description |
|-----|------|-------------|
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID |
| timestamp | string | Last pin timestamp |

---

## Thread Events

### thread_create
Thread created.

| Key | Type | Description |
|-----|------|-------------|
| thread | Handle (DiscordChannel) | Thread channel object |
| thread_id | string | Thread snowflake ID |
| guild_id | string | Guild snowflake ID |
| parent_id | string | Parent channel ID |
| name | string | Thread name |
| owner_id | string | Thread creator ID |
| type | int | Thread type |

### thread_update
Thread updated.

| Key | Type | Description |
|-----|------|-------------|
| thread | Handle (DiscordChannel) | Updated thread object |
| thread_id | string | Thread snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Thread name |

### thread_delete
Thread deleted.

| Key | Type | Description |
|-----|------|-------------|
| thread | Handle (DiscordChannel) | Deleted thread object |
| thread_id | string | Thread snowflake ID |
| guild_id | string | Guild snowflake ID |
| parent_id | string | Parent channel ID |

### thread_list_sync
Thread list synced.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| thread_count | int | Number of threads |

### thread_member_update
Thread member updated.

| Key | Type | Description |
|-----|------|-------------|
| member | Handle (DiscordThreadMember) | Thread member object |
| thread_id | string | Thread snowflake ID |
| user_id | string | User snowflake ID |

### thread_members_update
Thread members updated.

| Key | Type | Description |
|-----|------|-------------|
| added_members | Handle (DiscordHandleArray) | Array of DiscordThreadMember handles |
| removed_user_ids | Handle (DiscordHandleArray) | Array of removed user ID strings |
| thread_id | string | Thread snowflake ID |
| guild_id | string | Guild snowflake ID |
| member_count | int | Current member count |

---

## Guild Events

### guild_create
Bot joined guild or guild became available.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Guild object |
| guild_id | string | Guild snowflake ID |
| name | string | Guild name |
| member_count | int | Member count |

### guild_update
Guild settings updated.

| Key | Type | Description |
|-----|------|-------------|
| guild | Handle (DiscordGuild) | Updated guild object |
| guild_id | string | Guild snowflake ID |
| name | string | Guild name |

### guild_delete
Bot removed from guild or guild unavailable.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| unavailable | bool | True if temporarily unavailable |

### guild_ban_add
User banned from guild.

| Key | Type | Description |
|-----|------|-------------|
| user | Handle (DiscordUser) | Banned user object |
| guild_id | string | Guild snowflake ID |
| user_id | string | Banned user ID |
| username | string | Banned username |

### guild_ban_remove
User unbanned from guild.

| Key | Type | Description |
|-----|------|-------------|
| user | Handle (DiscordUser) | Unbanned user object |
| guild_id | string | Guild snowflake ID |
| user_id | string | Unbanned user ID |
| username | string | Unbanned username |

### guild_emojis_update
Guild emojis updated.

| Key | Type | Description |
|-----|------|-------------|
| emoji_ids | Handle (DiscordHandleArray) | Array of DiscordEmoji strings |
| guild_id | string | Guild snowflake ID |
| emoji_count | int | Number of emojis |

### guild_stickers_update
Guild stickers updated.

| Key | Type | Description |
|-----|------|-------------|
| stickers | Handle (DiscordHandleArray) | Array of DiscordSticker handles |
| guild_id | string | Guild snowflake ID |
| sticker_count | int | Number of stickers |

### guild_integrations_update
Guild integrations updated.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |

### guild_member_add
Member joined guild.

| Key | Type | Description |
|-----|------|-------------|
| member | Handle (DiscordGuildMember) | Member object |
| user_id | string | New member user ID |
| guild_id | string | Guild snowflake ID |
| username | string | Username |

### guild_member_remove
Member left guild.

| Key | Type | Description |
|-----|------|-------------|
| user | Handle (DiscordUser) | Removed user object |
| user_id | string | Removed member user ID |
| guild_id | string | Guild snowflake ID |
| username | string | Username |

### guild_member_update
Member updated (roles, nickname, etc).

| Key | Type | Description |
|-----|------|-------------|
| member | Handle (DiscordGuildMember) | Updated member object |
| guild_id | string | Guild snowflake ID |
| user_id | string | User snowflake ID |
| nickname | string | Member nickname |

### guild_members_chunk
Chunk of guild members received.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| member_count | int | Members in chunk |

### guild_role_create
Role created.

| Key | Type | Description |
|-----|------|-------------|
| role | Handle (DiscordRole) | Created role object |
| guild_id | string | Guild snowflake ID |
| role_id | string | Role snowflake ID |
| name | string | Role name |

### guild_role_update
Role updated.

| Key | Type | Description |
|-----|------|-------------|
| role | Handle (DiscordRole) | Updated role object |
| guild_id | string | Guild snowflake ID |
| role_id | string | Role snowflake ID |
| name | string | Role name |

### guild_role_delete
Role deleted.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| role_id | string | Deleted role ID |

### guild_scheduled_event_create
Scheduled event created.

| Key | Type | Description |
|-----|------|-------------|
| scheduled_event | Handle (DiscordScheduledEvent) | Scheduled event object |
| event_id | string | Event snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Event name |
| description | string | Event description |
| channel_id | string | Channel ID (if voice/stage) |
| creator_id | string | Creator user ID |
| scheduled_start_time | string | Start timestamp |
| scheduled_end_time | string | End timestamp |
| entity_type | int | Event entity type |
| status | int | Event status |
| user_count | int | Interested user count |

### guild_scheduled_event_update
Scheduled event updated.

| Key | Type | Description |
|-----|------|-------------|
| scheduled_event | Handle (DiscordScheduledEvent) | Scheduled event object |
| event_id | string | Event snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Event name |
| description | string | Event description |
| channel_id | string | Channel ID |
| scheduled_start_time | string | Start timestamp |
| scheduled_end_time | string | End timestamp |
| entity_type | int | Event entity type |
| status | int | Event status |
| user_count | int | Interested user count |

### guild_scheduled_event_delete
Scheduled event deleted.

| Key | Type | Description |
|-----|------|-------------|
| scheduled_event | Handle (DiscordScheduledEvent) | Scheduled event object |
| event_id | string | Event snowflake ID |
| guild_id | string | Guild snowflake ID |

### guild_scheduled_event_user_add
User subscribed to scheduled event.

| Key | Type | Description |
|-----|------|-------------|
| event_id | string | Event snowflake ID |
| guild_id | string | Guild snowflake ID |
| user_id | string | User snowflake ID |

### guild_scheduled_event_user_remove
User unsubscribed from scheduled event.

| Key | Type | Description |
|-----|------|-------------|
| event_id | string | Event snowflake ID |
| guild_id | string | Guild snowflake ID |
| user_id | string | User snowflake ID |

### guild_audit_log_entry_create
Audit log entry created.

| Key | Type | Description |
|-----|------|-------------|
| entry_id | string | Entry snowflake ID |
| user_id | string | User who performed action |
| target_id | string | Target of action |
| action_type | int | Audit log action type |

### guild_join_request_delete
Guild join request deleted.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| user_id | string | User snowflake ID |

---

## Integration Events

### integration_create
Integration created.

| Key | Type | Description |
|-----|------|-------------|
| integration_id | string | Integration snowflake ID |
| name | string | Integration name |
| type | int | Integration type |

### integration_update
Integration updated.

| Key | Type | Description |
|-----|------|-------------|
| integration_id | string | Integration snowflake ID |
| name | string | Integration name |

### integration_delete
Integration deleted.

| Key | Type | Description |
|-----|------|-------------|
| integration_id | string | Integration snowflake ID |

---

## Invite Events

### invite_create
Invite created.

| Key | Type | Description |
|-----|------|-------------|
| invite | Handle (DiscordInvite) | Invite object |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| code | string | Invite code |
| inviter_id | string | Creator user ID |
| max_uses | int | Maximum uses |
| max_age | int | Maximum age in seconds |
| temporary | bool | Temporary membership |

### invite_delete
Invite deleted.

| Key | Type | Description |
|-----|------|-------------|
| invite | Handle (DiscordInvite) | Invite object |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| code | string | Invite code |

---

## Interaction Events

### interaction_create
Generic interaction received.

| Key | Type | Description |
|-----|------|-------------|
| interaction_id | string | Interaction snowflake ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |
| type | int | Interaction type |

### slashcommand
Slash command invoked.

| Key | Type | Description |
|-----|------|-------------|
| interaction | Handle (DiscordInteraction) | Interaction object |
| command_name | string | Command name |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |

### autocomplete
Autocomplete request received.

| Key | Type | Description |
|-----|------|-------------|
| interaction | Handle (DiscordAutocompleteInteraction) | Autocomplete interaction |
| command_name | string | Command name |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |

### button_click
Button clicked.

| Key | Type | Description |
|-----|------|-------------|
| interaction | Handle (DiscordButtonInteraction) | Button interaction |
| custom_id | string | Button custom ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |

### select_click
Select menu option selected.

| Key | Type | Description |
|-----|------|-------------|
| interaction | Handle (DiscordSelectInteraction) | Select interaction |
| custom_id | string | Select menu custom ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |

### form_submit
Modal form submitted.

| Key | Type | Description |
|-----|------|-------------|
| interaction | Handle (DiscordModalInteraction) | Modal interaction |
| custom_id | string | Modal custom ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |

### user_context_menu
User context menu used.

| Key | Type | Description |
|-----|------|-------------|
| interaction_id | string | Interaction snowflake ID |
| command_name | string | Command name |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |
| target_user_id | string | Target user ID |

### message_context_menu
Message context menu used.

| Key | Type | Description |
|-----|------|-------------|
| interaction_id | string | Interaction snowflake ID |
| command_name | string | Command name |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |
| user_id | string | User snowflake ID |
| target_message_id | string | Target message ID |

---

## Voice Events

### voice_state_update
User voice state changed.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| channel_id | string | Voice channel ID |
| user_id | string | User snowflake ID |
| session_id | string | Voice session ID |
| deaf | bool | Server deafened |
| mute | bool | Server muted |
| self_deaf | bool | Self deafened |
| self_mute | bool | Self muted |
| suppress | bool | Suppressed in stage |
| self_video | bool | Video enabled |
| self_stream | bool | Streaming |
| request_to_speak_timestamp | string | Request to speak timestamp |

### voice_server_update
Voice server updated.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| token | string | Voice token |
| endpoint | string | Voice server endpoint |

### voice_ready
Voice connection ready.

| Key | Type | Description |
|-----|------|-------------|
| voice_channel_id | string | Voice channel ID |

### voice_receive
Voice data received.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | Speaking user ID |

### voice_track_marker
Voice track marker reached.

| Key | Type | Description |
|-----|------|-------------|
| track_meta | string | Track metadata |

### voice_client_disconnect
Voice client disconnected.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | Disconnected user ID |

### voice_client_speaking
Voice client speaking state changed.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | User snowflake ID |
| ssrc | int | SSRC identifier |

### voice_buffer_send
Voice buffer sent.

| Key | Type | Description |
|-----|------|-------------|
| buffer_size | string | Buffer size |
| packets_left | int | Remaining packets |

---

## Stage Events

### stage_instance_create
Stage instance created.

| Key | Type | Description |
|-----|------|-------------|
| stage_id | string | Stage instance ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Stage channel ID |
| topic | string | Stage topic |

### stage_instance_update
Stage instance updated.

| Key | Type | Description |
|-----|------|-------------|
| stage_id | string | Stage instance ID |
| guild_id | string | Guild snowflake ID |
| topic | string | Stage topic |

### stage_instance_delete
Stage instance deleted.

| Key | Type | Description |
|-----|------|-------------|
| stage_id | string | Stage instance ID |
| guild_id | string | Guild snowflake ID |
| channel_id | string | Stage channel ID |

---

## Presence Events

### presence_update
User presence updated.

| Key | Type | Description |
|-----|------|-------------|
| user_id | string | User snowflake ID |
| guild_id | string | Guild snowflake ID |
| status | int | Presence status |

### typing_start
User started typing.

| Key | Type | Description |
|-----|------|-------------|
| channel | Handle (DiscordChannel) | Channel object |
| guild | Handle (DiscordGuild) | Guild object (if in guild) |
| user | Handle (DiscordUser) | User object (if available) |
| user_id | string | User snowflake ID |
| channel_id | string | Channel snowflake ID |
| guild_id | string | Guild snowflake ID (empty if DM) |
| timestamp | string | Typing timestamp |

### user_update
Current user updated.

| Key | Type | Description |
|-----|------|-------------|
| user | Handle (DiscordUser) | Updated user object |
| user_id | string | User snowflake ID |
| username | string | Username |
| global_name | string | Display name |

---

## Automod Events

### automod_rule_create
Automod rule created.

| Key | Type | Description |
|-----|------|-------------|
| rule_id | string | Rule snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Rule name |
| event_type | int | Event type |
| trigger_type | int | Trigger type |

### automod_rule_update
Automod rule updated.

| Key | Type | Description |
|-----|------|-------------|
| rule_id | string | Rule snowflake ID |
| guild_id | string | Guild snowflake ID |
| name | string | Rule name |

### automod_rule_delete
Automod rule deleted.

| Key | Type | Description |
|-----|------|-------------|
| rule_id | string | Rule snowflake ID |
| guild_id | string | Guild snowflake ID |

### automod_rule_execute
Automod rule executed.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| rule_id | string | Rule snowflake ID |
| user_id | string | Triggering user ID |
| channel_id | string | Channel snowflake ID |
| message_id | string | Message snowflake ID |
| content | string | Message content |
| action_type | int | Action type |
| rule_trigger_type | int | Rule trigger type |
| alert_system_message_id | string | Alert message ID |
| matched_keyword | string | Matched keyword |
| matched_content | string | Matched content |

---

## Entitlement Events

### entitlement_create
Entitlement created (premium purchase).

| Key | Type | Description |
|-----|------|-------------|
| entitlement_id | string | Entitlement snowflake ID |
| sku_id | string | SKU snowflake ID |
| user_id | string | User snowflake ID |
| guild_id | string | Guild snowflake ID |
| type | int | Entitlement type |

### entitlement_update
Entitlement updated.

| Key | Type | Description |
|-----|------|-------------|
| entitlement_id | string | Entitlement snowflake ID |
| sku_id | string | SKU snowflake ID |

### entitlement_delete
Entitlement deleted.

| Key | Type | Description |
|-----|------|-------------|
| entitlement_id | string | Entitlement snowflake ID |
| sku_id | string | SKU snowflake ID |

---

## Webhook Events

### webhooks_update
Channel webhooks updated.

| Key | Type | Description |
|-----|------|-------------|
| guild_id | string | Guild snowflake ID |
| channel_id | string | Channel snowflake ID |

---

## Misc Events

### log
Internal log message.

| Key | Type | Description |
|-----|------|-------------|
| severity | int | Log severity level |
| message | string | Log message |

### shutdown
Bot is shutting down.

No parameters.
