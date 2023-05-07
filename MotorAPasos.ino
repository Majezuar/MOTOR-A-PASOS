
// Librerias Usadas en el programa
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  // Para realizar conexion con el LCD

int stepPerRevolution = 2048;
Stepper myStepper(stepPerRevolution, 5, 7, 9, 11);  // Cantidad pasos por vuelta,  IN1, IN3, IN2, IN4
int motSpeed = 8;  // del 1 al 15
int dt = 2000;
int stepSize = 100;
int stepsAdvanced = 0;

// Varaible bandera para terminar el programa
bool banderaTerminar = false;

// Variable Para activaciojn mediante botones
bool avanzarPasos = false;
bool disminuirPasos = false;

// Iniciar Variables LEDS
int pocosPasos = 4;
int variosPasos = 6;
int muchosPasos = 8;

void setup() {

  // Creacion de los LED como salidas
  pinMode(pocosPasos, OUTPUT);
  pinMode(variosPasos, OUTPUT);
  pinMode(muchosPasos, OUTPUT);

  // Establecemos la velocidad
  myStepper.setSpeed(motSpeed);

  // Inicializacion del LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  //interrupciones Para realizar cambiar de direccion de giro del motor a pasos
  attachInterrupt(digitalPinToInterrupt(2), avanzarPaso, RISING);
  attachInterrupt(digitalPinToInterrupt(3), disminuirPaso, RISING);
}


void loop() {
  if (banderaTerminar == false) {

    // Programa principal
    if (stepsAdvanced <= 20 && stepsAdvanced > 0 && disminuirPasos == true) {

      myStepper.step(-stepSize);  // Avance con el tamaña del paso dado
      stepsAdvanced--;
      condicionalesLeds();  //Condicionante de prendido de LEDS segun los pasos dados
      lcd.clear();
      lcd.setCursor(0, 0);
      if (stepsAdvanced == 0) {
        lcd.print("Punto Minimo");
      } else {
        lcd.print("Disminuyendo:");
      }
      lcd.setCursor(0, 1);
      lcd.print(stepsAdvanced);
      lcd.setCursor(3, 1);
      lcd.print("Paso(s)");
      delay(1000);
    }
    //Caso cuando avanza
    if (stepsAdvanced < 20 && avanzarPasos == true) {
      myStepper.step(stepSize);  // Avance con el tamaña del paso dado
      stepsAdvanced++;
      condicionalesLeds();  //Condicionante de prendido de LEDS segun los pasos dados
      lcd.clear();
      lcd.setCursor(0, 0);
      if (stepsAdvanced == 20) {
        lcd.print("Punto Maximo");
      } else {
        lcd.print("Aumentando:");
      }
      lcd.setCursor(0, 1);
      lcd.print(stepsAdvanced);
      lcd.setCursor(3, 1);
      lcd.print("Paso(s)");
      delay(1000);
    }
  }
}

// Funcion por interrupcion para disminuir pasos

void disminuirPaso() {
  disminuirPasos = true;
  avanzarPasos = false;
}

// Funcion por interrupcion Para aumentar pasos

void avanzarPaso() {
  avanzarPasos = true;
  disminuirPasos = false;
}

// Funcion para apagar LEDS

void apagarLeds() {
  digitalWrite(pocosPasos, LOW);
  digitalWrite(variosPasos, LOW);
  digitalWrite(muchosPasos, LOW);
}

// Funcion para determinar que LED encender segun los pasos dados

void condicionalesLeds() {
  if (stepsAdvanced >= 0 && stepsAdvanced <= 5) {
    digitalWrite(pocosPasos, HIGH);
  } else {
    digitalWrite(pocosPasos, LOW);
  }

  if (stepsAdvanced >= 6 && stepsAdvanced <= 11) {
    digitalWrite(variosPasos, HIGH);
  } else {
    digitalWrite(variosPasos, LOW);
  }

  if (stepsAdvanced >= 12 && stepsAdvanced <= 20) {
    digitalWrite(muchosPasos, HIGH);
  } else {
    digitalWrite(muchosPasos, LOW);
  }
}