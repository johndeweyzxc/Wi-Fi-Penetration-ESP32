# AWPS (Automatic Wi-Fi Penetration System) Launcher Module

AWPS is a powerful software crafted for effortlessly penetrating WiFi-enabled devices secured with WPA2. It harnesses the robust capabilities of the ESP32 microcontroller to execute a vital operation in Wi-Fi network penetration. By capturing key data elements like the PMKID and MIC, AWPS allows users to conduct offline brute force attacks. 

The Launcher Module executes attack based on serial inputs, it offers direct user commands or integration with a Command Launch module. The Launcher Module prioritizes simplicity so it requires pairing with an external Command Launch Module for swift and error free execution of attacks. The AWPS Command Launch Module is an Android app, which provides a user-friendly interface for easy interaction with the Launcher Module. It allows user to execute attack at a single press of a button, scan nearby access points, and data management for storing the result of the attack.

For a deeper dive into the Command Launch Module's capabilities and further insights, explore the repository here [INSERT REPO LINK].

## Features

- **PMKID Capture** captures the PMKID from the first message of the 4-way hanshake
- **MIC Capture** captures the MIC and the whole EAPOL authentication data from the second message of the 4-way handshake 
- **Reconnaissance** scans nearby AP and outputs the SSID, channel and RSSI of the AP.
- **Deauth** injects deauthentication frames within the AP, effectively terminating the connection between the STA and the AP.

## How to Hack Wi-Fi using AWPS and GPU

<p align="center">
    <img src="visuals/How-to-hack-the-Wi-Fi.png" alt="How to hack the Wi-Fi">
</p>

Note: The android device presented in the story is the Command Launch Module, the Launcher Module is attached to the Command Launch Module. The actor in step 3 does not send an HTTP request to the Rest API server. The actor directly configures the GPU via a software by typing in the commands. Also the Rest API is only available as a local Rest API server in the LAN. Its primary role is to receive hash from the AWPS android app and store it in a database.

## Software Architecture

<p align="center">
    <img src="visuals/Launcher-Module-Software-Architecture.png" alt="Launcher Module Software Architecture" width="900">
</p>

## Software Components

This module consists of 4 main components, an application entrypoint and one auxiliary component. Here is the brief description of each component:
- **main** The application's entry point initiates the spawning of the command input parser task.
- **cmd_parser** It conducts a scan and parsing process of the user input, subsequently forwarding the parsed instruction code to the armament system.
- **armament** It orchestrates the attack by controlling the Wi-Fi behavior and the data parser.
- **wifi_ctl** An interface for the management of Wi-Fi capabilities such as enabling monitor mode and connecting to the access point.
- **frame_parser** Analyzes incoming data frames received from the Wi-Fi controller to identify PMKID and MIC.
- **frame_bypasser** An auxiliary component to allow the injection of arbitrary frames.

## Hardware Components

In this project, three hardware components are employed: an Android device, USB OTG (On The Go), and the ESP32. While the project has been rigorously tested on the ESP32-WROOM-32D variant, it is expected to be compatible with any microcontroller based on the ESP32-WROOM-32 platform.

<p align="center">
    <img src="visuals/AWPS-Hardware.png" alt="AWPS Hardware" width="700">
</p>

- **[1] Android Phone** Any android-powered device that can be use to install a Command Launch Module which is the interface that the user will use to control the operations.
- **[2] USB OTG (On The Go)** Enables communication between the Command Launch Module and the Launcher Module. It also delivers power from the Command Launch Module to the Launcher Module.
- **[3] Micro USB Male to USB Male** Establishes connection between the ESP32 device and a USB male port.
- **[4] ESP32-WROOM-32D** This is where the Launcher Module Software is installed and it is use to execute attacks by receiving commands from the Command Launch Module.

## Usage
### Instruction Codes

Instruction language that the launcher module can understand. These codes consist of armament codes, directing the module's actions, and a payload containing the mac address of the target. Meanwhile the control codes serve as the directives for the Launcher Module, indicating whether to activate the attack, deactivate it, or simply display the currently selected type of attack.

### Armament Codes

```text
Code:               Name:                   Description:
"01"                Reconnaissance          Scan nearby AP
"02"                PMKID                   Capture PMKID from the AP
"03"                MIC                     Capture MIC from the AP
"04"                Deauth                  Deauthenticate STAs from AP
```

### Control Codes

```text
Code:               Name:                   Description:
"05"                Armament status         Outputs the currently selected armament
"06"                Armament activate       Actives the selected armament
"07"                Armament deactivate     Deactivates the selected armament
"08"                Armament reset          Resets the ESP32
```

For example, an instruction code "02A1B2C3D4E5F6" means that it will use PMKID based attack and the target is A1B2C3D4E5F6 which is the mac address of the target. An instruction code of "01" simply means do a scan, it does not need a target since it will only scan nearby access points.

## How to crack the PMKID or MIC to determine the PSK

Neither the Command Launch Module nor the Launcher Module possesses the requisite capabilities to perform cryptographic cracking operations due to their limited computational power. For comprehensive information on optimizing the process of cracking, kindly refer to the crypto directory

## DISCLAIMER

The project AWPS is intended for educational purposes, with the primary goal of raising awareness and understanding of cybersecurity in a legal and ethical context. It is essential to clarify that this tool is NOT INTENDED to encourage or promote any form of unauthorized or unethical hacking activities. Ethical hacking, conducted with proper authorization and consent, plays a crucial role in enhancing the security of digital systems. This project seeks to promote responsible use of technology and responsible disclosure of vulnerabilities to help protect and secure digital environments.

USAGE OF ALL TOOLS on this project for attacking targets without prior mutual consent is ILLEGAL. It is the end user’s responsibility to obey all applicable local, state, and federal laws. I assume no liability and are not responsible for any misuse or damage caused by this project or software.

## Glossary

- **AP (Access Point or Authenticator)** The wifi device that acts as a central hub or bridge to connect wireless devices to a wired network.
- **STA (Station or Supplicant)** The device or the client device that is connected to the access point
- **BSSID (Basic Service Set Identifier)** The mac address of the access point 
- **SSID (Service Set Identifier)** The SSID is a unique name assigned to a wireless network to identify it among other nearby wireless networks.
- **RSSI (Received Signal Strength Indicator)** It is a measurement of the power level or strength of a received wireless signal, typically expressed in decibels (dBm).
- **PSK (Pre-Shared Key)** PSK is a secret passphrase or shared password used to authenticate and secure wireless connections in a Wi-Fi network.
- **PMKID (Pairwise Master Key Identifier)** The PMKID is used in Wi-Fi networks for fast and secure roaming. It allows a client device to quickly transition from one access point to another within the same network without going through the full authentication process again. 
- **MIC (Message Integrity Code)** The MIC is a security feature used to ensure the integrity and authenticity of data frames in a Wi-Fi network.
- **WPA2 (Wi-Fi Protected Access 2)** WPA2 is a security protocol used in Wi-Fi networks to secure wireless communication. It was introduced as an improvement over the earlier WPA (Wi-Fi Protected Access) standard.

## More Information
- https://en.wikipedia.org/wiki/PBKDF2
- https://www.wifi-professionals.com/2019/01/4-way-handshake
- https://hashcat.net/forum/thread-7717.html
- https://hashcat.net/wiki/doku.php?id=cracking_wpawpa2
- https://stackoverflow.com/questions/12018920/wpa-handshake-with-python-hashing-difficulties
- https://www.candelatech.com/downloads/802.11-2016.pdf