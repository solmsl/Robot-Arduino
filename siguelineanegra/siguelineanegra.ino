const int PinIN1 = 7;
const int PinIN2 = 6;
const int PinIN3 = 5;
const int PinIN4 = 4;
const int PinSensorL = 2;   // Sensor izquierdo
const int PinSensorR = 3;  // Sensor derecho

void setup() {
  Serial.begin(9600);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinSensorL, INPUT);
  pinMode(PinSensorR, INPUT);
}

void moveForward() {
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
}

void moveBackwards() {
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
}

void moveLeft() {
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
}

void moveRight() {
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
}

void stop() {
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, LOW);
}

void sigueLineas() {
    if (sensorLeft == HIGH && sensorRight == HIGH) {
    // Ambos sensores detectan negro
    moveForward();
  } else if (sensorLeft == LOW && sensorRight == HIGH) {
    // Solo el sensor izquierdo detecta blanco, gira a la derecha
    moveRight();
   
  } else if (sensorLeft == HIGH && sensorRight == LOW) {
    // Solo el sensor derecho detecta blanco, gira a la izquierda
    moveLeft();

  } else {
    // Ambos sensores detectan blanco, detener
    stop();
    
  }
}

void robotdeuds() {
  // Hace algo
}

void loop() {
  //creamos dos variables bool para 
  bool sensorLeft = digitalRead(PinSensorL);   // Estado del sensor izquierdo
  bool sensorRight = digitalRead(PinSensorR); // Estado del sensor derecho

  //LOW es blanco y HIGH negro
  //sigueLineas();

}
