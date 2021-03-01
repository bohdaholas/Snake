void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
}

void loop() {
  int y = analogRead(A0);
  int x = analogRead(A1);
  Serial.print(y);
  Serial.print(" ");
  Serial.println(x);
  digitalWrite(2, 1);
  digitalWrite(3, 0);
  digitalWrite(4, 1);
  delay(100);
}
