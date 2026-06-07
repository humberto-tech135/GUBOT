
// declaracao dos pinos do sensor ultrassonico
const int PINO_SENSOR_ECHO = 13;
const int PINO_SENSOR_TRIGGER = 12;

// declaracao dos pinos do driver de motor
const int PIN_MOTOR_IN1 = 2;
const int PIN_MOTOR_IN2 = 3;
const int PIN_MOTOR_IN3 = 4;
const int PIN_MOTOR_IN4 = 5;

// declaracao das constantes auxiliares para controlar os motores
const int DISTANCIA_SEGURA = 25; // [cm]
const int PAUSA = 100; // [ms]

// ---------------------------------------------------

int ler_distancia(void);
void mover_frente(void);
void mover_tras(void);
void parar(void);

// ---------------------------------------------------

void setup() {
  // configura os pinos do sensor ultrassonico
  pinMode(PINO_SENSOR_ECHO, INPUT); // entrada
  pinMode(PINO_SENSOR_TRIGGER, OUTPUT); // saida
  digitalWrite(PINO_SENSOR_TRIGGER, LOW); // por padrao em nivel baixo (sem sinal)

  // configura os pinos do driver de motor
  pinMode(PIN_MOTOR_IN1, OUTPUT); // saida
  pinMode(PIN_MOTOR_IN2, OUTPUT); // saida
  pinMode(PIN_MOTOR_IN3, OUTPUT); // saida
  pinMode(PIN_MOTOR_IN4, OUTPUT); // saida
  parar(); // robo parado por padrao
}

// ---------------------------------------------------

void loop() {
  // le a distancia
  int distancia = ler_distancia();

  // verifica se ha um obstaculo na frente
  if(distancia < DISTANCIA_SEGURA){
    // para o robo
    parar();
    delay(500); // pausa para o proximo movimento

    // move o robo para tras
    mover_tras();

    // para o robo apos um curto intervalo
    delay(1000);
    parar();

    // escolhe um dos lados para girar
    bool par = (millis() % 2 == 0) ? true : false;
    if(par){
      // gira o motor 1 para a frente
      digitalWrite(PIN_MOTOR_IN1, HIGH);
      digitalWrite(PIN_MOTOR_IN2, LOW);
    } else {
      // gira o motor 2 para a frente
      digitalWrite(PIN_MOTOR_IN3, HIGH);
      digitalWrite(PIN_MOTOR_IN4, LOW);
    }

    // para o robo apos um curto intervalo
    delay(500);
    parar();
  } else { // senao
    // move o robo para a frente
    mover_frente();
  }

  // pausa para a proxima leitura
  delay(PAUSA);
}

// ---------------------------------------------------
// ---------------------------------------------------

// Ler a distancia com o sensor ultrassonico
int ler_distancia(void){
  // realiza o pulso de 10 microsegundos no trigger do sensor
  digitalWrite(PINO_SENSOR_TRIGGER,HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_SENSOR_TRIGGER,LOW);

  // mede o pulso em microsegundos retornado para o echo do sensor
  // e converte o tempo para distancia ao dividir por 58
  return pulseIn(PINO_SENSOR_ECHO, HIGH) / 58; // [cm]
}

// ---------------------------------------------------

// Mover o robo para a frente
void mover_frente(void){
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, HIGH);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// ---------------------------------------------------

// Mover o robo para tras
void mover_tras(void){
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

// ---------------------------------------------------

// Parar o robo
void parar(void){
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// ---------------------------------------------------
