 #include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7,6,5,4,3);

const int field_x = 84;
const int field_y = 48;

const int BUTTON_PIN = 2;

bool joystick_click;

int pin_c1 = 2;
int pin_a = 13;
int pin_f = 1;
int pin_b = 12;
int pin_e = 8;
int pin_d = 9;
int pin_c = 10;
int pin_g = 11;

int segment_pins[8] = {pin_c1, pin_a, pin_f, pin_b, pin_e, pin_d, pin_c, pin_g};

int zero[6] = {pin_a, pin_f, pin_b, pin_e, pin_d, pin_c};
int one[2] = {pin_b, pin_c};
int two[6] = {pin_a, pin_f, pin_b, pin_e, pin_d, pin_c};
int three[5] = {pin_a, pin_b, pin_e, pin_d, pin_g};
int four[4] = {pin_g, pin_f, pin_b, pin_d};
int five[5] = {pin_a, pin_f, pin_g, pin_d, pin_c};
int six[6] = {pin_a, pin_f, pin_e, pin_d, pin_c, pin_g};
int seven[3] = {pin_a, pin_b, pin_c};
int eight[7] = {pin_a, pin_f, pin_b, pin_e, pin_d, pin_c, pin_g};
int nine[5] = {pin_a, pin_f, pin_b, pin_c, pin_g};

//int numbers[10] = {zero, one, two, thre/e, four, five, six, seven, eight, nine};

int apple_x, apple_y;
bool apple_on_field = false;

int score;

bool dead = false;

int head_x = random(1, 84);
int head_y = random(1, 48);

int snake_x[field_x] = {head_x, head_x + 1, head_x + 2, head_x + 3, head_x + 4, head_x + 5, head_x + 6, head_x + 7, head_x + 8, head_x + 9, head_x + 10};
int snake_y[field_y] = {head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y};

//int snake_x[field_x] = {head_x, head_x + 1, head_x + 2};
//int snake_y[field_y] = {head_y, head_y, head_y};

int snake_max_length = field_x * field_y;

int x_diff, y_diff;

bool is_close(int VRx, int expected_VRx, int VRy, int expected_VRy) {
  int accuracy = 350;
    if ( abs(VRx - expected_VRx) < accuracy &&
          abs(VRy - expected_VRy) < accuracy) {
        return true;
    }
    return false;
}

bool self_kill(){
  for(int i = 1; i < current_snake_length(); i++){
    if(snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0]){
      return true;
    }
  }
  return false;
}

void print_joystick(int VRx, int VRy) {
  Serial.print(VRx);
  Serial.print("  ");
  Serial.println(VRy);
}

void read_joystick() {
  int VRx = analogRead(A0);
  int VRy = analogRead(A1);
  int click_state = digitalRead(BUTTON_PIN);
  if (is_close(VRx, 0, VRy, 500) && y_diff != -1) {
    x_diff = 0;
    y_diff = 1;
  }
  if (is_close(VRx, 980, VRy, 480) && y_diff != 1) {
    x_diff = 0;
    y_diff = -1;
  }
  if (is_close(VRx, 500, VRy, 1000) && x_diff != -1) {
    x_diff = 1;
    y_diff = 0;
  }
  if (is_close(VRx, 500, VRy, 0) && x_diff != 1) {
    x_diff = -1;
    y_diff = 0;
  }
  if (click_state == 1){
    joystick_click = false;
  }
  else if(click_state == 0){
    joystick_click = true;
  }
}

int current_snake_length() {
  for (int i = 0; i < snake_max_length; i++) {
    if (snake_x[i] == 0 && snake_x[i + 1] == 0 &&
        snake_y[i] == 0 && snake_y[i + 1] == 0) {
        return i;
    }
  }  
}

void snake_move() {
  for (int i = current_snake_length()-1; i > 0; i--) {
    snake_x[i] = snake_x[i - 1];
    snake_y[i] = snake_y[i - 1];
  } 
  snake_x[0] += x_diff;
  snake_y[0] += y_diff;
}

void snake_teleport() {
  for (int i = 0; i < current_snake_length(); i++) {
    if (snake_x[i] == 85) {
      snake_x[i] = 2;
    }
    if (snake_x[i] == 1) {
      snake_x[i] = 84;
    }
    if (snake_y[i] == 49) {
      snake_y[i] = 2;
    }
    if (snake_y[i] == 1) {
      snake_y[i] = 48;
    }
  } 
}

void generate_snake() {
  display.clearDisplay();
  for (int i = 0; i < current_snake_length(); i++) {
    display.drawPixel(snake_x[i], snake_y[i], BLACK);
  }
  
  display.drawPixel(apple_x, apple_y, BLACK);
  
  display.display();
  delay(100);
}

void snake_eat() {
  if (!apple_on_field){
    generate_apple();
  }

  if (snake_x[0] == apple_x && snake_y[0] == apple_y){
    score++;
    apple_on_field = false;
    int snake_length = current_snake_length();
    snake_x[snake_length] = snake_x[snake_length - 1] - x_diff;
    snake_y[snake_length] = snake_y[snake_length - 1] - y_diff;
  }

  
}

void generate_apple(){
  apple_x = random(1, 84);
  apple_y = random(1, 48);
  apple_on_field = true;
}

void snake_reborn(){
    Serial.println(current_snake_length());
    apple_on_field = false;   
    head_x = random(1, 84);
    head_y = random(1, 48);
    
    int new_snake_x[field_x] = {head_x, head_x + 1, head_x + 2, head_x + 3, head_x + 4, head_x + 5, head_x + 6, head_x + 7, head_x + 8, head_x + 9, head_x + 10};
    int new_snake_y[field_y] = {head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y, head_y};

    for(int i = 0; i < field_x; i++){
      snake_x[i] = new_snake_x[i];
    }
    for(int i = 0; i < field_y; i++){
      snake_y[i] = new_snake_y[i];
    }
    dead = false;
    score = 0;
    Serial.println(current_snake_length());
}

void init_display() {
  display.begin();
  display.clearDisplay();
  display.display();
  display.setContrast(40);
  delay(500);
}

void init_7_segments_board() {
  for (int i = 0; i < 8; i++) {
    pinMode(segment_pins[i], OUTPUT);  
  }
}

//void light_number(int score) {
//  for (int i = 0; i < 8; i++) {
//    digitalWrite(segment_pins[i], 0);  
//  }
//  digitalWrite(pin_c1, 1);
//  for (int i = 0; i < 8; i++) {
//    digitalWrite(number[i], 1);  
//  } 
//}

void light_number() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segment_pins[i], 0);  
  }
  digitalWrite(pin_c1, 1);
  for (int i = 0; i < 2; i++) {
    digitalWrite(one[i], 1);  
  } 
}


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), snake_reborn, FALLING);
  init_display();
}

void loop() {
  read_joystick();
  if (!dead){
    generate_snake();
    if (x_diff || y_diff){
      snake_move(); 
    }
    snake_teleport();
    if (self_kill()){
      dead = true;
    }
    snake_eat();
  }
  else if (joystick_click){
      snake_reborn();
  }
  
  init_7_segments_board();
  light_number();
} 
