#include <EnableInterrupt.h>

#include <EEPROM.h>
#define RedPin 9
#define BluePin 5
#define GreenPin 6






#include <CapacitiveSensor.h>

/*
* CapitiveSense Library Demo Sketch
* Paul Badger 2008
* Uses a high value resistor e.g. 10 megohm between send pin and receive pin
* Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
* Receive pin is the sensor pin - try different amounts of foil/metal on this pin
* Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
*/
int Sensors[9] = { A4,10,11,12,13,A0,A1,A2,A3 };
int HUE;
int HUE_new;
byte currentColor[3] = { 0,0,0 };
bool toDisable = false;
int disableCount = 0;
bool UV;
bool zeroColor = false;

void ChangeColorPress() {
//  Serial.println("Red");
  delay(200);
  int r, g, b;
//  bool set_r, set_g, set_b = false;
  r = !(bool)digitalRead(2);
  g= !(bool) digitalRead(3);
  b= !(bool)digitalRead(4);
  ///Serial.println(r, DEC);
  //Serial.println(g, DEC);
  //Serial.println(b, DEC);
  if (r + g + b == 3)
  {
    ChangeColor(new byte[3]{ 255,255,255 }, HUE);
  }
  else if (r)
  {
    if(g)
      ChangeColor(new byte[3]{ 255,255,0 }, HUE);
    else if (b)
      ChangeColor(new byte[3]{ 255,0,255 }, HUE);
    else
      ChangeColor(new byte[3]{ 255,0,0 }, HUE);
  }
  else if (g)
  {
    if (b)
      ChangeColor(new byte[3]{ 0,255,255 }, HUE);
    else
      ChangeColor(new byte[3]{ 0,255,0 }, HUE);
  }
  else if (b)
  {
      ChangeColor(new byte[3]{ 0,0,255 }, HUE);
  }


  delay(200);
}

void SaveStartup(){
  EEPROM.write(0, currentColor[0]);
  EEPROM.write(1, currentColor[1]);
  EEPROM.write(2, currentColor[2]);
  EEPROM.write(3, HUE);
  EEPROM.write(4, UV);
  EEPROM.write(5, zeroColor);
  delay(400);
  blink(3);
}
const byte dim_curve[] = {
  0,    10,    25,    45,  60, 90,  125,
  180  ,  255
};
void ChangeColor(byte* Color,int HUEtoset)
{
  if (!zeroColor) {
    currentColor[0] = Color[0];
    currentColor[1] = Color[1];
    currentColor[2] = Color[2];
    HUE = HUEtoset;
    Serial.println(HUE);
    for (size_t i = 0; i < 3; i++)
    {
      Serial.println((Color[i]) + (1 - ((Color[i]) / 255))*dim_curve[HUE]);
    }
    Serial.println();
    analogWrite(RedPin, ((Color[0]) + (1 - ((Color[0]) / 255))*dim_curve[HUE]));
    analogWrite(GreenPin, ((Color[1]) + (1 - ((Color[1]) / 255))*dim_curve[HUE]));
    analogWrite(BluePin, (Color[2]) + (1 - ((Color[2]) / 255))*dim_curve[HUE]);
  }
  else
  {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(9, LOW);

  }
}

void blink(int count)
{
  byte oldColor[3] = { currentColor[0],currentColor[1],currentColor[2] };
  for (int i = 0; i < count; i++)
  {
    ChangeColor(new byte[3]{ 0,0,0 },HUE);
    delay(200);
    ChangeColor(oldColor, 0);
    delay(200);
  }
  ChangeColor(oldColor, HUE);

}

void setup()
{

  EEPROM.begin();
  currentColor[0] = EEPROM.read(0);
  currentColor[1] = EEPROM.read(1);
  currentColor[2] = EEPROM.read(2);
  UV = !EEPROM.read(4);
  digitalWrite(8, UV);
  HUE = EEPROM.read(3);
  zeroColor = EEPROM.read(5);
  ChangeColor(currentColor,HUE);
  //cs = CapacitiveSensor[10];
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  // turn off autocalibrate on channel 1 - just as an example
    //Serial.begin(9600);
  Serial.println("Hello");
  HUE_new = HUE;
  switchUV();
    Serial.println(zeroColor);
  Serial.println(currentColor[0]);
    Serial.println(currentColor[1]);
      Serial.println(currentColor[2]);
      
}


void loop()
{
  if (digitalRead(2) == 0 || digitalRead(3) == 0 || digitalRead(4) == 0)
    ChangeColorPress();
  if (digitalRead(8) == 0)
    SaveStartup();
  
  for (size_t i = 0; i < 9; i++)
  {
    if (readCapacitivePin(Sensors[i])>2) {
      HUE_new = i ;
      //Serial.println(i);
    //  delay(300);
    }
  }
  if (HUE_new != HUE)
    ChangeColor(currentColor, HUE_new);
  //Serial.println(analogRead(A6));
  if (toDisable)
  {
    if (disableCount > 100)
      disableLight();
    else
    {
      if (analogRead(A7) >500)
      disableCount++;
      else
        switchUV();

    }

  }
  if(analogRead(A7)>500)
  {
    toDisable = true;
  }
  delay(10);
  /*for (int i =0; i<9; i++)
  {
    Serial.println((String)i+": "+(String)readCapacitivePin(Sensors[i]));
  }
  Serial.println();
  Serial.println();
  delay(100);   */                          // arbitrary delay to limit data to serial port 
  
}

void disableLight() {
  toDisable = false;
  disableCount = 0;
  Serial.println("Light");
  zeroColor = !zeroColor;
  if (zeroColor)
  {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(9, LOW);

  }
  else
  {
    ChangeColor(currentColor, HUE);
    digitalWrite(7, UV);

  }
  delay(500);

}
void switchUV() {
  Serial.println("UV");
  toDisable = false;
  disableCount = 0;
  UV = !UV;
  digitalWrite(7, UV);
  delay(100);
}
uint8_t readCapacitivePin(int pinToMeasure) {

  // Variables used to translate from Arduino to AVR pin naming

  volatile uint8_t* port;

  volatile uint8_t* ddr;

  volatile uint8_t* pin;

  // Here we translate the input pin number from

  //  Arduino pin number to the AVR PORT, PIN, DDR,

  //  and which bit of those registers we care about.

  byte bitmask;

  port = portOutputRegister(digitalPinToPort(pinToMeasure));

  ddr = portModeRegister(digitalPinToPort(pinToMeasure));

  bitmask = digitalPinToBitMask(pinToMeasure);

  pin = portInputRegister(digitalPinToPort(pinToMeasure));

  // Discharge the pin first by setting it low and output

  *port &= ~(bitmask);

  *ddr |= bitmask;

  delay(1);

  uint8_t SREG_old = SREG; //back up the AVR Status Register

               // Prevent the timer IRQ from disturbing our measurement

  noInterrupts();

  // Make the pin an input with the internal pull-up on

  *ddr &= ~(bitmask);

  *port |= bitmask;



  // Now see how long the pin to get pulled up. This manual unrolling of the loop

  // decreases the number of hardware cycles between each read of the pin,

  // thus increasing sensitivity.

  uint8_t cycles = 17;
//  Serial.println(*pin);
  if (*pin & bitmask) { cycles = 0; }

  else if (*pin & bitmask) { cycles = 1; }

  else if (*pin & bitmask) { cycles = 2; }

  else if (*pin & bitmask) { cycles = 3; }

  else if (*pin & bitmask) { cycles = 4; }

  else if (*pin & bitmask) { cycles = 5; }

  else if (*pin & bitmask) { cycles = 6; }

  else if (*pin & bitmask) { cycles = 7; }

  else if (*pin & bitmask) { cycles = 8; }

  else if (*pin & bitmask) { cycles = 9; }

  else if (*pin & bitmask) { cycles = 10; }

  else if (*pin & bitmask) { cycles = 11; }

  else if (*pin & bitmask) { cycles = 12; }

  else if (*pin & bitmask) { cycles = 13; }

  else if (*pin & bitmask) { cycles = 14; }

  else if (*pin & bitmask) { cycles = 15; }

  else if (*pin & bitmask) { cycles = 16; }



  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before

  SREG = SREG_old;



  // Discharge the pin again by setting it low and output

  //  It's important to leave the pins low if you want to 

  //  be able to touch more than 1 sensor at a time - if

  //  the sensor is left pulled high, when you touch

  //  two sensors, your body will transfer the charge between

  //  sensors.

  *port &= ~(bitmask);

  *ddr |= bitmask;



  return cycles;

}
