#include <Servo.h>

#define SERVO_PIN         (4)
#define SERVO_MOVE_DELAY  (5)

Servo myservo;

int pos = 0;

void setup() {
  myservo.attach(SERVO_PIN);

  Serial.begin(9600);

  Serial.println("Servo will start from position 0, and move to position 180, then back to 0.");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Servo movement starting ...");

  // for(pos = 0;  pos <= 180; pos +=1){
  //   myservo.write(pos);
  //   delay_servo();
  // }

  // delay(500);

  // for(pos = 180; pos >= 0; pos -= 1){
  //   myservo.write(pos);
  //   delay_servo();
  // }

  myservo.write(180);

  delay(1000);

  myservo.write(-180);

  delay(1000);
}

void delay_servo(){
  delay(SERVO_MOVE_DELAY);
}
