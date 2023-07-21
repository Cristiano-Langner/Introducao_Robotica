#include<Servo.h>
#include <Ultrasonic.h>

//Declarando variáveis globais.

volatile int canal_1, canal_2, canal_3, canal_4, canal_5, canal_6;          
Servo base, garra, altura, braco;
int angulobase=0, angulogarra=0, anguloaltura=0, angulobraco=0; 
int cont=0, cont2=0, cont3=0,contpos=0, pos=0, vpos=0, acho=0;
int cont4=0, cont5=0, soma1=0, soma2=0;
float cmMsec;
long tempoa=0;
long microsec =0;
#define pino_trigger 2
#define pino_echo 3
Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup(){
  
Serial.begin(9600);
pinMode(13, INPUT);                //Entrada canal 6.
pinMode(12, INPUT);                //Entrada canal 5.
pinMode(11, INPUT);                //Entrada canal 4.
pinMode(10, INPUT);                //Entrada canal 3.
pinMode(9, INPUT);                 //Entrada canal 2.
pinMode(8, INPUT);                 //Entrada canal 1.
base.attach(4);                    //Porta 4 controlar motor base.
garra.attach(5);                   //Porta 5 controlar motor garra.
altura.attach(6);                  //Porta 6 controlar motor altura.
braco.attach(7);                   //Porta 7 controlar motor distância.
base.write(90);                    //Inicia o motor da base no ângulo 90.
garra.write(100);                  //Inicia o motor da garra no ângulo 100.
altura.write(90);                  //Inicia o motor da altura no ângulo 90.
braco.write(50);                   //Inicia o motor da distância no ângulo 50.
}

//Loop código principal.
void loop(){
    canal_1 = pulseIn(8,HIGH);        //Aquisição pwm canal 1.
    canal_2 = pulseIn(9,HIGH);        //Aquisição pwm canal 2.
    canal_3 = pulseIn(10,HIGH);       //Aquisição pwm canal 3.
    canal_4 = pulseIn(11,HIGH);       //Aquisição pwm canal 4.
    canal_5 = pulseIn(12,HIGH);       //Aquisição pwm canal 5.
    canal_6 = pulseIn(13,HIGH);       //Aquisição pwm canal 6.
    
//Modo manual.
    if(canal_1>=950 & canal_1<=2010 & canal_6<=1200){          //Condições entrar no modo manual.
      cont=0;
      cont2=0;
      cont3=0;
      Serial.println("Modo manual: ");
      anguloaltura=map(canal_3,950,2010,0,180);                //Função transforma valor do pwm em ângulo para o motor altura.
      altura.write(anguloaltura);                              //Escreve ângulo no motor altura.
      angulogarra=map(canal_5,950,2010,100,120);               //Função transforma valor do pwm em ângulo para o motor garra.
      garra.write(angulogarra);                                //Escreve ângulo no motor garra.
      angulobase=map(canal_4,950,2010,180,0);                  //Função transforma valor do pwm em ângulo para o motor base.
      base.write(angulobase);                                  //Escreve ângulo no motor base.
      angulobraco=map(canal_2,950,2010,0,120);                 //Função transforma valor do pwm em ângulo para o motor distância.
      braco.write(angulobraco);                                //Escreve ângulo no motor distância.
    }

//Modo automático - sensor ultrassônico.
    if(canal_1>=950 & canal_1<=2010 & canal_6<=1800 & canal_6>=1200){ //Condições para entrar no modo automático.
      delay(50);
      if (cont==0){                 //Quando cont=0 os motores irão para a posição extrema lateral.
        base.write(90);             //Coloca o motor da base no ângulo 90.
        garra.write(100);           //Coloca o motor da garra no ângulo 100.
        altura.write(50);           //Coloca o motor da altura no ângulo 50.
        braco.write(90);            //Coloca o motor da distância no ângulo 90.
        pos=10;                     //Colocar valor do motor da base em 10.
        cont=1;
        cont2=0;
        delay(1000);
        }
      if (cont2==0){                       //Laço para varredura de objetos com o ultrassônico.
        digitalWrite(pino_trigger, LOW);
        delayMicroseconds(2);
        digitalWrite(pino_trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(pino_trigger, LOW);
        long tempo=micros();
        while((tempoa-tempo)<726){
          microsec = pulseIn(pino_echo, HIGH);
          tempoa=micros();
        }
        
        cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);   //Converte a informação para cm.
        Serial.print("Distancia em cm:   ");                                
        Serial.println(cmMsec);
        if(cmMsec<=17 & cmMsec>8){                      //Testa se o objeto está no raio de alcance (em cm).
          pos=pos+15;
          base.write(pos);                                             
          cont2=1;                                      //Para sair do laço de varredura.
          vpos=pos;
          acho=1;
        }
        else if(pos<=90){                           //Limita a área de busca em 90 graus.
          pos++;                                    
          base.write(pos);                          //Base incrementa um grau.                                                          
          }
          else{                                                                
            cont=0;                                                   
            cont3++;
            pos=0;                   
            }
        
          }
        if(acho==1){            //Quando encontra a uma peça executa este laço.
          braco.write(80);
          delay(400);
          altura.write(90);
          delay(400);
          braco.write(70);
          delay(400);
          altura.write(100);
          delay(400);
          braco.write(60);
          delay(400);
          altura.write(110);
          delay(400);
          braco.write(50);
          delay(400);
          altura.write(120);
          delay(400);
          braco.write(45);
          delay(400);
          angulobraco=map(cmMsec,8,23.5,45,30);
          anguloaltura=map(cmMsec,8,23.5,120,60);
          cont4=45-angulobraco;
          cont5=120-anguloaltura;
          soma1=45;
          soma2=120;
          while(soma1>angulobraco || soma2>anguloaltura){
            Serial.println("Entrouuuuuuuu: ");
            if(soma1>angulobraco){
              soma1--;
              braco.write(soma1);
              }
            if(soma2>anguloaltura){
              soma2--;
              altura.write(soma2);
              }
            delay(100);
          } 
          delay(1000);
          garra.write(120);
          delay(1000);
          braco.write(80);
          delay(800);
          altura.write(50);
          delay(800);
          base.write(170);
          delay(800);
          soma1=50;
          soma2=80;
          while(soma1<90 || soma2>40){
            if(soma1<90){
              soma1++;
              altura.write(soma1);
              delay(20);
              }
            if(soma2>40){
              soma2--;
              braco.write(soma2);
              delay(20);
              }
            }
          delay(100);
          garra.write(100);
          delay(800);
          altura.write(120);
          delay(800);
          braco.write(70);
          delay(800);
          altura.write(50);
          acho=0;
          cont=0;
          }
        if(cont3>=2){        //Após duas varreduras seguidas sem encontrar nada para de procurar e espera.
          cont=1;
          cont2=1;
          delay(500);
          canal_2 = pulseIn(9,HIGH);        //Aquisição pwm canal 2.
          Serial.println("Aguardando intervenção do usuário... ");
          if(canal_2<=1100){   //Caso levado o stick do canal 2 para baixo será iniciado novo processo de varredura.
            cont3=0;
            cont=0;
            cont2=0;
            }
        }
    }
    if(canal_6>=1900 & canal_1>=950 & canal_1<=2010 ){            //Condição pré-definido.

//Etapa 1: Primeira peça.
      
      base.write(10);                       //Coloca o motor da base no ângulo 10.
      garra.write(100);                      //Coloca o motor da garra no ângulo 100.
      altura.write(50);                     //Coloca o motor da altura no ângulo 50.
      braco.write(100);                      //Coloca o motor da distância no ângulo 100.
      delay(1000);
      braco.write(80);
      delay(400);
      altura.write(90);
      delay(400);
      braco.write(70);
      delay(400);
      altura.write(100);
      delay(400);
      braco.write(60);
      delay(400);
      altura.write(110);
      delay(400);
      braco.write(50);
      delay(400);
      altura.write(120);
      delay(400);
      braco.write(43);
      delay(400);
      soma1=43;
      soma2=120;
      while(soma1>40 || soma2>100){
        if(soma1>40){
          soma1--;
          braco.write(soma1);
          }
        if(soma2>100){
          soma2--;
          altura.write(soma2);
          }
            delay(100);
          }
      garra.write(100);
      delay(400);
      garra.write(120);
      delay(400);
      braco.write(80);
      delay(800);
      altura.write(50);
      delay(800);
      base.write(170);
      delay(800);
      soma1=50;
      soma2=80;
      while(soma1<90 || soma2>40){
        if(soma1<90){
          soma1++;
          altura.write(soma1);
          delay(20);
          }
        if(soma2>40){
          soma2--;
          braco.write(soma2);
          delay(20);
          }
        }
        delay(100);
        garra.write(100);
        delay(800);
        altura.write(120);
        delay(800);
        braco.write(70);
        delay(800);
        altura.write(50);
        
//Etapa 2: Segunda peça.
        
      base.write(10);               //Coloca o motor da base no ângulo 10.
      garra.write(100);             //Coloca o motor da garra no ângulo 100.
      altura.write(50);             //Coloca o motor da altura no ângulo 50.
      braco.write(100);             //Coloca o motor da distância no ângulo 100.
      delay(1000);
      braco.write(80);
      delay(400);
      altura.write(90);
      delay(400);
      braco.write(70);
      delay(400);
      altura.write(100);
      delay(400);
      braco.write(60);
      delay(400);
      altura.write(110);
      delay(400);
      braco.write(50);
      delay(400);
      altura.write(120);
      delay(400);
      braco.write(43);
      delay(400);
      soma1=43;
      soma2=120;
      while(soma1>40 || soma2>100){
        if(soma1>40){
          soma1--;
          braco.write(soma1);
          }
        if(soma2>100){
          soma2--;
          altura.write(soma2);
          }
            delay(100);
          }
      garra.write(100);
      delay(400);
      garra.write(120);
      delay(400);
      braco.write(80);
      delay(800);
      altura.write(50);
      delay(800);
      base.write(150);
      delay(800);
      soma1=50;
      soma2=80;
      while(soma1<90 || soma2>40){
        if(soma1<90){
          soma1++;
          altura.write(soma1);
          delay(20);
          }
        if(soma2>40){
          soma2--;
          braco.write(soma2);
          delay(20);
          }
        }
        delay(100);
        garra.write(100);
        delay(800);
        altura.write(120);
        delay(800);
        braco.write(70);
        delay(800);
        altura.write(50);

//Etapa 3: Terceira peça.
        
      base.write(10);                 //Coloca o motor da base no ângulo 10.
      garra.write(100);               //Coloca o motor da garra no ângulo 100.
      altura.write(50);               //Coloca o motor da altura no ângulo 50.
      braco.write(100);               //Coloca o motor da distância no ângulo 100.
      delay(1000);
      braco.write(80);
      delay(400);
      altura.write(90);
      delay(400);
      braco.write(70);
      delay(400);
      altura.write(100);
      delay(400);
      braco.write(60);
      delay(400);
      altura.write(110);
      delay(400);
      braco.write(50);
      delay(400);
      altura.write(120);
      delay(400);
      braco.write(43);
      delay(400);
      soma1=43;
      soma2=120;
      while(soma1>40 || soma2>100){
        if(soma1>40){
          soma1--;
          braco.write(soma1);
          }
        if(soma2>100){
          soma2--;
          altura.write(soma2);
          }
            delay(100);
          }
      garra.write(100);
      delay(400);
      garra.write(120);
      delay(400);
      braco.write(80);
      delay(800);
      altura.write(50);
      delay(800);
      base.write(130);
      delay(800);
      soma1=50;
      soma2=80;
      while(soma1<90 || soma2>40){
        if(soma1<90){
          soma1++;
          altura.write(soma1);
          delay(20);
          }
        if(soma2>40){
          soma2--;
          braco.write(soma2);
          delay(20);
          }
        }
        delay(100);
        garra.write(100);
        delay(800);
        altura.write(120);
        delay(800);
        braco.write(70);
        delay(800);
        altura.write(50);    
      }
}
