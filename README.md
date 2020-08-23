# blackboard-esp32
Connect to the on-board ESP32 on the [RealDigital Blackboard](https://realdigital.org/).

:radioactive:  **Note:** This is only tested on the Rev. B board.


## Current status

| Feature                                 | Status      |
|-----------------------------------------|-------------|
| Simple IO between the ESP32 and Zynq PS | Done        |
| UART interrupts for receiving data      | Done        |
| Simple Tx/Rx queues                     | Done        |
| Wifi connectivity                       | Done        |
| Socket wrapper                          | Done        |
| TCP socket                              | Done        |
| HTTP wrapper                            | Done        |
| Socket & HTTP refactoring               | In progress |
| Wifi AP mode                            | Not started |
| Bluetooth                               | Not started |
| UDP and SSL                             | Not started |
| Documentation                           | Not started |

## Useful Links
* [ESP32 AT Instruction Set Documentation](https://www.espressif.com/sites/default/files/documentation/esp32_at_instruction_set_and_examples_en.pdf) (pdf)
* [RealDigital Blackboard Manual](https://www.realdigital.org/doc/9c908d94497abb1eac41175d1ab05b88) (web)
* [RealDigital Blackboard Rev. B Manual](https://www.realdigital.org/downloads/1a63155aa0d45e6af2d1ca0ea5d1e53a.pdf) (pdf)
* [esp8266-arduino](http://github.com/sharpCoder/esp8266-arduino/) - a similar project but using C++ for Arduino
