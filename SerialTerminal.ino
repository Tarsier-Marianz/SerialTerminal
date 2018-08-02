#include <EEPROM.h>

int addressLength   = 10;                 // maximum index/address where to write
int address         = 0;                  //EEPROM address counter
String readKeys, eepromContent;

void(* resetFunc) (void) = 0;             // declaring reset function to be able reset programatically
                                          // sources: https://www.theengineeringprojects.com/2015/11/reset-arduino-programmatically.html

void setup() {
  Serial.begin(9600);                     // turn on Serial Port

  startup();
}

void loop() {
  showMenu();
  while (Serial.available() == 0) {       //Wait for user input
  }
  //readKeys = Serial.readString();
  int choice = Serial.parseInt();

  switch (choice) {
    case 1:
      readEEPROM();
      break;
    case 2:
      showWriteMenu();
      break;
    case 3:
      clearEEPROM();
      break;
    case 4:
      clearScreen();
      Serial.flush();
      resetFunc();
      break;
    case 5:
      break;
    default:
      Serial.println();
      Serial.print("\t Error: Invalid command choice: ");
      Serial.println(readKeys); \
      printNewLine(2);
      break;
  }
}

void startup() {
  Serial.println();
  Serial.println("Tarsier Terminal v1");
  Serial.println("Copyright Tarsier 2018");
  Serial.println();
  Serial.print(">Press S to start...");
  while (readKeys != "s" && readKeys != "S") {
    while (Serial.available() == 0) { }
    readKeys = Serial.readString();
  }
  Serial.println();
}

void showMenu() {
  printNewLine(2);
  Serial.println(">PLEASE SELECT CHOICES:");
  Serial.println("\t1. Read EEPROM ");
  Serial.println("\t2. Write EEPROM");
  Serial.println("\t3. Clear EEPROM");
  Serial.println("\t4. Reset Terminal");
  Serial.print(">");
}

void showWriteMenu() {
  printNewLine(2);
  Serial.print("(EEPROM LENGTH =");
  Serial.print(EEPROM.length());
  Serial.println(")");
  Serial.println("Please enter string with length of 10 for our demo.");
  Serial.println("Only 10 chars will be saved, the rest will be ignored");
  Serial.print(">");
  while (Serial.available() == 0) {           //Wait for user input
  }
  //byte val = Serial.read();
  //writeEEPROM(val);
  readKeys = Serial.readString();
  //int count = readKeys.length();              // get the total length of the entered string
  int count   = addressLength;                  // count from given addressLength (10), it helps also clear
                                                // automatically previous content having more than length of
                                                // new entered string without calling clearEEPROM() function
  for (int c = 0; c < count; c++) {             // extract all chars in entered string
    byte byteValue = readKeys[c];
    writeEEPROM(byteValue);
  }
  Serial.println();
  Serial.print(readKeys);
  Serial.println(" successfully saved!");
}

void readEEPROM() {
  Serial.println();
  Serial.print("EEPROM content: ");
  //for (int i = 0 ; i < EEPROM.length() ; i++) {
  for (int i = 0 ; i < addressLength ; i++) {   // to make it safe we limit out writing addresses
    byte byteValue = EEPROM.read(i);            //read EEPROM data at address i
    Serial.write(byteValue);                    // show byte in ASCII mode, use "write" command not "print"
  }
  Serial.println();
}

void clearEEPROM() {
  //for (int i = 0 ; i < EEPROM.length() ; i++) {
  for (int i = 0 ; i < addressLength ; i++) {   // to be safe, we clear only the given addressLength (10 in my sample)
    if (EEPROM.read(i) != 0) {                  //skip already "empty" addresses
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  address = 0;                                  //reset address counter
  Serial.println();
  Serial.println("EEPROM cleared!");
}

void writeEEPROM(byte value) {
  EEPROM.write(address, value);                 //write value to current address counter address
  address++;                                    //increment address counter
  //if (address == EEPROM.length()) {           //check if address counter has reached the end of EEPROM
  if (address == addressLength) {               // to make it safe we limit out writing addresses
    address = 0;                                //if yes: reset address counter
  }
}

void printNewLine(int count) {
  for (int i = 0; i < count; i++) {
    Serial.println();
  }
}

void clearScreen(){
  //let us try to clear screen hyperterminal by sending ESC"[2J"
  Serial.write("\033[2J");                    // it WORKS! in windows hyperterminal    
}

