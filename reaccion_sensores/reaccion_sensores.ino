const int PinIN1 = 7;
const int PinIN2 = 6;
const int PinIN3 = 5;
const int PinIN4 = 4;
const int PinSensorL = 2;   // Sensor izquierdo, antes era el puerto 2
const int PinSensorR = 3; // Sensor derecho, antes era el puerto 3

//sensor ultrasonico
const int Trigger = 8;   //Pin digital 2 para el Trigger del sensor
const int Echo = 9;   //Pin digital 3 para el Echo del sensor

void setup() {
  Serial.begin(9600);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinSensorL, INPUT);
  pinMode(PinSensorR, INPUT);

  //SENSOR SETUP
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  pinMode(led, OUTPUT);
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
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
  
  //creamos dos variables bool para 
  bool sensorLeft = digitalRead(PinSensorL);   // Estado del sensor izquierdo
  bool sensorRight = digitalRead(PinSensorR); // Estado del sensor derecho

  //LOW es blanco y HIGH negro
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

void sensor_de_prox() {
  // Hace algo
  long t; //tiempo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t / 59;           //escalamos el tiempo a una distancia en cm

  if(d<=25){
    Serial.print("Distancia: ");
    Serial.print(d);
    Serial.print("cm");
    Serial.println();
    delay(100);
    moveForward();
    }
    else{   
        moveBackwards();
        sigueLineas();
      }
}

void loop() {
  sigueLineas();
  sensor_de_prox();
}
