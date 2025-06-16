#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//********** LCD ************
LiquidCrystal_I2C lcd(0x27, 16, 2);

//********** BOTÃO **********
const int botao = 8;
bool estadoBotao = false;

//********** MOTOR DE VIBRAÇÃO **********
const int motorvib = 2;
bool motorAtivo = false;

//********** MILLIS **********
unsigned long tempoUltimaAtivacao = 0;
const unsigned long intervalo = 43200000;  // 12 horas em milissegundos
long restam;

void setup() {
  //LCD
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(botao, INPUT_PULLUP);  // botão com resistor pullup interno
  pinMode(motorvib, OUTPUT);     // motor como saída
  digitalWrite(motorvib, LOW);   // motor começa desligado

  tempoUltimaAtivacao = millis();  // marca o tempo inicial
}

void loop() {
  unsigned long tempoAtual = millis();
  Serial.println(tempoAtual);
  lerBotao();
  verificarTempo(tempoAtual);
  controlarMotor();
}

//********** LER BOTÃO **********
void lerBotao() {
  estadoBotao = digitalRead(botao) == LOW;  // true se pressionado
}

//********** VERIFICA TEMPO **********
void verificarTempo(unsigned long tempoAtual) {
  if (!motorAtivo && (tempoAtual - tempoUltimaAtivacao >= intervalo)) {
    motorAtivo = true;
    Serial.println("12 horas se passaram. Ativando motor.");
    mostrarAlerta();
  } else {
    mostrarMsgPadrao();;
  }
}

//********** MOSTRA MENSAGEM NO LCD **********
void mostrarAlerta() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Hora da");
  lcd.setCursor(2, 1);
  lcd.print("INSULINA!");
  Serial.println("Hora da insulina!");
}

//********** CONTROLE DO MOTOR **********
void controlarMotor() {
  if (motorAtivo) {
    digitalWrite(motorvib, HIGH);  // ativa motor

    if (estadoBotao) {
      // botão pressionado: desativa motor e LCD, reinicia contador
      digitalWrite(motorvib, LOW);
      motorAtivo = false;
      tempoUltimaAtivacao = millis();
      lcd.clear();
      Serial.println("Botão pressionado. Motor desligado. Contador reiniciado.");
    }
  } else {
    digitalWrite(motorvib, LOW);  // garante que o motor está desligado
  }
}
void mostrarMsgPadrao() {

  medirTempoRestante();

  lcd.print("Faltam");
  lcd.setCursor(7, 0);
  lcd.print(restam / 60 / 60 / 1000);
  lcd.print(" Horas");
  lcd.setCursor(0, 1);
  lcd.print("para a insulina!");

  delay(1000);
  lcd.clear();
}

unsigned long medirTempoRestante() {
  unsigned long tempoAtual = millis();
  restam = intervalo - tempoAtual;
  return restam;
}
