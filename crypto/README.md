# Key Hierarchy

<img src="../docs/Key-Hierarchy.png" alt="Key Hierarchy">

## 4-Way Hanshake Protocol

<img src="../docs/4-Way-Handshake.png" alt="4-Way Handshake Protocol">

## How to crack the PMKID and MIC

The process of deducing the PSK by cracking the PMKID and MIC involves the extraction of critical cryptographic information from the initial and subsequent messages of the 4-way handshake. There are two primary methods for PSK determination: PMKID cracking and MIC cracking.

## PMKID Cracking

PMKID cracking is a relatively straightforward process. It entails capturing the BSSID, the station's mac address, and the PMKID from the first message of the 4-way handshake. When the PMKID is computed, the result is compared to the PMKID of the captured packet and if the result matches then we found the correct PSK.

```text
PMK = PBKDF2('sha1', PSK, SSID, 4096, 32)
PMKID = HMAC_SHA1_128(PMK, "PMK Name" | BSSID | STA_MAC)
```

## MIC Cracking

On the other hand, MIC cracking is a more intricate process. It necessitates capturing Anonce, BSSID, STA_MAC from the first message, the entire authentication packet from the second message which inludes the MIC and the Snonce. When the MIC is computed, the result is compared to the MIC of the captured packet and if the result matches then we found the correct PSK.

```text
PMK = PBKDF2('sha1', PSK, SSID, 4096, 32)
PTK = PRF512(PMK, 'Pairwise key expansion', min(BSSID, STA_MAC) + max(BSSID, STA_MAC) + min(Anonce, Snonce) + max(Anonce, Snonce))
MIC = HMAC_SHA1(KCK, Second message authentication data)
```

## Crack PMKID and MIC using GPU with hashcat

With the utilization of hashcat, we can harness the parallel computing capabilities of a GPU to perform cryptographic operations, specifically for the cracking of PMKID and MIC. To initiate this process, begin by creating a file with the .hc22000 extension. Subsequently, populate the file with the following data:

### PMKID Cracking
```text
WPA*01*PMKID*MAC_AP*MAC_CLIENT*ESSID***
```
The columns are the following (all hex encoded):
- PMKID
- Mac address of the access point or BSSID
- Mac address of the client
- Name of the access point or SSID

For more information see:
- https://hashcat.net/wiki/doku.php?id=cracking_wpawpa2
- https://hashcat.net/forum/thread-7717.html

### MIC Cracking
```text
WPA*02*MIC*MAC_AP*MAC_CLIENT*ESSID*NONCE_AP*EAPOL_CLIENT**00
```

The columns are the following (all hex encoded):
- MIC
- Mac address of the access point or BSSID
- Mac address of the client
- Name of the access point or SSID
- Anonce
- The authentication data of the second message with MIC value set to all zero

For more information see:
- https://hashcat.net/wiki/doku.php?id=cracking_wpawpa2

## How to calculate the PMKID and MIC

You can use the 'key_hierarchy_calculation.py' Python script to see how to calculate the MIC and PMKID. Just make sure you put the necessary information in the YAML file provided. Here's how to use the Python script:

### PMKID Calculation
```shell
python key_hierarchy_calculation.py PMKID debug
```
Note: Make sure to put the necessary information in the pmkid.yaml file if prod is set or pmkid_debug.yaml if debug is set

### MIC Calculation
```shell
python key_hierarchy_calculation.py MIC prod
```
Note: Make sure to put the necessary information in the mic.yaml file if prod is set or pmkid_debug.yaml if debug is set. It is also important to remember that the script is used in conjuction with the module_test.py in the test directory. The module_test.py processes the output from the ESP32 where it will generate a mic.yaml or pmkid.yaml file to store the information from the EAPOl. The key_hierarchy_calculation.py will take this information and calculates the PMKID or MIC. A MIC or PMKID must be provided in the yaml file, it will compare this calculated PMKID or MIC, if it matches then the launcher and its output is working as expected. Also to calculate the correct PMKID or MIC, a PSK or the plain text password must be provided in the yaml file.

## DISCLAIMER

The project AWPS is intended for educational purposes, with the primary goal of raising awareness and understanding of cybersecurity in a legal and ethical context. It is essential to clarify that this tool is NOT INTENDED to encourage or promote any form of unauthorized or unethical hacking activities. Ethical hacking, conducted with proper authorization and consent, plays a crucial role in enhancing the security of digital systems. This project seeks to promote responsible use of technology and responsible disclosure of vulnerabilities to help protect and secure digital environments.

USAGE OF ALL TOOLS on this project for attacking targets without prior mutual consent is ILLEGAL. It is the end user’s responsibility to obey all applicable local, state, and federal laws. I assume no liability and are not responsible for any misuse or damage caused by this project or software.

## Glossary
- **STA_MAC (Station Mac Address)** The mac address of the client that is connected to the AP
- **Anonce (Access Point Nonce)** The Anonce, which stands for "Authentication Nonce," is a random value generated by the access point during the 4-way handshake.
- **Snonce (Station Nonce)** The Snonce, which stands for "Supplicant Nonce" or "Client Nonce," is a random value generated by the client during the 4-way handshake.
- **PBKDF2 (Password-Based Key Derivation Function 2)** PBKDF2 is key derivation function with a sliding computational cost, used to reduce vulnerability to brute-force attacks.
- **PRF512 (Pseudo Random Function 512)** The PRF-512 function is used to compute four 128-bit keys (KCK, KEK, TK1, TK2).
- **PMK (Pairwise Master Key)** The PMK is generated based on a PSK, also known as a network passphrase or Wi-Fi password. 
- **PTK (Pairwise Transient Key)** The PTK is generated as part of the 4-way handshake, a key exchange process that occurs when a wireless client wants to connect to a secured Wi-Fi network. The PTK is unique to the client-AP pair and is generated from the PMK.
- **KCK (Key Confirmation Key)** Used for confirming the authenticity of the messages exchanged during the 4-way handshake.
- **KEK (Key Encryption Key)** Used for securing the communication between the client and the access point, specifically for encrypting the pairwise traffic.
- **TK1 and TK2 (Temporal Key 1 and 2)** These are symmetric keys used for encrypting and decrypting the data frames exchanged between the client and the access point.
- **HMAC_SHA1_128 (Hash-Based Message Authentication Code Secure Hash Algorithm 1)** Specific hash-based message authentication code (HMAC) algorithm that uses the SHA-1 (Secure Hash Algorithm 1) hash function with a 128-bit output.
- **HMAC_SHA1 (Hash-Based Message Authentication Code Secure Hash Algorithm 1)** Method of creating a keyed hash value that is used for message authentication and integrity verification in cryptography.