/*          
 *           
 *           - Programa que realiza o acionamento dos motores para a entrega dos growlers 
 *           - Fechamento de malha de com sensor de entrega baseado em laser e sensor LDR
 *           - Modificado por Raphael Fernando Oliveira 
 *           
 *           $$$$$$$$$$ VERSÃO (( 2.0 - PÁ PARA CIMA E MICRO-PASSOS )) $$$$$$$$$$$$
 *           
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>


OneWire pinoSensorTemperatura(A0);
DallasTemperature barramento(&pinoSensorTemperatura);
DeviceAddress tempSensor;

volatile char captura = 'x'; //Usado na função de interrupção
volatile int contaGrowler = 0; //Usado na função de interrupção
int count = 0;
char numeroCanaleta;
char canaletaStatus[10];
char lista[10];
char validaSensor;
char evitaContagemTravamento = 'y'; //Variável auxiliar para permitir 5 tentativas quando há travamento (No if que trata o tempo, erro sensor posição)

int media = 0;
int valormedia = 0;
int passo = 0;
int alto = 0;
int baixo = 0;
long int estouro = 0;

//Entrada sensores de barreira laser
int barreiraEsquerda = {2};
int barreiraDireita =  {3};

//Entrada sensores mecânicos
int S1 = {13};
int S2 = {14};
int S3 = {15};
int S4 = {16};
int S5 = {17};
int S6 = {22};
int S7 = {23};
int S8 = {24};
int S9 = {25};
int S10 = {26};

//Portas de saída motores e fechadura magnética
int M1 = {39};
int M2 = {40};
int M3 = {41};
int M4 = {42};
int M5 = {46};
int M6 = {47};
int M7 = {48};
int M8 = {49};
int M9 = {50};
int M10 = {51};
int FECHADURA = {52};

//Declaração das funções existentes no programa
void CAPTURAR_ENTREGA();
void FUNCAO_ENTREGA();
void STATUS_GERAL_CANALETAS();
void ABRE_PORTA();
void TEMPERATURA_SENSOR_INTERNO();

void setup() {
    Serial1.begin(9600);    
    barramento.begin();
    barramento.getAddress(tempSensor, 0);        
    pinMode(barreiraEsquerda, INPUT_PULLUP);
    pinMode(barreiraDireita, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(barreiraEsquerda), CAPTURAR_ENTREGA, RISING);
    attachInterrupt(digitalPinToInterrupt(barreiraDireita), CAPTURAR_ENTREGA , RISING);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(M3, OUTPUT);
    pinMode(M4, OUTPUT);
    pinMode(M5, OUTPUT);
    pinMode(M6, OUTPUT);
    pinMode(M7, OUTPUT);
    pinMode(M8, OUTPUT);
    pinMode(M9, OUTPUT);
    pinMode(M10, OUTPUT);
    pinMode(FECHADURA, OUTPUT);    
         
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT);
    pinMode(S4, INPUT);
    pinMode(S5, INPUT);
    pinMode(S6, INPUT);
    pinMode(S7, INPUT);
    pinMode(S8, INPUT);
    pinMode(S9, INPUT);
    pinMode(S10, INPUT);
           
}//Fim do VOID_SETUP

void loop() {                 
char valorSerial;    
  while(Serial1.available() == 0);
  if(Serial1.available() > 0){
  valorSerial = Serial1.read();
  numeroCanaleta = valorSerial;                           
}
delay(500);
  switch (valorSerial) {    
    case 'v':
      Serial1.println("v2.0 - (Conjunto giratório em pé)");
      break;        
    case 't':      
      TEMPERATURA_SENSOR_INTERNO();
      break;
    case 's':      
      STATUS_GERAL_CANALETAS(lista);
      break;
    case 'A':
      ABRE_PORTA();      
      break;
    case '1':
     FUNCAO_ENTREGA(M1, S1);
      break;
    case '2':
     FUNCAO_ENTREGA(M2, S2);
      break;
    case '3':
     FUNCAO_ENTREGA(M3, S3);
      break;
    case '4':
     FUNCAO_ENTREGA(M4, S4);
      break;
    case '5':
     FUNCAO_ENTREGA(M5, S5);
      break;
    case '6':
     FUNCAO_ENTREGA(M6, S6);
      break;
    case '7':
     FUNCAO_ENTREGA(M7, S7);
      break;
    case '8':
     FUNCAO_ENTREGA(M8, S8);
      break;
    case '9':
     FUNCAO_ENTREGA(M9, S9);
      break;
    case '0':
     FUNCAO_ENTREGA(M10, S10);
      break;   
    default:
      break;  
  }      
  for(int i = 0; i < 10; i++){          
    lista[i] = canaletaStatus[i];                          
  }    
  
}//Fim do VOID_LOOP

void FUNCAO_ENTREGA(int CANALETA, int SENSOR){       
  unsigned int posicao = String(numeroCanaleta).toInt(); 
  do{
      if((digitalRead(barreiraEsquerda) == HIGH) || (digitalRead(barreiraDireita) == HIGH)){  
      validaSensor = 'x'; //Sem sinal de luz no sensor - sensor inoperante - Nivel logico alto na entrada     
      }else{
      validaSensor = 'y'; //Com sinal de luz no sensor - sensor ok - Nivel logico baixo na entrada
      } 
      contaGrowler = 0; 
      captura = numeroCanaleta;
      evitaContagemTravamento = 'y';            
      passo = 0;
      media = 0;
      if(numeroCanaleta == '3' || numeroCanaleta == '8') {
        digitalWrite(CANALETA, HIGH); 
        delay(10);                           
        digitalWrite(CANALETA, LOW);
        delay(300);        
        digitalWrite(CANALETA, HIGH);
      }
      if(numeroCanaleta == '1' || numeroCanaleta == '5' || numeroCanaleta == '6' || numeroCanaleta == '0') {
        digitalWrite(CANALETA, HIGH); 
        delay(10);                           
        digitalWrite(CANALETA, LOW);
        delay(300);        
        digitalWrite(CANALETA, HIGH);
      }        
      if(numeroCanaleta == '2' || numeroCanaleta == '4' || numeroCanaleta == '7' || numeroCanaleta == '9') {        
        digitalWrite(CANALETA, HIGH); 
        delay(10);                           
        digitalWrite(CANALETA, LOW);
        delay(300);        
        digitalWrite(CANALETA, HIGH);                 
      }                    
  while(passo < 2){
      media++;        
      estouro++;              
  if(estouro > 40000){    
      digitalWrite(CANALETA, LOW); //Desliga motor por tempo, não detectou sensor de posição
      evitaContagemTravamento = 'x';
      if(validaSensor == 'x'){
        validaSensor = 'z'; // Valida o status de entrega por tempo, não deixa os outros status assumirem
        count = 5;      
      }           
      canaletaStatus[posicao]= '2';// Status de travamento ou sensor de posição desconectado ou danificado      
      delay(1000); //Tempo adicional para não sobrecarregar a saída(Mosfet)
      if(captura == 'y' ){
        validaSensor = 'z'; // Valida o status de entrega por tempo, não deixa os outros status assumirem        
        canaletaStatus[posicao] = '4';// Status de entrega por estouro de tempo sensor com problema, houve o destravamento em alguma das tentativas
        count = 5;           
      }
      estouro = 0;
      passo = 3;
  }      
  if(digitalRead(SENSOR) == HIGH){
      baixo++;      
  }
  if(digitalRead(SENSOR) == LOW){
      alto++;                 
  } 
   //Media == 50 original   
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
      digitalWrite(CANALETA, LOW);
      count++;
      estouro = 0;     
      delay(1500);           
   if(captura == 'y' && validaSensor == 'y'){               
      count = 5;
      canaletaStatus[posicao]= '0';// Status de entrega Ok                    
   }
   if(validaSensor == 'x'){               
      count = 5;
      canaletaStatus[posicao]= '3';// Status de sensor de barreira não operante, executa a tentativa atual e encerra                     
   }      
   if(captura != 'y' && count == 3 && validaSensor == 'y' && evitaContagemTravamento == 'y'){
      count = 5;        
      canaletaStatus[posicao] = '1';// Status de canaleta sem produto                  
   }
   if(captura == 'e'){
    count = 5;
    canaletaStatus[posicao] = '5';// Status de descarrilamento ou queda de growler a mais                                    
   }
 }while(count < 5);
 if(posicao == 0){
    Serial1.print("10");             
 }else{    
    Serial1.print(posicao);
 }
  Serial1.print(" = ");
  Serial1.println(canaletaStatus[posicao]);
  Serial1.println("");          
 count = 0;
 captura = 'x';                     
}

void CAPTURAR_ENTREGA(){
  static unsigned long debounce;
  captura = 'y';    
    if((millis() - debounce) > 150){
      contaGrowler++;
      debounce = millis();
    }
    if(contaGrowler > 3){
      captura = 'e';     
    }
    delayMicroseconds(100000);                   
}

void STATUS_GERAL_CANALETAS(char lista[10]){ 
  String geralStatus = "";      
  for(int i = 1; i <= 10; i++){
    if(i == 10){
      lista[10] = lista[0];    
  }   
    geralStatus += lista[i];//Essa variavel contem a lista em formato string   
    Serial1.print(i);
    Serial1.print(" = ");  
    Serial1.println(lista[i]);                                               
  }
       
}

void ABRE_PORTA(){
  for(int i = 0; i < 10; i++){
    digitalWrite(FECHADURA, HIGH);
    delay(50);      
    digitalWrite(FECHADURA, LOW);
    delay(50);
  }
}

void TEMPERATURA_SENSOR_INTERNO(){
    String temperaturaInterna = "";
    barramento.requestTemperatures();
    temperaturaInterna = lround(barramento.getTempC(tempSensor));
    temperaturaInterna += "\n";    
    Serial1.print(temperaturaInterna);
}
