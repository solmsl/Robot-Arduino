#include <Bluepad32.h>
//#include <Servo.h>

//Servo myservo;

const int PinIN1 = 26;
const int PinIN2 = 27;
const int PinIN3 = 16;
const int PinIN4 = 17;
const int IR_Sensor_Left = 34;
const int IR_Sensor_Right = 32;
bool estado=true;
// const int motor = 19;

// Arduino setup function. Runs in CPU 1
void setup() {
  // myservo.attach(motor);
  
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
}

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void moveForward() {
  // Mueve el robot hacia delante
  digitalWrite(PinIN1, HIGH);
  digitalWrite(PinIN2, LOW);
  digitalWrite(PinIN3, LOW);
  digitalWrite(PinIN4, HIGH);
  Serial.print("Adelante");
}

// void Ataque()
// {
//   digitalWrite(motor, HIGH);
// }

void moveBackwards() {
  // Mueve el robot hacia atrás
  digitalWrite(PinIN1, LOW);
  digitalWrite(PinIN2, HIGH);
  digitalWrite(PinIN3, HIGH);
  digitalWrite(PinIN4, LOW);
  Serial.print("Atras");
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
  delay(200);  // Ajusta este tiempo según la velocidad de giro
  beQuiet();   // Detiene el motor después de girar
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
  if (ctl->a()) {
    moveForward();
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
    moveBackwards();
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
  dumpGamepad(ctl);
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
void inicio(state){
  if (state==true){
    moveForward();
    serial.printIn("avanza");
    beQuiet();
    serial.printIn("para");
    delay(200);
    serial.printIn("avanza");
    moveForward();
    beQuiet();
    serial.printIn("para");
  }
}
// Arduino loop function. Runs in CPU 1.
void loop() {
  if(estado==true)
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