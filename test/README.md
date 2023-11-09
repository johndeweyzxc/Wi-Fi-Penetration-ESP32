## Launcher Module Testing

The script `module_test.py` serves the purpose of validating the accuracy and correctness of the Launcher Module. It accomplishes this by data, such as MIC and PMKID, from the output of the module, and subsequently creating a YAML file for storing the output. The extracted information within this YAML file can then be accessed and processed by the script `key_hierarchy_calculation.py`, situated in the crypto directory.

Within the `key_hierarchy_calculation.py` script, the system performs key computations, resulting in the generation of essential cryptographic keys, namely the PMK, PTK, and MIC or PMKID.  If the calculated MIC or PMKID aligns with the corresponding output, the Launcher Module is working as expected.

### Install packages
```shell
pip install pyserial
pip install serial
pip install PyYaml
```

### Launch the script
```shell
python module_test.py debug
```
Where debug is set so that it knows which file it should store the output. If debug is set for example it will generate pmkid_debug.yaml or mic_debug.yaml in the crypto directory but if prod is set then it will generate pmkid.yaml or mic.yaml

## DISCLAIMER

The project AWPS is intended for educational purposes, with the primary goal of raising awareness and understanding of cybersecurity in a legal and ethical context. It is essential to clarify that this tool is NOT INTENDED to encourage or promote any form of unauthorized or unethical hacking activities. Ethical hacking, conducted with proper authorization and consent, plays a crucial role in enhancing the security of digital systems. This project seeks to promote responsible use of technology and responsible disclosure of vulnerabilities to help protect and secure digital environments.

USAGE OF ALL TOOLS on this project for attacking targets without prior mutual consent is ILLEGAL. It is the end user’s responsibility to obey all applicable local, state, and federal laws. I assume no liability and are not responsible for any misuse or damage caused by this project or software.