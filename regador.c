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

int humidade;
int rele = 6;
int botao = 7;
int aux_botao = 0;

void setup()
{
  lcd.begin(16, 2);        //Inicializa o LCD
  pinMode(rele, OUTPUT);   //Pino do rele
  pinMode(botao, OUTPUT);  //Pino do botao
}

void loop()
{
  digitalWrite(rele, LOW);                     //Inicializa o rele como estado 0
  lcd.setCursor(0, 0);                         //Seta a posicao do cursor do LCD para (0,0)
  
  humidade = analogRead(A0);                   //Le o sensor
  humidade = map(humidade, 1023, 0, 0, 100);   //Mapeia a entrada analogica do sensor
  
  lcd.print("Humidade em:");                   //Printa a humidade atual em porcentagem
  lcd.print(humidade);              
  lcd.print(" %");
  
  aux_botao = digitalRead(botao);          //Le o botao

  if(aux_botao == HIGH)
  {      
    if (humidade <= 70)
    {                                      //Se o botao for apertado e a humidade for menos que 70%, rega
      digitalWrite(rele, HIGH);
      lcd.clear();
      lcd.print("Regando...");
    }
    else{
      lcd.clear();                        //Impede do usuario inundar a planta
      lcd.print("Humidade alta");         //Nao sei se cabe no LCD
      digitalWrite(rele, LOW);
    }
  }
  else
  {                                        //Modo automatico
    if(humidade < 25)
    {
      lcd.clear();
      lcd.setCursor(0, 0);                //Seta o cursor do LCD na posicao (0,0)
      lcd.print("Irrigando");             //Printa o estado atual do sistema
      lcd.print(humidade);
      lcd.print("%");
      digitalWrite(rele, HIGH);           //Liga a bomba d'agua
    }
    if (humidade >=25 && humidade <=75)
    {
      lcd.clear();
      lcd.setCursor(0, 0);                //Seta o cursor do LCD na posicao (0,0)
      lcd.print("Irrigando");             //Printa o estado atual do sistema
      lcd.print(humidade);
      lcd.print("%");
      digitalWrite(rele, HIGH);           //Liga a bomba d'agua
    }
    if (humidade >= 75)
    {
      lcd.clear();
      lcd.setCursor(0, 0);                //Seta o cursor do LCD na posicao (0,0)
      lcd.print("Irrigada! ");             //Printa o estado atual do sistema
      lcd.print(humidade);
      lcd.print("%");
      digitalWrite(rele, LOW);           //Desliga a bomba d'agua
    }
  }
  
  delay(500);
  lcd.clear();
}