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

![Figura 1 - Passo 1 -> Configurando Arduino para o ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem1.png)

![Figura 2 - Passo 2 -> Configurando Arduino para o ESP32 (continuação)](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem2.png)

![Figura 3 - Passo 3 -> Instalando pacote ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem3.png)

![Figura 4 - Passo 4 -> Selecionando placa ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem4.png)

![Figura 5 - Passo 5 -> Instalando biblioteca MQTT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem5.png)

![Figura 6 - Passo 6 -> Instalando biblioteca ArduinoJson](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem6.png)

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

![Figura 7 - Diagrama de Atividades](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem7.png)

&ensp;&ensp;&ensp;&ensp;Conforme documentação oficial da Amazon o fluxo de trabalho a seguir demonstra como uma habilidade do Alexa funciona. Neste exemplo, o usuário invoca uma habilidade simples do Alexa chamada "Hello World".

*******
+ Passo 1 - Para iniciar a habilidade, o usuário diz: "Alexa, abra o Hello World". Uma declaração ou solicitação completa ao Alexa pelo usuário é chamada de enunciado.  
+ Passo 2 - O dispositivo habilitado para Alexa envia o enunciado para o serviço Alexa na nuvem. O serviço Alexa processa o enunciado por meio do reconhecimento automático de fala para converter o enunciado em texto e, em seguida, por meio do entendimento de linguagem natural para reconhecer a intenção do texto.  
+ Passo 3 - O Alexa envia uma solicitação JavaScript Object Notation (JSON) para manipular a intenção para o serviço de computação de recursos que hospeda sua habilidade (geralmente uma função do AWS Lambda). O serviço de computação de recursos atua como back-end e executa seu código de habilidade para lidar com a intenção. Nesse caso, o servidor retorna: "Bem-vindo à habilidade Hello World".  
*******

&ensp;&ensp;&ensp;&ensp;O diagrama na Figura 8 demonstra o que acontece quando um usuário interage com uma habilidade do Alexa. Ele pressupõe que você esteja usando o serviço de computação sem servidor do AWS Lambda para hospedar seu código de habilidade.

![Figura 8 - Diagrama da skill Hello World](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem8.png)

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

![Figura 9- Página inicial do site da Amazon Alexa](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem9.png)

![Figura 10 - Página de login para o console do desenvolvedor](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem10.png)

![Figura 11 - Página para gerenciamos das skills](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem11.png)

![Figura 12 - Selecionar o idioma e modelo Custom](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem12.png)

![Figura 13 – Selecionar a linguagem de programação](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem13.png)

![Figura 14 - Selecionar “Start from Scratch”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem14.png)

![Figura 15 - Inserindo um nome de invocação da skill](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem15.png)

![Figura 16 - Adicionando novo intent](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem16.png)

![Figura 17 - Criando um intent personalizável](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem17.png)

![Figura 18 - Criando um intent e slot personalizável](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem18.png)

![Figura 19 - Adicionando valores no slot personalizado](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem19.png)

&ensp;&ensp;&ensp;&ensp;No item apresentado na Figura 20 você deverá cadastrar qual declaração para ativar essa intenção.  

![Figura 20 - Inserindo uma declaração](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem20.png)

&ensp;&ensp;&ensp;&ensp;Neste exemplo essa declaração serve apenas para iniciar o jogo escolhendo o modo e a dificuldade com dois slots personalizados.  
&ensp;&ensp;&ensp;&ensp;Após realizado todas as alterações deve-se salvar e compilar o modelo.

![Figura 21 - Salvar e construir o modelo](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem21.png)

![Figura 22 - Mensagem de sucesso após construção do modelo](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem22.png)

&ensp;&ensp;&ensp;&ensp;Também está disponibilizado no github o JSON que pode ser importado para realizar todos esses cadastrados de forma automática.

![Figura 23 - Importando JSON](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem23.png)

&ensp;&ensp;&ensp;&ensp;Após isso, toda vez que é executada uma intenção será gerado um Callback para o servidor Lambda executando o método vinculado a esta intenção.

## Criando servidor AWS Lambda

&ensp;&ensp;&ensp;&ensp;Nesta etapa, deverá ser criada uma função no AWS Lambda. O site para utilizar esses serviços seria o:  
<p align="center"> https://aws.amazon.com   </p>
&ensp;&ensp;&ensp;&ensp;Os passos são descritos a partir de Figura 24 até a Figura 41.

![Figura 24 - Página inicial da AWS Amazon](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem24.png)

![Figura 25 - Página de login da AWS Amazon](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem25.png)

![Figura 26 - Selecionar o serviço Lambda](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem26.png)

![Figura 27 - Criar a função](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem27.png)

![Figura 28 - Escolher um nome e linguagem de programação](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem28.png)

![Figura 29 - Adicionar um gatilho](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem29.png)

&ensp;&ensp;&ensp;&ensp;Nesta etapa será necessário o ID gerado na Amazon Alexa. A Figura 30 aponta o lugar que se obtém na Amazon Alexa. Na Figura 31 mostra onde deve ser inserido o ID da Amazon Alexa. Após a inserção do ID, clicar em salvar.

![Figura 30 - Utilizar a opção “Copy to clipboard”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem30.png)

![Figura 31 - Escolher “Alexa IoT” e colar o código copiado anteriormente no “Skill ID”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem31.png)

&ensp;&ensp;&ensp;&ensp;Será necessário realizar o processo inverso, copiar o código ARN da função Lambda e colar no endpoint da Skill para ela conseguir acessar o backend.  A Figura 32 aponta onde se obtém na função Lambda. Na Figura 33mostra onde deve ser inserido o ID na Skill. Após a inserção do ID, clicar em salvar.

![Figura 32 - Copiar o “Function ARN”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem32.png)

![Figura 33 - Colar o “Function ARN”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem33.png)

&ensp;&ensp;&ensp;&ensp;Deve ser realizado a configuração de permissão para a função criada, para isso deve-se seguir o passo-a-passo da Figura 34 até Figura 37.

![Figura 34 - Clicar no hiperlink](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem34.png)

![Figura 35 - Expandir e editar a política](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem35.png)

&ensp;&ensp;&ensp;&ensp;Se as permissões corretas não forem concedidas, dificultará o acesso a determinados tópicos ou a execução de determinadas ações. Neste caso, define-se a política no nível mais permissivo. As configurações podem ser alteradas para restringir determinadas ações e tópicos conforme necessários, por isto muita atenção às permissões.  
&ensp;&ensp;&ensp;&ensp;Na tela da Figura 36, incluir o código de permissão do AWS IoT seguindo o exemplo do Quadro 1.

![Figura 36 - Colar e salvar](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem36.png)

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

![Figura 37 - Painel atualizado](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem37.png)

&ensp;&ensp;&ensp;&ensp;O Lambda não tem acesso as bibliotecas/recursos do pacote ask_sdk, esse pacote seria o padrão para utilizar os recursos da Alexa com isso é necessário adicionar uma camada para o Lamba ter de onde buscar esse recurso, para isso na parte inferior do site clicar em “Add a Layer”.

![Figura 38 - Adicionando uma camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem38.png)

&ensp;&ensp;&ensp;&ensp;Após isso, devemos especificar qual camada iremos adicionar no caso já disponibilizaram o pacote ask_sdk na url “arn:aws:lambda:us-east-1:173334852312:layer:ask-sdk-for-python-36:1”, desta forma deve-se utilizar a opção “Specify na ARN” e colar no campo este valor.

![Figura 39 - Especificando a camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem39.png)

![Figura 40 - Como ficará o item camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem40.png)

&ensp;&ensp;&ensp;&ensp;No item “Code” é onde deverá ser feito a lógica após receber o JSON da Skill, esse código se encontra no github. Após subir o servidor ele já deverá se comunicar com a skill e vice-versa.

![Figura 41 - Código do backend](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem41.png)

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


![Figura 42 - Página inicial DynamoDB](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem42.png)
![Figura 43 - Criação de tabela](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem43.png)
![Figura 44 - Permissão](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem44.png)
![Figura 45 - Página de permissões](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem45.png)
![Figura 46 - Adicionando nova permissão](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem46.png)
![Figura 47 - Lista de permissões](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem47.png)
![Figura 48 - Estrutura do AWS IoT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem48.png)
![Figura 49 - Página inicial AWS IoT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem49.png)
![Figura 50 - Criação da “coisa”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem50.png)
![Figura 51 - Propriedades da “coisa”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem51.png)
![Figura 52 - Escolha do certificado](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem52.png)
![Figura 53 - Política da “coisa”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem53.png)
![Figura 54 - Criação de política](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem54.png)
![Figura 55 - Vínculo da política](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem55.png)
![Figura 56 - Download dos certificados](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem56.png)
![Figura 57 – Endpoint IoT Core](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem57.png)
![Figura 58 - Configuração do tópico MQTT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem58.png)
![Figura 59 - Configuração do tópico MQTT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem59.png)
![Figura 60 - Envio da mensagem ao Arduino](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem60.png)
![Figura 61 - Log de recebimento no Arduino](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem61.png)
![Figura 62 - Log de sucesso IoT Core](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem62.png)
