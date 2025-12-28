// Helper function to convert permission enum to string
stock void DiscordPermissionToString(DiscordPermission perm, char[] buffer, int maxlen)
{
  if (perm < Permission_CreateInstantInvite || perm > Permission_PinMessages)
  {
    strcopy(buffer, maxlen, "0");
    return;
  }

  int result[2];
  int permInt = view_as<int>(perm);

  if (permInt < 32)
  {
    // Permission fits in lower 32 bits
    result[0] = 1 << permInt;
    result[1] = 0;
  }
  else
  {
    // Permission requires upper 32 bits
    result[0] = 0;
    result[1] = 1 << (permInt - 32);
  }

  // Convert 64-bit result to string
  Int64ToString(result, buffer, maxlen);
}

// Helper function to combine multiple permissions into a bitmask string
stock void DiscordCombinePermissions(const DiscordPermission[] permissions, int numPermissions, char[] buffer, int maxlen)
{
  int combinedValue[2] = {0, 0}; // 64-bit integer as [lower32, upper32]

  for (int i = 0; i < numPermissions; i++)
  {
    DiscordPermission perm = permissions[i];

    // Skip invalid permissions
    if (perm < Permission_CreateInstantInvite || perm > Permission_PinMessages)
      continue;

    int permInt = view_as<int>(perm);

    if (permInt < 32)
    {
      combinedValue[0] |= (1 << permInt); // Set bit in lower 32 bits
    }
    else
    {
      combinedValue[1] |= (1 << (permInt - 32)); // Set bit in upper 32 bits
    }
  }

  // Convert 64-bit result back to string
  Int64ToString(combinedValue, buffer, maxlen);
}