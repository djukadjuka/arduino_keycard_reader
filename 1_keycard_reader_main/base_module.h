#ifndef __BASE_MODULE_H
#define __BASE_MODULE_H

class BaseModule{

protected:
    unsigned long last_update_millis;
    unsigned long current_millis;
    unsigned int internal_state;

    void change_state(unsigned int next_state){
        this->internal_state = next_state;
        this->last_update_millis = current_millis;
    }

public:

    BaseModule(){

    }

};

#endif // __BASE_MODULE_H