#define botaoInicio 7    
#define buzzer 9         
#define motor 6          

void pomodori() {
  for (int i = 0; i < 4; i++) {           
    
    // Início do trabalho (10s)
    tone(buzzer, 1500);                   
    digitalWrite(motor, HIGH);            
    delay(150);
    noTone(buzzer);                       
    digitalWrite(motor, LOW);             
    delay(10000);                         

    // Início do descanso (5s)
    tone(buzzer, 1850);                   
    digitalWrite(motor, HIGH);            
    delay(150);
    noTone(buzzer);                       
    digitalWrite(motor, LOW);             
    delay(5000);                        
  }

  tone(buzzer, 1000);
  digitalWrite(motor, HIGH);
  delay(500);
  noTone(buzzer);
  digitalWrite(motor, LOW);
}

void setup() {
  pinMode(botaoInicio, INPUT_PULLUP);     
  pinMode(buzzer, OUTPUT);                
  pinMode(motor, OUTPUT);                 
}

void loop() {
  if (digitalRead(botaoInicio) == LOW) {  
    delay(200);                           
    pomodori();                           
  }
}
