#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
//SoftwareSerial Blue(2,3);  //(rx.tx)
long int data;

OneWire pino(12);
DallasTemperature barramento(&pino);
DeviceAddress sensor;

//portas saidas
int  mec1 = {39};
int  mec2 = {40};
int  mec3 = {41};
int  mec4 = {42};
int  mec5 = {46};
int  mec6 = {47};
int  mec7 = {48};
int  mec8 = {49};
int  mec9 = {50};
int  mec10 = {51};


int  sen1 = {13};
int  sen2 = {14};
int  sen3 = {15};
int  sen4 = {16};
int  sen5 = {17};
int  sen6 = {22};
int  sen7 = {23};
int  sen8 = {24};
int  sen9 = {25};
int  sen10 = {26};


int  fechadura_porta = {52}; 
int led_teste = {11};

//portas sinais de entrada
int media = 0;
int valormedia = 0;
int passo = 0;
int alto = 0;
int baixo = 0;
long int estouro = 0;
boolean chegouNoSensor = false;

// ENTRADAS = {S1, S2, S3, S4, S5, S6, S7, S8, S9, S10,S11}
// SENSORES DOS MOTORES DO 13 AO 26 SENSOR 27 SEM USO
// 0 = MOTOR NA POSIÇÃO INICIAL
// 1 = EM MOVIMENTO OU FORA DE POSIÇÃO OU SEM SINAL DE SENSOR
uint8_t sensoresMotor[] = {13, 14, 15, 16, 17, 22, 23, 24, 25, 26, 27};
int numeroMaximoMotores = 10;

void setup() {
  // inicializa a comunicação serial 9600 bits por segundo:
  Serial.begin(9600);     
  Serial1.begin(9600);
  pinMode(mec1, OUTPUT);
  pinMode(mec2, OUTPUT);
  pinMode(mec3, OUTPUT);
  pinMode(mec4, OUTPUT);
  pinMode(mec5, OUTPUT);
  pinMode(mec6, OUTPUT);
  pinMode(mec7, OUTPUT);
  pinMode(mec8, OUTPUT);
  pinMode(mec9, OUTPUT);
  pinMode(mec10, OUTPUT);
  pinMode(fechadura_porta, OUTPUT);
 
}

boolean passoMotor(int numeroCanaleta, int tempo)
{
    digitalWrite(numeroCanaleta, HIGH);
    delay(tempo);
    digitalWrite(numeroCanaleta, LOW);
    delay(100);
    //int leitura = digitalRead(sensoresMotor[numeroCanaleta - 1]);
    int leitura = digitalRead(14);
    return leitura == LOW;
}

/**
 * Realiza o procedimento de abrir a canaleta e aguardar o sensor de final de curso.
 **/
String abrirCanaleta(int numeroCanaleta)
{
    String resultado = "OK";
    int media = 0;
    int valormedia = 0;
    int passo = 0;
    int alto = 0;
    int baixo = 0;numeroCanaleta;
    int idxCanaleta = numeroCanaleta - 1;
    int pinCanaleta = numeroCanaleta;//entradasMotor[idxCanaleta];
    
    chegouNoSensor = false;

    // Esse primeiro acionamento precisa ser um pouco mais longo para tirar o motor do sensor.
    for (int i=0;i<6;i++) {
        passoMotor(numeroCanaleta, 1);
    }
    chegouNoSensor = passoMotor(numeroCanaleta, 10);
    if (chegouNoSensor)
    {
        // Se ainda esta no sensor, nao parece que o motor esta funcionando
        digitalWrite(pinCanaleta, LOW);
        delay(100);
        digitalWrite(pinCanaleta, LOW);
        return "ERRO:NAO DETECTOU ABERTURA";
    }
    long tempoInicio = micros();
    long tempoLimite = tempoInicio + 15000000L;
    long tempoAbertura = micros();
    long tempoAtual = tempoInicio;
    // Enquanto não atingiu o limite de tempo e não finalizou a abertura e o fechamento
    while (tempoAtual < tempoLimite && !chegouNoSensor)
    {
        chegouNoSensor = passoMotor(numeroCanaleta, 10);
        tempoAtual = micros();
    }
    if (chegouNoSensor)
    {
        resultado = (tempoAtual - tempoInicio > 1000) ? "OK" : "ERRO:FECHOU RAPIDO DEMAIS";
    }
    else
    {
        resultado = "ERRO:NAO DETECTOU SENSORES";
    }
    digitalWrite(pinCanaleta, LOW);
    delay(100);
    digitalWrite(pinCanaleta, LOW);
    estouro = 0;
    return resultado;
}
void loop() {
  char valorSerial;  
  

  while(Serial1.available() == 0);

  if(Serial1.available() > 0){
    valorSerial = Serial1.read();
    Serial.println(valorSerial);
    Serial1.write(valorSerial);
  }
  delay(400);

/////funçao abrir porta////////////////  
     if(valorSerial == 'A'){
    digitalWrite(fechadura_porta, HIGH);
    delay(30);      
    digitalWrite(fechadura_porta, LOW);        
   }  
///////////////////////////////////////  
    //Canaleta 1
    if(valorSerial == '1'){
      passo = 0;    
      digitalWrite(mec1,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;       
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec1,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen1) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen1) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec1,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 1");
      Serial1.println("EXECUTADO - CANALETA 1"); 

   }

    //Canaleta 2
    if(valorSerial == '2'){
      passo = 0;    
      digitalWrite(mec2,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec2,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen2) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen2) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec2,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 2");
      Serial1.println("EXECUTADO - CANALETA 2"); 

   }

    //Canaleta 3
    if(valorSerial == '3'){
      passo = 0;    
      digitalWrite(mec3,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec3,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen3) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen3) == LOW){
          alto = alto+1;
        }
          
        if(media > 150){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec3,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 3");
      Serial1.println("EXECUTADO - CANALETA 3"); 

   }

    //Canaleta 4
    if(valorSerial == '4'){
      passo = 0;    
      digitalWrite(mec4,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec4,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen4) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen4) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec4,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 4");
      Serial1.println("EXECUTADO - CANALETA 4"); 

   }

    //Canaleta 5
    if(valorSerial == '5'){
      passo = 0;    
      digitalWrite(mec5,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec5,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen5) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen5) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec5,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 5");
      Serial1.println("EXECUTADO - CANALETA 5"); 

   }

    //Canaleta 6
    if(valorSerial == '6'){
      passo = 0;    
      digitalWrite(mec6,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec6,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen6) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen6) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec6,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 6");
      Serial1.println("EXECUTADO - CANALETA 6"); 

   }   

    //Canaleta 7
    if(valorSerial == '7'){
      passo = 0;    
      digitalWrite(mec7,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec7,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen7) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen7) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec7,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 7");
      Serial1.println("EXECUTADO - CANALETA 7"); 

   }

    //Canaleta 8
    if(valorSerial == '8'){
      passo = 0;    
      digitalWrite(mec8,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec8,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen8) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen8) == LOW){
          alto = alto+1;
        }
          
        if(media > 150){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec8,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 8");
      Serial1.println("EXECUTADO - CANALETA 8"); 

   }

    //Canaleta 9
    if(valorSerial == '9'){
      passo = 0;    
      digitalWrite(mec9,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec9,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen9) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen9) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec9,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 9");
      Serial1.println("EXECUTADO - CANALETA 9"); 

   }  

    //Canaleta 10
    if(valorSerial == '0'){
      passo = 0;    
      digitalWrite(mec10,HIGH);
      media = 0;
      while(passo < 2){
        media = media+1;        
        estouro = estouro+1;
          
        if(estouro > 250000){
          digitalWrite(mec10,LOW);
          estouro = 0;
          passo = 3;
        }
          
        if(digitalRead(sen10) == HIGH){
          baixo = baixo+1;          
        }
        if(digitalRead(sen10) == LOW){
          alto = alto+1;
        }
          
        if(media > 50){ 
          media = 0;
          
          if(baixo > alto){
            valormedia = 0;
           }
          if(baixo < alto){
            
            valormedia = 1;
          }
          
          if(valormedia == 1 && passo == 0 ){
            passo = 1;
          }
          if(valormedia == 0 && passo == 1 ){
            passo = 2;
          }
                    
          baixo = 0;
          alto = 0;
        }        
      }
      digitalWrite(mec10,LOW);
      estouro = 0;
      Serial.println("SUCESSO - CANALETA 10");
      Serial1.println("EXECUTADO - CANALETA 10"); 

   }
}
