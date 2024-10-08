#include <Bluepad32.h>

const int PinIN1 = 26;
const int PinIN2 = 27;
const int PinIN3 = 16;
const int PinIN4 = 17;
const int ENA = 23;
const int ENB = 22;
const int IR_Sensor_Left = 34;
const int IR_Sensor_Right = 32;



ControllerPtr myControllers[BP32_MAX_GAMEPADS];
// Declaración de la función

void Girar(int valor) {
  if (valor < 90) {
    int valorInvertido = 255 - valor;
    // Gira a la izquierda
    analogWrite(ENA, valorInvertido);  // Controla la velocidad del motor A
    analogWrite(ENB, 0);               // Detiene el motor B

    // Configura los pines para girar a la izquierda
    digitalWrite(PinIN1, LOW);
    digitalWrite(PinIN2, HIGH);
    digitalWrite(PinIN3, LOW);
    digitalWrite(PinIN4, HIGH);

    Serial.print("Gira a la izquierda con velocidad: ");
    Serial.print(valorInvertido);
    Serial.println();
  } else if (valor > 180) {
    // Gira a la derecha
    analogWrite(ENA, 0);      // Detiene el motor A
    analogWrite(ENB, valor);  // Controla la velocidad del motor B

    // Configura los pines para girar a la derecha
    digitalWrite(PinIN1, HIGH);
    digitalWrite(PinIN2, LOW);
    digitalWrite(PinIN3, HIGH);
    digitalWrite(PinIN4, LOW);

    Serial.print("Gira a la derecha con velocidad: ");
    Serial.print(valor);
    Serial.println();
  } else {
    Serial.println("No gira.");
  }
}


// Definición de la función
int ajustarRangoStick(int valor) {
  int valorMapeado = map(valor, -504, 512, 0, 255);
  return constrain(valorMapeado, 0, 255);  // Asegura que el valor esté entre 0 y 255
}


int ajustarRango(int valor) {
  int valorMapeado = map(valor, 0, 800, 0, 255);
  return min(valorMapeado, 255);  // Asegura que el valor máximo sea 255
}

void moveForward(int velocidad) {
  // Configura la velocidad del motor usando PWM
  analogWrite(ENA, velocidad);  // Controla el motor A
  analogWrite(ENB, velocidad);  // Controla el motor B

  // Mueve el robot hacia delante
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);

  Serial.print("Adelante: ");
  Serial.print(velocidad);
  Serial.println(" ");
}


void moveBackwards(int velocidad) {
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);

  // Mueve el robot hacia atrás
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
  Serial.print("Atras: ");
  Serial.print(velocidad);
  Serial.println(" ");
}


void moveLeft() {
  // Mueve el robot a la izquierda
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
  Serial.print("Izquierda");
}


void moveRight() {
  // Mueve el robot a la derecha
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  Serial.print("Derecha");
}

void beQuiet() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, LOW);
}

void rotate100() {
  //Giro de 100 grados
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  delay(200);  // Ajusta este tiempo según la velocidad de giro
  beQuiet();   // Detiene el motor después de girar
}

void despejaCosas() {
  /* Lee el estado del pin del sensor */
  /* 1 - Negro || 0- Blanco  */
  bool leftState = digitalRead(IR_Sensor_Left);
  bool rightState = digitalRead(IR_Sensor_Right);

  if (leftState == LOW && rightState == LOW) {
    moveForward(255);
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
    moveForward(255);
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

// Este callback se llama cuando se conecta un joystick.
// Soporta hasta 4 joysticks.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}
/// Este callback es para cuando se desconecta.
void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}
/// Esto imprime por serial todos los botoenes del joystick.
/// Para hacer los controles deberiamos usar esto
/*
  void adelanteAtras(valor_regulable) ---El valor puede ser positivo o negativo
  {...}
  void izquierdaDerecha(valor_regulable) ---El valor puede ser positivo o negativo
  {...}

  ----processGamepad----
  adelanteAtras(ctl->axisRY()); 
  izquierdaDerecha(ctl-axisX());

  - o mas bien
  adelante(ctl->throttle());
  atras(ctl-brake());



*/

// Estaria bueno poder regular el puente h para que se pueda usar el analogico del joystick
// TENER EN CUENTA ZONAS MUERTAS


void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
    "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
    ctl->index(),        // Controller Index
    ctl->dpad(),         // D-pad
    ctl->buttons(),      // bitmask of pressed buttons
    ctl->axisX(),        // (-511 - 512) left X Axis
    ctl->axisY(),        // (-511 - 512) left Y axis
    ctl->axisRX(),       // (-511 - 512) right X axis
    ctl->axisRY(),       // (-511 - 512) right Y axis
    ctl->brake(),        // (0 - 1023): brake button
    ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
    ctl->miscButtons(),  // bitmask of pressed "misc" buttons
    ctl->gyroX(),        // Gyro X
    ctl->gyroY(),        // Gyro Y
    ctl->gyroZ(),        // Gyro Z // Se imaginan usar el giroscopio??? jajajaj
    ctl->accelX(),       // Accelerometer X
    ctl->accelY(),       // Accelerometer Y
    ctl->accelZ()        // Accelerometer Z
  );
}





void processGamepad(ControllerPtr ctl) {
  // There are different ways to query whether a button is pressed.
  // By query each button individually:
  //  a(), b(), x(), y(), l1(), etc...

  // if (ctl->a()) {
  //   moveForward(255);
  // }

  //Serial.print(ctl->throttle());
  //Serial.print(ctl->brake());
  Serial.println(ajustarRangoStick(ctl->axisX()));
  if (ajustarRangoStick(ctl->axisX()) >= 160 || ajustarRangoStick(ctl->axisX()) <= 120) {
    Girar(ajustarRangoStick(ctl->axisX()));
  } else if (ajustarRango(ctl->throttle()) >= 100) {
    moveForward(ajustarRango(ctl->throttle()));
  } else if (ajustarRango(ctl->brake()) >= 100) {
    moveBackwards(ajustarRango(ctl->brake()));
  } else {
    beQuiet();
  }



  if (ctl->b()) {
    beQuiet();
  }
  /*
    if (ctl->left()){
      moveLeft();
    }
    if (ctl->right()){
      moveRight();
      
    }
*/
  uint8_t dpadState = ctl->dpad();

  if (dpadState & DPAD_UP) {
    Serial.print("D-pad UP pressed");
  }
  if (dpadState & DPAD_DOWN) {
    Serial.print("D-pad DOWN pressed");
  }
  if (dpadState & DPAD_LEFT) {
    Serial.print("D-pad LEFT pressed");
    moveLeft();
  }
  if (dpadState & DPAD_RIGHT) {
    Serial.print("D-pad RIGHT pressed");
    moveRight();
  }
  if (ctl->y()) {
    moveBackwards(255);
  }

  /*
    if (ctl->start()) {
        Serial.print("Siguiendo Lineas");
    }
    
    if (ctl->select()){
      Serial.print("Despejando Cosas");
    }
*/
  uint8_t miscState = ctl->miscButtons();

  if (miscState & MISC_BUTTON_HOME) {
    Serial.println("Despejando Cosas");
    despejaCosas();
  }

  if (miscState & MISC_BUTTON_BACK) {
    Serial.println("Siguiendo Lineas");
    sigueLineas();
  }
  // Another way to query controller data is by getting the buttons() function.
  // See how the different "dump*" functions dump the Controller info.
  ///dumpGamepad(ctl);
}





void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {


  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);

  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(IR_Sensor_Left, INPUT);
  pinMode(IR_Sensor_Right, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}



// Arduino loop function. Runs in CPU 1.
void loop() {
  // This call fetches all the controllers' data.
  // Call this function in your main loop.
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

  // The main loop must have some kind of "yield to lower priority task" event.
  // Otherwise, the watchdog will get triggered.
  // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
  // Detailed info here:
  // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

  //     vTaskDelay(1);
  delay(150);
}
