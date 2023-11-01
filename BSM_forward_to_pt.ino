#include <mcp_can.h>  //lib for gmlan
#include <SPI.h>
#include <FlexCAN_T4.h>  //lib for sending
#include <Arduino.h>
//setup teensy internal can controller
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can;

//setup external can controller for lowspeed
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];  // Array to store serial string

#define CAN0_INT 2  // Set INT to pin 2
MCP_CAN CAN0(10);   // Set CS to pin 10

void setup() {
  Serial.begin(9600);
  //setup external can bus
  // Initialize MCP2515 running at 16MHz with a baudrate of 33.3kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_33K3BPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);  // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);  // Configuring pin for /INT input

  //----------------------------can hs setup--------------

  Can.begin();
  Can.setBaudRate(500000);
  Can.setMaxMB(16);
  Can.enableFIFO();
  Can.enableFIFOInterrupt();

  //-----------------------------------------------
}
void loop() {
  if (!digitalRead(CAN0_INT))  // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);  // Read data

    if ((rxId & 0x1FFFFFFF) == 0x1072C0B9) {
      // Extract the single bit value (0x1 or 0x0) from the received data
      byte receivedBit = rxBuf[0];  // Assuming the bit is in the first byte of the received data
      // Modify your payload based on the received bit
      String payload = String(receivedBit, HEX);  // Convert receivedBit to a hexadecimal string

      //----------------------------- Print the message ID and payload
      Serial.print("Message ID: 0x");
      Serial.print(rxId & 0x1FFFFFFF, HEX);
      Serial.print(" BSM ACTIVE: 0x");
      Serial.print(payload);
      Serial.print(" LENGTH: ");
      Serial.println(len);
      //----------------------
      CAN_message_t msg;
      // Assign message ID
      msg.id = 0x1;  // Set the message ID to the received ID
      // Prepare CAN-BUS payload
      msg.buf[0] = receivedBit;  // Set the payload to the received bit

      // Send message on CAN-BUS
      Can.write(msg);
    } 
    if ((rxId & 0x1FFFFFFF) == 0x1073205B) {
      // Extract the single bit value (0x1 or 0x0) from the received data
      byte receivedBit = rxBuf[0];  // Assuming the bit is in the first byte of the received data
      // Modify your payload based on the received bit
      String payload = String(receivedBit, HEX);  // Convert receivedBit to a hexadecimal string

      //----------------------------- Print the message ID and payload
      Serial.print("Message ID: 0x");
      Serial.print(rxId & 0x1FFFFFFF, HEX);
      Serial.print(" BSM ACTIVE: 0x");
      Serial.print(payload);
      Serial.print(" LENGTH: ");
      Serial.println(len);
      //----------------------
      CAN_message_t msg;
      // Assign message ID
      msg.id = 0x2 ;  // Set the message ID to the received ID
      // Prepare CAN-BUS payload
      msg.buf[0] = receivedBit;  // Set the payload to the received bit

      // Send message on CAN-BUS
      Can.write(msg);
    } 
  }
}
