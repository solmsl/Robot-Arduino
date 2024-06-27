const int PinIN1 = 26;
const int PinIN2 = 27;
const int PinIN3 = 16;
const int PinIN4 = 17;
const int IR_Sensor_Left = 34;
const int IR_Sensor_Right = 32;

void setup() {
  // Serial.begin(115200);  // Inicia la consola de arduino en el puerto 9600
  Serial.begin(9600);
  // Establecemos al motor como salida
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(IR_Sensor_Left, INPUT);
  pinMode(IR_Sensor_Right, INPUT);
}


// La funcion loop se repite una y otra vez
void loop() {
  despejaCosas();
  //sigueLineas();
}


/* ----- My functions ----- */

void moveForward() {
  // Mueve el robot hacia delante
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
}

void moveBackwards() {
  // Mueve el robot hacia atrás
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
}

void moveLeft() {
  // Mueve el robot a la izquierda
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
}


void moveRight() {
  // Mueve el robot a la derecha
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
}

void beQuiet() {
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, LOW);
}

void rotate100() {
  //Giro de 100 grados
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  delay(200); // Ajusta este tiempo según la velocidad de giro
  beQuiet(); // Detiene el motor después de girar
}

void despejaCosas() {
  /* Lee el estado del pin del sensor */
  /* 1 - Negro || 0- Blanco  */
  bool leftState = digitalRead(IR_Sensor_Left);
  bool rightState = digitalRead(IR_Sensor_Right);

    if (leftState == LOW && rightState == LOW) {
      moveForward();
    } else if (leftState == HIGH && rightState == LOW) {
      moveLeft();
      Serial.print("mueve izquierda");
    } else if (leftState == LOW && rightState == HIGH) {
      moveRight();
      Serial.print("mueve derecha");
    } else {
      rotate100();
    }

}

void sigueLineas() {
  /* Lee el estado del pin del sensor */
  /* 1 - Negro || 0- Blanco  */
  bool leftState = digitalRead(IR_Sensor_Left);
  bool rightState = digitalRead(IR_Sensor_Right);

    if (leftState == HIGH && rightState == HIGH) {
      moveForward();
    } else if (leftState == HIGH && rightState == LOW) {
      moveLeft();
      Serial.print("mueve izquierda");
    } else if (leftState == LOW && rightState == HIGH) {
      moveRight();
      Serial.print("mueve derecha");
    } else {
      rotate100();
    }

}