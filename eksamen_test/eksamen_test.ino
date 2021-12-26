#include <arduino-timer.h> // Timer libary

auto timer = timer_create_default();

//Globals
const int CHECK_TEMP_UPPER_LIMIT = 0;
const int CHECK_TEMP_LOWER_LIMIT = 1;

const int ROOM_TEMP_UPPER_LIMIT = 4;
const int ROOM_TEMP_LOWER_LIMIT = 1;

const int ARRAY_SIZE = 50;
const int LED1 = 4;
const int LED2 = 7;
const int LED3 = 8;
const int LED4 = 10;
const int LED9 = 3; //Brugt port 3 istedet for port 9, da port9 ville opvarme temperaturmåleren
const int TEMP_PORT = A0;
float CurrentTemp, RoomTemp, TempArray[ARRAY_SIZE];
int State, TempArrayIndex;
bool blinking;



float GetMiddleTemp() //Henter tallene fra Array, lægger sammen og finder middelværdien
{
  float total = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    total += TempArray[i];
  }
  return (total / ARRAY_SIZE);
}

float GetMaxTemp() //Henter tallene fra arrayet og finder max værdien
{
  float maxVal = TempArray[0];
  for (int i = 0; i < ARRAY_SIZE; i ++) {
    if (maxVal < TempArray[i]) {
      maxVal = TempArray[i];
    }
  }
  return maxVal;
}

float GetMinTemp() // Henter tallene fra arrayet og finder min værdien
{
  float minVal = TempArray[0];
  for (int i = 0; i < ARRAY_SIZE; i ++) {
    if (minVal > TempArray[i]) {
      minVal = TempArray[i];
    }
  }
  return minVal;
}

float GetStdDeviation()
{
  float total = 0;
  float variance;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    total += pow(TempArray[i] - GetMiddleTemp(), 2);
  }
  variance = (total / ARRAY_SIZE);
  return (sqrt(variance));
}

float ReadTemp() // Function til at læse temperaturen
{
  return ((analogRead(TEMP_PORT) - 82) / 4.0); //400 mVolt/4,88mVolt = 82 og 19,5 mVolt/4,88mVolt= 4 */
}

int SaveTemp(float Temp) // Function til at gemme data i mit array
{
  TempArray[TempArrayIndex] = Temp;
  TempArrayIndex++;
  if (TempArrayIndex >= ARRAY_SIZE) {
    TempArrayIndex = 0;
  }
}

bool ToggleLed(void *) { //Function til at få mine lampe til at blinke
    digitalWrite(LED1, blinking ? !digitalRead(LED1) : LOW);
    digitalWrite(LED2, blinking ? !digitalRead(LED2) : LOW);
    digitalWrite(LED3, blinking ? !digitalRead(LED3) : LOW);
    digitalWrite(LED4, blinking ? !digitalRead(LED4) : LOW);
  return true;
}

bool DocumentData(void *) { // Function der dokumentere og printer den ønsket data
  Serial.print("Maks værdi: ");
  Serial.print(GetMaxTemp());

  Serial.print("  Min værdi: ");
  Serial.print(GetMinTemp());

  Serial.print("  Middelværdi: ");
  Serial.print(GetMiddleTemp());

  Serial.print("  Standard afvigelsen: ");
  Serial.println(GetStdDeviation());

  Serial.println("");

  return true;
}

void SetPwmValueLed9() //Opgave C
{
  float DiffTemp = constrain(DiffTemp, 0.0, 5.0);
  float LampVal;
  if (CurrentTemp >= RoomTemp) {
    DiffTemp = CurrentTemp - RoomTemp;
    LampVal = ((255 / 5) * DiffTemp);
    analogWrite(LED9, LampVal);
  }
}

bool ReadData(void *) //Funktion til at læse temperaturen
{ 
  CurrentTemp = ReadTemp();
  SaveTemp(CurrentTemp);
  SetPwmValueLed9();
  return true;
}

void setup()
{
  //Initialaize
  State = CHECK_TEMP_UPPER_LIMIT;
  TempArrayIndex = 0;
  TempArray[ARRAY_SIZE] = {0};
  blinking = false;

  Serial.begin(9600);
  pinMode(TEMP_PORT, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED9, OUTPUT);
  RoomTemp = ReadTemp();
  Serial.print("Nuværende rumtemperatur: ");
  Serial.println(RoomTemp);
  timer.at(0, ReadData); // Læser i tid 0 for at sikre komplet datasæt før første udskrift
  timer.every(200, ReadData); // Timer der eksikveres hvert 200ms og læser/opsamler data
  timer.every(500, ToggleLed); // Timer der hvert 500ms tjekker om lamperne skal blinke
  timer.every(10000, DocumentData); // Timer der hvert 10s printer mine dataer
}


void loop()
{
  timer.tick(); // Timer sættes igang med at ticke

  switch (State)
  {
    case CHECK_TEMP_UPPER_LIMIT:
      if (CurrentTemp >= (RoomTemp + ROOM_TEMP_UPPER_LIMIT)) {
        blinking = true;
        Serial.print("Temperatur har ovreskredet start temperatur med 4 grader: ");
        Serial.println(CurrentTemp);
        State = CHECK_TEMP_LOWER_LIMIT;
      }
      break;
    case CHECK_TEMP_LOWER_LIMIT:
      if (CurrentTemp <= (RoomTemp + ROOM_TEMP_LOWER_LIMIT)) {
        blinking = false;
        Serial.print("Temperaturen er faldet til: ");
        Serial.println(CurrentTemp);
        State = CHECK_TEMP_UPPER_LIMIT;
      }
      break;
    default:
      Serial.print("Uventet state: ");
      Serial.println(State);
      break;
  }
}
