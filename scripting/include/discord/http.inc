// Discord HTTP
methodmap HttpHeaders < Handle {
  /**
   * Creates a new HTTP headers collection
   *
   * @return          New HttpHeaders handle, or null on failure
   */
  public native HttpHeaders();

  /**
   * Sets a header value
   *
   * @param name      Header name
   * @param value     Header value
   */
  public native void SetHeader(const char[] name, const char[] value);

  /**
   * Gets a header value
   *
   * @param name      Header name
   * @param buffer    Buffer to store the value
   * @param maxlength Maximum length of the buffer
   * @return          True if header exists, false otherwise
   * @note            Header names are stored and looked up exactly as provided
   */
  public native bool GetHeader(const char[] name, char[] buffer, int maxlength);

  /**
   * Removes a header
   *
   * @param name      Header name to remove
   * @return          True if header was removed, false if it didn't exist
   */
  public native bool RemoveHeader(const char[] name);

  /**
   * Clears all headers
   */
  public native void ClearHeaders();

  /**
   * Gets the number of headers
   */
  property int Count {
    public native get();
  }

  /**
   * Gets a header by index
   *
   * @param index     Header index (0-based)
   * @param name      Buffer to store header name
   * @param namelen   Maximum length of name buffer
   * @param value     Buffer to store header value
   * @param valuelen  Maximum length of value buffer
   * @return          True if header exists at index, false otherwise
   */
  public native bool GetHeaderByIndex(int index, char[] name, int namelen, char[] value, int valuelen);
}

/**
 * HTTP completion object containing full response data
 */
methodmap HttpCompletion < Handle {
  /**
   * Gets the HTTP status code
   */
  property int Status {
    public native get();
  }

  /**
   * Gets the response body
   *
   * @param buffer    Buffer to store the body
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetBody(char[] buffer, int maxlength);

  /**
   * Gets the response body length
   * @note not including any null-termination
   */
  property int BodyLength {
    public native get();
  }

  /**
   * Gets response headers as HttpHeaders object
   * @note Automatically managed - no need to manually free this handle
   */
  property HttpHeaders ResponseHeaders {
    public native get();
  }

  /**
   * Gets the HTTP protocol version used
   *
   * @param buffer    Buffer to store the protocol version
   * @param maxlength    Maximum length of the buffer
   */
  public native void GetProtocol(char[] buffer, int maxlength);

  /**
   * Checks if the request was successful (status 200-299)
   */
  property bool IsSuccess {
    public native get();
  }
}
