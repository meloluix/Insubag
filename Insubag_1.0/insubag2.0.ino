//********** BOTÕES **********
const int botao = 8;            // Botão que para o motor
const int botaoTroca = 7;       // Botão que altera o tempo de espera
bool estadoBotao = false;
bool estadoBotaoTroca = false;
bool modoTeste = false;

//********** MOTOR DE VIBRAÇÃO **********
const int motorvib = 2;
bool motorAtivo = false;

//********** MILLIS **********
unsigned long tempoUltimaAtivacao = 0;
unsigned long intervalo = 43200000UL;  // Começa com 12h padrão
const unsigned long intervaloTeste = 5000UL;   // 5 segundos
const unsigned long intervaloPadrao = 43200000UL; // 12 horas

void setup() {
  Serial.begin(9600);

  pinMode(botao, INPUT_PULLUP);
  pinMode(botaoTroca, INPUT_PULLUP);
  pinMode(motorvib, OUTPUT);
  digitalWrite(motorvib, LOW);

  tempoUltimaAtivacao = millis();//Marca o tempo inicial, por isso é declarada no setup();
}

void loop() {
  unsigned long tempoAtual = millis();
  lerBotoes();
  verificarTempo(tempoAtual);
  controlarMotor();
}

//********** LER BOTÕES **********
void lerBotoes() {
  // Botão principal: para motor
  estadoBotao = digitalRead(botao) == LOW;

  // Segundo botão, troca de modo, de 5s para 12h ou o contrário
  static bool botaoTrocaPressionado = false;

  if (digitalRead(botaoTroca) == LOW && !botaoTrocaPressionado) {
    botaoTrocaPressionado = true;
    modoTeste = !modoTeste; // alterna entre os modos
    intervalo = modoTeste ? intervaloTeste : intervaloPadrao;
    Serial.print("Modo alterado para: ");
    Serial.println(modoTeste ? "TESTE (5s)" : "PADRÃO (12h)");
    delay(200); // debounce simples
  }

  if (digitalRead(botaoTroca) == HIGH) {
    botaoTrocaPressionado = false;
  }
}

//********** VERIFICA TEMPO **********
void verificarTempo(unsigned long tempoAtual) {
  if (!motorAtivo && (tempoAtual - tempoUltimaAtivacao >= intervalo)) {
    motorAtivo = true;
    Serial.println("Tempo decorrido. Motor ativado.");
  }
}

//********** CONTROLE DO MOTOR **********
void controlarMotor() {
  if (motorAtivo) {
    digitalWrite(motorvib, HIGH);

    if (estadoBotao) {
      digitalWrite(motorvib, LOW);
      motorAtivo = false;
      tempoUltimaAtivacao = millis();
      Serial.println("Botão pressionado. Motor desligado e tempo reiniciado.");
    }
  } else {
    digitalWrite(motorvib, LOW);
  }
}
