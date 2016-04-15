// send ACKNOWLEDGED message to the STmicro
// after recieving an ERROR message
void ser1Ack()
{
  Serial1.write("*ACK:");
}
// ============================================================
// Update LED script based on the new serial command recieved
void updateLED(byte script_id, byte fadespeed, byte timeadj) {
  BlinkM_stopScript(0x00);
  BlinkM_playScript(0x00, script_id, 0x00, 0x00);
  BlinkM_setFadeSpeed(0x00, fadespeed);
  BlinkM_setTimeAdj(0x00, timeadj);
}
// ============================================================
// Parse the command read over serial1
void ser1MessageParse(String message)
{
  // print the new message ot the serial port 0 (USB)
  Serial.print("\nNew command is: ");
  Serial.print(message);

  // parse message
  char command = message[0];
  int value = message.substring(1).toInt();

  switch (command)
  {
    // VOLTAGE
    case 'H':
      Serial.print("\nNew VOLTAGE is: ");
      Serial.print( String(value) );
      Serial.print(" V");
      // LED: keep previous status
      break;
    // CURRENT
    case 'c':
      Serial.print("\nNew CURRENT is: ");
      Serial.print( String(value) );
      Serial.print(" nA");
      // LED: keep previous status
      break;
    // ARC
    case 'a':
      Serial.print("\nNumber of ARCs detected: ");
      Serial.print( String(value) );
      // LED: BLUE Blinking
      updateLED(0x05, 0xff, 0xff);
      break;
    // CURRENT STEP LIMIT EXCEEDED
    case 'i':
      Serial.print("\nCurrent step limit exceeded:");
      Serial.print( String(value) );
      Serial.print(" nA");
      // LED: OFF
      updateLED(0x09, 0xff, 0xff);
      break;
    // ERROR
    case 'E':
      ser1Ack();
      Serial.print("\nError detected, code:");
      Serial.print( String(value) );
      // LED: RED Blinking
      updateLED(0x03, 0xff, 0xff);
      break;
    default:
      break;
  }

  Serial.print("\n*************************");
}
// ============================================================

// Serial: this is the COMMAND variable
// it holds the most recent command from the serial_1 port
String ser1Message = "";

// set flag once a '*' is recieved:
// the '*' represents the start of a new command
// all commands need to start with an '*'
bool ser1MessageFlag = false;

// parse teh character recieved over serial1 port (Tx/Rx)
void ser1CharParse(char c)
{
  if (c == '*')
  {
    ser1Message = "";
    ser1MessageFlag = true;
  }
  else if (ser1MessageFlag == true && c == ':')
  {
    ser1MessageParse(ser1Message);
    ser1Message = "";
    ser1MessageFlag = false;
    Serial.println("");
  }
  else if (ser1MessageFlag == true)
  {
    ser1Message = ser1Message + c;
    Serial.print(c);
  }
}
// ============================================================
