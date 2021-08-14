- Made for arduino uno
- See _arduino_pins.png_ for pin setup.
- Main directory is 1_keycard_reader_main, flash project from that directory
- Other directories made for testing individual components and finding i2c address
- Use LCD_find_i2c_address to check for the address of the lcd screen i2c bus. This address needs to then be used
in test_i2c_LCD and other lcd projects.
