## Launcher Module Testing

The script `module_test.py` serves the purpose of validating the accuracy and correctness of the Launcher Module. It accomplishes this by extracting pertinent data, such as MIC and PMKID, from the output of the module, and subsequently creating a YAML file for storing the output. The extracted information within this YAML file can then be accessed and processed by the script `key_hierarchy_calculation.py`, situated in the crypto directory.

Within the `key_hierarchy_calculation.py` script, the system performs key computations, resulting in the generation of essential cryptographic keys, namely the PMK, PTK, and MIC or PMKID.  If the calculated MIC or PMKID aligns with the corresponding output, the Launcher Module is working as expected.

### Install packages
```shell
pip install pyserial
pip install serial
```

### Launch the script
```shell
python serial_com.py
```