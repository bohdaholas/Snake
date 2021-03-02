class Snake {
      
};

class Apple {
    
};

void turn_on_led(int row, int col) {
  turn_off_all_leds();
  if (1 <= row <= 6) {
    pinMode(row + 1, OUTPUT);
    digitalWrite(row + 1, 0);
  }
  if (1 <= col <= 6) {
    pinMode(col + 7, OUTPUT);
    digitalWrite(col + 7, 1);
  }
}

void turn_off_all_leds() {
  for (int i = 0; i <= 13; i++) {
        pinMode(i, INPUT);
  }  
}    

void turn_on_all_leds() {
  for (int i = 1; i <= 7; i++) {
      pinMode(i, OUTPUT);
      digitalWrite(i, 0);
  }  
  for (int i = 8; i <= 13; i++) {
      pinMode(i, OUTPUT);
      digitalWrite(i, 1);
  }  
}

void joystick() {
  int y = analogRead(A0);
  int x = analogRead(A1);
  Serial.print(y * 5 / 1023);
  Serial.print(" ");
  Serial.println(x * 5 / 1023);  
}

void setup() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    Serial.begin(9600);
}

void loop() {
  
}
