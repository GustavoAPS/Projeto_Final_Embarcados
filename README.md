# Projeto Final - Fundamentos de Sistemas Embarcados - 2022/2


## Alunos

| Nome                          | Matrícula  |
| ----------------------------- | ---------- |
| Ugor Marcílio Brandão         | 17/0164357 |
| Gustavo Afonso Pires Severo   | 17/0034992 |


## 1. Contexto do projeto

Este trabalho tem por objetivo a simulação de uma serra circular indústrial, abstraida por meio de sensores e atuadores baseados nos microcontroladores ESP32 
interconectados via Wifi através do protocolo MQTT. O microcontrolador ESP32 controla a aquisição de dados de sensores, botões e chaves e aciona 
saídas do led da placa e do módulo de led bicolor além de enviar os dados para o Dashboard que apresenta os dados no navegador do usuário, toda a comunicação é feita via rede Wifi.


## 2. Regras do sistema

Para as regras de funcionamento da serra circular ficaram estabelecidas as seguintes regras:

- O motor da serra é simulado por um Led Bi-color, para a cor verde a serrra funciona em estado normal e vermelho em estado de alta rotação.
- O botão boot da Esp32 simula um desligamento da serra, que só deve ser possível se esta não estiver em alta rotação.
- Para a serra entrar em modo de alta rotação, o sensor photo interruptor deve ser acionado.
- O sensor de touch liga o Led azul da Esp 32, indicando que a serra está sendo manuseada.
- O PWM é simulado por meio dos leds, indicando a rotação do motor da serra.


## 3. Implementação do sistema

- O código da ESP32 foi desenvolvido em C utilizando o framework do PlatformIO no VsCode.
- A interconexão entre os dispositivos foi feita através do Protocolo MQTT via rede Ethernet/Wifi. 
- A rede MQTT foi coordenada por um Broker disponível no serviço junto ao Thingsboard.
- Todas as mensagens via MQTT foram formatadas no formato JSON, como mostrado abaixo.
    - Telemetria: `v1/devices/me/telemetry`
    - Atributos: `v1/devices/me/attributes`
- O envio e recebimento de comandos deverá foi realizado através de **Remote Procedure Calls (RPC)**.
- Quando inciada pela primeira vez a ESP32:  
    - Inicializa o serviço MQTT e conecta ao broker com o token de acesso do dispositivo.  
    - Se inscreve no tópico `v1/devices/me/rpc/request/+` para receber comandos remotos.
    - Envia telemetria e atributos pelos respectivos tópicos da API.  
- Armazenar o estado dos atributos do sensor DHT11 caso de reinicio do dispositivo por falta de energia. A informação da ultima leitura fica armzenada na memória não volátil (NVS).
- Realizar telemetria dos sensores a cada 1/2 segundo;
- Simula o PWM do motor da serra por meio do LED bicolor usando o mesmo princípio.

#### Esp 32

<img src=img/esp32.jpg  width="300" height="300">

#### Sensor Temperatura/Umidade DHT11

<img src=img/dht11.jpg  width="300" height="300">

#### LED Bi-Color

<img src=img/led_bicolor.png  width="300" height="300">

#### Sensor tempertatura lm25dz

<img src=img/lm35dz.jpg  width="300" height="300">

#### Sensor Photo Interruptor

<img src=img/photo_interruptor.jpg  width="300" height="300">

#### Sensor De Toque

<img src=img/sensor_toque.jpg  width="300" height="300">


## Apresentação

[![IMAGE ALT TEXT HERE](imagens/menu_principal.png)](https://youtu.be/4v6FVHhwNKY)


## Imagens do Sistema

![Dashboard](imagens/dashboard.png)

![Placa](imagens/Placa.jpg)

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

