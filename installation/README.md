# Installation

You can install the program with ESP-IDF by building the binaries `idf.py build` then flash `idf.py flash`

If you don't have the ESP-IDF setup, you can use the binaries Awps.bin, bootloader.bin and partition-table.bin to directly flash them using [`esptool.py`](https://github.com/espressif/esptool) which requires Python to be installed

## Download Python and esptool

esptool is a Python-based, open-source, platform-independent utility to communicate with the ROM bootloader in Espressif chips. You need to have Python and pip installed on your computer. Then install esptool via pip `pip install esptool`

### Flash
```
python -m esptool -p COM3 -b 460800 --before default_reset --after hard_reset --chip esp32 write_flash --flash_mode dio --flash_freq 80m --flash_size detect 0x1000 bootloader.bin 0x10000 Awps.bin 0x8000 partition-table.bin
```
Where COM3 is the port of the device, this depends on the operating system you are using, for example in linux the port might be /dev/ttyS4