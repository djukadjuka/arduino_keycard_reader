#ifndef __SERVO_MODULE_H
#define __SERVO_MODULE_H

#include <Servo.h>

#define SERVO_PIN           (4)
#define SERVO_ANGLE_CLOSED  (-180)
#define SERVO_ANGLE_OPEN    (180)

Servo myservo;

class ServoModule{
    int pos;

public:

    void reset(){
        this->pos = SERVO_ANGLE_CLOSED;
        myservo.write(this->pos);
    }

    ServoModule(){
        this->pos = SERVO_ANGLE_CLOSED;
    }

    void open_door(){
        this->pos = SERVO_ANGLE_OPEN;
        myservo.write(pos);
    }

    void close_door(){
        this->pos = SERVO_ANGLE_CLOSED;
        myservo.write(pos);
    }

};

#endif // __SERVO_MODULE_H