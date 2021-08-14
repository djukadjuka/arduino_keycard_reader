#ifndef __RFID_MODULE_H
#define __RFID_MODULE_H

#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
// MFRC522 config
#define SS_PIN 			(10)
#define RST_PIN 		(9)
#define RFID_BYTES_SIZE (4)
#define KEY_NOT_FOUND (-1)
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define RFID_KEY_BYTES (4)
#define RFID_NUMBER_OF_STORED_KEYS (0)
#define RFID_MASTER_ADDRESS (1)
#define RFID_MASTER_KEY_MAGIC_NUMBER (147)

class RFIDModule{

    //  Use to wipe eeprom
    //  For testing purposes
    //  Can be used in programming state
    void wipe_EEPROM(){
        // LOOP for entire EEPROM address
        for (uint16_t x = 0; x < EEPROM.length(); x++)
        {
            // Do not write 0 to every address regardless if it is or isn't 0
            // because the write takes 3.3ms
            if (EEPROM.read(x) == 0)
            {
                // If the address at X is 0, this address is clear
            }
            else
            {
                EEPROM.write(x, 0);
            }
        }
    }
    
public:

    // A key that is read on the RFID module
    byte key_read[RFID_KEY_BYTES];

    // A key that was last read on the RFID module after get_id
    byte last_key_read[RFID_KEY_BYTES];
    
    // Flag set true if a master key exists
    bool master_key_exists;
    
    void reset(){
        this->wipe_EEPROM();
        this->master_key_exists = false;
    }

    RFIDModule(){
        
    }

    // Checks if master key is memorized
    void check_if_master_key_in_memory(){
        if(EEPROM.read(RFID_MASTER_ADDRESS) != RFID_MASTER_KEY_MAGIC_NUMBER){
            this->master_key_exists = false;
        }
        else{
            this->master_key_exists = true;
        }
    }

    bool get_master_key_exists(){
        return this->master_key_exists;
    }

    // Gets the ID of a scanned key and store into key_read, return TRUE on successfull read
    uint8_t get_id(){
        // If a new key is placed on the reader then break out
        if(!mfrc522.PICC_IsNewCardPresent()){
            return FALSE;
        }
        // Since a PICC placed get Serial and continue
        if(!mfrc522.PICC_ReadCardSerial()){
            return FALSE;
        }
        // PICCs have 4b or 7b UID
        // Lets say there are 4  byte UIDs
        // TODO: Add support for 7 byte UIDs
        for(uint8_t i = 0;  i < RFID_BYTES_SIZE;  i++){
            last_key_read[i] = key_read[i];
            key_read[i] = mfrc522.uid.uidByte[i];
        }

        mfrc522.PICC_HaltA(); // stop reading
        return TRUE;
    }
    
    // Increases number of stored keys on EEPROM 0 by 1
    void increment_stored_keys(){
        EEPROM.write(RFID_NUMBER_OF_STORED_KEYS, EEPROM.read(RFID_NUMBER_OF_STORED_KEYS) + 1);
    }
    
    // Decreases number of stored keys on EEPROM 0 by 1
    void decrement_stored_keys(){
        EEPROM.write(RFID_NUMBER_OF_STORED_KEYS, EEPROM.read(RFID_NUMBER_OF_STORED_KEYS) - 1);
    }

    // Returns the number of stored keys
    int get_number_of_stored_keys(){
        return EEPROM.read(RFID_NUMBER_OF_STORED_KEYS);
    }

    // Returns true if the cards are the same
    bool compare_two_keys(byte key1[], byte key2[]){
        for(int i=0;    i < RFID_BYTES_SIZE;    i++){
            if(key1[i] != key2[i]){
                return FALSE;
            }
        }
        return TRUE;
    }

    // Reads a key on a given position and filles array[]
    void get_key_from_EEPROM_on_position(int position, byte array[]){
        for(int eeprom_idx = position * RFID_BYTES_SIZE + 2, key_idx = 0; eeprom_idx < position * RFID_BYTES_SIZE + 2 + RFID_BYTES_SIZE; eeprom_idx++, key_idx++){
            array[key_idx] = EEPROM.read(eeprom_idx);
        }
    }

    // Returns TRUE if key can be found in EEPROM memory
    bool key_exists_in_memory(byte key[]){
        int number_of_keys = this->get_number_of_stored_keys();
        byte loaded_key[RFID_BYTES_SIZE];
        for(int i=0;    i < number_of_keys;  i++){
            this->get_key_from_EEPROM_on_position(i, loaded_key);
            if(this->compare_two_keys(key, loaded_key) == TRUE){
                return TRUE;
            }
        }
        return FALSE;
    }

    // Returns TRUE if scanned key is MASTER KEY
    bool is_key_master_key(byte key[]){
        byte loaded_key[RFID_BYTES_SIZE];
        this->get_key_from_EEPROM_on_position(0, loaded_key);
        return this->compare_two_keys(key, loaded_key);
    }

    // Writes a key to a specific position
    // Does not increment number of keys
    void write_key_to_position(int position, byte key[]){
        for(int eeprom_idx = position * RFID_BYTES_SIZE + 2, key_idx = 0; eeprom_idx < position * RFID_BYTES_SIZE + 2 + RFID_BYTES_SIZE;    eeprom_idx++, key_idx++){
            EEPROM.write(eeprom_idx, key[key_idx]);
        }
    }

    // Gets the index of the key in memory or -1 if it doesn't exist
    int get_key_position(byte key[]){
        int number_of_keys_in_memory = this->get_number_of_stored_keys();
        byte loaded_key[RFID_BYTES_SIZE];
        for(int i=0;    i < number_of_keys_in_memory;   i++){
            this->get_key_from_EEPROM_on_position(i, loaded_key);
            if(this->compare_two_keys(loaded_key, key) == TRUE){
                return i;
            }
        }
        return KEY_NOT_FOUND;
    }

    // Removes a key from memory and does all housekeeping stuff
    bool remove_key_from_memory(byte key[]){
        // Check if the key is in memory
        int key_index_in_memory = this->get_key_position(key);
        if(key_index_in_memory == KEY_NOT_FOUND){
            return FALSE;
        }
        // Write last key to its index
        byte loaded_last_key[RFID_BYTES_SIZE];
        this->get_key_from_EEPROM_on_position(this->get_number_of_stored_keys()-1, loaded_last_key);
        this->write_key_to_position(key_index_in_memory, loaded_last_key);
        // decrease number of stored keys
        this->decrement_stored_keys();
        return TRUE;
    }

    // Adds a key to memory and does all housekeeping stuff
    bool add_key_to_memory(byte key[]){
        // Check if the key is in memory
        int key_index_in_memory = this->get_key_position(key);
        // If its in memory it cannot be added
        if(key_index_in_memory != KEY_NOT_FOUND){
            return FALSE;
        }
        // Write this key to the last index
        this->write_key_to_position(this->get_number_of_stored_keys(), key);
        // Increase number of keys
        this->increment_stored_keys();
        return TRUE;
    }
    
    // Saves key to master key position, and saves magic number
    void save_master_key(byte key[]){
        for(int i=0;    i < RFID_BYTES_SIZE;    i++){
            EEPROM.write(2+i, key[i]);
        }
        EEPROM.write(RFID_MASTER_ADDRESS, RFID_MASTER_KEY_MAGIC_NUMBER);
        this->master_key_exists = true;
        this->increment_stored_keys();
    }

};

#endif // __RFID_MODULE_H