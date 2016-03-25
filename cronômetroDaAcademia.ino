#include <NewTone.h>
#include <IRremote.h>

#define lachSegUni 2
#define clockSegUni 3
#define dataSegUni 4
#define lachSegDez 5
#define clockSegDez 6
#define dataSegDez 7
#define lachMinUni 8
#define clockMinUni 9
#define dataMinUni 10
#define receptor 12
#define buzzer 11

#define cima_A 0x490
//#define cima_B 0xFF629D
#define baixo_A 0xC90
//#define baixo_B 0xA3C8EDDB
#define esq_A 0x90
//#define esq_B 0xFF22DD
#define dir_A 0x890
//#define dir_B 0xFFC23D
#define ok_A 0xA90
#define ok_B 0xF124B0B6
#define um_A 0x10
//#define um_B 0xC101E57B
#define dois_A 0x810
//#define dois_B 0x97483BFB
#define tres_A 0x410
//#define tres_B 0xF0C41643
#define quatro_A 0xC10
//#define quatro_B 0x9716BE3F
#define cinco_A 0x210
//#define cinco_B 0x3D9AE3F7
#define seis_A 0xA10
//#define seis_B 0x6182021B
#define sete_A 0x610
//#define sete_B 0x8C22657B
#define oito_A 0xE10
//#define oito_B 0x488F3CBB
#define nove_A 0x110
//#define nove_B 0x449E79F
#define zero_A 0x910
//#define zero_B 0x1BC0157B
#define ast_A 0x290
//#define ast_B 0x32C6FDF7
#define tralha_A 0xA50
//#define tralha_B 0x3EC3FC1B

IRrecv sensor(receptor);
decode_results sinal;

void setup ()
{
  pinMode(lachSegUni, OUTPUT);
  pinMode(clockSegUni, OUTPUT);
  pinMode(dataSegUni, OUTPUT);
  pinMode(lachSegDez, OUTPUT);
  pinMode(clockSegDez, OUTPUT);
  pinMode(dataSegDez, OUTPUT);
  pinMode(lachMinUni, OUTPUT);
  pinMode(clockMinUni, OUTPUT);
  pinMode(dataMinUni, OUTPUT);
  pinMode (buzzer, OUTPUT);

  escreverNoCronometro (8,8,8,1,1,1);
  delay(2000);
  escreverNoCronometro (10,10,10,0,0,0);
  
  sensor.enableIRIn();  
}

byte segU1 = 0, segD1 = 0, minU1 = 0, segU2 = 0, segD2 = 0, minU2 = 0;

void loop()
{
  byte escolha;
    
  escolha = menu();
  
  switch (escolha)
  {
    case 1: //tempo simples
          escreverNoCronometro(minU1, segD1, segU1, 1, 0, 0); //o '1' significa que o cronometro tá sendo ajustado
          escolha = escolherTempo(&minU1, &segD1, &segU1, 0); // o '0' indica que é a contagem simples
          while(escolha != '#')
          {
            escreverNoCronometro(minU1, segD1, segU1, 0, 1, 0);
            escolha = cronometrar (minU1, segD1, segU1, 0, 0);//1º '0' contagem simples, 2º '0' contagem unica
          }
          escreverNoCronometro(10, 10, 10, 0, 0, 0);
          break;
          
    case 2: //tempo com descanso
          escreverNoCronometro(minU1, segD1, segU1, 1, 0, 0); //o '1' significa que o cronometro tá sendo ajustado
          escolha = escolherTempo(&minU1, &segD1, &segU1, 0); // o '0' indica que é a contagem do exercício
          escreverNoCronometro(minU2, segD2, segU2, 1, 0, 0); //o '1' significa que o cronometro tá sendo ajustado
          escolha = escolherTempo(&minU2, &segD2, &segU2, 1); // o '1' indica que é a contagem do descanso
          escreverNoCronometro(minU1, segD1, segU1, 0, 1, 0);
          escolha = cronometrar (minU1, segD1, segU1, 0, 0);//'0' contagem simples, '0' contagem será ininterrupta, mas precisa ser iniciada
          
          while(escolha != '#')
          {
            escreverNoCronometro(minU2, segD2, segU2, 0, 1, 1);
            escolha = cronometrar (minU2, segD2, segU2, 1, 1);//1º '1' contagem descanso, 2º '1' contagem ininterrupta
            if (escolha != '#')
            {
              escreverNoCronometro(minU1, segD1, segU1, 0, 1, 0);
              escolha = cronometrar (minU1, segD1, segU1, 0, 1);//'0' contagem simples, '1' contagem ininterrupta
            }
          }
          escreverNoCronometro(10, 10, 10, 0, 0, 0);
          break;
    default:
          mensagemDeErro();
          escreverNoCronometro(10, 10, 10, 0, 0, 0);
          break;    
  }  
}

byte menu ()
{  
  byte e = 1;
  char interrompe;
    
  do
  {
    interrompe = pegarComandoComTempo (30000);
    
    switch (interrompe)
    {
      case '*':
        escreverNoCronometro(8,8,8,1,0,0);
        delay (2000);
        break;
      case '1':
        e=1;
        break;
      case '2':
        e=2;
        break;  
      case 'o':
        break;
      case 'c':
        e++;
        break;
      case 'b':
        e++;
        break;
      case '#':
        return 0;
        break;
      case 'N':
        return 0;
        break;
      default:      
        break;
    }
    
    if(e == 3)
    {
      e=1;
    }  
    escreverNoCronometro(0,0,e,1,0,0);
    
  }while (interrompe != 'o');

  return e;
}

void mensagemDeErro()
{
  digitalWrite(lachSegUni, LOW);
  shiftOut(dataSegUni, clockSegUni, MSBFIRST, B10100000);
  digitalWrite(lachSegUni, HIGH);
  digitalWrite(lachSegDez, LOW);  
  shiftOut(dataSegDez, clockSegDez, MSBFIRST, B10100000);
  digitalWrite(lachSegDez, HIGH);
  digitalWrite(lachMinUni, LOW);
  shiftOut(dataMinUni, clockMinUni, MSBFIRST, B11110010);
  digitalWrite(lachMinUni, HIGH);
  delay (2000);
}

char pegarComandoComTempo (unsigned long tempo)
{
  unsigned long agora = millis();
  char tecla;
  
  while (millis() - agora < tempo)
  {
    if (sensor.decode(&sinal))
    {
      tecla = comando(sinal.value);
      delay (500);
      sensor.resume();
      return tecla;
    }
  }
  return 'N';
}

char pegarComandoSemTempo ()
{
  char tecla;
  
  while (!sensor.decode(&sinal));
  
  tecla = comando(sinal.value);
  delay (200);
  sensor.resume();
  return tecla;
}

char comando (long int botao)
{
  char escolhido;
  switch (botao)
  {
     case cima_A:
        escolhido = 'c';
        break;
//     case cima_B:
//        escolhido = 'c';
//        break;
     case baixo_A:
        escolhido = 'b';
        break;
//     case baixo_B:
//      escolhido = 'b';
//    break;
     case esq_A:
        escolhido = 'e';
        break;
//     case esq_B:
//        escolhido = 'e';
//        break;
     case dir_A:
        escolhido = 'd';
        break;
//     case dir_B:
//        escolhido = 'd';
//        break;
     case ok_A:
        escolhido = 'o';
        break;
     case ok_B:
        escolhido = 'o';
        break;
     case um_A:
        escolhido = '1';
        break;
//     case um_B:
//        escolhido = '1';
//        break;
     case dois_A:
        escolhido = '2';
        break;
//     case dois_B:
//        escolhido = '2';
//        break;
     case tres_A:
        escolhido = '3';
        break;
//     case tres_B:
//        escolhido = '3';
//        break;
     case quatro_A:
        escolhido = '4';
        break;
//     case quatro_B:
//        escolhido = '4';
//        break;
     case cinco_A:
        escolhido = '5';
        break;
//     case cinco_B:
//        escolhido = '5';
//        break;
     case seis_A:
        escolhido = '6';
        break;
//     case seis_B:
//        escolhido = '6';
//        break;
     case sete_A:
        escolhido = '7';
        break;
//     case sete_B:
//        escolhido = '7';
//        break;
     case oito_A:
        escolhido = '8';
        break;
//     case oito_B:
//        escolhido = '8';
//        break;
     case nove_A:
        escolhido = '9';
        break;
//     case nove_B:
//        escolhido = '9';
//        break;
     case zero_A:
        escolhido = '0';
        break;
//     case zero_B:
//        escolhido = '0';
//        break;
     case ast_A:
        escolhido = '*';
        break;
//     case ast_B:
//        escolhido = '*';
//        break;
     case tralha_A:
        escolhido = '#';
        break;
//     case tralha_B:
//        escolhido = '#';
//        break;
     default:
        escolhido = NULL;
  }
  return escolhido;
}

void escreverNoCronometro(byte unidMin, byte dezSeg, byte unidSeg, byte pontoUM, byte pontoDS, byte pontoUS)
{
  escreverNumero595 (unidSeg, pontoUS, lachSegUni, clockSegUni, dataSegUni);
  escreverNumero595 (dezSeg, pontoDS, lachSegDez, clockSegDez, dataSegDez);
  escreverNumero595 (unidMin, pontoUM,lachMinUni, clockMinUni, dataMinUni);
  return;
}

byte escolherTempo(byte *minU, byte *segD, byte *segU, byte sinal)
{
  int casaDecimal = 1, tecla;
  
  do
  {
    switch (casaDecimal)
    {
      case 1:
        tecla = alterarNumNoVisor (segU, 9, lachSegUni, clockSegUni, dataSegUni, sinal);
        if(tecla == 1 || tecla == -1)
        {
          casaDecimal += tecla;
        }
        break;
      case 2:
        tecla = alterarNumNoVisor (segD, 5, lachSegDez, clockSegDez, dataSegDez, 0);
        if(tecla == 1 || tecla == -1)
        {
          casaDecimal += tecla;
        }
        break;
      case 3:
        tecla = alterarNumNoVisor (minU, 9, lachMinUni, clockMinUni, dataMinUni, 1); //o '1' sinaliza ajuste do cronometro
        if(tecla == 1 || tecla == -1)
        {
          casaDecimal += tecla;
        }
        break;
    }
    if(casaDecimal == 4)
    {
      casaDecimal = 1;
    }
    else
    {
      if(casaDecimal == 0)
      {
        casaDecimal = 3;
      }
    }
  }while (tecla != 'o' && tecla != '#');
  
  return tecla;
}

int alterarNumNoVisor (byte *num, byte limite, byte lach, byte clock, byte data, byte ponto)
{
  byte escolha;
  
  do
  {
    escreverNumero595 (*num,  ponto, lach, clock, data);
    escolha = pegarComandoComTempo(700);
    switch (escolha)
    {
      case 'N':
        break;
      case 'c':
        if(*num == limite)
        {
          *num = 0;
        }
        else
        {
          *num = *num + 1;
        }
        break;
      case 'b':
        if(*num == 0)
        {
          *num = limite;
        }
        else
        {
          *num = *num - 1;
        }
        break;
      case 'd':
        return -1;
        
      case 'e':
        return 1;
        
      case '*':
        break;
      case '#':
        break;
      case 'o':
        break;
      default:
        if(escolha - 48 >= 0 && escolha - 48 <= limite)
        {
          *num = escolha - 48;
        }
        break;
    }
    escreverNumero595 (10,  ponto, lach, clock, data);    
    delay (300);
  }while (escolha != 'o' && escolha != '#');
  
  escreverNumero595 (*num,  ponto, lach, clock, data);
  
  return escolha;
}

void sirene(byte mU, byte sD, int sU, byte ajuste, byte pronto, byte tipoAtiv, int tempo)
{  
  for(int i=0 ; i<2 ; i++)
  {
    NewTone(buzzer, 1500);
    delay (tempo);
    
    noNewTone(buzzer);
    escreverNoCronometro(10, 10, 10, 0, 0, tipoAtiv);
    delay (tempo);
    
    if(sU < 0)
    {
       escreverNoCronometro(mU, sD, 0, ajuste, pronto, tipoAtiv);  
    }
    else
    {
      escreverNoCronometro(mU, sD, sU, ajuste, pronto, tipoAtiv);
    }    
  }
  return;
}

byte cronometrar (int minU, int segD, int segU, int tipoAtiv, int semParar)
{
  byte botao;
  int segundo = 1000, copiaMinU, copiaSegD, copiaSegU;
  unsigned long instante;
  
  if(semParar)
  {
    botao = 'o';
  }
  else
  {
    do
    {
      botao = pegarComandoSemTempo();
    }while (botao != 'o' && botao != '#');
  }
  
  if(botao == 'o')
  {    
    while(botao != '#')
    {
      instante = millis ();
      sirene (minU, segD, segU, 0, 0, tipoAtiv, 100);
      do
      {
        while (millis() - instante < segundo);
        instante = millis ();

        escreverNoCronometro(minU, segD, segU, 0, 0, tipoAtiv);
        
        copiaMinU = minU;
        copiaSegD = segD;
        copiaSegU = segU;
        
        segU--;
        if(segU < 0  &&  (segD > 0  ||  minU > 0))
        {
          segU = 9;
          segD--;
          
          if(segD < 0   &&   minU > 0)
          {
            segD = 5;
            minU--;
          }
        }
        segundo = 1000;
        
      }while(pegarComandoComTempo(500) != 'o'  &&  segU >= 0);
                 
      if(segU < 0)
      {
        sirene(minU, segD, segU, 0, 0, tipoAtiv, 500);
        return botao;
      }
      else
      {
        segundo = 1000 - (millis() - instante);
        minU = copiaMinU;
        segD = copiaSegD;
        segU = copiaSegU;
        sirene(minU, segD, segU, 0, 0, tipoAtiv, 500);        
        do
        {         
          escreverNoCronometro(minU, segD, segU, 0, 1, tipoAtiv); //o '1' acende o led de pronto caso o tempo tenha sido interrompido
          botao = pegarComandoSemTempo();
        }while(botao != 'o' && botao != '#');
      }     
    }       
  }  
  return botao;
}

void escreverNumero595 (byte num, byte ponto, byte l, byte c, byte d)
{
  digitalWrite(l, LOW);

  if (num<5)
  {
    if(num<3)
    {
      if (num<2)
      {
        if(num<1)
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B01111110));
        }
        else
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B00001100));
        }
      }
      else
      {
        shiftOut(d, c, MSBFIRST, soma (ponto, B10110110));
      }
    }
    else
    {
      if(num<4)
      {
        shiftOut(d, c, MSBFIRST, soma (ponto, B10011110));
      }
      else
      {
        shiftOut(d, c, MSBFIRST, soma (ponto, B11001100));
      }
    }
  }
  else
  {
    if(num<8)
    {
      if (num<7)
      {
        if(num<6)
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B11011010));
        }
        else
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B11111010));
        }
      }
      else
      {
        shiftOut(d, c, MSBFIRST, soma (ponto, B00001110));
      }
    }
    else
    {
      if(num<9)
      {
        shiftOut(d, c, MSBFIRST, soma (ponto, B11111110));
      }
      else
      {
        if(num==9)
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B11011110));
        }
        else
        {
          shiftOut(d, c, MSBFIRST, soma (ponto, B00000000));
        }
      }
    }
  }
  digitalWrite(l, HIGH);
}

byte soma (byte a, byte b)
{
  return a+b;
}
