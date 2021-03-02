int snake_body_x[64] = {random(1, 6)};
int snake_body_y[64] = {random(1, 6)};

int apple_on_field = false;

void generate_apple() {
  int row, column;
  row = random(1, 6);
  column = random(1, 6);
  turn_on_additional_led(row, column);
  apple_on_field = true;
}

void turn_on_additional_led(int row, int col) {
  if (1 <= row <= 6) {
    pinMode(row + 1, OUTPUT);
    digitalWrite(row + 1, 0);
  }
  if (1 <= col <= 6) {
    pinMode(col + 7, OUTPUT);
    digitalWrite(col + 7, 1);
  }
}

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

int joystick() {
  int y = analogRead(A0) * 5 / 1023;
  int x = analogRead(A1) * 5 / 1023;
  Serial.print(x);
  Serial.print("     ");
  Serial.println(y);
  if (x == 2 && y > 2) {
    // up
    return -2; 
  }
  if (x == 2 && y < 2) {
    // down
    return 2;  
  }
  if (x == 5) {
    return -1;
  }
  if (x == 0) {
    // right
    return 1;   
  }
  
  /*if (x > 3 && y > 3) {
    // left
    return -1;  
  }
  if (x > 3 && y > 3) {
    // right
    return 1;   
  }*/
  
  return 0;
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void snake_move(int dir) {
  if (dir == 2) {
  // up
    snake_body_y[0]++; 
  }
  if (dir == -2) {
    // down
    snake_body_y[0]--; 
  }
  if (dir == -1) {
    // left
    snake_body_x[0]--; 
  }
  if (dir == 1) {
    // right
    snake_body_x[0]++; 
  }
  if (snake_body_x[0] == 7) {
    snake_body_x[0] = 1;
  }
  if (snake_body_x[0] == 0) {
    snake_body_x[0] = 6;
  }
  
  // joystick down
  if (snake_body_y[0] == 7) {
    snake_body_y[0] = 1;
  }

  // joystick up
  if (snake_body_y[0] == 0) {
    snake_body_y[0] = 6;
  }
  turn_on_led(snake_body_x[0], snake_body_y[0]);
}

void loop() {
  int dir = joystick();
  snake_move(dir);
  if (!apple_on_field) {
    generate_apple();
  }
  delay(200);
}
