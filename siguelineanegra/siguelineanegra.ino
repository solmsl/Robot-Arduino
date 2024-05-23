const int PinIN1 = 7;
const int PinIN2 = 6;
const int PinIN3 = 5;
const int PinIN4 = 4;
const int PinSensorL = 2;   // Sensor izquierdo
const int PinSensorR = 3;   // Sensor derecho
const int trigPin = 9;      // Pin Trig del sensor ultrasónico
const int echoPin = 10;     // Pin Echo del sensor ultrasónico
const int distanceThreshold = 20; // Distancia mínima para detectar un obstáculo (en cm)

void setup() {
  Serial.begin(9600);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinSensorL, INPUT);
  pinMode(PinSensorR, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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

void rotate100() {
  // Gira a la derecha (o izquierda) durante un tiempo suficiente para hacer un giro de 100 grados
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  delay(200); // Ajusta este tiempo según la velocidad de giro del robot
  stop(); // Detiene el motor después de girar
}

long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void sigueLineas() {
  bool sensorLeft = digitalRead(PinSensorL);   // Estado del sensor izquierdo
  bool sensorRight = digitalRead(PinSensorR);  // Estado del sensor derecho
  long distance = readUltrasonicDistance();    // Leer la distancia del sensor ultrasónico

  // Si hay un obstáculo cercano, esquivar
  if (distance < distanceThreshold) {
    stop();
    Serial.println("Obstáculo detectado");
    // Implementar la lógica de evasión de obstáculos aquí
    moveRight();
    delay(500); // Ajusta el tiempo según sea necesario para esquivar el obstáculo
    stop();
  } else {
    //LOW es blanco y HIGH negro
    if (sensorLeft == LOW && sensorRight == LOW) {
      // Ambos sensores detectan negro
      moveForward();
    } else if (sensorLeft == HIGH && sensorRight == LOW) {
      // Solo el sensor izquierdo detecta blanco, gira a la derecha
      moveRight();
    } else if (sensorLeft == LOW && sensorRight == HIGH) {
      // Solo el sensor derecho detecta blanco, gira a la izquierda
      moveLeft();
    } else {
      // Ambos sensores detectan blanco, realiza un giro de 100 grados
      rotate100();
    }
  }
}

void loop() {
  sigueLineas();
}