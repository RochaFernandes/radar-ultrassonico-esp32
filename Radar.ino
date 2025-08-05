#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SERVO_PIN 15
#define TRIG_PIN 12
#define ECHO_PIN 13
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Servo meuServo; //Cria um objeto do tipo servo
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int angulo = 0; //Posição atual do servo
int incremento = 1; //Passo de varredura
unsigned long ultimaMudanca = 0; // Tempo da última movimentação
const int intervalo = 40; //Tempo entre o movimento (em ms)


void setup() {
  meuServo.attach(SERVO_PIN); //Conecta o servo ao pino 15
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);

  //Inicializa o OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("Display não encontrado!");
    while(true);

   }

   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE);

}

float medirDistancia(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia = duracao * 0.034 / 2;
  return distancia;
}

void loop() {
  unsigned long agora = millis();

  if(agora - ultimaMudanca >= intervalo){
    ultimaMudanca = agora;
    
    meuServo.write(angulo);
    float distancia = medirDistancia();

    //Exibe OLED
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Angulo: ");
    display.print(angulo);
    display.println("graus");

    display.setCursor(0, 30);
    display.print("Distancia: ");
    display.print(distancia);
    display.println(" cm");
    display.display();
    angulo += incremento;
    //Inverte a direção se chegar nos limites
    if(angulo>= 180 || angulo <= 0){
      incremento = -incremento;
    }
  }

}
