/*********************************************************************
 * Arduino GitHub IoT Integration
 * 
 * This sketch demonstrates IoT data logging using GitHub as a backend storage.
 * It connects to WiFi, reads sensor data, and uploads it to a GitHub repository
 * using the REST API.
 * 
 * Author: GALIH RIDHO UTOMO,  Fionita Fahra Azzahra
 * Created: February 24, 2025
 * License: MIT License at https://github.com/4211421036/githubiot/?tab=MIT-1-ov-file
 * 
 * Please Readme Documentation for usage this module at https://github.com/4211421036/githubiot/?tab=readme-ov-file
 * Repository: https://github.com/username/arduino-github-iot
 * Documentation: https://github.com/username/arduino-github-iot/wiki
 * 
 * Dependencies:
 * - githubiot library (https://github.com/username/githubiot)
 * - ArduinoJson library
 * - ESP8266WiFi library (for ESP8266) or WiFi library (for ESP32)
 *
 * Integrated GitHub Action
 * - IoT GitHub Action (https://github.com/marketplace/actions/generate-iot-dashboard)
 * - Rules create GitHub Action Integration Automation Develoment
 *   1. Go to your repository settings, then select the Action menu then select the General menu after that select Read and write permissions 
 *      The workflow has read and write permissions in the repository for all scopes in the Workflow permissions section.
 *   2. Copy the following .yml code, then paste it into your .yml file
 
     - name: Generate IoT Dashboard
      uses: 4211421036/iotgithub@v1.0.1
      with:
        json_filename: 'data.json'
        html_filename: 'index.html'
        css_filename: 'styles.css'
        site_title: 'My IoT Dashboard'
        chart_title: 'IoT Data Chart'
    - name: Commit and push changes
      run: |
        git config --global user.name "GitHub Actions"
        git config --global user.email "actions@github.com"
        git add .
        if git commit -m "Auto-generated IoT dashboard files"; then
          git push
        else
          echo "No changes to commit."
        fi

 *   3. In order to deploy a page website to GitHub with GitHub Pages Jekyll, first access the Settings menu. 
 *      Within the Settings menu, select GitHub Pages. Next, select Source GitHub Action. Finally, select Configure GitHub Pages Jekyll.
 *********************************************************************/

#include "githubiot.h"

// WiFi Configuration
// Replace with your network credentials
const char* ssidk = "your-SSID";         // Your WiFi network name
const char* passwordk = "your-PASSWORD";  // Your WiFi password

// GitHub Configuration
// Replace with your GitHub authentication token and repository details
const char* tokenk = "Bearer your_token";  // GitHub Personal Access Token with repo scope
const char* repo_urlk = "https://api.github.com/repos/username/repo/contents/sensor.json";  // GitHub API endpoint for your file

// Initialize GitHub IoT module with authentication token and repository URL
githubiot iot_module(tokenk, repo_urlk);

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    
    // Initialize WiFi connection
    WiFi.begin(ssidk, passwordk);
    
    // Wait for WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    
    // Retrieve the latest SHA from the file in GitHub
    // SHA is required for file updates to prevent conflicts
    String last_sha = iot_module.get_current_sha();
    if (last_sha != "") {
        Serial.println("Current SHA: " + last_sha);
    } else {
        Serial.println("Failed to get SHA");
    }
    
    // Create sensor data JSON document
    // Adjust document size (1024) based on your data requirements
    DynamicJsonDocument doc(1024);
    doc["sensor"] = "temperature";    // Sensor identifier
    doc["value"] = 25.5;             // Sample sensor reading
    
    // Upload data to GitHub repository
    // This will create or update the sensor.json file
    iot_module.upload_to_github(doc, last_sha);
    Serial.println("Data uploaded to GitHub");
}

void loop() {
    
}
