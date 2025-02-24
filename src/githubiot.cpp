/*********************************************************************
 * GitHub IoT Library Implementation
 * 
 * This file implements the githubiot class which provides functionality
 * for Arduino devices to store and retrieve IoT data using GitHub as a 
 * backend storage solution.
 * 
 * The library handles HTTP communication with GitHub's API, including
 * authentication, JSON formatting, and Base64 encoding of data.
 * 
 * Author: GALIH RIDHO UTOMO,  Fionita Fahra Azzahra
 * Created: February 24, 2025
 * License: MIT License at https://github.com/4211421036/githubiot/?tab=MIT-1-ov-file
 * 
 * Please Readme Documentation for usage this module at https://github.com/4211421036/githubiot/?tab=readme-ov-file
 * Repository: https://github.com/4211421036/githubiot
 * Documentation: https://github.com/4211421036/githubiot/wiki
 * 
 * Dependencies:
 * - ArduinoJson library
 * - HTTPClient (part of ESP8266/ESP32 core)
 * - base64 encoding library
 *********************************************************************/

#include "githubiot.h"

/**
 * Constructor for the githubiot class
 * 
 * @param token    GitHub authentication token with 'Bearer ' prefix
 * @param repo_url GitHub API URL for the file to be updated
 *
 */
githubiot::githubiot(const char* token, const char* repo_url) {
    _token = token;
    _repo_url = repo_url;  // Note: There was a syntax error here in the original code (*repo*url)
}

/**
 * Retrieve the current SHA of the file in the GitHub repository
 * 
 * The SHA is required for updating files in GitHub to prevent conflicts.
 * This method performs an HTTP GET request to the GitHub API to fetch
 * the file's metadata, then extracts the SHA value.
 * 
 * @return String containing the SHA hash of the file, or empty string if failed
 */
String githubiot::get_current_sha() {
    HTTPClient http;
    String sha = "";
    
    // Initialize HTTP request to the repository URL
    http.begin(_repo_url);
    
    // Add GitHub authentication header
    http.addHeader("Authorization", _token);
    
    // Execute the GET request
    int httpCode = http.GET();
    
    // Process response if successful
    if (httpCode == HTTP_CODE_OK) {
        // Parse JSON response
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, http.getString());
        
        // Extract SHA from the response
        sha = doc["sha"].as<String>();
    }
    
    // Clean up HTTP resources
    http.end();
    
    return sha;
}

/**
 * Upload data to GitHub repository
 * 
 * This method performs an HTTP PUT request to update a file in GitHub.
 * The data is converted to JSON, Base64 encoded, and sent with appropriate
 * headers and metadata.
 * 
 * @param doc      JsonDocument containing the data to be uploaded
 * @param last_sha Reference to the current SHA of the file, will be updated with new SHA
 */
void githubiot::upload_to_github(DynamicJsonDocument doc, String& last_sha) {
    HTTPClient http;
    
    // Initialize HTTP request to the repository URL
    http.begin(_repo_url);
    
    // Set required headers for GitHub API
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", _token);
    
    // Serialize the JSON document to a string
    String jsonStr;
    serializeJson(doc, jsonStr);
    
    // Base64 encode the JSON string (GitHub requires Base64 for file content)
    String encodedData = base64::encode(jsonStr);
    
    // Create the payload for the PUT request
    // This includes commit message, encoded content, and the file's SHA
    String payload = "{\"message\":\"Update data\",\"content\":\"" + encodedData + "\",\"sha\":\"" + last_sha + "\"}";
    
    // Execute the PUT request with the payload
    int httpCode = http.PUT(payload);
    
    // If successful, update the SHA reference with the new value
    if (httpCode == HTTP_CODE_OK) {
        DynamicJsonDocument respDoc(1024);
        deserializeJson(respDoc, http.getString());
        last_sha = respDoc["content"]["sha"].as<String>();
    }
    
    // Clean up HTTP resources
    http.end();
}
