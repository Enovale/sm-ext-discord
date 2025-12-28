# SourceMod Discord Extension

## Overview
A comprehensive [SourceMod](http://www.sourcemod.net/) extension that provides full Discord bot integration, covering nearly all Discord API features. Create and manage Discord bots directly from your SourceMod plugins with support for messages, embeds, slash commands, interactions, webhooks, and more.

## What's New in 3.0
> **⚠️ Breaking Change**: Version 3.0 is not backward compatible. All existing plugins must be rewritten to use the new API. This release has not been fully tested - some features may not work as expected.

Version 3.0 is a complete architecture rewrite with significant improvements:
* **Modular Architecture**: Completely refactor project for better maintainability
* **New Event System**: Simple and easy use event handling via `RegisterEvent`/`UnregisterEvent`
* **Components V2 Support**: Support for Discord's new message components (sections, containers, media galleries, separators, etc.)
* **New Result System**: `DiscordResult` provides unified error handling and structured data access
* **Unified Callback Pattern**: Consistent callback signature across all async operations
* **Git Submodules**: Third-party dependencies (DPP, zlib) managed via git submodules
* **Windows Static Linking**: DPP library statically compiled on Windows - no manual DLL copying required
* **Better Performance**: Optimized callback and handle management

## Available Features
* **Bot Management**: Start, stop, status checking, bot information retrieval, presence setting
* **Message System**: Send, edit, delete messages with rich formatting, attachments, and allowed mentions
* **Embed Support**: Complete Discord embed functionality with all fields, colors, and media
* **Webhook Integration**: Create, modify, delete, and execute webhooks with full embed support
* **Slash Commands**: Full slash command support with all option types, choices, and autocomplete
* **Permission System**: Complete Discord permission checking with enum support
* **User Management**: Comprehensive user information, role management, moderation actions
* **Role System**: Role checking, permission validation, role name resolution
* **Event Handling**: Unified event system with ALL Discord events via `RegisterEvent`/`UnregisterEvent`
* **Channel Management**: Channel information retrieval, modification, and permission checking
* **Forum Support**: Complete forum channel functionality with tags, thread creation, and management
* **Message Components**: Buttons, select menus, and Components V2 (sections, containers, media galleries, separators, etc.)
* **Modal Support**: Interactive modal dialogs with text inputs
* **Guild Management**: Guild information and member management
* **Guild Member**: Member information, roles, permissions, nickname management
* **Invite System**: Create and manage server invites
* **Moderation Tools**: Ban, kick, timeout, role management with reason support
* **Poll Support**: Create and manage message polls with multiple choice options
* **Scheduled Events**: Create and manage guild scheduled events
* **Emoji & Sticker**: Emoji and sticker information retrieval
* **Reaction System**: Add, remove, and manage message reactions
* **HTTP Client**: Built-in HTTP client for external API requests with header support and response handling

## Dependencies
* **For Linux users** - **Required**: Install OpenSSL development packages
 - For x64: `libssl-dev`
 - For x86: `libssl-dev:i386`

## Building
```sh
# Install dependencies
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install build-essential cmake libssl-dev libssl-dev:i386

# Clone and build
git clone --recursive https://github.com/ProjectSky/sm-ext-discord.git
cd sm-ext-discord
mkdir build && cd build
python ../configure.py --enable-optimize --sm-path=YOUR_SOURCEMOD_PATH --targets=x86,x64
ambuild
```

## Documentation
* [Event Reference](docs/events.md) - Complete list of all Discord events and their data fields

## Examples
Example plugins can be found in the [scripting](scripting/) folder

# Binary files
* [GitHub Releases](https://github.com/ProjectSky/sm-ext-discord/releases)

## TODO
- [x] Reaction support (AddReaction, RemoveReaction, RemoveAllReactions)
- [x] Forum support (forum channels, tags, thread creation)
- [x] Message component (V2) support
- [x] Modal dialog support
- [x] Thread support (general thread management)
- [x] Reorganize the directory and file structure
- [x] Refactor event/callback system
- [x] Add example scripts for all major features
- [x] DPP library static compilation support on windows
- [x] Include DPP and libz via git submodules
