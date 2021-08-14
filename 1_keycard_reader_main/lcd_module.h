#ifndef __LCD_MODULE_H
#define __LCD_MODULE_H

#include "base_module.h"
//
// LCD constants
//
#define LCD_ROWS (2)
#define LCD_COLS (16)
#define LCD_ROTATE_START_TIME (2000)
#define LCD_ROTATE_END_TIME   (2000)
// ==========================================================================================
//
// Include Wire Library for I2C
//
#include <Wire.h>
//
// Include NewLiquidCrystal Library for I2C
//
#include <LiquidCrystal_I2C.h>
//
// Define LCD pinout
//
const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
//
// Define I2C Address - change if reqiuired
//
const int i2c_addr = 0x27;
LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

class LCDModule{

    void print_string_rotate_inner(){
        lcd.setCursor(0, 0);
        // Configure last index of first string    
        int substring_end_index = this->last_rotate_index + LCD_COLS;
        if(substring_end_index > this->rotate_text_top.length()){
            substring_end_index = this->rotate_text_top.length();
        }
        lcd.print(this->rotate_text_top.substring(this->last_rotate_index, substring_end_index));

        lcd.setCursor(0, 1);
        substring_end_index = this->last_rotate_index + LCD_COLS;
        if(substring_end_index > this->rotate_text_bot.length()){
            substring_end_index = this->rotate_text_bot.length();
        }
        lcd.print(this->rotate_text_bot.substring(this->last_rotate_index, substring_end_index));
    }

public:
    LCDModule(){
        this->last_update_millis = millis();
    }

    void print_string(String string){
        String lcd_strings[LCD_ROWS];
        short row = 0;

        int string_index = 0;
        String word = "";

        while(string[string_index] != '\0'){

            if(string[string_index] == ' '){
                if(word.length() + lcd_strings[row].length() > LCD_COLS){
                    row++;
                }
                lcd_strings[row] += word;
                if(lcd_strings[row].length() + 1 <= LCD_COLS){
                    lcd_strings[row] += " ";
                }
                word = "";
            }else{
                word += string[string_index];
            }
            
            string_index++;
        }
        if(word.length() + lcd_strings[row].length() > LCD_COLS){
            row++;
        }
        lcd_strings[row] += word;
        

        lcd.clear();
        lcd.setCursor(0, 0);    
        lcd.print(lcd_strings[0]);
        lcd.setCursor(0, 1);
        lcd.print(lcd_strings[1]);
    }

    void reset(){
        this->last_update_millis = millis();
        lcd.clear();
        lcd.setCursor(0, 0); 
    }
    
    unsigned long last_update_millis;
    String rotate_text_top;
    String rotate_text_bot;
    int max_rotate_index;
    int last_rotate_index;

    void configure_rotate_text(String string){
        string.trim();
        int string_length = string.length();
        
        this->rotate_text_top = "";
        this->rotate_text_bot = "";

        int string_index = 0;
        // Fill out the first string
        for(;    string_index < string_length;  string_index++){
            if(string[string_index] == ' ' && this->rotate_text_top.length() > string_length / 2){
                string_index++;
                break;
            }
            this->rotate_text_top += string[string_index];
        }
        
        // Fill out the second string
        for(;   string_index < string_length;   string_index++){
            this->rotate_text_bot += string[string_index];
        }

        last_rotate_index = 0;
        max_rotate_index = ((this->rotate_text_top.length() > this->rotate_text_bot.length()) ? this->rotate_text_top.length() : this->rotate_text_bot.length()) - LCD_COLS;
    }

	void print_string_rotate(unsigned long current_millis, unsigned int rotate_time){
        // First time the string is printed
        // Print the string with no problems
        if(this->last_rotate_index == 0){
            this->print_string_rotate_inner();
            this->last_rotate_index++;
            this->last_update_millis = current_millis;
        // If the string was printed for the first time (next index is 1), wait for
        // LCD_ROTATE_START_TIME
        }else if(this->last_rotate_index == 1 && current_millis - this->last_update_millis < LCD_ROTATE_START_TIME){
            return;
        // If the string index is more than max rotate index, then you printed the whole string
        }else if(this->last_rotate_index > this->max_rotate_index){
            if(current_millis - this->last_update_millis < LCD_ROTATE_END_TIME){
                return;
            }else{
                this->last_rotate_index = 0;
                this->last_update_millis = current_millis;
            }
        // If all of this passes, print the string if enough time passed and increase last rotate index regardless
        }else if(current_millis - this->last_update_millis >= rotate_time){
            this->print_string_rotate_inner();
            this->last_rotate_index++;
            this->last_update_millis = current_millis;
        }
	}

};

#endif // __LCD_MODULE_H