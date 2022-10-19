#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include "LoRaMESH.h"

#define BTN_CNT      3
#define BIKE1_LED     A2
#define BIKE2_LED     A1
#define BIKE3_LED     A0 
#define RFID_LED_OK   A3
#define RFID_LED_ERR  A4
#define BIKE1_BTN     4
#define BIKE2_BTN     3
#define BIKE3_BTN     2
#define LORA_TX       5
#define LORA_RX       6
#define SS_PIN 10 //PINO SDA
#define RST_PIN 9 //PINO DE RESET

enum Status {
	READ_RFID,
	SELECT_BIKE,
	SEND_REQUEST,
	WAIT_FOR_ESP,
	UNLOCK_BIKE,
	ERROR_STATE
};

typedef struct {
	uint8_t last;
} Button_t;

typedef struct {
	uint8_t name_idx;
	uint8_t bike_idx;
} Selection_t;

/* private variables */
static int8_t last_state = -1; //debug
static uint8_t state = READ_RFID;
static Button_t bts[BTN_CNT] = {};
static uint8_t bike_status[3] = {};
static Selection_t choice = {};
static String names[] = {"Rosinaldo", "Rafael", "Marco", "Bois"};
static String tags[]  = {"tag rosi", "tag rafael", "tag arco", "tag bois"};
static String bikes[] = {"EMPTY", "BIKE 1", "BIKE 2", "BIKE 3"};
static DynamicJsonDocument json_data(20);
static MFRC522 rfid(SS_PIN, RST_PIN);

/* function */
void fsm();
void quick_blink_green();
void quick_blink_red();
void quick_blink_pin(uint8_t pin);

void setup() {
  // Serial
  Serial.begin(9600);

  json_data["tenant"] = "";
  json_data["name"] = "";

  // coloca as bikes
  bike_status[0] = 1;
  bike_status[1] = 1;
  bike_status[2] = 1;

  // leds e botões
	pinMode(BIKE1_LED, OUTPUT);
	pinMode(BIKE2_LED, OUTPUT);
	pinMode(BIKE3_LED, OUTPUT);
	pinMode(RFID_LED_OK, OUTPUT);
	pinMode(RFID_LED_ERR, OUTPUT);
	pinMode(BIKE1_BTN, INPUT);
	pinMode(BIKE2_BTN, INPUT);
	pinMode(BIKE3_BTN, INPUT);

  // acende as bikes
  digitalWrite(BIKE1_LED, HIGH);
  digitalWrite(BIKE2_LED, HIGH);
  digitalWrite(BIKE3_LED, HIGH);

	//RFID
  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522

	//LoRaMESH
	SerialCommandsInit(LORA_RX, LORA_TX, 9600);
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
  String json_string;
  const char* c_json_str;
	uint8_t btn_state;
  String strID;
  uint16_t lixo;
  uint8_t resposta[10];
  uint8_t resp_size = 0;

	switch (state) {

	case READ_RFID:
    // verifica se tem algum RFID pra ler
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) break;
 
    // se tiver RFID pra ler, pega o valor da tag dele
    strID = "";
    for (byte i = 0; i < 4; i++) {
      strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i!=3 ? ":" : "");
    }
    strID.toUpperCase();
 
    // printa tag lida
    Serial.print("Identificador (UID) da tag: ");
    Serial.println(strID);

    // set name index
    if (strID.equals(tags[0])){
      // tag de rosi
      choice.name_idx = 0;
      state = SELECT_BIKE;
      quick_blink_green();
      break;
    }
    else if (strID.equals(tags[1])){
      // tag de rafael
      choice.name_idx = 1;
      state = SELECT_BIKE;
      quick_blink_green();
      break;
    }
    else if (strID.equals(tags[2])){
      // tag de marco
      choice.name_idx = 2;
      state = SELECT_BIKE;
      quick_blink_green();
      break;
    }
    else if (strID.equals(tags[3])){
      // tag de bois
      choice.name_idx = 3;
      state = SELECT_BIKE;
      quick_blink_green();
      break;
    }
    else {
      // tag não reconhecida
      Serial.print("tag nao reconhecida!");
      quick_blink_red();
      break;
    }

		break;

	case SELECT_BIKE:
		// if select bike 1
		btn_state = digitalRead(BIKE1_BTN);
		if (btn_state != bts[0].last && btn_state == HIGH) {
			choice.bike_idx = 1;
      // se tiver bike no lugar
      if(bike_status[0] == 1){
        state = SEND_REQUEST;
      }
      else {
        state = ERROR_STATE;
      }
			// quick blink select bike
      quick_blink_pin(BIKE1_LED);
		}
		bts[0].last = btn_state;

		// if select bike 2
		btn_state = digitalRead(BIKE2_BTN);
		if (btn_state != bts[1].last && btn_state == HIGH) {
			choice.bike_idx = 2;
      // se tiver bike no lugar
			if(bike_status[1] == 1){
        state = SEND_REQUEST;
      }
      else {
        state = ERROR_STATE;
      }
			// quick blink select bike
      quick_blink_pin(BIKE2_LED);
		}
		bts[1].last = btn_state;

		// if select bike 3
		btn_state = digitalRead(BIKE3_BTN);
		if (btn_state != bts[2].last && btn_state == HIGH) {
			choice.bike_idx = 3;
      // se tiver bike no lugar
			if(bike_status[2] == 1){
        state = SEND_REQUEST;
      }
      else {
        state = ERROR_STATE;
      }
			// quick blink select bike
      quick_blink_pin(BIKE3_LED);
		}
		bts[2].last = btn_state;
		break;

	case SEND_REQUEST:
    // set json data
    json_data["tenant"] = names[choice.name_idx];
    json_data["name"] = bikes[choice.bike_idx];
    // stringfy json
    serializeJson(json_data, json_string);
    c_json_str = json_string.c_str();
		// send lora
    PrepareFrameCommand(0, 0x7F, (uint8_t *)c_json_str, json_string.length());
    SendPacket();
    // wait esp
    state = WAIT_FOR_ESP;
		break;
  
  case WAIT_FOR_ESP:
    // ve se tem pacote
    ReceivePacketCommand(&lixo, (uint8_t *)&lixo, resposta, &resp_size, 5000);
    // se teve resposta
    if (resp_size != 0) {
      if (resposta[0] == 'Y'){
        state = UNLOCK_BIKE;
        break;
      }
      else if(resposta[0] == 'N'){
        state = ERROR_STATE;
        break;
      }
    }
    else {
      break;
    }
    break;

	case UNLOCK_BIKE:
    bike_status[(choice.bike_idx-1)] = 0;
    quick_blink_green();
    if (choice.bike_idx == 1) {
      digitalWrite(BIKE1_LED, LOW);
    }
    else if (choice.bike_idx == 2) {
      digitalWrite(BIKE2_LED, LOW);
    }
    else if (choice.bike_idx == 3) {
      digitalWrite(BIKE3_LED, LOW);
    }
    choice = {};
    json_data["tenant"] = "";
    json_data["name"] = "";
    state = READ_RFID;
		break;
  
  case ERROR_STATE:
    quick_blink_red();
    state = READ_RFID;
    choice = {};
    json_data["tenant"] = "";
    json_data["name"] = "";
    break;
	}
}

void quick_blink_pin(uint8_t pin){
  digitalWrite(pin, !digitalRead(pin));
	delay(100);
	digitalWrite(pin, !digitalRead(pin));
	delay(100);
	digitalWrite(pin, !digitalRead(pin));
	delay(100);
	digitalWrite(pin, !digitalRead(pin));
	delay(100);
}

void quick_blink_green()
{
  quick_blink_pin(RFID_LED_OK);
  digitalWrite(RFID_LED_OK, LOW);
}

void quick_blink_red()
{
  quick_blink_pin(RFID_LED_ERR);
  digitalWrite(RFID_LED_ERR, LOW);
}