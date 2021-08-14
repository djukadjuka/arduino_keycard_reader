#ifndef __LED_MODULE_H
#define __LED_MODULE_H

#include "Arduino.h"

#define LED_GRANTED_PIN (7)
#define LED_DENIED_PIN (6)
#define LED_PROG_PIN (5)

class LEDModule{

    unsigned long last_inner_update_time;
    unsigned long on_time;
    unsigned long off_time;

public:

    void set_last_inner_update_time(unsigned long time){
        this->last_inner_update_time = time;
    }

    void set_on_time(unsigned long on_time){
        this->on_time = on_time;
    }

    void set_off_time(unsigned long off_time){
        this->off_time = off_time;
    }

    void set_on_off_time(unsigned long on_time, unsigned long off_time){
        this->set_on_time(on_time);
        this->set_off_time(off_time);
    }

    void reset(){
        this->set_last_inner_update_time(0);
    }

    LEDModule(){
        this->set_last_inner_update_time(millis());
    }

    void led_on(int led_id){
        digitalWrite(led_id, HIGH);
    }
    
    void led_off(int led_id){
        digitalWrite(led_id, LOW);
    }
    
    void on_granted(){
        this->led_on(LED_GRANTED_PIN);
    }
    
    void on_denied(){
        this->led_on(LED_DENIED_PIN);
    }
    
    void on_prog(){
        this->led_on(LED_PROG_PIN);
    } 
    
    void off_granted(){
        this->led_off(LED_GRANTED_PIN);
    }
    
    void off_denied(){
        this->led_off(LED_DENIED_PIN);
    }
    
    void off_prog(){
        this->led_off(LED_PROG_PIN);
    }
    
    short is_led_on(int led_id){
        if(digitalRead(led_id)){
            return TRUE;
        }
        return FALSE;
    }

    void toggle_led(int led_id){
        if(is_led_on(led_id) == TRUE){
            led_off(led_id);
        }else{
            led_on(led_id);
        }
    }

    void off_all(){
        this->off_prog();
        this->off_granted();
        this->off_denied();
    }

    void on_all(){
        this->on_prog();
        this->on_granted();
        this->on_denied();
    }

    unsigned short toggle_diff(unsigned long current_millis, unsigned long on_time, unsigned long off_time, int led_id){
        // Returns TRUE if led is toggled
        short led_state = is_led_on(led_id);

        unsigned long update_time;
        if(led_state == FALSE){
            update_time = off_time;
        }else{
            update_time = on_time;
        }
        
        if(current_millis - this->last_inner_update_time >= update_time){
            this->toggle_led(led_id);
            this->last_inner_update_time = current_millis;
            return TRUE;
        }else{
            return FALSE;
        }
    }


};

#endif // __LED_MODULE_H