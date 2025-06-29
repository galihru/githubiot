# Documentation and Usage Guide GitHub IoT Arduino Module
*Version 1.0.0 - February 24, 2025*

<p align="center">
  <img style="width: -webkit-fill-available;"src="https://galihru.github.io/githubiotpy/img/GitHub%20IoT%20Logo.png" alt="GitHubIoT Logo">
</p>

## Introduction

The `githubiot` library enables IoT devices based on ESP8266 or ESP32 microcontrollers to use GitHub as a data storage backend. This innovative approach eliminates the need for dedicated database servers while providing versioning capabilities, web accessibility, and integration with existing GitHub workflows.

This library handles all aspects of GitHub API communication, including authentication, file retrieval, content encoding/decoding, and update operations, allowing developers to focus on their IoT application rather than backend infrastructure.

---

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Dependencies](#dependencies)
4. [Installation](#installation)
5. [Integration GitHub Action](#integration-github-action)
7. [Library Components](#library-components)
8. [API Reference](#api-reference)
9. [Usage Examples](#usage-examples)
10. [Technical Background](#technical-background)
11. [Performance Considerations](#performance-considerations)
12. [Troubleshooting](#troubleshooting)
13. [Advanced Usage](#advanced-usage)
14. [License](#license)

---

## Features

- **GitHub Integration**: Store and retrieve data from GitHub repositories
- **JSON Support**: Native handling of JSON data structures
- **Cross-Platform**: Compatible with both ESP8266 and ESP32 devices
- **Version Control**: Automatic handling of GitHub's SHA versioning
- **Low Overhead**: Minimal memory and processing requirements
- **Secure Authentication**: Support for GitHub token-based authentication
- **Base64 Encoding**: Automatic handling of GitHub's content encoding requirements

---

## Dependencies

This library requires the following dependencies:

- **Arduino Core** (ESP8266 or ESP32)
- **ArduinoJson** (v6.x or later)
- **base64** library
- **WiFi** library (included in ESP cores)
- **HTTPClient** library (included in ESP cores)

---

## Installation

### Installation by Search Arduino IDE

1. In the Arduino IDE, navigate to Library Manager > `GitHubIoT`
2. Klik <mark>INSTALL</mark>
3. Restart the Arduino IDE

### Manual Installation

1. Download the library as a ZIP file from the [GitHub repository](https://github.com/username/githubiot)
2. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart the Arduino IDE

### Using Library Manager

1. Open the Arduino IDE
2. Navigate to Sketch > Include Library > Manage Libraries
3. In the search box, type "githubiot"
4. Find the library in the list and click "Install"

### Installing Dependencies

Make sure to install all required dependencies using the Arduino Library Manager.

---

## Integration GitHub Action
> [!NOTE]
> This module integration with GitHub Action Workflow. Please README Documentation at Repository [IoT GitHub](https://github.com/galihru/iotgithub?tab=readme-ov-file)

### Usage Integration it
> [!IMPORTANT]
> 1. Go to your repository settings, then select the Action menu then select the General menu after that select Read and write permissions The workflow has read and write permissions in the repository for all scopes in the Workflow permissions section.

2. Copy and paste the following snippet into your .yml file. Detail by [IoT GitHub Marketplace](https://github.com/marketplace/actions/generate-iot-dashboard)
3. You can see detail snippet .yml file below

```yml
- name: Generate IoT Dashboard
  uses: galihru/iotgithub@v1.0.1
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
```

---

## Library Components

The library consists of two main files:

1. **githubiot.h**: Header file defining the class interface
2. **githubiot.cpp**: Implementation file containing the method definitions

### Class Structure

```cpp
class githubiot {
public:
    githubiot(const char* token, const char* repoUrl);
    String get_current_sha();
    void upload_to_github(DynamicJsonDocument doc, String& last_sha);
private:
    const char* _token;
    const char* _repo_url;
    String _last_sha;
};
```

---

## API Reference

### Constructor

```cpp
githubiot::githubiot(const char* token, const char* repo_url)
```

Creates a new instance of the githubiot class.

**Parameters:**
- `token`: GitHub authentication token with 'Bearer ' prefix
- `repo_url`: GitHub API URL for the file to be updated

**Example:**
```cpp
const char* token = "Bearer ghp_xxxxxxxxxxxxxxxxxxxx";
const char* repo_url = "https://api.github.com/repos/username/repo/contents/data.json";
githubiot iot_module(token, repo_url);
```

### Get Current SHA

```cpp
String githubiot::get_current_sha()
```

Retrieves the current SHA hash of the file from GitHub.

**Returns:**
- String containing the SHA hash, or empty string if the operation failed

**Example:**
```cpp
String sha = iot_module.get_current_sha();
if (sha != "") {
    Serial.println("Current SHA: " + sha);
} else {
    Serial.println("Failed to get SHA");
}
```

### Upload to GitHub

```cpp
void githubiot::upload_to_github(DynamicJsonDocument doc, String& last_sha)
```

Uploads data to GitHub by updating the specified file.

**Parameters:**
- `doc`: ArduinoJson DynamicJsonDocument containing the data to upload
- `last_sha`: Reference to the current SHA string (will be updated with the new SHA)

**Example:**
```cpp
DynamicJsonDocument doc(1024);
doc["sensor"] = "temperature";
doc["value"] = 25.5;
doc["timestamp"] = 1645729845;

String sha = iot_module.get_current_sha();
iot_module.upload_to_github(doc, sha);
```

---

## Usage Examples

### Basic Data Logging

```cpp
#include <githubiot.h>

// Configuration
const char* ssid = "WiFi_SSID";
const char* password = "WiFi_Password";
const char* token = "Bearer ghp_xxxxxxxxxxxxxxxxxxxx";
const char* repo_url = "https://api.github.com/repos/username/repo/contents/sensor_log.json";

// Initialize the module
githubiot iot_module(token, repo_url);

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    
    // Get current SHA
    String sha = iot_module.get_current_sha();
    
    // Create data
    DynamicJsonDocument doc(1024);
    doc["device_id"] = "esp32_01";
    doc["readings"][0]["sensor"] = "temperature";
    doc["readings"][0]["value"] = 24.5;
    doc["readings"][1]["sensor"] = "humidity";
    doc["readings"][1]["value"] = 65.2;
    doc["timestamp"] = 1645729845;
    
    // Upload to GitHub
    iot_module.upload_to_github(doc, sha);
    Serial.println("Data uploaded");
}

void loop() {
    // Add periodic data logging as needed
    delay(3600000); // Update every hour
}
```

### Time Series Data Collection

```cpp
#include <githubiot.h>
#include <ArduinoJson.h>
#include <time.h>

// Configuration
const char* ssid = "WiFi_SSID";
const char* password = "WiFi_Password";
const char* token = "Bearer ghp_xxxxxxxxxxxxxxxxxxxx";
const char* repo_url = "https://api.github.com/repos/username/repo/contents/timeseries.json";
const char* ntpServer = "pool.ntp.org";

// Initialize the module
githubiot iot_module(token, repo_url);

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    
    // Initialize time
    configTime(0, 0, ntpServer);
}

void loop() {
    // Get current time
    time_t now;
    time(&now);
    
    // Read sensor data
    float temperature = readTemperature(); // Your sensor reading function
    
    // Get current SHA
    String sha = iot_module.get_current_sha();
    
    // Create or update time series data
    DynamicJsonDocument doc(2048);
    
    // If we have existing data, we need to parse it first
    if (sha != "") {
        // Here we would retrieve existing data and merge
        // This example simply creates new data each time
    }
    
    // Add new data point
    JsonObject newReading = doc["readings"].createNestedObject();
    newReading["timestamp"] = now;
    newReading["temperature"] = temperature;
    
    // Upload to GitHub
    iot_module.upload_to_github(doc, sha);
    Serial.println("Data uploaded");
    
    // Wait before next reading
    delay(900000); // 15 minutes
}

float readTemperature() {
    // Implementation of your sensor reading
    return 23.5; // Example value
}
```

---

## Technical Background

### GitHub API Interaction

The library communicates with the GitHub API using HTTP requests. Each file in a GitHub repository has a unique SHA hash that changes when the file is modified. When updating a file, you must provide the current SHA to ensure you're updating the latest version.

The workflow follows this sequence:

1. Retrieve the current file metadata (including SHA) via GET request
2. Prepare the new content
3. Encode the content in Base64 (GitHub requirement)
4. Send a PUT request with the new content and current SHA
5. Receive the updated file metadata (including new SHA)

### JSON Data Format

JSON (JavaScript Object Notation) is used as the data format due to its flexibility and widespread support. The ArduinoJson library provides efficient JSON processing on memory-constrained devices.

### Base64 Encoding

GitHub requires file content to be Base64 encoded in API requests. The encoding process converts binary data to ASCII text using the following transformation:

$$ \text{Base64}(M) = \text{concat}(B_1, B_2, \ldots, B_n) $$

Where each block $B_i$ is computed from 3 octets of the input message $M$:

$$ B_i = \text{enc}(M_{3i-2}, M_{3i-1}, M_{3i}) $$

The encoding function maps 3 bytes (24 bits) to 4 ASCII characters (6 bits each) according to the Base64 alphabet.

### HTTP Status Codes

> [!NOTE]
> The library handles various HTTP status codes returned by the GitHub API:

| Status Code | Description | Library Action |
|-------------|-------------|----------------|
| 200 (OK) | Request successful | Process response |
| 401 (Unauthorized) | Invalid token | Return error |
| 404 (Not Found) | File or repository not found | Return error |
| 409 (Conflict) | SHA mismatch (file changed) | Return error |
| 422 (Unprocessable Entity) | Validation failed | Return error |

---

## Performance Considerations

### Memory Usage

The ArduinoJson library uses dynamic memory allocation. The size of the `DynamicJsonDocument` should be adjusted based on your data complexity:

$$ \text{requiredSize} \approx \text{jsonLength} \times 1.1 + 10 $$

For ESP8266 devices with limited memory, consider:
- Keeping JSON structures simple
- Reducing the size of string literals
- Processing data in smaller chunks

### Network Performance

> [!CAUTION]
> GitHub API has rate limits:
> - Authenticated requests: 5,000 requests/hour
> - For optimal performance, limit update frequency to no more than once per minute

The average request-response cycle takes approximately 500-1500ms depending on network conditions.

### Power Consumption

To optimize power consumption for battery-operated devices:
- Use deep sleep between updates
- Schedule updates at regular intervals
- Implement error backoff strategy

---

## Troubleshooting

### Common Issues and Solutions

#### "Failed to get SHA"

**Possible causes:**
> [!IMPORTANT]
> - Invalid GitHub token
> - Incorrect repository URL
> - File doesn't exist yet
> - Network connectivity issues

**Solutions:**
- Verify token has correct permissions
- Check repository URL format
- Create file manually for first use
- Test network connection

#### "HTTP Error 401"

> [!CAUTION]
> **Possible causes:**
> - Invalid or expired token
> - Missing "Bearer " prefix in token

**Solutions:**
- Generate a new GitHub token
- Ensure token includes "Bearer " prefix

#### "HTTP Error 422"

> [!CAUTION]
> **Possible causes:**
> - Invalid JSON format
> - Base64 encoding issues

**Solutions:**
- Verify JSON structure
- Check Base64 encoding implementation

### Debugging Techniques

Enable debug output by defining `DEBUG_GITHUBIOT` before including the library:

```cpp
#define DEBUG_GITHUBIOT
#include <githubiot.h>
```

---

## Advanced Usage

### Updating Existing JSON Files

To update an existing JSON file while preserving its structure:

```cpp
// Get current file
String sha = iot_module.get_current_sha();

// Download and parse existing content
// (implementation not shown - would require custom HTTP GET handler)

// Update specific fields
existingDoc["last_updated"] = time(NULL);
existingDoc["readings"][0]["value"] = newValue;

// Upload updated content
iot_module.upload_to_github(existingDoc, sha);
```

### Custom Commit Messages

The current implementation uses a fixed commit message ("Update data"). For custom messages, the library would need to be extended.

### Error Handling Strategy

Implement a robust error handling strategy:

```cpp
String sha = iot_module.get_current_sha();
if (sha == "") {
    // Handle error: could not get SHA
    retryWithBackoff();
    return;
}

// Prepare data
DynamicJsonDocument doc(1024);
doc["sensor"] = "temperature";
doc["value"] = readSensor();

// Try to upload with retry
bool success = false;
int retries = 0;
while (!success && retries < MAX_RETRIES) {
    // Upload to GitHub
    int result = iot_module.upload_to_github(doc, sha);
    if (result == 200) {
        success = true;
    } else {
        retries++;
        delay(1000 * retries); // Exponential backoff
    }
}
```

---

## License

This library is released under the `MIT` License.

## Author
1. GALIH RIDHO UTOMO
2. Fionita Fahra Azzahra
