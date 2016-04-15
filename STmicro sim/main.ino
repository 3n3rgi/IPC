
// ===============================================
// Serial: this is the COMMAND variable
// it holds the most recent command from the serial_1 port
String serial1Command = "";

// set flag once a '*' is recieved:
// the '*' represents the start of a new command
// all commands need to start with an '*'
bool serialCommandFlag = false;

Timer timer(5000, sendUpdateToPhoton);


// ===============================================
void setup() {

    // Open serial port (via USB port)
    Serial.begin(9600);

    // Open serial port 1 (via Tx/Rx pins)
    Serial1.begin(115200);

    // start a software timer for updateing the PHOTON with
    // the latest VOLTAGE and CURRENT
    timer.start();
}



// ===============================================
// runs continuously
void loop()
{

}



// ===============================================
// timer Interrupt
void sendUpdateToPhoton()
{
  // Send new voltage and current to the Photon every 400ms
  sendToPhoton("voltage", random(3000, 4000));
  sendToPhoton("current", random(1, 100));
}

// Interrupt for serial 0 (USB)
// called when data is rady to be read
void serialEvent()
{
    char c = Serial.read();
    //Serial.println(c);
    switch (c)
    {
      case 'a':
        Serial1.write("*a4:");
        break;
      case 'i':
        Serial1.write("*i12:");
        break;
      case 'E':
        Serial1.write("*E1:");
        break;
      default:
        break;
    }
}

// Interrupt for serial 1 (Tx/Rx)
// called when data is rady to be read
void serialEvent1()
{
    char c = Serial1.read();

    if (c == '*')
    {
      serial1Command = "";
      serialCommandFlag = true;
    }
    else if (serialCommandFlag == true && c == ':')
    {
      parseSerialCommand(serial1Command);
      serial1Command = "";
      serialCommandFlag = false;
      Serial.println("");
    }
    else if (serialCommandFlag == true)
    {
      serial1Command = serial1Command + c;
      Serial.print(c);
    }
}

void sendToPhoton(String command, int value)
{
  // start character '*'
  Serial1.write("*");

  // command
  if (command == "voltage")
  {
    Serial1.write("H");
  }
  else if (command == "current")
  {
    Serial1.write("c");
  }

  // value
  Serial1.write( String(value) );

  // end of message
  Serial1.write(":");
}

// Parse the command read over serial1
void parseSerialCommand(String command)
{
  Serial.print("\nNew command is: ");
  Serial.print(command);
  Serial.print("\n");

  if (command == "on")
  {
    // BlinkM_fadeToRGB(0x00, 0x00, 0xff, 0xff);
    //updateLED(0x02, 0xff, 0xff);
  }
  else if (command == "off")
  {
    //updateLED(0x09, 0xff, 0xff);
  }
  else if (command == "error")
  {
    //updateLED(0x03, 0xff, 0xff);
  }
  else
  {
    //updateLED(0x0A, 0xff, 0xff);
  }
}

// Update LED script based on the new serial command recieved
/*
void updateLED(byte script_id, byte fadespeed, byte timeadj)
{
  BlinkM_stopScript(0x00);
  BlinkM_playScript(0x00, script_id, 0x00, 0x00);
  BlinkM_setFadeSpeed(0x00, fadespeed);
  BlinkM_setTimeAdj(0x00, timeadj);
}
*/
