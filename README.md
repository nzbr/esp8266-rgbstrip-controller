# ESP8266 RGB-Strip controller

## Configuring

Copy `src/config-template.h` to `src/config.h`

The following settings **must** be changed:
- Uncomment _one_ device driver to be used
- Enter your WiFi credentials (WIFISSID and WIFIPASS)

## How-To

1. Install [PlatformIO Core](https://platformio.org/install/cli)
	- On most systems this boils down to `pip3 install --user -U platformio`
2. Connect your ESP8266 board via USB
3. Execute `pio run`
	- This will flash the software to the board and open a serial terminal afterwards
		- You can safely close the serial terminal by pressing CTRL+C
	- If the flash falis on Linux you may want to use `sh linuxrun.sh` instead
		- This assumes that the serial connection registers as `/dev/ttyUSB0`
