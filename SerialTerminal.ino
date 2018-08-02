#include <EEPROM.h>

int addressLength   = 10;
int address         = 0;            //EEPROM address counter
String readKeys, eepromContent;

void setup() {
  Serial.begin(9600);      // turn on Serial Port

  startup();
}

void loop() {
  showMenu();
  while (Serial.available() == 0) {           //Wait for user input
  }
  //readKeys = Serial.readString();

  int choice = Serial.parseInt();

  switch (choice) {
    case 1:
      eepromContent = readEEPROM();
      //Serial.println(eepromContent);
      Serial.println(eepromContent);
      break;
    case 2:
      showWriteMenu();
      break;
    case 3:
      clearEEPROM();
      break;
    case 4:
    case 5:
      break;
    default:
      Serial.print("\t Error: Invalid command choice: \t");
      Serial.println(readKeys); \
      printNewLine(2);
      break;
  }
}

void startup() {
  Serial.println("Tarsier Terminal v1");
  Serial.println("Copyright Tarsier 2018");
  Serial.println();
  Serial.print(">Press S to start...");
  while (readKeys != "s") {
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
  Serial.println();
  Serial.print(">");
}

void showWriteMenu() {
  printNewLine(2);
  Serial.print("(EEPROM LENGTH =");
  Serial.print(EEPROM.length());
  Serial.println(")");
  Serial.println("Please enter string with length of 10 for our demo.");
  Serial.println("Only 10 chars will be saved, the reast will be ignored");
  Serial.print(">");
  while (Serial.available() == 0) {           //Wait for user input
  }
  //byte val = Serial.read();
  //writeEEPROM(val);
  readKeys = Serial.readString();
  //int count = readKeys.length();              // get the total length of the entered string
  int count   = addressLength;
  for (int c = 0; c < count; c++) {           // extract all chars in entered string
    byte byteValue = readKeys[c];
    writeEEPROM(byteValue);
  }
}

String readEEPROM() {
  String data;
  //for (int i = 0 ; i < EEPROM.length() ; i++) {
  for (int i = 0 ; i < addressLength ; i++) {       // to make it safe we limit out writing addresses
    byte byteValue = EEPROM.read(i);                //read EEPROM data at address i
    Serial.write(byteValue);                      // show byte in ASCII mode, use "write" command not "print"
    //data += byteValue;
    //data += ", ";
  }
  return data;
}

void clearEEPROM() {
  //for (int i = 0 ; i < EEPROM.length() ; i++) {
  for (int i = 0 ; i < addressLength ; i++) {     // to be safe, we clear only the given addressLength (10 in my sample)
    if (EEPROM.read(i) != 0) {                  //skip already "empty" addresses
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  address = 0;                                  //reset address counter
}

void writeEEPROM(byte value) {
  EEPROM.write(address, value);         //write value to current address counter address
  address++;                      //increment address counter
  //if (address == EEPROM.length()) { //check if address counter has reached the end of EEPROM
  if (address == addressLength) { // to make it safe we limit out writing addresses
    address = 0;              //if yes: reset address counter
  }
}

void printNewLine(int count) {
  for (int i = 0; i < count; i++) {
    Serial.println();
  }
}

