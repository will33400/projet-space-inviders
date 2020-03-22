
# projet-space-inviders

# Objectives

* Code Space Invaders handled by a serial monitor with STM32F4 Discovery card

# Installation
* Install Atollic TrueSTUDIO https://atollic.com/resources/download/
* Open the program space_invaders
* plug the STM32F4 Discovery card equiped with Discovery shield by the two USB ports (serial and program) to your computer
* compile and throw then quit the debug by typing F11 then Ctrl+F2 to transfer the program

 ## Windows
* install PuTTY
* select Serial, type the right port number (could be found on Control Panel>view devices and printers>FT232R USB UART>(right-click)properties>hardware)
* type 115200 baudrate (Speed statement)
* click Open

## Linux
* type the command $ screen /dev/ttyACM* 115200

# Controls

## Keyboard
* s         Start game
* q         LEFT
* d         RIGHT
* ENTER     SHOOT
* p         PAUSE

## STM32F4 Discovery
* Black button to RESET (CAUTION : causes highscore reset)