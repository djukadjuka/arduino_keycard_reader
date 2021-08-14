// Flash leds

#define LED_OK (7)
#define LED_NO (6)
#define LED_PG (5)

#define DELAY_LONG    (2000)
#define DELAY_MEDI    (1000)
#define DELAY_SHOR    (200)
#define DELAY_VSHO    (50)

#define FLASH_X_TIMES (5)
#define START_PROGRAM_FLASH_X_TIMES (10)

#define FLASH_ROTATE (3)

void setup() {
  Serial.begin(9600);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_NO, OUTPUT);
  pinMode(LED_PG, OUTPUT);

  Serial.println("> flash_LEDs start");
  flash_start();
}

void loop() {
  flash_constant();
  flash_blink();
  flash_example_one_by_one();
}

void flash_constant(){
  // FLASH CONSTANT
  Serial.println("Constant flash.");
  on_all();
  delay(DELAY_LONG);
  off_all();
  delay(DELAY_MEDI);
}

void flash_blink(){
  // FLASH IN SUCCESSION
  Serial.print("Short flash "); Serial.print(FLASH_X_TIMES); Serial.println(" times.");
  for(int i=0;  i < FLASH_X_TIMES;  i++){
    Serial.print("Flash ["); Serial.print(i); Serial.println("]");
    on_all();
    delay(DELAY_SHOR);
    off_all();
    delay(DELAY_SHOR);
  }
}

void flash_start(){
  // Flash quick to designate program start
  Serial.println("Program starting ...");
  Serial.print("You will see "); Serial.print(START_PROGRAM_FLASH_X_TIMES); Serial.println(" quick flashes");
  for(int i=0;  i < START_PROGRAM_FLASH_X_TIMES;  i++){
    on_all();
    delay(DELAY_VSHO);
    off_all();
    delay(DELAY_VSHO);
  }

  Serial.println("Initialization done.");
  delay(1000);
}

void flash_example_one_by_one(){
  off_all();
  Serial.print("Flashig "); Serial.print(FLASH_ROTATE); Serial.println(" times one by one LED");
  for(int i=0;  i < FLASH_ROTATE; i++){
    flash_one_by_one();
  }
}

void flash_one_by_one(){
  off_all();
  on_ok();
  delay(DELAY_MEDI);
  off_ok();
  on_no();
  delay(DELAY_MEDI);
  off_no();
  on_pg();
  delay(DELAY_MEDI);
  off_pg();
  delay(DELAY_MEDI);
}

void on_ok(){
  on(LED_OK);
}

void on_no(){
  on(LED_NO);
}

void on_pg(){
  on(LED_PG);
}

void off_ok(){
  off(LED_OK);
}

void off_no(){
  off(LED_NO);
}

void off_pg(){
  off(LED_PG);
}

void on(int led){
  digitalWrite(led, HIGH);
}
void off(int led){
  digitalWrite(led, LOW);
}

void on_all(){
  on_ok();
  on_no();
  on_pg();
}
void off_all(){
  off_ok();
  off_no();
  off_pg();
}
