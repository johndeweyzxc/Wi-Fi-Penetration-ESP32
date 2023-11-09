# -*- coding: utf-8 -*-

# region Description
"""
module_test.py: Script for testing the correctness of the Launcher Module
Author: johndeweyzxc (johndewey02003@gmail.com)
"""
# endregion

from serial import Serial
import threading
import yaml
import argparse
import sys

MAX_BUFF_LEN = 255
OUT_YELLOW = "\033[93m"
COLOR_RESET = "\033[0m"
stop_thread = threading.Event()
scanned_aps = {}
mic_info = {}
pmkid_info = {}
test_mode = "debug"


def generate_eapol_message_1_yaml(message_1: list):
    """ 
    Saves important info to create a mic yaml file that can be read by 
    key_hierarchy_calculation.py 
    """

    for (key, value) in scanned_aps.items():
        # * Get the SSID from the scanned access point dictionary by checking if the
        # * transmitter mac address is equal to the key in dictionary
        if key == message_1[2]:
            mic_info["ssid"] = value

    mic_info["bssid"] = message_1[2]
    mic_info["anonce"] = message_1[4]  # Anonce or Access point nonce


def generate_eapol_message_2_yaml(message_2: list):
    """ 
    Generates a mic yaml file that can be read by 
    key_hierarchy_calculation.py 
    """

    mic_info["sta_mac"] = message_2[2]
    mic_info["snonce"] = message_2[11]
    m2_data = []
    m2_data.append(message_2[4])  # Version
    m2_data.append(message_2[5])  # Type
    m2_data.append(message_2[6])  # Length
    m2_data.append(message_2[7])  # Key Descriptor Type
    m2_data.append(message_2[8])  # Key Information
    m2_data.append(message_2[9])  # Key Length
    m2_data.append(message_2[10])  # Replay Counter
    m2_data.append(message_2[11])  # WPA Key Nonce (Snonce or Station nonce)
    m2_data.append(message_2[12])  # WPA Key IV
    m2_data.append(message_2[13])  # WPA Key RSC
    m2_data.append(message_2[14])  # WPA Key ID
    m2_data.append("00000000000000000000000000000000")  # MIC, all set to zero
    m2_data.append(message_2[16])  # WPA Key Data Length
    m2_data.append(message_2[17])  # WPA Key Data

    mic_info["mic"] = message_2[15]
    mic_info["m2_data"] = "".join(m2_data)

    # Converts the SSID from hex string to ascii
    ascii_ssid = bytes.fromhex(mic_info['ssid']).decode("utf-8")

    # ! A PSK or passphrase should be provided in the mic yaml file
    mic_yaml = f"""
    psk:
    ssid: {ascii_ssid}
    bssid: {mic_info['bssid']}
    sta_mac: {mic_info['sta_mac']}
    anonce: {mic_info['anonce']}
    snonce: {mic_info['snonce']}
    m2_data: {mic_info['m2_data']}
    mic: {mic_info['mic']}
    """
    mic = yaml.safe_load(mic_yaml)

    file_path = ""
    if test_mode == "debug":
        file_path = "../crypto/mic_debug.yaml"
    elif test_mode == "prod":
        file_path = "../crypto/mic.yaml"

    with open(file_path, "w") as mic_file:
        yaml.dump(mic, mic_file)


def generate_eapol_pmkid_message_1_yaml(message_1: list):
    """ Generates a pmkid yaml file that can be read by key_hierarchy_calculation.py """

    ap_ssid = ""
    for (key, value) in scanned_aps.items():
        # * Get the SSID from the scanned access point dictionary by checking if the
        # * transmitter mac address is equal to the key in dictionary
        if key == message_1[2]:
            ap_ssid = value

    # Converts the SSID from hex string to ascii
    ascii_ssid = bytes.fromhex(ap_ssid).decode("utf-8")

    # ! A PSK or passphrase should be provided in the pmkid yaml file
    pmkid_yaml = f"""
    psk: 
    ssid: {ascii_ssid}
    bssid: {message_1[2]}
    sta_mac: {message_1[3]}
    pmkid: {message_1[4]}
    """
    pmkid = yaml.safe_load(pmkid_yaml)

    file_path = ""
    if test_mode == "debug":
        file_path = "../crypto/pmkid_debug.yaml"
    elif test_mode == "prod":
        file_path = "../crypto/pmkid.yaml"

    with open(file_path, "w") as pmkid_file:
        yaml.dump(pmkid, pmkid_file)


def handle_ssid_from_scanned_aps(scan: list):
    """
    This creates new key value pair where key is the mac address and the 
    value is the SSID
    """
    scanned_aps[f"{scan[2]}"] = scan[3]


def generate_eapol_message(value_content: list):
    """ Determine what type of serial output """

    """
    Size of PMKID message 1: 6
    Size of MIC message 1: 6
    Size of scan output: 7
    Size of MIC message 2: 15
    """
    if len(value_content) == 19 or len(value_content) == 6 or len(value_content) == 7:
        if value_content[0] == "MIC" and value_content[1] == "MSG_1":
            generate_eapol_message_1_yaml(value_content)
        elif value_content[0] == "MIC" and value_content[1] == "MSG_2":
            generate_eapol_message_2_yaml(value_content)
        elif value_content[0] == "PMKID" and value_content[1] == "MSG_1":
            generate_eapol_pmkid_message_1_yaml(value_content)
        elif value_content[1] == "SCAN":
            handle_ssid_from_scanned_aps(value_content)


def is_formatted_output(value_str: str):
    """ Determine if the serial output has curly brackets """

    value_list = list(value_str)
    last_item = len(value_list) - 1

    if value_list[0] == "{" and value_list[last_item] == "}":
        print(f"{OUT_YELLOW}module_test.py> {value_str}{COLOR_RESET}")
        return True
    else:
        print(value_str)
        return False


def execute_read_thread(port: Serial):
    """ Listen for serial output and do a processing on those outputs """

    while True:
        if stop_thread.is_set():
            break

        value = port.readline().replace(b'\n', b'').replace(b'\r', b'')
        try:
            value_str = str(value, 'UTF-8')
            if len(value_str) < 1:
                continue
            if not is_formatted_output(value_str):
                continue

            value_list = list(value_str)
            value_list.remove("{")
            value_list.remove("}")
            """
            Convert the list back to string then split it by "," which makes it a 
            list again
            """
            value_content = "".join(value_list).split(",")
            generate_eapol_message(value_content)

        except UnicodeDecodeError:
            pass

    print("module_test.py> Stopped write thread")


def execute_write_thread(port: Serial):
    """ Listen for user input then sends it into the Launcher Module """

    while True:
        if stop_thread.is_set():
            break
        serial_input = input()
        if len(serial_input) < 1:
            continue

        if serial_input == "close":
            stop_thread.set()
        else:
            port.write(serial_input.encode())

    print("module_test.py> Stopped read thread")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("test_mode")
    args = parser.parse_args()

    """
    When debug is set the script will write the PMKID or MIC information in 
    mic_debug.yaml or pmkid_debug.yaml
    """
    if args.test_mode == "debug":
        test_mode = "debug"
    elif args.test_mode == "prod":
        test_mode = "prod"
    else:
        print("Invalid argument")
        sys.exit()

    port = Serial(port="COM3", baudrate=19200, timeout=1)
    read_thread = threading.Thread(target=execute_read_thread, args=(port,))
    write_thread = threading.Thread(target=execute_write_thread, args=(port,))
    read_thread.start()
    write_thread.start()
