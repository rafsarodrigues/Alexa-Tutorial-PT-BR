# Alexa-Tutorial-PT-BR


<p align="center"><b>Tutorial de desenvolvimento de uma skill Alexa </br> 
Rafael dos Santos Rodrigues, Dalton Solano dos Reis</b> – Orientador </br>  
Curso de Bacharel em Sistemas de Informação </br>
Departamento de Sistemas e Computação </br>
Universidade Regional de Blumenau (FURB) – Blumenau, SC – Brazil</p>

<p align="center"><img src="https://user-images.githubusercontent.com/42719744/200699251-b6ca4f74-c1be-4cc1-a761-bc7b0d0f56b6.png" alt="Logo Alexa" align="center"></p>

# Objetivo

&ensp;&ensp;&ensp;&ensp;Este documento tem como objetivo apresentar alguns conceitos ao desenvolver uma skill para Alexa, como criar uma skill para a Alexa, como criar e hospedar um servidor na AWS Lambda, criar uma “coisa” e vincular ao ESP32 e utilizar alguns serviços auxiliares sendo eles: DynamoDB e CloudWatch.

# Configurando Arduino IDE:

&ensp;&ensp;&ensp;&ensp;O Arduino IDE é um ambiente de desenvolvimento de código aberto para programação de microcontroladores. Ele está em constante desenvolvimento e pode suportar um número crescente de plataformas, incluindo a maioria dos módulos baseados em ESP32. Ele deve ser instalado junto com as definições da placa ESP32, biblioteca MQTT e biblioteca ArduinoJson.

&ensp;&ensp;&ensp;&ensp;O Arduino IDE pode ser baixado no site do Arduino: 

<p align="center">https://www.arduino.cc/en/Main/Software</p>

&ensp;&ensp;&ensp;&ensp;Após a instalação, abra o Arduino IDE e siga a sequência e as figuras abaixo: 

*******
+ Passo 1 - Para configurar o ESP32 no Arduino IDE, entre em "File" -> "Preferences";  
+ Passo 2 - E cole o seguinte URL no campo "Additional Boards Manager URLs":  
<p align="center">https://dl.espressif.com/dl/package_esp32_index.json</p>  
&ensp;&ensp;&ensp;&ensp;Obs.: Caso altere o idioma é necessário reiniciar o Arduino IDE.  

+ Passo 3 - Entre em "Tools" -> "Board" -> "Boards Manager" procure por “esp32” e instale a última versão da “esp32 by Espressif Systems” da lista, nesse processo o Arduino IDE irá baixar os arquivos necessários para o ESP32.  
  Ao abrir o "Boards Manager" ele pode demorar cerca de 20 segundos para atualizar todos os arquivos de hardware (se a rede estiver em más condições, pode levar mais tempo).  
+ Passo 4 - Abrir novamente o “Tools” e validar se o item “Board” se encontra com o modelo do seu componente, caso não alterar e selecionar o correto.  
  É possível conferir os modelos de placas compatíveis com o Arduino IDE pelo documento disponibilizado pela “Espressif Systems” no site:  
  <p align="center">https://github.com/espressif/arduino-esp32/blob/master/boards.txt</p>  
+ Passo 5 - Abra o item “Sketch” -> “Include Library” -> “Manage Libraries” procure por “MQTT” e instale a última versão do criado “Joel Gaehwiler”.  
+ Passo 6 - Abra o item “Sketch” -> “Include Library” -> “Manage Libraries” procure por “ArduinoJson” e instale a última versão do criado “Benoit Blanchon”.  
*******

<p align="center">Figura 1 - Passo 1 -> Configurando Arduino para o ESP32</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem1.png" alt="Figura 1 - Passo 1 -> Configurando Arduino para o ESP32" align="center"></p>

<p align="center">Figura 2 - Passo 2 -> Configurando Arduino para o ESP32 (continuação)</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem2.png" alt="Figura 2 - Passo 2 -> Configurando Arduino para o ESP32 (continuação)" align="center"></p>

<p align="center">Figura 3 - Passo 3 -> Instalando pacote ESP32</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem3.png" alt="Figura 3 - Passo 3 -> Instalando pacote ESP32" align="center"></p>

<p align="center">Figura 4 - Passo 4 -> Selecionando placa ESP32</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem4.png" alt="Figura 4 - Passo 4 -> Selecionando placa ESP32" align="center"></p>

<p align="center">Figura 5 - Passo 5 -> Instalando biblioteca MQTT</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem5.png" alt="Figura 5 - Passo 5 -> Instalando biblioteca MQTT" align="center"></p>

<p align="center">Figura 6 - Passo 6 -> Instalando biblioteca ArduinoJson</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem6.png" alt="Figura 6 - Passo 6 -> Instalando biblioteca ArduinoJson"></p>

# Código Arduino IDE

&ensp;&ensp;&ensp;&ensp;O código a seguir se encontra no item “Código Arduino” que foi disponibilizado no github pelo link:  
<p align="center">https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/</p>  
&ensp;&ensp;&ensp;&ensp;Será quebrado o código explicando para que serve cada parte e o que é necessário ser alterado para executar com os seus dados.

## Secrets.h

&ensp;&ensp;&ensp;&ensp;No arquivo “secrets.h” ele serve como auxiliar para armazenar os certificados, chaves e dados do Wi-Fi que será utilizado pelo ESP32.  

&ensp;&ensp;&ensp;&ensp;Na variável “THINGNAME” seria o nome da “coisa” que será criada posteriormente na Amazon que será o ponto de acesso entre o ESP32 e o AWS Lambda.

~~~C
#define THINGNAME "ESP32_FURB_G"
~~~

&ensp;&ensp;&ensp;&ensp;Nas constantes WIFI_SSID, WIFI_PASSWORD e AWS_IOT_ENDPOINT, respectivamente são o login e senha do Wi-Fi e o endpoint disponibilizado no AWS Lambda.

~~~C
const char WIFI_SSID[] = "";               
const char WIFI_PASSWORD[] = "";           
const char AWS_IOT_ENDPOINT[] = "a1tisats.iot.sa-east-1.amazonaws.com";  
~~~

&ensp;&ensp;&ensp;&ensp;As constantes AWS_CERT_CA, AWS_CERT_CRT e AWS_CERT_PRIVATE são também informações que terão que ser preenchidas conforme “coisa” criada na Amazon sendo respectivamente a Amazon Root CA 1, Device Certificate e Device Private Key.  

~~~C
static const char AWS_CERT_CA[] PROGMEM = R"EOF()EOF";
static const char AWS_CERT_CRT[] PROGMEM = R"KEY()KEY";
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY()KEY"; 
~~~

&ensp;&ensp;&ensp;&ensp;Essas informações são sensíveis pois dependendo do tipo de política utilizada na Amazon qualquer pessoa com esses dados poderá acessar o seu endpoint e executar requisições.

## AWS_BULB_SELECT.ino

&ensp;&ensp;&ensp;&ensp;O arquivo “AWS_BULB_SELECT.ino” contém o código principal onde será realizada o recebimento e envio das informações para a Amazon AWS e realizada a conexão com o Wi-Fi.
&ensp;&ensp;&ensp;&ensp;Primeiro é realizado a importação do arquivo “secrets.h” e as bibliotecas importadas que serão utilizadas no código.

~~~C
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
~~~

&ensp;&ensp;&ensp;&ensp;Definimos em constantes os tópicos MQTT que iremos utilizar para publicar e se inscrever.

~~~C
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
~~~

&ensp;&ensp;&ensp;&ensp;Com isso a ordem de execução primeiro será o método “setup()” que executa o “connectAWS()” que irá se conectar ao Wi-Fi e após sucesso se inscrever no tópico criado na Amazon AWS. Posteriormente, será executado o método “setupBulb()” que irá escolher quais pinos do ESP32 serão utilizados.

~~~C
void setup() {
  Serial.begin(9600);
  connectAWS();
  setupBulb();
}
~~~

~~~C
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".wifi");
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println(", failed to connect to WiFi");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}
~~~

~~~C
void setupBulb()
{
  pinMode(14, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT); 
} 
~~~

&ensp;&ensp;&ensp;&ensp;Com isso será executado em loop o método “loop()” que irá validar a conexão com a rede e Amazon AWS e caso algum deles caia o método “reconnect()” irá tentar reconectar com esses serviços.

~~~C
void loop() {
  if (!client.connected()) {reconnect();}
  client.loop();
  delay(1000);
}
~~~

~~~C
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(THINGNAME)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(AWS_IOT_PUBLISH_TOPIC, "Reconnected");
      // ... and resubscribe
      client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
~~~

&ensp;&ensp;&ensp;&ensp;Ao receber um mensagem via tópico MQTT será chamado o método messageHandler(String &topic, String &payload) que recebe o nome do tópico e alguma “String”, neste caso será esperado um arquivo JSON. Esse método irá “deserializar” o JSON criando uma lista sendo que cada item será referenciado a uma variável para realizar as tratativas, neste caso seria gerar a ordem de execução das lâmpadas e por fim é executado o método “publishReturnMessage()” para retornar uma mensagem de sucesso para a Amazon AWS.

~~~C
void messageHandler(String &topic, String &payload) {
  Serial.println("Log incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
    String sequence = doc["sequence"];
    char str_array[sequence.length()];
    sequence.toCharArray(str_array, sequence.length());
    const String location = doc["location"];
    const int delayBulb = doc["delay"];
    byte index = 0;
    uint8_t strings[128]; // an array of pointers to the pieces of the above array after strtok()
    char *ptr = NULL;
    ptr = strtok(str_array, ",");  // delimiter
   while (ptr != NULL)
   {
      strings[index] = (uint8_t)atoi(ptr);
      index++;
      ptr = strtok(NULL, ",");
   }
   Serial.println("The Pieces separated by strtok()");
   for (int n = 0; n < index; n++)
   {
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);
      digitalWrite(strings[n], HIGH);
      delay(delayBulb);
      digitalWrite(strings[n], LOW);
      delay(100);
   }
   publishReturnMessage();}
~~~

~~~C
void publishReturnMessage()
{
  StaticJsonDocument<200> doc;
  doc["finish"] = "OK_ALEXA";
  doc["language"] = "en-US";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
~~~

# Criação de uma skill para a Amazon Alexa

&ensp;&ensp;&ensp;&ensp;A Amazon Alexa fornece alguns tipos de serviços como exemplo a serem alterados: Smart Home Skills que são modelos já existentes para o uso de conexão com aparatos na nuvem, Game Skills com jogo interativos, Music & Audio Skills para músicas e controlar a playlist, Food Ordering Skill que procura restaurantes próximos ou que pode realizar pedidos via comando de voz, Video Skills para visualizar e controlar vídeos por comando de voz, Flash Briefing & News Skills que apresenta as principais noticiais baseado no perfil do usuário, Connected Vehicle Skills para controlar e verificar o status de seus veículos conectados de sua casa ou em movimento e Custom Skills as quais são habilidades customizáveis para que o desenvolvedor criar conforme sua necessidade.  
&ensp;&ensp;&ensp;&ensp;Para mais informações referentes a exemplos de Skills é possível visualizar no site: 

<p align="center"> https://developer.amazon.com/en-US/alexa/alexa-skills-kit/get-deeper </p>

&ensp;&ensp;&ensp;&ensp;É interessante para quem tem o domínio, ou entende, o idioma inglês verifique o canal oficial dos desenvolvedores da da Alexa “Alexa Developers”.  
&ensp;&ensp;&ensp;&ensp;Segue link com lista de reprodução com uma série de vídeos com desenvolvimentos e conceitos da Alexa de forma básica:

<p align="center"><a href="https://www.youtube.com/playlist?list=PLdZn93YfA_1ZP1WFkz6bm08v3zFfWwfGW" target="_blank">
 <img src="https://user-images.githubusercontent.com/42719744/200813228-ba5ad0ff-af70-4a9a-91f4-f142ce5820d9.png" alt="Veja a playlist" width="550" height="350" border="10" />
</a></p>

## Diagrama de Atividades

&ensp;&ensp;&ensp;&ensp;O processo inicia-se pelo comando de voz efetuado pelo usuário. A Amazon Alexa sintetiza o comando em formato JSON para o servidor AWS tomar a ação de acordo com o comando. Se for um comando para alterar o estado da luz ou abrir a porta, o AWS envia a requisição para o Arduino efetuar a ação correspondente.

<p align="center">Figura 7 - Diagrama de Atividades</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem7.png" alt="Figura 7 - Diagrama de Atividades"></p>

&ensp;&ensp;&ensp;&ensp;Conforme documentação oficial da Amazon o fluxo de trabalho a seguir demonstra como uma habilidade do Alexa funciona. Neste exemplo, o usuário invoca uma habilidade simples do Alexa chamada "Hello World".

*******
+ Passo 1 - Para iniciar a habilidade, o usuário diz: "Alexa, abra o Hello World". Uma declaração ou solicitação completa ao Alexa pelo usuário é chamada de enunciado.  
+ Passo 2 - O dispositivo habilitado para Alexa envia o enunciado para o serviço Alexa na nuvem. O serviço Alexa processa o enunciado por meio do reconhecimento automático de fala para converter o enunciado em texto e, em seguida, por meio do entendimento de linguagem natural para reconhecer a intenção do texto.  
+ Passo 3 - O Alexa envia uma solicitação JavaScript Object Notation (JSON) para manipular a intenção para o serviço de computação de recursos que hospeda sua habilidade (geralmente uma função do AWS Lambda). O serviço de computação de recursos atua como back-end e executa seu código de habilidade para lidar com a intenção. Nesse caso, o servidor retorna: "Bem-vindo à habilidade Hello World".  
*******

&ensp;&ensp;&ensp;&ensp;O diagrama na Figura 8 demonstra o que acontece quando um usuário interage com uma habilidade do Alexa. Ele pressupõe que você esteja usando o serviço de computação sem servidor do AWS Lambda para hospedar seu código de habilidade.

<p align="center">Figura 8 - Diagrama da skill Hello World</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem8.png" alt="Figura 8 - Diagrama da skill Hello World"></p>

&ensp;&ensp;&ensp;&ensp;Esta documentação você pode encontrar no link:  
<p align="center"> https://developer.amazon.com/en-US/docs/alexa/workshops/build-an-engaging-skill/why-build/index.html#how-an-alexa-skill-works   </p>

&ensp;&ensp;&ensp;&ensp;E o vídeo com esta explicação se encontra no link a seguir: 

<p align="center"><a href="https://www.youtube.com/watch?v=cakcsuzS2DY&t=17s" target="_blank">
 <img src="https://user-images.githubusercontent.com/42719744/200817507-60b03efd-7de9-4d78-85e7-4cf3af2b4875.png" alt="Assista o vídeo" width="550" height="350" border="10" />
</a></p>

## Conceitos

&ensp;&ensp;&ensp;&ensp;Neste item será apresentado como realizar a criação de uma skill customizável para a Alexa utilizando os recursos do console do desenvolvedor da Alexa, AWS Lambda, DynamoDB, CloudWatch e IotCore. Mas antes se faz necessário entender alguns conceitos importantes: Alexa ajuda com várias etapas no processamento de entrada. Primeiro, ela pega a fala do usuário e a transforma em texto escrito (fala em texto). Posteriormente, ele usa um modelo de linguagem para entender o que o usuário quer dizer (compreensão da linguagem natural).  
&ensp;&ensp;&ensp;&ensp;Existem duas formas de prover o servidor, deixar hospedado na própria Alexa ou vincular a um terceiro que neste caso será o AWS Lambda.   
&ensp;&ensp;&ensp;&ensp;Um modelo de interação simples para o Alexa consiste nos seguintes elementos: Nome de invocação, intenções, enunciados, slots.  

### Nome de invocação

&ensp;&ensp;&ensp;&ensp;Um 'nome de invocação' é a palavra ou frase usada para acionar sua habilidade. De certa forma, é o equivalente da voz a um ícone de aplicativo. Esse nome de invocação geralmente corresponde ao nome da sua habilidade, mas dadas as regras sobre a escolha de um nome de invocação, poderá ser sendo um pouco diferente.

### Intenções

&ensp;&ensp;&ensp;&ensp;Uma intenção, ou intent, é o que um usuário está tentando realizar. Dentro do código, é o método a ser executado. 'Intenção' não se relaciona com as palavras específicas que um usuário diz, mas com o objetivo de alto nível que ele almeja.

### Declarações

&ensp;&ensp;&ensp;&ensp; Declarações são as frases específicas que os usuários usarão ao fazer uma solicitação após abertura da skill e que por consequência executará uma determinada ‘Intenção’. Muitas vezes, há uma grande variedade de enunciados que se encaixam na mesma intenção. E às vezes pode até ser um pouco mais variável.

### Slots

&ensp;&ensp;&ensp;&ensp; Um slot é uma variável relacionada a uma intenção que permite ao Alexa entender as informações sobre a solicitação. Por exemplo, em uma skill que sugere um filme aleatório o usuário poderá falar qual gênero do filme ele quer assistir, sendo assim o gênero é a variável que o sistema precisa interpretar de forma não fixa.

#### Tipos de slots

&ensp;&ensp;&ensp;&ensp;A Amazon fornece vários tipos de slots integrados, como datas, números, durações, hora etc. Mas os desenvolvedores podem criar slots personalizados para variáveis específicas de sua skill.

## Console do desenvolvedor Alexa

&ensp;&ensp;&ensp;&ensp;A partir da Figura 9 até Figura 23 é apresentado como configurar uma nova Skill. Será utilizado a localização do Brasil, ressalto que existem algumas funcionalidades disponíveis apenas em outras localidades então o layout da tela poderá ser diferente dependendo do idioma.

<p align="center">Figura 9- Página inicial do site da Amazon Alexa</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem9.png" alt="Figura 9- Página inicial do site da Amazon Alexa"></p>

<p align="center">Figura 10 - Página de login para o console do desenvolvedor</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem10.png" alt="Figura 10 - Página de login para o console do desenvolvedor"></p>

<p align="center">Figura 11 - Página para gerenciamos das skills</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem11.png" alt="Figura 11 - Página para gerenciamos das skills"></p>

<p align="center">Figura 12 - Selecionar o idioma e modelo Custom</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem12.png" alt="Figura 12 - Selecionar o idioma e modelo Custom"></p>

<p align="center">Figura 13 – Selecionar a linguagem de programação</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem13.png" alt="Figura 13 – Selecionar a linguagem de programação"></p>

<p align="center">Figura 14 - Selecionar “Start from Scratch”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem14.png" alt="Figura 14 - Selecionar “Start from Scratch"></p>

<p align="center">Figura 15 - Inserindo um nome de invocação da skill</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem15.png" alt="Figura 15 - Inserindo um nome de invocação da skill"></p>

<p align="center">Figura 16 - Adicionando novo intent</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem16.png" alt="Figura 16 - Adicionando novo intent"></p>

<p align="center">Figura 17 - Criando um intent personalizável</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem17.png" alt="Figura 17 - Criando um intent personalizável"></p>

<p align="center">Figura 18 - Criando um intent e slot personalizável</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem18.png" alt="Figura 18 - Criando um intent e slot personalizável"></p>

<p align="center">Figura 19 - Adicionando valores no slot personalizado</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem19.png" alt="Figura 19 - Adicionando valores no slot personalizado"></p>

&ensp;&ensp;&ensp;&ensp;No item apresentado na Figura 20 você deverá cadastrar qual declaração para ativar essa intenção.  

<p align="center">Figura 20 - Inserindo uma declaração</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem20.png" alt="Figura 20 - Inserindo uma declaração"></p>

&ensp;&ensp;&ensp;&ensp;Neste exemplo essa declaração serve apenas para iniciar o jogo escolhendo o modo e a dificuldade com dois slots personalizados.  
&ensp;&ensp;&ensp;&ensp;Após realizado todas as alterações deve-se salvar e compilar o modelo.

<p align="center">Figura 21 - Salvar e construir o modelo</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem21.png" alt="Figura 21 - Salvar e construir o modelo"></p>

<p align="center">Figura 22 - Mensagem de sucesso após construção do modelo</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem22.png" alt="Figura 22 - Mensagem de sucesso após construção do modelo"></p>

&ensp;&ensp;&ensp;&ensp;Também está disponibilizado no github o JSON que pode ser importado para realizar todos esses cadastrados de forma automática.

<p align="center">Figura 23 - Importando JSON</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem23.png" alt="Figura 23 - Importando JSON"></p>

&ensp;&ensp;&ensp;&ensp;Após isso, toda vez que é executada uma intenção será gerado um Callback para o servidor Lambda executando o método vinculado a esta intenção.

## Criando servidor AWS Lambda

&ensp;&ensp;&ensp;&ensp;Nesta etapa, deverá ser criada uma função no AWS Lambda. O site para utilizar esses serviços seria o:  
<p align="center"> https://aws.amazon.com   </p>
&ensp;&ensp;&ensp;&ensp;Os passos são descritos a partir de Figura 24 até a Figura 41.

<p align="center">Figura 24 - Página inicial da AWS Amazon</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem24.png" alt="Figura 24 - Página inicial da AWS Amazon"></p>

<p align="center">Figura 25 - Página de login da AWS Amazon</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem25.png" alt="Figura 25 - Página de login da AWS Amazon"></p>

<p align="center">Figura 26 - Selecionar o serviço Lambda</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem26.png" alt="Figura 26 - Selecionar o serviço Lambda"></p>

<p align="center">Figura 27 - Criar a função</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem27.png" alt="Figura 27 - Criar a função"></p>

<p align="center">Figura 28 - Escolher um nome e linguagem de programação</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem28.png" alt="Figura 28 - Escolher um nome e linguagem de programação"></p>

<p align="center">Figura 29 - Adicionar um gatilho</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem29.png" alt="Figura 29 - Adicionar um gatilho"></p>

&ensp;&ensp;&ensp;&ensp;Nesta etapa será necessário o ID gerado na Amazon Alexa. A Figura 30 aponta o lugar que se obtém na Amazon Alexa. Na Figura 31 mostra onde deve ser inserido o ID da Amazon Alexa. Após a inserção do ID, clicar em salvar.

<p align="center">Figura 30 - Utilizar a opção “Copy to clipboard”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem30.png" alt="Figura 30 - Utilizar a opção “Copy to clipboard”"></p>

<p align="center">Figura 31 - Escolher “Alexa IoT” e colar o código copiado anteriormente no “Skill ID”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem31.png" alt="Figura 31 - Escolher “Alexa IoT” e colar o código copiado anteriormente no “Skill ID”"></p>

&ensp;&ensp;&ensp;&ensp;Será necessário realizar o processo inverso, copiar o código ARN da função Lambda e colar no endpoint da Skill para ela conseguir acessar o backend.  A Figura 32 aponta onde se obtém na função Lambda. Na Figura 33mostra onde deve ser inserido o ID na Skill. Após a inserção do ID, clicar em salvar.

<p align="center">Figura 32 - Copiar o “Function ARN”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem32.png" alt="Figura 32 - Copiar o “Function ARN”"></p>

<p align="center">Figura 33 - Colar o “Function ARN”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem33.png" alt="Figura 33 - Colar o “Function ARN”"></p>

&ensp;&ensp;&ensp;&ensp;Deve ser realizado a configuração de permissão para a função criada, para isso deve-se seguir o passo-a-passo da Figura 34 até Figura 37.

<p align="center">Figura 34 - Clicar no hiperlink</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem34.png" alt="Figura 34 - Clicar no hiperlink"></p>

<p align="center">Figura 35 - Expandir e editar a política</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem35.png" alt="Figura 35 - Expandir e editar a política"></p>

&ensp;&ensp;&ensp;&ensp;Se as permissões corretas não forem concedidas, dificultará o acesso a determinados tópicos ou a execução de determinadas ações. Neste caso, define-se a política no nível mais permissivo. As configurações podem ser alteradas para restringir determinadas ações e tópicos conforme necessários, por isto muita atenção às permissões.  
&ensp;&ensp;&ensp;&ensp;Na tela da Figura 36, incluir o código de permissão do AWS IoT seguindo o exemplo do Quadro 1.

<p align="center">Figura 36 - Colar e salvar</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem36.png" alt="Figura 36 - Colar e salvar"></p>

~~~JSON
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "logs:CreateLogGroup",
        "logs:CreateLogStream",
        "logs:PutLogEvents"
      ],
      "Resource": "arn:aws:logs:*:*:*"
    },
    {
      "Effect": "Allow",
      "Action": [
        "iot:*"
      ],
      "Resource": "arn:aws:iot:*:*:*"
    }
  ]
}
~~~

&ensp;&ensp;&ensp;&ensp;Após salvar e revisar a política a página deverá ficar como Figura 37.

<p align="center">Figura 37 - Painel atualizado</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem37.png" alt="Figura 37 - Painel atualizado"></p>

&ensp;&ensp;&ensp;&ensp;O Lambda não tem acesso as bibliotecas/recursos do pacote ask_sdk, esse pacote seria o padrão para utilizar os recursos da Alexa com isso é necessário adicionar uma camada para o Lamba ter de onde buscar esse recurso, para isso na parte inferior do site clicar em “Add a Layer”.

<p align="center">Figura 38 - Adicionando uma camada</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem38.png" alt="Figura 38 - Adicionando uma camada"></p>

&ensp;&ensp;&ensp;&ensp;Após isso, devemos especificar qual camada iremos adicionar no caso já disponibilizaram o pacote ask_sdk na url “arn:aws:lambda:us-east-1:173334852312:layer:ask-sdk-for-python-36:1”, desta forma deve-se utilizar a opção “Specify na ARN” e colar no campo este valor.

<p align="center">Figura 39 - Especificando a camada</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem39.png" alt="Figura 39 - Especificando a camada"></p>

<p align="center">Figura 40 - Como ficará o item camada</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem40.png" alt="Figura 40 - Como ficará o item camada"></p>

&ensp;&ensp;&ensp;&ensp;No item “Code” é onde deverá ser feito a lógica após receber o JSON da Skill, esse código se encontra no github. Após subir o servidor ele já deverá se comunicar com a skill e vice-versa.

<p align="center">Figura 41 - Código do backend</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem41.png" alt="Figura 41 - Código do backend"></p>

## Alexa Presentation Language

&ensp;&ensp;&ensp;&ensp;A Alexa tem uma estrutura de design visual chamada Alexa Presentation Language (APL), que permite criar experiências interativas de voz e visual utilizando a tela do dispositivo. APL fornece elementos visuais, incluindo: gráficos, imagens, apresentações de slides e vídeo.  
&ensp;&ensp;&ensp;&ensp;Segundo documentação oficial é possível criar elementos visuais personalizados para dispositivos padrão habilitados para Alexa, como Echo Show, Echo Spot, Fire TV e dispositivos Fire Tablet selecionados. Dispositivos de terceiros criados usando o Alexa Smart Screen e o SDK do dispositivo de TV também suportam a estrutura de design APL.  
&ensp;&ensp;&ensp;&ensp;O APL suporta comandos de voz para que os usuários possam solicitar um item na tela em vez de depender apenas de interações de toque.  
&ensp;&ensp;&ensp;&ensp;A APL cria um arquivo JSON enviado de sua skill que contém as especificações para seus elementos de design. O dispositivo avalia o que pode suportar e, em seguida, importa imagens e outros dados conforme necessário para renderizar a experiência correta.  
&ensp;&ensp;&ensp;&ensp;Exemplos de documentos APL podem ser encontrados no site:  
<p align="center"> https://apl.ninja </p>
&ensp;&ensp;&ensp;&ensp;Neste projeto foram utilizados os JSON disponibilizados nos links:  
  <p align="center"> https://apl.ninja/xeladotbe/you-win-0mfh e https://apl.ninja/xeladotbe/jeff-blankenburgs-twitch-stream-intro-k8a9 </p>
&ensp;&ensp;&ensp;&ensp;Link para a documentação oficial da APL:  
  <p align="center"> https://developer.amazon.com/en-US/docs/alexa/alexa-design/apl.html#:~:text=Alexa%20has%20a%20visual%20design,and%20engaging%20to%20the%20customer. </p>

## DynamoDB

&ensp;&ensp;&ensp;&enspA skill está utilizando um servidor hospedado na Lambda com isso alguns recursos da Alexa não estão disponíveis, desta forma se faz necessário criar permissões de uso para a skill ter acesso a outras funcionalidades uma delas seria a persistência dos dados, neste caso será utilizado outro serviço da Amazon chamado de DynamoDB.
&ensp;&ensp;&ensp;&enspEm suma, o que será feito é: criar um usuário com permissão de acesso ao DynamoDB, criar uma tabela, adicionar a permissão na função.

<p align="center">Figura 42 - Página inicial DynamoDB</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem42.png" alt="Figura 42 - Página inicial DynamoDB"></p>

&ensp;&ensp;&ensp;&ensp;Criar a tabela informando o nome desejado.

<p align="center">Figura 43 - Criação de tabela</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem43.png" alt="Figura 43 - Criação de tabela"></p>

&ensp;&ensp;&ensp;&ensp;No Lambda clicar no hiperlink da permissão para abrir a página de edição.

<p align="center">Figura 44 - Permissão</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem44.png" alt="Figura 44 - Permissão"></p>

&ensp;&ensp;&ensp;&ensp;Deve ser adicionado a permissão para acesso ao DynamoDB, desta forma clicar na opção “Add permissions -> Attach policies”.

<p align="center">Figura 45 - Página de permissões</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem45.png" alt="Figura 45 - Página de permissões"></p>

&ensp;&ensp;&ensp;&ensp;Procurar pela política “AmazonDynamoDBFullAccess” e clicar na opção “Attach policies”.

<p align="center">Figura 46 - Adicionando nova permissão</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem46.png" alt="Figura 46 - Adicionando nova permissão"></p>

&ensp;&ensp;&ensp;&ensp;Voltar a página do Lambda, e verificar se será apresentado na lista de permissões o DynamoDB.

<p align="center">Figura 47 - Lista de permissões</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem47.png" alt="Figura 47 - Lista de permissões"></p>

## Exemplos de utilização DynamoDB

&ensp;&ensp;&ensp;&ensp;Neste item será mostrado alguns exemplos de uso utilizando o DynamoDB, os exemplos são de inserir, atualizar, consultar e excluir dados. Esses exemplos estão disponibilizados no github e são baseados na documentação oficial que pode ser encontrado no site abaixo:  

<p align="center"> https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/dynamodb.html#dynamodb </p>

### Inserir

&ensp;&ensp;&ensp;&ensp;Exemplo para inserir um novo registro na tabela no Quadro 2.

~~~Python
--INSERT

#importing packages
import json
import boto3
#function definition
def lambda_handler(event,context):
    dynamodb = boto3.resource('dynamodb')
    #table name
    table = dynamodb.Table('users_score')
    #inserting values into table
    response = table.put_item(
       Item={
            'username': 'Teste',
            'score':3000
        }
    )
    return response
~~~

### Atualizar

&ensp;&ensp;&ensp;&ensp;Exemplo para atualizar um dado na tabela no Quadro 3.

~~~Python
--UPDATE

#importing packages
import json
import boto3
#function definition
def lambda_handler(event,context):
    dynamodb = boto3.resource('dynamodb')
    #table name
    table = dynamodb.Table('users_score')
    #inserting values into table
    response = table.update_item(
        Key={"username":"Teste"},
        UpdateExpression="SET score= :s",
        ExpressionAttributeValues={':s':100},
        #Só precisa desse se for utilizar o response
        ReturnValues="UPDATED_NEW"
    )
    return response['Attributes']
~~~

### Consultar

&ensp;&ensp;&ensp;&ensp;Exemplo para retorar um dado na tabela no Quadro 4.

~~~Python
--GET

#importing packages
import json
import boto3
#function definition
def lambda_handler(event,context):
    dynamodb = boto3.resource('dynamodb')
    #table name
    table = dynamodb.Table('users_score')
    users = table.scan()['Items']
    users_test = ""
    for users_l in users:
        if "bhagiii" == users_l['username']:
            users_test += users_l['username'] + " - > " + users_l['score']
    print(users_test)
    return None
~~~

###

&ensp;&ensp;&ensp;&ensp;Exemplo para excluir um dado na tabela no Quadro 5.

~~~Python
--DELETE

#importing packages
import json
import boto3
#function definition
def lambda_handler(event,context):
    dynamodb = boto3.resource('dynamodb')
    #table name
    table = dynamodb.Table('users_score')
    #inserting values into table
    response = table.delete_item(
        Key={"username":"Teste"},
        ReturnValues="ALL_OLD"
    )
    return response['Attributes']
~~~

## Criação de uma “coisa “

&ensp;&ensp;&ensp;&ensp;Conforme AMEBA, na arquitetura, ESP32 pertence ao bloco "Coisas" superior esquerdo. Um canal seguro TLS será estabelecido entre "Things" e o MQTT Message Broker. Em seguida, "coisa" e "Message Broker" se comunicam usando o Protocolo MQTT por meio desse canal seguro. Atrás do "Message Broker", as "Thing Shadows" mantêm mensagens temporariamente quando o ESP32 está offline, e envia a mensagem de controle para o ESP32 na próxima vez que ele estiver conectado. O "Mecanismo de Regras" permite que você coloque restrições ao comportamento das Coisas ou conecte Coisas a outros serviços da Amazon.  
&ensp;&ensp;&ensp;&ensp;Na Figura 48 se encontra uma exemplificação desta arquitetura.

<p align="center">Figura 48 - Estrutura do AWS IoT</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem48.png" alt="Figura 48 - Estrutura do AWS IoT"></p>

&ensp;&ensp;&ensp;&ensp;A partir da Figura 49 está descrito o passo-a-passo de como criar uma “coisa”, ou um novo dispositivo, à função em AWS IoT, terminando na Figura X.  
&ensp;&ensp;&ensp;&ensp;Em suma, os passos são os seguintes: Registro no Console do AWS IoT; Criação/registro de uma "Coisa" no console; Criação de um certificado TLS X.509; Criação de uma política para acessar os tópicos da AWS IoT; Anexando a Política ao Certificado; testes via MQTT.  

*******
+ Passo 1: Acessar a página do IoT Core, acessar o item “Things” e utilizar a opção “Create things”.  
<p align="center">Figura 49 - Página inicial AWS IoT</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem49.png" alt="Figura 49 - Página inicial AWS IoT"></p>

+ Passo 2: Utilizar a opção “Create single thing” e clicar em “Next”.  
<p align="center">Figura 50 - Criação da “coisa”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem50.png" alt="Figura 50 - Criação da “coisa”"></p>

+ Passo 3: Inserir um novo nome e demais valores são opcionais.  
<p align="center">Figura 51 - Propriedades da “coisa”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem51.png" alt="Figura 51 - Propriedades da “coisa”"></p>

+ Passo 4: Utilizar a opção “Auto-generate a new certificate (recommended)” e clicar em “Next”.  
<p align="center">Figura 52 - Escolha do certificado</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem52.png" alt="Figura 52 - Escolha do certificado"></p>

+ Passo 5: Criar uma política. 
<p align="center">Figura 53 - Política da “coisa”</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem53.png" alt="Figura 53 - Política da “coisa”"></p>

+ Passo 6: Inserir o código do Quadro 1 e alterar com os dados de região, ID da conta e alterar o nome ESP32_SP para o nome da “coisa”.  
<p align="center">Figura 54 - Criação de política</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem54.png" alt="Figura 54 - Criação de política"></p>

~~~JSON
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:Connect",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:client/MyNewESP32"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Subscribe",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topicfilter/esp32/sub"
    },
	{
      "Effect": "Allow",
      "Action": "iot:Receive",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/sub"
    },
    {
      "Effect": "Allow",
      "Action": "iot:Publish",
      "Resource": "arn:aws:iot:REGION:ACCOUNT_ID:topic/esp32/pub"
    }
  ]
}
~~~

+ Passo 7: Vincular a política e criar a “coisa”  
<p align="center">Figura 55 - Vínculo da política</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem55.png" alt="Figura 55 - Vínculo da política"></p>

+ Passo 8: Download de todos os certificados, esses certificados são os citados dentro do item “Secrets.h” para serem inseridos no código do Arduino.  
<p align="center">Figura 56 - Download dos certificados</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem56.png" alt="Figura 56 - Download dos certificados"></p>

*******

## Testes MQTT

&ensp;&ensp;&ensp;&ensp;Nas subseções será apresentado como realizar testes via MQTT dentro do serviço IoT Core.

### Variáveis

&ensp;&ensp;&ensp;&ensp;Entrar no arquivo “Secrets.h” do Arduino e atualizar as seguintes informações:  
+	THINGNAME: Nome da “coisa” criada.  
+	AWS_IOT_ENDPOINT[] : Este valor pode ser encontrado dentro do item “Settings” da página do IoT Core, conforme Figura 57.  
+	AWS_CERT_CA[],AWS_CERT_CRT[] e AWS_CERT_PRIVATE[]: Conteúdo se encontra dentro dos arquivos baixados ao criar a “coisa”.  
Após isso compilar e utilizar a opção “Upload” para carregar o código dentro da memória do ESP32.  

<p align="center">Figura 57 – Endpoint IoT Core</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem57.png" alt="Figura 57 – Endpoint IoT Core"></p>

### Ferramenta de testes MQTT Lambda

&ensp;&ensp;&ensp;&ensp;Após acessar a página do IoT Core, acessar o item “MQTT test client”. E configurar os tópicos conforme Figura 58 e Figura 59.

<p align="center">Figura 58 - Configuração do tópico MQTT</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem58.png" alt="Figura 58 - Configuração do tópico MQTT"></p>

<p align="center">Figura 59 - Configuração do tópico MQTT</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem59.png" alt="Figura 59 - Configuração do tópico MQTT"></p>

&ensp;&ensp;&ensp;&ensp;Para realizar o teste basta inserir uma nova mensagem e utilizar a opção “Publish”, dentro do Arduino é possível ver os logs pelo canal escolhido e posteriormente a isto será enviado uma mensagem de sucesso novamente para a Amazon. Segue figuras com esse processo.

<p align="center">Figura 60 - Envio da mensagem ao Arduino</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem60.png" alt="Figura 60 - Envio da mensagem ao Arduino"></p>

<p align="center">Figura 61 - Log de recebimento no Arduino</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem61.png" alt="Figura 61 - Log de recebimento no Arduino"></p>

<p align="center">Figura 62 - Log de sucesso IoT Core</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem62.png" alt="Figura 62 - Log de sucesso IoT Core"></p>

## Logs CloudWatch

&ensp;&ensp;&ensp;&ensp;Por padrão ao se criar uma skill e essa esteja na AWS Lambda ou dentro do servidor criado automaticamente pela Amazon, é vinculado o serviço CloudWatch que gera os logs da sessão da skill sejam erro ou de execução, ou até mesmo logs manuais inseridos dentro do código fonte.  

&ensp;&ensp;&ensp;&ensp;Para logs manuais, em Python, é necessário importar a biblioteca logger, criar uma instância e o nível do log a ser gerado. Segue no Quadro 7.  
~~~Python
#Biblioteca
import logging

#Criar uma nova instância
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

#Exemplo de uso
logger.info("Locale is {} e {}".format(len(wordList), len(list_c)))
~~~  

&ensp;&ensp;&ensp;&ensp;Desta forma, é possível acessar esse serviço de duas formas, conforme será mostrado nas subseções a seguir.  

### Servidor hospedado na Alexa  
Caso seja servidor hospedado na própria Alexa, ir na aba “Code” e clicar no ícone “CloudWatch Logs”.  

<p align="center">Figura 63 - Icone CloudWatch Logs</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem63.png" alt="Figura 63 - Icone CloudWatch Logs" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Será aberta a página da CloudWatch, nela terá que selecionar o item “Log groups”, e selecionar algum registro. Esse registro constará com o que foi feito na seção, contendo o número da sessão, tempo inicial e final sendo possível visualizar em tempo real.  
&ensp;&ensp;&ensp;&ensp;Obs.: Na parte superior a direita é possível ver um usuário com o prefixo “VoiceHubSSORole” esse usuário é gerado pela própria skill e não temos controle dele ou das permissões dele.  

<p align="center">Figura 64 - CloudWatch item LogGroups</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem64.png" alt="Figura 64 - CloudWatch item LogGroups" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Por fim, um exemplo de como o log será gerado.  

<p align="center">Figura 65 - Exemplo de log</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem65.png" alt="Figura 65 - Exemplo de log" align="center"></p>  

### Servidor hospedado na AWS Lambda

&ensp;&ensp;&ensp;&ensp;Para verificar os logs gerados em uma skill hospedado na AWS Lambda, é necessário apenas procurar o serviço na barra de procurar e novamente entrar no item “Log Groups” que será apresentado os logs gerados na sessão ao realizar um teste pelo console do desenvolvedor ou pela própria Alexa.  

<p align="center">Figura 66 - Exemplo de log lambda</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem66.png" alt="Figura 66 - Exemplo de log lambda" align="center"></p>  

# Informações adicionais

&ensp;&ensp;&ensp;&ensp;Nos tópicos a seguir, seriam informações interessantes de ter em mente ao desenvolver uma skill.

## Internacionalização

&ensp;&ensp;&ensp;&ensp;Na computação, a internacionalização é o processo de projetar software para que possa ser adaptado a diferentes idiomas e regiões sem a necessidade de reescrever ou alterar o código fonte.  
&ensp;&ensp;&ensp;&ensp;A internacionalização permite que você crie uma skill para a Alexa que suporte vários idiomas, mas todos os idiomas irão utilizar o mesmo servidor e consequentemente o mesmo código fonte. Quando o usuário faz uma solicitação, parte do JSON de entrada contém as informações de localidade do dispositivo que está usando. Sua skill pode usar essas informações para falar no idioma apropriado para a solicitação do usuário.  

&ensp;&ensp;&ensp;&ensp;Existem duas formas possíveis para se realizar isso, a primeira seria identificar o valor do JSON e toda vez executar um "IF" para conseguir utilizar o idioma de entrada e retornasse a resposta do idioma adequado. Para skills extensas não seria ideal pois toda alteração poderia gerar uma grande manutenção, um grande esforço e mesmo assim poderia ocorrer erros.  

&ensp;&ensp;&ensp;&ensp;Outra consideração ao pensar em uma estratégia de internacionalização para grandes skills é terceirizar a tradução para outra empresa. Obviamente, você não deseja enviar a eles seu código fonte para que eles possam atualizá-lo; portanto, as sequências de texto geralmente são colocadas em um arquivo separado que você pode enviar à empresa de tradução para edição.  

### Modelo de interação - Parte 1  

&ensp;&ensp;&ensp;&ensp;O console do desenvolvedor da Alexa já consta com uma opção para criar um modelo de forma simples, para isso você deve abrir a lista de idiomas e clicar na opção "Language Settings", conforme figura 67.  

<p align="center">Figura 67 - Lista de idiomas</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem67.png" alt="Figura 67 - Lista de idiomas" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Será aberta uma nova página, nesta página deve selecionar "Add new language", escolher o idioma desejado e clicar em "Save". Após salvar, será apresentado na lista o novo idioma adicionado. Na figura 68 é apresentado botão para adicionar um novo idioma e salvar.  

<p align="center">Figura 68 - Configurações de idiomas</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem68.png" alt="Figura 68 - Configurações de idiomas" align="center"></p>

&ensp;&ensp;&ensp;&ensp;Para ficar simples e não ser necessário adicionar novamente todas as intenções, slots etc. Você pode entrar no idioma onde consta já todas as configurações, ir à opção "Intents -> JSON Editor", copiar o conteúdo do json e traduzir manualmente as frases para então entrar no novo idioma e colar esse JSON traduzido e salvar esse novo modelos fazendo todos os registros de forma automática. Na figura 69 é apresentado onde se pode encontrar o JSON.  

<p align="center">Figura 69 - Item JSON Editor</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem69.png" alt="Figura 69 - Item JSON Editor" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Após esses passos é recomendado alterar o nome de invocação, caso não seja um nome próprio, buildar o novo modelo e realizar novos testes para validar o modelo de interação.  

### Modelo de interação – Parte 2  

&ensp;&ensp;&ensp;&ensp;Na primeira parte cuidamos dos dados de entrada, agora será feito os dados de saída para isso no código fonte serão necessárias algumas alterações.  

&ensp;&ensp;&ensp;&ensp;Teremos que utilizar o conceito de interceptor, existem duas variantes de interceptores o de resposta que será executado após finalizar algum método e o de requisição que será executado antes de entrar no método. No caso do idioma é necessário utilizar o de requisição.  

&ensp;&ensp;&ensp;&ensp;Para isso, devemos importar o componente "AbstractRequestInterceptor". Abaixo se encontra a linha padrão para importar os interceptores mais utilizados.  

~~~Python
from ask_sdk_core.dispatch_components import (AbstractRequestHandler, AbstractExceptionHandler,AbstractResponseInterceptor, AbstractRequestInterceptor)
~~~  

&ensp;&ensp;&ensp;&ensp;Em seguida, para ficar legível e de fácil controle o ideal é criar uma pasta com documentos JSON que irá tratar das strings de cada idioma. Você pode criar qualquer estrutura de JSON, neste caso deixamos simples apenas com um id para cada valor. Conforme figura 70.  

<p align="center">Figura 70 - Idiomas JSON</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem70.png" alt="Figura 70 - Idiomas JSON" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Agora devemos criar um método passando como parâmetro o tipo do interceptor, nesse método basicamente será identificar no JSON de entrada qual é a localidade recebida e abrir o arquivo JSON e passar os dados para a sessão.  

~~~Python
class LocalizationInterceptor(AbstractRequestInterceptor):

    def process(self, handler_input):
        locale = handler_input.request_envelope.request.locale
        logger.info("Locale is {}".format(locale))
        try:
            with open("languages/"+str(locale)+".json") as language_data:
                language_prompts = json.load(language_data)
        except:
            with open("languages/"+ str(locale[:2]) +".json") as language_data:
                language_prompts = json.load(language_data)
        handler_input.attributes_manager.request_attributes["_"] = language_prompts
~~~  

&ensp;&ensp;&ensp;&ensp;Por fim, nos demais métodos seria apenas buscar na sessão o valor do id necessário. No quadro a seguir é possível verificar um exemplo simples:  
~~~Python
class InitializeGameIntentHandler(AbstractRequestHandler):
    """Handler for Initialize Game Intent."""
    def can_handle(self, handler_input):
        # type: (HandlerInput) -> bool
        return ask_utils.is_intent_name("InitializeGameIntent")(handler_input)

    def handle(self, handler_input):
        # type: (HandlerInput) -> Response
        language_prompts = handler_input.attributes_manager.request_attributes["_"] #Buscar o valor da sessão
speak_output = language_prompts["PHRASE_REPEAT_SEQ"] #Buscar pela tag qual é a frase

return (
handler_input.response_builder
.speak(speak_output)
.set_should_end_session(False)
.response
)
~~~  

&ensp;&ensp;&ensp;&ensp;Ressalto que é necessário colocar no construtor este método criado.  

~~~Python
sb.add_global_request_interceptor(LocalizationInterceptor())
~~~  

## Dados persistentes em servidor hospedado Alexa  

&ensp;&ensp;&ensp;&ensp;Diferente do que foi feito nas sessões para se utilizar o DynamoDB na skill, caso o servidor da skill seja hospedado na própria Alexa devemos utilizar os próprios componentes da ask-sdk disponibilizados pela Amazon, visto que o usuário dos serviços Lambdas é gerado de forma aleatória e não temos controle das regras de acesso dele e por isso não conseguimos realizar os mesmos passos descritos no item “DynamoDB”. Por isso o ideal é utilizar o serviço S3 bucket por ser simples a sua utilização, pelo código ser curto e os dados a serem salvados pequenos a utilização dele é o suficiente.  

### Alterações no código  

&ensp;&ensp;&ensp;&ensp;No arquivo de “requirements.txt” da skill, deve-se inserir a seguinte linha:  

~~~Python
ask-sdk-dynamodb-persistence-adapter==1.15.0
~~~  

&ensp;&ensp;&ensp;&ensp;Na figura 71 é possível validar como irá ficar após a inserção desta linha.

<p align="center">Figura 71 - Arquivo 'requirements.txt'</p>  

<p align="center"><img src="https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem70.png" alt="Figura 71 - Arquivo 'requirements.txt'" align="center"></p>  

&ensp;&ensp;&ensp;&ensp;Abra o arquivo ‘lambda_function.py’, e insira as linhas que não existir:  

~~~Python
import os 
import ask_sdk_core.utils as ask_utils
from ask_sdk_s3.adapter import S3Adapter 
s3_adapter = S3Adapter(bucket_name=os.environ["S3_PERSISTENCE_BUCKET"])
~~~  

&ensp;&ensp;&ensp;&ensp; Será necessário substituir a linha:

~~~Python
from ask_sdk_core.skill_builder import SkillBuilder
~~~  

&ensp;&ensp;&ensp;&ensp;Pela linha:  

~~~Python
from ask_sdk_core.skill_builder import CustomSkillBuilder
~~~  

&ensp;&ensp;&ensp;&ensp;Exemplo de como poderá ficar após essas alterações:

~~~Python
import logging
import ask_sdk_core.utils as ask_utils
import os
from ask_sdk_s3.adapter import S3Adapter
s3_adapter = S3Adapter(bucket_name=os.environ["S3_PERSISTENCE_BUCKET"])

from ask_sdk_core.skill_builder import CustomSkillBuilder
from ask_sdk_core.dispatch_components import AbstractRequestHandler
from ask_sdk_core.dispatch_components import AbstractExceptionHandler
from ask_sdk_core.handler_input import HandlerInput
from ask_sdk_model import Response

logger = logging.getLogger(name)
logger.setLevel(logging.INFO)
~~~  

&ensp;&ensp;&ensp;&ensp;Na parte inferior do arquivo também será necessário alterar outra linha de código, no caso deverá procurar essa linha:  

~~~Python
sb = SkillBuilder()
~~~  

&ensp;&ensp;&ensp;&ensp; E substitui-la por esta linha:

~~~Python
sb = CustomSkillBuilder(persistence_adapter=s3_adapter)
~~~  

&ensp;&ensp;&ensp;&ensp;Após esses passos utilizar os botões “Save” e “Deploy” para testar se a skill irá abrir sem erros.  

### Salvando dados

&ensp;&ensp;&ensp;&ensp;Para salvar os dados é apenas necessário utilizar duas linhas:

~~~Python
handler_input.attributes_manager.persistent_attributes = banco
handler_input.attributes_manager.save_persistent_attributes()
~~~  

&ensp;&ensp;&ensp;&ensp;Sendo o que o valor “banco” são os dados que precisa guardar.  

### Recuperando os dados

&ensp;&ensp;&ensp;&ensp;Para pegar os dados salvos é preciso apenas utilizar a linha:  

~~~Python
banco = handler_input.attributes_manager.persistent_attributes
~~~  

&ensp;&ensp;&ensp;&ensp;Sendo que a variável “banco” irá receber todas as informações que já estão guardadas, com isso é possível manipular os dados e posteriormente utilizar o comando de salvar novamente.  

# Fóruns

&ensp;&ensp;&ensp;&ensp;A Amazon conta com diversos fóruns específicos para retirar dúvidas de como criar skills ou de recursos específicos dos serviços, sendo possível conversar com os desenvolvedores da Alexa. Será lista os principais fóruns e o slack da comunidade.  

&ensp;&ensp;&ensp;&ensp;Fórum para postar ideias do que pode ser desenvolvido para a Alexa:  

<p align="center">https://alexa.design/uservoice</p>

&ensp;&ensp;&ensp;&ensp;Fórum voltado para serviços da AWS:  

<p align="center">https://repost.aws</p>

&ensp;&ensp;&ensp;&ensp;Fórum focado para retirar dúvidas de funcionamento ou como criar algum recurso da skill:  

<p align="center">https://amazon.developer.forums.answerhub.com/index.html</p>

&ensp;&ensp;&ensp;&ensp;Slack da comunidade que conta com os programadores da Alexa além de outros programadores:  

<p align="center">https://alexacommunity.slack.com</p>

# Referências

https://developer.amazon.com/en-US/docs/alexa/custom-skills/choose-the-invocation-name-for-a-custom-skill.html  
https://apl.ninja  
https://developer.amazon.com/en-US/docs/alexa/alexa-presentation-language/apl-commands.html  
https://dabblelab.com/tutorials  
https://apl.ninja/xeladotbe/you-win-0mfh  
https://apl.ninja/document/xeladotbe/jeff-blankenburgs-twitch-stream-intro-k8a9  
https://aws.amazon.com/iot-core/  
https://aws.amazon.com/cloudwatch/features/#:~:text=CloudWatch%20enables%20you%20to%20monitor,building%20applications%20and%20business%20value.  
https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Introduction.html  
https://developer.amazon.com/en-US/docs/alexa/custom-skills/steps-to-build-a-custom-skill.html  
https://www.amebaiot.com/en/ameba-arduino-amazon-alexa/  
https://aws.amazon.com/pt/lambda/?nc2=h_m1  
https://developer.amazon.com/en-US/docs/alexa/ask-overviews/what-is-the-alexa-skills-kit.html  
https://github.com/alexa-samples/skill-sample-python-first-skill/tree/master/module-4  
https://medium.com/captech-corner/automating-a-tower-fan-with-alexa-skills-aws-lambda-aws-iot-and-an-esp32-ad0d4ba1da22  
https://developer.amazon.com/en-US/docs/alexa/custom-skills/choose-the-invocation-name-for-a-custom-skill.html  
https://developer.amazon.com/en-US/docs/alexa/custom-skills/create-intents-utterances-and-slots.html  
https://www.youtube.com/watch?v=8zhv6GDSDE8  
https://www.amebaiot.com.cn/en/amebad-arduino-aws-shadow/  
