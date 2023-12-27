#include <Arduino.h>
 

// Modalità: 
//  0 Sempre accese
//  1 Fade
//  2 Intermittenti
//  3 Pazze
int mode = 0;


// Decremento ad ogni giro, se siamo da 100 a 0 regolo la luminosità, altrimenti sempre accese
int maininterval = 1000;
int interval = 100;
int brightness = 0;
int delayms = 100;

bool pulsantePremuto = false;
int pulsanteBounceCycle = 0;

void updateMode() { 
  pulsantePremuto = true;
  pulsanteBounceCycle = 2;
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  Serial_begin(115200);

	Serial_println_s("Starting...");

  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_IN_PU_IT);
  
  disableInterrupts();
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);  
  enableInterrupts();

  attachInterrupt(INT_PORTA & 0xFF, updateMode, 0);

}

void loop() {

  if (pulsantePremuto && pulsanteBounceCycle == 0) {
    mode++;
    if (mode >= 4)
      mode = 0;
    pulsantePremuto = false;
  }
  if (pulsanteBounceCycle > 0)
    pulsanteBounceCycle--;

  if (mode == 0) {

    // Sempre accese
    for (int i = 0; i < 100; i++) {
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);   
      delay(1);
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);   
      delay(1);
    }

  } else if (mode == 1) {

    // FADE
    digitalWrite(5, LOW);
    for (int b = 0; b < 255; b++) {
      analogWrite(6, b);
      delay(10);
    }
    for (int b = 255; b >= 0; b--) {
      analogWrite(6, b);
      delay(10);
    }

    digitalWrite(6, LOW);
    for (int b = 0; b < 255; b++) {
      analogWrite(5, b);
      delay(10);
    }
    for (int b = 255; b >= 0; b--) {
      analogWrite(5, b);
      delay(10);
    }

  } else if (mode == 2) {

      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);   
      delay(350);
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);   
      delay(350);

  } else if (mode == 3) {
        
    for (int i = 0; i < 10; i++) {
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);   
      delay(40);
      digitalWrite(5, HIGH);   
      digitalWrite(6, LOW);
      delay(40);
    }

  }

  Serial_print_s("Mode: ");
  Serial_println_i(mode);

}
 