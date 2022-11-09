# Alexa-Tutorial-PT-BR


<p align="center"><b>Tutorial de desenvolvimento de uma skill Alexa </br> 
Rafael dos Santos Rodrigues, Dalton Solano dos Reis</b> – Orientador </br>  
Curso de Bacharel em Sistemas de Informação </br>
Departamento de Sistemas e Computação </br>
Universidade Regional de Blumenau (FURB) – Blumenau, SC – Brazil</p>

<p align="center"><img src="https://user-images.githubusercontent.com/42719744/200699251-b6ca4f74-c1be-4cc1-a761-bc7b0d0f56b6.png" alt="Logo Alexa" align="center"></p>

# Objetivo

Este documento tem como objetivo apresentar alguns conceitos ao desenvolver uma skill para Alexa, como criar uma skill para a Alexa, como criar e hospedar um servidor na AWS Lambda, criar uma “coisa” e vincular ao ESP32 e utilizar alguns serviços auxiliares sendo eles: DynamoDB e CloudWatch.

# Configurando Arduino IDE:

O Arduino IDE é um ambiente de desenvolvimento de código aberto para programação de microcontroladores. Ele está em constante desenvolvimento e pode suportar um número crescente de plataformas, incluindo a maioria dos módulos baseados em ESP32. Ele deve ser instalado junto com as definições da placa ESP32, biblioteca MQTT e biblioteca ArduinoJson.

O Arduino IDE pode ser baixado no site do Arduino: 

<p align="center">https://www.arduino.cc/en/Main/Software</p>

Após a instalação, abra o Arduino IDE e siga a sequência e as figuras abaixo: 

*******
+ Passo 1 - Para configurar o ESP32 no Arduino IDE, entre em "File" -> "Preferences";  
+ Passo 2 - E cole o seguinte URL no campo "Additional Boards Manager URLs":  
<p align="center">https://dl.espressif.com/dl/package_esp32_index.json</p>  
Obs.: Caso altere o idioma é necessário reiniciar o Arduino IDE.  

+ Passo 3 - Entre em "Tools" -> "Board" -> "Boards Manager" procure por “esp32” e instale a última versão da “esp32 by Espressif Systems” da lista, nesse processo o Arduino IDE irá baixar os arquivos necessários para o ESP32.  
  Ao abrir o "Boards Manager" ele pode demorar cerca de 20 segundos para atualizar todos os arquivos de hardware (se a rede estiver em más condições, pode levar mais tempo).  
+ Passo 4 - Abrir novamente o “Tools” e validar se o item “Board” se encontra com o modelo do seu componente, caso não alterar e selecionar o correto.  
  É possível conferir os modelos de placas compatíveis com o Arduino IDE pelo documento disponibilizado pela “Espressif Systems” no site:  
  <p align="center">https://github.com/espressif/arduino-esp32/blob/master/boards.txt</p>  
+ Passo 5 - Abra o item “Sketch” -> “Include Library” -> “Manage Libraries” procure por “MQTT” e instale a última versão do criado “Joel Gaehwiler”.  
+ Passo 6 - Abra o item “Sketch” -> “Include Library” -> “Manage Libraries” procure por “ArduinoJson” e instale a última versão do criado “Benoit Blanchon”.  
*******

# Código Arduino IDE

O código a seguir se encontra no item “Código Arduino” que foi disponibilizado no github pelo link:  
<p align="center">https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/</p>  
Será quebrado o código explicando para que serve cada parte e o que é necessário ser alterado para executar com os seus dados.

## Secrets.h

No arquivo “secrets.h” ele serve como auxiliar para armazenar os certificados, chaves e dados do Wi-Fi que será utilizado pelo ESP32.  

Na variável “THINGNAME” seria o nome da “coisa” que será criada posteriormente na Amazon que será o ponto de acesso entre o ESP32 e o AWS Lambda.

~~~arduino
#define THINGNAME "ESP32_FURB_G"
~~~

![Figura 1 - Passo 1 -> Configurando Arduino para o ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem1.png)
![Figura 2 - Passo 2 -> Configurando Arduino para o ESP32 (continuação)](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem2.png)
![Figura 3 - Passo 3 -> Instalando pacote ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem3.png)
![Figura 4 - Passo 4 -> Selecionando placa ESP32](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem4.png)
![Figura 5 - Passo 5 -> Instalando biblioteca MQTT](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem5.png)
![Figura 6 - Passo 6 -> Instalando biblioteca ArduinoJson](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem6.png)
![Figura 7 - Diagrama de Atividades](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem7.png)
![Figura 8 - Diagrama da skill Hello World](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem8.png)
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
![Figura 20 - Inserindo uma declaração](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem20.png)
![Figura 21 - Salvar e construir o modelo](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem21.png)
![Figura 22 - Mensagem de sucesso após construção do modelo](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem22.png)
![Figura 23 - Importando JSON](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem23.png)
![Figura 24 - Página inicial da AWS Amazon](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem24.png)
![Figura 25 - Página de login da AWS Amazon](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem25.png)
![Figura 26 - Selecionar o serviço Lambda](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem26.png)
![Figura 27 - Criar a função](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem27.png)
![Figura 28 - Escolher um nome e linguagem de programação](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem28.png)
![Figura 29 - Adicionar um gatilho](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem29.png)
![Figura 30 - Utilizar a opção “Copy to clipboard”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem30.png)
![Figura 31 - Escolher “Alexa IoT” e colar o código copiado anteriormente no “Skill ID”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem31.png)
![Figura 32 - Copiar o “Function ARN”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem32.png)
![Figura 33 - Colar o “Function ARN”](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem33.png)
![Figura 34 - Clicar no hiperlink](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem34.png)
![Figura 35 - Expandir e editar a política](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem35.png)
![Figura 36 - Colar e salvar](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem36.png)
![Figura 37 - Painel atualizado](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem37.png)
![Figura 38 - Adicionando uma camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem38.png)
![Figura 39 - Especificando a camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem39.png)
![Figura 40 - Como ficará o item camada](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem40.png)
![Figura 41 - Código do backend](https://github.com/rafsarodrigues/Alexa-Tutorial-PT-BR/blob/main/Imagens%20Tutorial/Imagem41.png)
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
