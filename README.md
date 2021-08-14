- Made for arduino uno
- See _arduino_pins.png_ for pin setup.
- Main directory is 1_keycard_reader_main, flash project from that directory
- Other directories made for testing individual components and finding i2c address
- Use _LCD_find_i2c_address_ project to check for the address of the lcd screen i2c bus. This address needs to then be used
in test_i2c_LCD and other lcd projects.
- _pins.txt_ contains pin map for arduino uno configuration.


How to use
===
- Initially (and if EEPROM reset), there will be no master key
    - To set master key, scan same card twice when prompted on LCD
    - If it takes too long to scan same card twice you will be prompted to scan a card for the first time again
    - If different cards scanned you will be prompted to scan a card for the first time again
    - Master key (MK) is now set and can be used to enter, or activate programming mode
- Scanning saved card will send signal to servo to open and activates GRANTED led diode
- Scanning unknown card will not send any signals to servo and activates DENIED led diode
- Scanning MK and in time interval of X seconds scanning it again will activate PROGRAMMING mode
- While in PRORGRAMMING mode
    - If new card scanned, it will be saved and GRANTED flash
    - If existing card scanned, it will be removed and DENIED flash
    - If MK scanned, exits programming mode
- If MK exists, activating device again will find it and enter normal mode for waiting for new cards
- Holding push button for ~2 secs will wipe EEPROM and enter initial state where no MK is set - USE AT OWN RISK
