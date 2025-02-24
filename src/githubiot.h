/*********************************************************************
 * GitHub IoT Library Header
 * 
 * This header file defines the githubiot class which provides functionality
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
 * - base64 encoding library
 * - HTTPClient (from ESP8266 or ESP32 core libraries)
 *********************************************************************/

#ifndef GITHUBIOT_H
#define GITHUBIOT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <base64.h>
// Select the appropriate HTTPClient library based on the platform
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#endif

/**
 * @class githubiot
 * @brief Class for interacting with GitHub API to store IoT data
 * 
 * This class provides methods to get and update files in a GitHub repository,
 * allowing Arduino devices to use GitHub as a simple database for IoT data.
 */
class githubiot {
public:
    /**
     * @brief Constructor for the githubiot class
     * 
     * @param token    GitHub authentication token with 'Bearer ' prefix
     * @param repoUrl  GitHub API URL for the file to be updated
     *
     */
    githubiot(const char* token, const char* repoUrl);
    
    /**
     * @brief Retrieve the current SHA of the file in the GitHub repository
     * 
     * The SHA is required for updating files in GitHub to prevent conflicts.
     * 
     * @return String containing the SHA hash of the file, or empty string if failed
     */
    String get_current_sha();
    
    /**
     * @brief Upload data to GitHub repository
     * 
     * @param doc      JsonDocument containing the data to be uploaded
     * @param last_sha Reference to the current SHA of the file, will be updated with new SHA
     */
    void upload_to_github(DynamicJsonDocument doc, String& last_sha);

private:
    const char* _token;     // GitHub authentication token
    const char* _repo_url;  // GitHub repository API URL (Note: corrected from *repo*url)
    String _last_sha;       // Stores the last known SHA hash (Note: corrected from *last*sha)
};

#endif // GITHUBIOT_H
