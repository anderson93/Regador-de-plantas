/*
# Autor: Anderson Henrique & Eduardo Emery
# Data: 10/06/2017
# Linguagem: C - Processing
# ========= IMPORTANTE ===========
# O codigo esta livre para usar,
# citar e compartilhar desde que
# mantida sua fonte e seu autor.
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variaveis
int humidade;
int humidade_ant = 0;
int setpoint = 50;
int setpoint_ant = 0;
int releState = 0;
int releState_ant = 1;

// Constantes
int const EPSILON = 5;

// Pinos
const int rele = 6;
const int botao_direita = 7;
const int botao_esquerda = 8;
const int botao_manual = 9;

void setup()
{
  lcd.begin(16, 2);     						             // Inicializa o LCD
  pinMode(rele, OUTPUT);   						           // Pino do rele
  pinMode(botao_direita, OUTPUT); 				       // Pino do botao da direita
  pinMode(botao_esquerda, OUTPUT);				       // Pino do botao da esquerda
  pinMode(botao_manual, OUTPUT);				         // Pino do botao da manual
  
  digitalWrite(rele, LOW);                       // Inicializa o rele como estado 0
  Serial.begin(9600);
}

void loop()
{  
  humidade = analogRead(A0);                   	 // Le o sensor
  humidade = map(humidade, 0, 330, 0, 100);      // Mapeia a entrada analogica do sensor
    
    
  // Mostra a humidade atual em porcentagem, caso seja diferente da passada
  if (humidade != humidade_ant)
  {
    lcd.setCursor(0, 0);
  	lcd.print("Humidade: ");
  	lcd.print(humidade);
    humidade_ant = humidade;
  }
  
  // Mostra o setpoin atual em porcentagem, caso seja diferente do passado
  if (setpoint != setpoint_ant)
  {
    lcd.setCursor(0, 1);
  	lcd.print("Setpoint: ");
  	lcd.print(setpoint);
    setpoint_ant = setpoint;
  }
  
  // Escreve no rele o estado dele
  if (releState != releState_ant)
  {
    Serial.println(releState);
    releState_ant = releState;
    if (releState == 1) digitalWrite(rele, HIGH);
    if (releState == 0) digitalWrite(rele, LOW);
  }
  
  
  // Varia o setpoint
  if (digitalRead(botao_direita) && setpoint < 100)		// Aumenta o setpoint (limite 100)(botao direito)
    setpoint += 1;
  
  if (digitalRead(botao_esquerda) && setpoint > 0)		// Diminui o setpoint (limite 0)(botao esquerdo)
    setpoint -= 1;
  
  
  // Rega se o humidade estiver abaixo do setpoint ou comando manual for acionado
  if ((setpoint - humidade) > 0 || (digitalRead(botao_manual) && (humidade - setpoint) <= EPSILON))
    releState = 1;
  
  // Para de regar se o humidade estiver acima do setpoint a um nivel EPSILON
  if ((humidade - setpoint) > EPSILON)
    releState = 0;

  delay(100);
}