#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7,6,5,4,3);

const int field_x = 84;
const int field_y = 48;

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
    x_diff = -1;
    y_diff = 0;
  }
  if (is_close(VRx, 500, VRy, 0)) {
//    Serial.println("right");
    x_diff = 1;
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
  for (int i = 0; i < current_snake_length(); i++) {
    snake_x[i] += x_diff;
    snake_y[i] += y_diff;
  } 
}

void snake_teleport() {
  for (int i = 0; i < current_snake_length(); i++) {
    if (snake_x[i] > 84) {
      snake_x[i] = 0;
    }
    if (snake_x[i] < 1) {
      snake_x[i] = 84;
    }
    if (snake_y[i] > 48) {
      snake_y[i] = 0;
    }
    if (snake_y[i] < 1) {
      snake_y[i] = 48;
    }
  } 
}

void generate_snake() {
  display.clearDisplay();
  for (int i = 0; i < current_snake_length(); i++) {
    display.drawPixel(snake_x[i], snake_y[i], BLACK); 
  } 
  display.display();
  delay(500);
}

void init_display() {
  display.begin();
  display.clearDisplay();
  display.display();
  display.setContrast(70);
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
  snake_move();
  snake_teleport();
} 
