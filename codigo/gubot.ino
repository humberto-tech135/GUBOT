#include <Servo.h>

Servo sensorServo;

// ===== MAPEAMENTO DE PINOS =====
#define TRIG 9
#define ECHO 10
#define SERVO_PIN 4 // Porta corrigida para liberar os PWMs dos motores

// Motores (Todos conectados em portas PWM d Arduino Uno!)
#define M1A 5   // Motor Esquerdo (Avanço)
#define M1B 6   // Motor Esquerdo (Recuo)
#define M2A 3   // Motor Direito (Avanço)
#define M2B 11  // Motor Direito (Recuo)

// ===== CONFIGURAÇÕES GERAIS =====
// Altere este valor (de 0 a 255) para controlar a velocidade de AMBOS os motores juntos.
// 140 é um excelente valor para o robô andar calmo e seguro em cima de uma mesa.
int velocidade = 90;     

int distanciaLimite = 20; // Distância de segurança para desviar (em cm)
int frente = 90;          // Ângulo central do servo
int direita = 20;         // Ângulo para olhar à direita
int esquerda = 160;       // Ângulo para olhar à esquerda

void setup() {
  Serial.begin(9600);

  // Inicialização do Sensor Ultrassônico
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Inicialização dos Pinos dos Motores
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);

  // Inicialização do Servo no centro
  sensorServo.attach(SERVO_PIN);
  sensorServo.write(frente);

  delay(1000); // Aguarda 1 segundo antes de iniciar para dar estabilidade elétrica
}

void loop() {
  int distanciaFrente = medirDistancia();

  // Se o caminho estiver livre, o GUBOT avança na velocidade controlada
  if (distanciaFrente > distanciaLimite) {
    andarFrente();
  }
  // Se encontrar um obstáculo à frente, para e analisa as laterais
  else {
    pararMotores();
    delay(300);

    // Varredura para a DIREITA
    sensorServo.write(direita);
    delay(600);
    int distDireita = medirDistancia();

    // Retorno rápido ao centro
    sensorServo.write(frente);
    delay(300);

    // Varredura para a ESQUERDA
    sensorServo.write(esquerda);
    delay(600);
    int distEsquerda = medirDistancia();

    // Retorno definitivo ao centro
    sensorServo.write(frente);
    delay(300);

    // Envia dados para o monitor serial (ajuda a debugar caso precise)
    Serial.print("Direita: ");
    Serial.println(distDireita);
    Serial.print("Esquerda: ");
    Serial.println(distEsquerda);

    // Decisão de rota: vira para o lado que tiver maior espaço livre
    if (distDireita > distEsquerda) {
      virarDireita();
      delay(500); // Tempo necessário para o robô girar o corpo (ajuste se necessário)
    } 
    else {
      virarEsquerda();
      delay(500); // Tempo necessário para o robô girar o corpo (ajuste se necessário)
    }

    pararMotores();
    delay(200);
  }
}

// ===== FUNÇÕES DE LEITURA =====

int medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  int distancia = duracao * 0.034 / 2;

  // Filtro de erro: se a leitura falhar (retornar 0), assume caminho livre temporariamente
  if (distancia == 0) return 999; 
  return distancia;
}

// ===== FUNÇÕES DE MOVIMENTAÇÃO SIMÉTRICA =====

void andarFrente() {
  analogWrite(M1A, velocidade); // Motor Esquerdo com velocidade controlada
  digitalWrite(M1B, LOW);

  analogWrite(M2A, velocidade); // Motor Direito com a MESMA velocidade controlada
  digitalWrite(M2B, LOW);
}

void pararMotores() {
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}

void virarDireita() {
  analogWrite(M1A, velocidade); // Avança motor esquerdo
  digitalWrite(M1B, LOW);

  digitalWrite(M2A, LOW);
  analogWrite(M2B, velocidade); // Recua motor direito (giro sob o próprio eixo)
}

void virarEsquerda() {
  digitalWrite(M1A, LOW);
  analogWrite(M1B, velocidade); // Recua motor esquerdo

  analogWrite(M2A, velocidade); // Avança motor direito (giro sob o próprio eixo)
  digitalWrite(M2B, LOW);
}

//codigo por humberto
