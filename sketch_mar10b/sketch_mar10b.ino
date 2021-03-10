#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7,6,5,4,3);

const int field_x = 84;
const int field_y = 48;

int apple_x, apple_y;
bool apple_on_field = false;

int score;

int head_x = random(1, 84);
int head_y = random(1, 48);

int snake_x[field_x] = {head_x, head_x + 1, head_x + 2};
int snake_y[field_y] = {head_y, head_y + 1, head_y + 2};
int snake_max_length = field_x * field_y;

int x_diff, y_diff;

bool is_close(int VRx, int expected_VRx, int VRy, int expected_VRy) {
  int accuracy = 50;
    if ( abs(VRx - expected_VRx) < accuracy &&
          abs(VRy - expected_VRy) < accuracy) {
        return true;
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
  // print_joystick(VRx, VRy);
  if (is_close(VRx, 0, VRy, 500)) {
//    Serial.println("up");
    x_diff = 0;
    y_diff = 1;
  }
  if (is_close(VRx, 980, VRy, 480)) {
//    Serial.println("down");
    x_diff = 0;
    y_diff = -1;
  }
  if (is_close(VRx, 500, VRy, 1000)) {
//    Serial.println("left");
    x_diff = 1;
    y_diff = 0;
  }
  if (is_close(VRx, 500, VRy, 0)) {
//    Serial.println("right");
    x_diff = -1;
    y_diff = 0;
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
  for (int i = 1; i < current_snake_length(); i++) {
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
//    for (int i = 0; i < current_snake_length(); i++) {
//      Serial.print(snake_x[i]);
//      Serial.print(", ");
//      Serial.print(snake_y[i]);
//      Serial.print(" | ");
//    }
    
    int snake_length = current_snake_length();
    Serial.print(snake_length);
    Serial.print(" .... ");
    Serial.print(snake_x[snake_length]);
    Serial.print("- ");
    Serial.print(snake_y[current_snake_length()]);
    snake_x[snake_length] = snake_x[snake_length - 1] - x_diff - 1;
    snake_y[snake_length] = snake_y[snake_length - 1] - y_diff - 1;
    
    Serial.print(" |||| ");
    Serial.print(snake_x[snake_length]);
    Serial.print(" - ");
    Serial.print(snake_y[snake_length]);
    Serial.println();
    
  }

  
}

void generate_apple(){
  apple_x = random(1, 84);
  apple_y = random(1, 48);
  apple_on_field = true;
}

void init_display() {
  display.begin();
  display.clearDisplay();
  display.display();
  display.setContrast(40);
  delay(500);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  init_display();
}

void loop() {
  // put your main code here, to run repeatedly:
  read_joystick();
  generate_snake();
  snake_eat();
  snake_move();
  snake_teleport();
} 
