#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "lcd_module.h"
#include "led_module.h"
#include "master_controller.h"
#include "rfid_module.h"
#include "servo_module.h"

#define BACKGROUND_CONTROLLER_PIN (3)

//////////////////////////////////////////////////////////////////////////////////////////////////
// STATES
//////////////////////////////////////////////////////////////////////////////////////////////////
// SS -> State Start
// MK -> Master Key

#define SS_RESET_ALL_MODULES        (0)
#define S_RESET_ALL_MODULES_DELAY   (1)
#define S_RESET_ALL_MODULES_REAL    (2)

#define SS_CHECK_MK_EXISTS      (100)
#define S_CHECK_MK_EXISTS_DELAY (101)
#define S_CHECK_MK_EXISTS_REAL  (102)

#define SS_MASTER_KEY_EXISTS        (200)
#define S_MASTER_KEY_EXISTS_DELAY   (202)

#define SS_MASTER_KEY_DOES_NOT_EXIST        (300)
#define S_MASTER_KEY_DOES_NOT_EXIST_DELAY   (301)

#define SS_WAIT_FOR_NEW_KEYS (400)
#define S_WAIT_FOR_NEW_KEYS (401)

#define SS_NO_MK_ONE_KEY_SCANNED (500)
#define S_NO_MK_ONE_KEY_SCANNED  (501)

#define SS_NO_MK_SECOND_SCAN (600)

#define SS_MK_SET                   (700)
#define S_MK_SET_BLINK_PROG_DELAY   (701)

#define SS_INVALID_SECOND_KEY_SCAN_FOR_MK (800)
#define S_INVALID_SECOND_KEY_SCAN_FOR_MK_DELAY (801)

#define SS_NO_MK_PRINT_INSTRUCTIONS (900)
#define S_NO_MK_PRINT_INSTRUCTIONS_WAIT_FOR_KEY (901)

#define SS_KEY_SCANNED (1000)

#define SS_PROGRAMMING_KEYS_IN_MEMORY (1100)
#define S_PROGRAMMING_KEYS_IN_MEMORY  (1101)

#define SS_SCANNED_KEY_ACCESS_GRANTED (1200)
#define S_SCANNED_KEY_ACCESS_GRANTED_DELAY (1201)

#define SS_SCANNED_KEY_ACCESS_DENIED (1300)
#define S_SCANNED_KEY_ACCESS_DENIED_DELAY (1301)

#define SS_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS (1400)
#define S_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS  (1401)

#define SS_PROGRAMMING_KEY_EXISTS_IN_MEMORY (1500)
#define S_PROGRAMMING_KEY_EXISTS_IN_MEMORY_DELAY (1501)

#define SS_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY (1600)
#define S_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY_DELAY (1601)





//////////////////////////////////////////////////////////////////////////////////////////////////
// LCD STRINGS
//////////////////////////////////////////////////////////////////////////////////////////////////

#define LCD_STR_CHECK_MK_EXISTS (String("Checking for master key..."))
#define LCD_STR_MK_DOESNT_EXIST (String("Master key not found."))
#define LCD_STR_MK_EXISTS (String("Master key found."))
#define LCD_STR_SCAN_MASTER_KEY_INSTRUCTIONS (String("Scan key again to set as master"))
#define LCD_STR_MK_SET (String("Master key saved."))
#define LCD_STRING_INVALID_SECOND_MK_SCAN (String("Invalid second scan."))
#define LCD_STR_MK_DOESNT_EXIST_INSTRUCTIONS (String("Scan any key..."))
#define LCD_STR_WAITING_FOR_NEW_KEYS (String("[ F U R Y ] Industries"))
#define LCD_STR_START_PROGRAMMING_MODE (String("Programming mode beginning."))
#define LCD_STR_PROGRAMMING_KEYS_INSTRUCTIONS (String("Scan master key to exit programming mode. Scan keys that exist to delete them from memory. Scan keys that do not exist to add them to memory."))
#define LCD_STR_ACCESS_GRANTED (String("Access granted."))
#define LCD_STR_ACCESS_DENIED  (String("Access denied."))
#define LCD_STR_KEY_REMOVED_FROM_MEMORY (String("Key removed from memory."))
#define LCD_STR_KEY_ADDED_TO_MEMORY (String("Key added to memory."))








//////////////////////////////////////////////////////////////////////////////////////////////////
// MILLIS
//////////////////////////////////////////////////////////////////////////////////////////////////
#define MS_LED_CHECK_MK_EXISTS_ON                   (75)
#define MS_LED_CHECK_MK_EXISTS_OFF                  (75)
#define MS_STATE_MACHINE_CHECK_DELAY                (2000)
#define MS_RESET_ALL_STATES_DELAY                   (2000)
#define MS_MK_FOUND_TEXT_DELAY                      (2000)
#define MS_MK_NOT_FOUND_TEXT_DELAY                  (2000)
#define MS_LED_ON_MASTER_KEY_DOESNT_EXIST           (2000)
#define MS_LED_OFF_MASTER_KEY_DOESNT_EXIST          (500)
#define MS_LED_ON_NO_MK_ONE_KEY_SCANNED             (100)
#define MS_LED_OFF_NO_MK_ONE_KEY_SCANNED            (100)
#define MS_NO_MK_ON_KEY_SCANNED_WAIT_DELAY          (5000)
#define MS_LED_ON_INVALID_MK                        (250)
#define MS_LED_OFF_INVALID_MK                       (100)
#define MS_LED_ON_MK_SET                            (200)
#define MS_LED_OFF_MK_SET                           (200)
#define MS_MK_SET_DELAY_PROG                        (2000)
#define MS_INVALID_MK_DELAY                         (2000)
#define MS_DELAY_PROGRAMMING_MODE_START             (2000)
#define MS_LED_ON_PROGRAMMING_MODE_WAIT             (2000)
#define MS_LED_OFF_PROGRAMMING_MODE_WAIT            (500)
#define MS_LCD_ROTATE_PROGRAMMING_INSTRUCTIONS      (300)
#define MS_ACCESS_GRANTED_DELAY                     (1500)
#define MS_ACCESS_DENIED_DELAY                      (1500)
#define MS_LED_PROGRAMMING_KEY_EXISTS_ON            (250)
#define MS_LED_PROGRAMMING_KEY_EXISTS_OFF           (250)
#define MS_PROGRAMMING_KEY_EXISTS_IN_MEMORY_DELAY   (2000)
#define MS_LED_PROGRAMMING_KEY_DOES_NOT_EXISTS_ON   (250)
#define MS_LED_PROGRAMMING_KEY_DOES_NOT_EXISTS_OFF  (250)
#define MS_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY_DELAY (2000)




class StateMachine{

    LCDModule lcd_module;
    LEDModule led_module;
    RFIDModule rfid_module;
    ServoModule servo_module;

    unsigned int state;
    unsigned int state_reset;
    unsigned int previous_state;

    unsigned long last_update_millis;
    unsigned long last_update_reset_millis;

    void change_state(unsigned int next_state){
        this->previous_state = this->state;
        this->state = next_state;
        this->last_update_millis = millis();
    }

public:
    
    void reset_all_modules(){
        this->lcd_module.reset();
        this->led_module.reset();
        this->rfid_module.reset();
        this->servo_module.reset();

        this->state_reset = SS_RESET_ALL_MODULES;
        this->state = SS_CHECK_MK_EXISTS;
        this->last_update_millis = millis();
    }

    StateMachine(){
        this->state = SS_CHECK_MK_EXISTS;
        this->previous_state = SS_CHECK_MK_EXISTS;
        this->state_reset = SS_RESET_ALL_MODULES;
    }

    void UpdateResetModules(int button_state){
        unsigned long current_millis = millis();

        // Checks button state and switches on that
        if(SS_RESET_ALL_MODULES == this->state_reset){
            if(button_state == LOW){
                this->state_reset = S_RESET_ALL_MODULES_DELAY;
                this->last_update_reset_millis = current_millis;
            }
        }
        // Checks if button released or enough time passed to reset
        else if(S_RESET_ALL_MODULES_DELAY == this->state_reset){
            if(button_state == HIGH){
                this->state_reset = SS_RESET_ALL_MODULES;
            }
            else if(current_millis - this->last_update_reset_millis >= MS_RESET_ALL_STATES_DELAY){
                this->state_reset = S_RESET_ALL_MODULES_REAL;
            }
        }
        // Resets everything to beginning state (Even EEPROM)
        else if(S_RESET_ALL_MODULES_REAL == this->state_reset){
            this->reset_all_modules();
        }
    }

    void Update(){
        Serial.println(this->state);
        unsigned long current_millis = millis();

        // Initial state that sets master key flag when turning on the device
        if(SS_CHECK_MK_EXISTS == this->state){
            this->change_state(S_CHECK_MK_EXISTS_DELAY);

            this->lcd_module.print_string(LCD_STR_CHECK_MK_EXISTS);
            this->led_module.on_prog();
            
            this->rfid_module.check_if_master_key_in_memory();
        }
        // Delay for checking master key existence
        else if(S_CHECK_MK_EXISTS_DELAY == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_CHECK_MK_EXISTS_ON, MS_LED_CHECK_MK_EXISTS_OFF, LED_PROG_PIN);
            if(current_millis - this->last_update_millis >= MS_STATE_MACHINE_CHECK_DELAY){
                this->change_state(S_CHECK_MK_EXISTS_REAL);
                this->led_module.off_all();
            }
        }
        // Checks the master key flag and switches states based on that
        else if(S_CHECK_MK_EXISTS_REAL == this->state){
            if(this->rfid_module.get_master_key_exists()){
                this->change_state(SS_MASTER_KEY_EXISTS);
            }else{
                this->change_state(SS_MASTER_KEY_DOES_NOT_EXIST);
            }
        }
        // Before delaying print for master key exists
        else if(SS_MASTER_KEY_EXISTS == this->state){
            this->change_state(S_MASTER_KEY_EXISTS_DELAY);
            
            this->lcd_module.print_string(LCD_STR_MK_EXISTS);
            this->led_module.off_all();
            this->led_module.on_prog();
            this->led_module.on_granted();
        }
        // Delays for printing lcd master key exists
        else if(S_MASTER_KEY_EXISTS_DELAY == this->state){
            if(current_millis - this->last_update_millis >= MS_MK_FOUND_TEXT_DELAY){
                this->change_state(SS_WAIT_FOR_NEW_KEYS);
            }
        }
        // MILESTONE STATE - master key exists, wait for new keys
        else if(SS_WAIT_FOR_NEW_KEYS == this->state){
            this->servo_module.close_door();
            this->led_module.off_all();
            this->lcd_module.print_string(LCD_STR_WAITING_FOR_NEW_KEYS);
            this->change_state(S_WAIT_FOR_NEW_KEYS);
        }
        // Spins until ANY key is scanned
        else if(S_WAIT_FOR_NEW_KEYS == this->state){
            if(this->rfid_module.get_id() == TRUE){
                this->change_state(SS_KEY_SCANNED);
            }
        }
        else if(SS_KEY_SCANNED == this->state){
            if(this->rfid_module.key_exists_in_memory(this->rfid_module.key_read) == TRUE){
                this->change_state(SS_SCANNED_KEY_ACCESS_GRANTED);
            }
            else{
                this->change_state(SS_SCANNED_KEY_ACCESS_DENIED);
            }
        }
        // Start programming keys when master key is scanned in waiting state
        else if(SS_PROGRAMMING_KEYS_IN_MEMORY == this->state){
            this->led_module.off_all();
            this->led_module.on_prog();
            this->lcd_module.print_string(LCD_STR_START_PROGRAMMING_MODE);
            this->change_state(S_PROGRAMMING_KEYS_IN_MEMORY);
        }
        // Delay print of programming mode
        else if(S_PROGRAMMING_KEYS_IN_MEMORY == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_ON_PROGRAMMING_MODE_WAIT, MS_LED_OFF_PROGRAMMING_MODE_WAIT, LED_PROG_PIN);
            if(current_millis - this->last_update_millis >= MS_DELAY_PROGRAMMING_MODE_START){
                this->change_state(SS_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS);
            }
        }
        // Configure string that is rotated for instrunctions when saving or deleting keys
        else if(SS_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS == this->state){
            this->led_module.off_all();
            this->led_module.toggle_diff(current_millis, MS_LED_ON_PROGRAMMING_MODE_WAIT, MS_LED_OFF_PROGRAMMING_MODE_WAIT, LED_PROG_PIN);
            this->lcd_module.configure_rotate_text(LCD_STR_PROGRAMMING_KEYS_INSTRUCTIONS);
            this->change_state(S_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS);
        }
        // Prints new key instructions and rotates the string
        // Waits for new keys
        else if(S_PROGRAMMING_KEYS_IN_MEMORY_NEW_KEY_INSTRUCTIONS == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_ON_PROGRAMMING_MODE_WAIT, MS_LED_OFF_PROGRAMMING_MODE_WAIT, LED_PROG_PIN);
            this->lcd_module.print_string_rotate(current_millis, MS_LCD_ROTATE_PROGRAMMING_INSTRUCTIONS);
            // Any key scanned
            if(this->rfid_module.get_id() == TRUE){
                if(this->rfid_module.is_key_master_key(this->rfid_module.key_read) == TRUE){
                    this->change_state(SS_WAIT_FOR_NEW_KEYS);
                }
                // Scanned key already exists in memory
                else if(this->rfid_module.key_exists_in_memory(this->rfid_module.key_read) == TRUE){
                    this->change_state(SS_PROGRAMMING_KEY_EXISTS_IN_MEMORY);
                }
                else if(this->rfid_module.key_exists_in_memory(this->rfid_module.key_read) == FALSE){
                    this->change_state(SS_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY);
                }
            }
        }
        // Key exists in memory and should be removed
        else if(SS_PROGRAMMING_KEY_EXISTS_IN_MEMORY == this->state){
            this->led_module.off_all();
            this->led_module.on_denied();
            this->lcd_module.print_string(LCD_STR_KEY_REMOVED_FROM_MEMORY);
            this->change_state(S_PROGRAMMING_KEY_EXISTS_IN_MEMORY_DELAY);
            this->rfid_module.remove_key_from_memory(this->rfid_module.key_read);
        }
        else if(S_PROGRAMMING_KEY_EXISTS_IN_MEMORY_DELAY == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_PROGRAMMING_KEY_EXISTS_ON, MS_LED_PROGRAMMING_KEY_EXISTS_OFF, LED_DENIED_PIN);
            if(current_millis - this->last_update_millis >= MS_PROGRAMMING_KEY_EXISTS_IN_MEMORY_DELAY){
                this->change_state(SS_PROGRAMMING_KEYS_IN_MEMORY);
            }
        }
        // Key does not exist in memory and should be added
        else if(SS_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY == this->state){
            this->led_module.off_all();
            this->led_module.on_granted();
            this->lcd_module.print_string(LCD_STR_KEY_ADDED_TO_MEMORY);
            this->change_state(S_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY_DELAY);
            this->rfid_module.add_key_to_memory(this->rfid_module.key_read);
        }
        else if(S_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY_DELAY == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_PROGRAMMING_KEY_DOES_NOT_EXISTS_ON, MS_LED_PROGRAMMING_KEY_DOES_NOT_EXISTS_OFF, LED_GRANTED_PIN);
            if(current_millis - this->last_update_millis >= MS_PROGRAMMING_KEY_DOES_NOT_EXIST_IN_MEMORY_DELAY){
                this->change_state(SS_PROGRAMMING_KEYS_IN_MEMORY);
            }
        }
        // Key not in memory, deny access
        else if(SS_SCANNED_KEY_ACCESS_DENIED == this->state){
            this->change_state(S_SCANNED_KEY_ACCESS_DENIED_DELAY);
            this->led_module.off_all();
            this->led_module.on_denied();
            this->lcd_module.print_string(LCD_STR_ACCESS_DENIED);
        }
        else if(S_SCANNED_KEY_ACCESS_DENIED_DELAY == this->state){
            if(current_millis - this->last_update_millis >= MS_ACCESS_DENIED_DELAY){
                this->change_state(SS_WAIT_FOR_NEW_KEYS);
            }
        }
        // Key exists in memory, grant access
        else if(SS_SCANNED_KEY_ACCESS_GRANTED == this->state){
            this->lcd_module.print_string(LCD_STR_ACCESS_GRANTED);
            this->change_state(S_SCANNED_KEY_ACCESS_GRANTED_DELAY);
            this->led_module.off_all();
            this->led_module.on_granted();
            this->servo_module.open_door();
        }
        else if(S_SCANNED_KEY_ACCESS_GRANTED_DELAY == this->state){
            // If enough time passes, go back to waiting for keys state
            if(current_millis - this->last_update_millis >= MS_ACCESS_GRANTED_DELAY){
                this->change_state(SS_WAIT_FOR_NEW_KEYS);
            }
            // If another key is scanned
            // If the scanned key is master key
            // If the last key and current key are the same -> go to programming mode
            else if(    this->rfid_module.get_id() == TRUE && 
                        this->rfid_module.is_key_master_key(this->rfid_module.key_read) == TRUE && 
                        this->rfid_module.compare_two_keys(this->rfid_module.last_key_read, this->rfid_module.key_read)){
                this->change_state(SS_PROGRAMMING_KEYS_IN_MEMORY);
            }
        }
        // MILESTONE STATE - master key doesnt exist
        else if(SS_MASTER_KEY_DOES_NOT_EXIST == this->state){
            this->change_state(S_MASTER_KEY_DOES_NOT_EXIST_DELAY);
            this->led_module.off_all();
            this->led_module.on_prog();
            this->lcd_module.print_string(LCD_STR_MK_DOESNT_EXIST);
        }
        // Delay for printing master key not exists
        else if(S_MASTER_KEY_DOES_NOT_EXIST_DELAY == this->state){
            if(current_millis - this->last_update_millis >= MS_MK_NOT_FOUND_TEXT_DELAY){
                this->change_state(SS_NO_MK_PRINT_INSTRUCTIONS);
            }
        }
        // Print instructions for scanning master key
        else if(SS_NO_MK_PRINT_INSTRUCTIONS == this->state){
            this->led_module.off_all();
            this->led_module.on_prog();
            this->lcd_module.print_string(LCD_STR_MK_DOESNT_EXIST_INSTRUCTIONS);
            this->change_state(S_NO_MK_PRINT_INSTRUCTIONS_WAIT_FOR_KEY);
        }
        // Toggles led and waits for any key to be scanned
        else if(S_NO_MK_PRINT_INSTRUCTIONS_WAIT_FOR_KEY == this->state){
            // toggle blinking prog led
            this->led_module.toggle_diff(current_millis, MS_LED_ON_MASTER_KEY_DOESNT_EXIST, MS_LED_OFF_MASTER_KEY_DOESNT_EXIST, LED_PROG_PIN);
            // Check for a scanned MK
            if(this->rfid_module.get_id() == TRUE){
                this->change_state(SS_NO_MK_ONE_KEY_SCANNED);
            }
        }
        // First time a key scanned when waiting for new MK
        else if(SS_NO_MK_ONE_KEY_SCANNED == this->state){
            this->change_state(S_NO_MK_ONE_KEY_SCANNED);
            this->led_module.on_prog();
            this->lcd_module.print_string(LCD_STR_SCAN_MASTER_KEY_INSTRUCTIONS);
        }
        // Toggling and waiting x seconds for another key to be scanned for new MK
        else if(S_NO_MK_ONE_KEY_SCANNED == this->state){
            // Toggle fast
            this->led_module.toggle_diff(current_millis, MS_LED_ON_NO_MK_ONE_KEY_SCANNED, MS_LED_OFF_NO_MK_ONE_KEY_SCANNED, LED_PROG_PIN);
            // If key not scanned within delay, go back a state
            if(current_millis - this->last_update_millis >= MS_NO_MK_ON_KEY_SCANNED_WAIT_DELAY){
                this->change_state(SS_MASTER_KEY_DOES_NOT_EXIST);
            }
            // Key scanned, go to state to check if its the same one
            else if(this->rfid_module.get_id() == TRUE){
                this->change_state(SS_NO_MK_SECOND_SCAN);
            }
        }
        // Second time a key scanned for new MK, checking if they are the same
        else if(SS_NO_MK_SECOND_SCAN == this->state){
            // check if its the same key then go to memorize it as the master key
            if(this->rfid_module.compare_two_keys(this->rfid_module.last_key_read, this->rfid_module.key_read) == TRUE){
                this->change_state(SS_MK_SET);
            }
            // not the same key
            else{
                this->change_state(SS_INVALID_SECOND_KEY_SCAN_FOR_MK);
            }
        }
        // Second key scanned isnt same as first one when waiting for MK
        else if(SS_INVALID_SECOND_KEY_SCAN_FOR_MK == this->state){
            // Set text and go to state for waiting
            this->lcd_module.print_string(LCD_STRING_INVALID_SECOND_MK_SCAN);
            this->led_module.off_all();
            this->led_module.on_denied();
            this->change_state(S_INVALID_SECOND_KEY_SCAN_FOR_MK_DELAY);
        }
        // Toggle denied and wait for printing whats the matter when scanning second key for new MK
        else if(S_INVALID_SECOND_KEY_SCAN_FOR_MK_DELAY == this->state){
            // toggle denied flash
            this->led_module.toggle_diff(current_millis, MS_LED_ON_INVALID_MK, MS_LED_OFF_INVALID_MK, LED_DENIED_PIN);
            // wait for ms and then change state
            if(current_millis - this->last_update_millis >= MS_INVALID_MK_DELAY){
                this->change_state(SS_MASTER_KEY_DOES_NOT_EXIST);
            }
        }
        // Save new master key
        else if(SS_MK_SET == this->state){
            this->lcd_module.print_string(LCD_STR_MK_SET);
            this->change_state(S_MK_SET_BLINK_PROG_DELAY);
            this->rfid_module.save_master_key(this->rfid_module.key_read);
            this->led_module.off_all();
            this->led_module.on_prog();
        }
        // Blink led for saving new master key and delay, goes to waiting for new keys
        else if(S_MK_SET_BLINK_PROG_DELAY == this->state){
            this->led_module.toggle_diff(current_millis, MS_LED_ON_MK_SET, MS_LED_OFF_MK_SET, LED_PROG_PIN);
            if(current_millis - this->last_update_millis >= MS_MK_SET_DELAY_PROG){
                this->change_state(SS_WAIT_FOR_NEW_KEYS);
            }
        }

    }

};

#endif // __STATE_MACHINE_H