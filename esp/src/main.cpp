#include <Arduino.h>
#include <ArduinoJson.h>
#include "LoRaMESH.h"

#define LORA_RX 5
#define LORA_TX 18

enum Status {
	WAIT_ARDUINO,
	CHECK_JSON,
	ASK_SERVER,
	SEND_OK,
	SEND_ERROR
};

static int8_t last_state = -1; // debug
static uint8_t state = WAIT_ARDUINO;
static uint8_t reposta[10];
static uint8_t resp_size = 0;
static uint8_t ok_msg = 'Y';
static uint8_t error_msg = 'N';

void fsm();

void setup() {
  //debug
  Serial.begin(9600);

  //LORA
  Serial.println("initing lora");
  SerialCommandsInit(LORA_RX, LORA_TX, 9600);
  Serial.println("lora ok");
}

void loop() {
  fsm();
}
void fsm()
{
  delay(50);
  // debug
  if (last_state != state) {
    Serial.println(state);
    last_state = state;
  }

  uint16_t lixo;

  switch (state) {
  case WAIT_ARDUINO:
    // ve se tem mensagem do arduino
    resp_size = 0;
    ReceivePacketCommand(&lixo, (uint8_t *)&lixo, reposta, &resp_size, 5000);
    // se teve resposta
    if (resp_size != 0) {
      Serial.print("msg do arduino: ");
      Serial.write(reposta, resp_size);
      Serial.println();
      state = CHECK_JSON;
    }
    break;

  case CHECK_JSON:
    // tratar o json dentro do array de char "reposta" com tamanho "resp_size"
    state = ASK_SERVER;
    break;

  case ASK_SERVER:
    // perguntar ao servidor se pode tirar a bike

    // se pode tirar:
    state = SEND_OK;
    break;

    // se não pode tirar:
    //state = SEND_ERROR;
    //break;

  case SEND_OK:
    PrepareFrameCommand(1, 0x7F, &ok_msg, 1);
    SendPacket();
    state = WAIT_ARDUINO;
    break;

  case SEND_ERROR:
  PrepareFrameCommand(1, 0x7F, &error_msg, 1);
    SendPacket();
    state = WAIT_ARDUINO;
    break;
  }
}