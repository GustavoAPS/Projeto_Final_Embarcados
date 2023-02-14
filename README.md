# Projeto Final - Fundamentos de Sistemas Embarcados - 2022/2


## Alunos

| Nome                          | Matrícula  |
| ----------------------------- | ---------- |
| Ugor Marcílio Brandão         | 17/0164357 |
| Gustavo Afonso Pires Severo   | 17/0034992 |


## Contexto do projeto

Este trabalho tem por objetivo a simulação de uma serra circular indústrial, abstraida por meio de sensores e atuadores baseados nos microcontroladores ESP32 
interconectados via Wifi através do protocolo MQTT. O microcontrolador ESP32 controla a aquisição de dados de sensores, botões e chaves e aciona 
saídas do led da placa e do módulo de led bicolor além de enviar os dados para o Dashboard que apresenta os dados no navegador do usuário, toda a comunicação é feita via rede Wifi.


## 2. Componentes do Sistema

(COLOCAR FOTO DE CADA COISA)


O **Servidor Central Thingsboard** será composto por:
1. Serviço disponibilizado pelo Laboratório de Software (Lappis);
2. Disponibiliza um Broker MQTT;
3. Serviço de Cadastro, registro e monitoramento de dispositivos;
4. Serviço de criação de Dashboards.

**Cliente ESP32 distribuído - Energia**:
1. Dev Kit ESP32;
2. Botão (Presente na placa);
3. LED (Presente na placa).
4. Sensor a ser escolhido pelos alunos;

**Cliente ESP32 distribuído - Bateria**:
1. Dev Kit ESP32;
2. Botão (Presente na placa);
3. LED (Presente na placa).

## 3. Conexões entre os módulos do sistema

1. A interconexão entre os dispositivos será toda realizada através do Protocolo MQTT via rede Ethernet/Wifi. 
2. A rede MQTT será coordenada por um Broker disponível no serviço junto ao Thingsboard;
3. Todas as mensagens via MQTT devem estar no formato JSON;
4. **ESP32**: o botão e o LED a serem usados são os dispositivos já integrados no próprio kit de desenvolimento (Botão = GPIO 0 / LED = GPIO 2);
5. **ESP32**: um conjunto de sensores (analógicos e/ou digitais) serão disponibilizados aos alunos cuja telemetria deve ser implementada e enviada ao Thingsboard;
6. **Servidor Central Thingsboard**: serão criados usuários para cada aluno/grupo que ficarão reposnsáveis por criar os dispositivos e dashboards necessários para a implementação do trabalho;

## 4. Requisitos

Abaixo estão detalhados os requisitos de cada um dos componentes do sistema.

#### **Servidor Central Thingsboard**:
1. Serão criadas as contas/usuários para cada aluno/grupo;
2. Cada aluno/grupo será responsável por cadastrar seus dispositivos e gerar o token de acesso;
3. Cada aluno/grupo será responsável por criar um dashboard que apresente dados de telemetria e dos atributos dos dispositivos, bem como comandos em tela para acionamento de atuadores (elementos controlados) de cada dispositivos distribuído;

Todo o envio de Telemetria e Atributos e recebimento de comandos para o Thingsboard é documentado da API de referência: [ThingsBoard MQTT API reference](https://thingsboard.io/docs/reference/mqtt-api/). 

Cada dispositivo criado no Thingsboard deverá ser configurado com o tipo de transporte (`Tranport Type`) MQTT. A conexão MQTT cujos tópicos para envio de dados são:

- Telemetria: `v1/devices/me/telemetry`
- Atributos: `v1/devices/me/attributes`

O envio e recebimento de comandos deverá ser realizado através de **Remote Procedure Calls (RPC)**, com a documentação descrita na API. Em resumo, o cliente deve se inscrever no tópido `v1/devices/me/rpc/request/+` e irá receber cada comando em um subtópico com `ID` único em `v1/devices/me/rpc/response/$request_id`. Quanto necessário, a confirmação do comando deve ser retornada pelo mesmo tópico com o `ID` enviado (`v1/devices/me/rpc/response/$request_id`). O conteúdo de todas as mensagens será sempre no formato Json.

#### **Cliente ESP32**:

Haverão duas configurações possíveis dos clientes ESP32. A **ESP32-Energia** irá funcionar conectada permanentemente à alimentação e a **ESP32-Bateria** representa um dispositivo operado por baterias (que deve funcionar em modo Low Power). **Atenção**! O firmware para as duas variações deverá ser o mesmo e a opção de funcionamento por um modo ou outro deverá ser definida através de uma variável de ambiente no ***menuconfig***.

1. O código da ESP32 deve ser desenvolvido em C utilizando o framework ESP-IDF (Deve ser indicado no README a versão do framework utilizada para o desenvolvimento e se foi feito usando a ESP-IDF nativa ou via PlatformIO);
2. A ESP32 deverá se conectar via Wifi (com as credenciais sendo definidas em variável de ambiente pelo Menuconfig);
3. Cada cliente ESP32, ao ser iniciado pela primeira vez, deve:  
    3.1 Inicializar o serviço MQTT e conectar ao broker com o token de acesso do dispositivo;  
    3.2 Se inscrever no tópico `v1/devices/me/rpc/request/+` para receber comandos remotos;  
    3.3 Enviar telemetria e atributos pelos respectivos tópicos da API;  

4. Armazenar o estado dos atributos (estado das saídas) em caso de reinicio do dispositivo por falta de energia. A informação de estado deve ser armzenada na memória não volátil (NVS).
5. Realizar telemetria dos sensores selecionados e atualização do dashboard de maneira periódica (no mínimo a cada 1 segundo);
6. Monitorar o botão utilizando interrupções e enviar por mensagem push a cada mudança do estado do botão;
7. Acionar saídas como o LED, dentre outras, à partir dos comandos RPC enviados pelo Dashboard de maneira dimerizável, sua intensidade sendo controlada à partir da técnica de PWM;

## Apresentação

[![IMAGE ALT TEXT HERE](imagens/menu_principal.png)](https://youtu.be/4v6FVHhwNKY)


## Imagens do Sistema

![Dashboard](imagens/dashboard.png)

![Placa](imagens/placa.png)

## 7. Referências

[ESP-IDF - Documentação](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
[ESP-IDF - Github](https://github.com/espressif/esp-idf)
[Thingsboard](https://thingsboard.io)
[ThingsBoard MQTT API reference](https://thingsboard.io/docs/reference/mqtt-api/)
[Biblioteca DHT11 para ESP-IDF ESP32](https://github.com/0nism/esp32-DHT11)  
[Sensor de toque](https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-sensor-detector-de-toque-ky-036)  
[LED RG](https://www.criandorobocomarduino.com/2014/07/como-usar-e-testar-o-led-bicolor-com.html)  
[Sensor foto resistor LDR](https://portal.vidadesilicio.com.br/sensor-de-luz-com-ldr/)     
[Sensor de temperatura digital DS18B20](https://www.curtocircuito.com.br/blog/Categoria%20Arduino/como-utilizar-o-ds18b20)  
[Sensor de temperatura digital DS18B20](https://github.com/DavidAntliff/esp32-ds18b20-example)  

