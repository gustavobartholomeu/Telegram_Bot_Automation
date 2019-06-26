//Biblioteca para conexão WiFi no ESP32 
#include <WiFi.h>
#include <WiFiClientSecure.h>
//Biblioteca com as funçoes do Bot
#include <UniversalTelegramBot.h>
//Biblioteca do Servo
#include <Servo.h>

//-------------------------------------------------------------------------------------------------------------

#define TOKEN "Token do Bot fornecido pelo Telegram - Ocultado por segurança"
#define USR "Nome da rede WiFi"
#define SENHA "Senha da rede WiFi"
#define MAXMSG 24

//-------------------------------------------------------------------------------------------------------------

//Cliente para conexões seguras
WiFiClientSecure client;
//Objeto que permite a comunicação via Telegram
UniversalTelegramBot LE_bot(TOKEN, client);
//Objeto do Servo Motor
Servo fechadura;

//-------------------------------------------------------------------------------------------------------------

//Pinos dos LEDs
int LED_1=2;
int LED_2=4;
int LED_3=5;
//Pino do Servo(com PWM)
int SERVO=13;
//Vetor de Strings com frases desmotivacionais
String fracasso[MAXMSG]={"Não foi fácil,\nmas você fracassou novamente!",
"Tudo vai piorar. É só questão de tempo!",
"Esqueça os erros do passado.\nPlaneje os erros do futuro!",
"Nunca é tarde para desistir!",
"O pior está por vir. \nAcredite!",
"Lute como nunca.\nPerca como sempre!",
"Você é mais fraco do que pensa \ne mais limitado do que imagina!",
"Reclame menos.\nFracasse mais!",
"Nenhuma solução é maior que o seu problema.",
"Se quiser algo mal feito,\nfaça você mesmo!",
"Só vai dar errado se você tentar!",
"Acreditar que você pode \njá é metade do caminho errado.",
"Fica tranquilo,\ntudo vai dar errado!",
"Foco.\nForça.\nFracasso.",
"Nenhum obstáculo é grande para quem desiste!",
"Se você pode sonhar,\nvocê pode desistir!",
"Nada como um fracasso após o outro!",
"O caminho é longo,\nmas a derrota é certa.",
"Primeiro eles riem dos seus sonhos.\nDepois eles riem dos seus fracassos.",
"Não importa o seu hoje, você tem total capacidade de piorar o seu amanhã!",
"Seu maior problema é você!",
"Se você quiser,\nse você se esforçar,\nse você treinar,\nse você entrar de cabeça,\ne se você se concentrar,\nnada garante que você vai conseguir.",
"Todo fracasso começa com a decisão de tentar.",
"Sem lutas, não há derrotas!"};


//-------------------------------------------------------------------------------------------------------------

//Função de conectara WiFi
void InitWiFi();
//Função que seleciona uma frase desmotivacional aleatória
String get_frase();

//-------------------------------------------------------------------------------------------------------------

void setup(){
  Serial.begin(115200);
 
  //Inicializa o WiFi e se conecta à rede
  InitWiFi();
 
  //Coloca o pino do LED como saída e o apaga
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);

  //Coloca o servo na porta 13 da placa
   fechadura.attach(SERVO);
   fechadura.write(0);
}

//-------------------------------------------------------------------------------------------------------------

void loop(){
  int numNewMessages = LE_bot.getUpdates(LE_bot.last_message_received + 1);
  for(int i=0; i<numNewMessages; i++){
     String comando = LE_bot.messages[i].text;
    if(comando.equalsIgnoreCase("/start")){

      String menu = "Conexão Estabelecida\nLiga LED N - Liga o led de níumero N (1 a 3)\nDesliga LED N - Desliga o LED de número N(1 a 3)\nAbre porta - Abre a porta acionando um servo\nFecha porta - Fecha a porta acionando um servo\nAjuda - Envia uma frase desmotivacional";
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), menu);
   
    }else if(comando.equalsIgnoreCase("Liga LED 1")){
     
      digitalWrite(LED_1, HIGH);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 1 Ligado!");
   
    }else if(comando.equalsIgnoreCase("Desliga LED 1")){
     
      digitalWrite(LED_1, LOW);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 1 Desligado!");
   
    }else if(comando.equalsIgnoreCase("Liga LED 2")){

      digitalWrite(LED_2, HIGH);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 2 Ligado!");
   
    }else if(comando.equalsIgnoreCase("Desliga LED 2")){
     
      digitalWrite(LED_2, LOW);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 2 Desligado!");
   
    }else if(comando.equalsIgnoreCase("Liga LED 3")){

      digitalWrite(LED_3, HIGH);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 3 Ligado!");
   
    }else if(comando.equalsIgnoreCase("Desliga LED 3")){
     
      digitalWrite(LED_3, LOW);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "LED 3 Desligado!");
   
    }else if(comando.equalsIgnoreCase("Abre porta")){

       fechadura.write(90);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "Porta Aberta!");
   
    }else if(comando.equalsIgnoreCase("Fecha porta")){
     
       fechadura.write(0);
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), "Porta Fechada!");
   
    }else if(comando.equalsIgnoreCase("ajuda")){
     
      String frase;
      frase=get_frase();
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), frase);
   
    }else{
      
      String resp;
      resp = "ERRO! Comando não encontrado!";
      LE_bot.sendMessage(String(LE_bot.messages[i].chat_id), resp);
   
    }
  }
}

//-------------------------------------------------------------------------------------------------------------

void InitWiFi(){
 
  Serial.print("Conectando a rede: ");
  Serial.println(USR);
 
  //Inicia em modo station(mais um dispositivo na rede, o outro modo é o Access Point) e se conecta à rede WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(USR, SENHA);
 
  //Enquanto não estiver conectado à rede imprime um ponto como se estivesse carregando
  while(WiFi.status()!=WL_CONNECTED){
   
    Serial.print(".");
    delay(500);
 
  }
 
  //Quando Conectar imprime que está conectado
  Serial.println();
  Serial.println("Conectado com sucesso!");
}

//-------------------------------------------------------------------------------------------------------------

String get_frase(){
  int num=random(MAXMSG);
  return fracasso[num];
}

//-------------------------------------------------------------------------------------------------------------
