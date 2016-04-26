#include <CapacitiveSensor.h>

/*
* CapitiveSense Library Demo Sketch
* Paul Badger 2008
* Uses a high value resistor e.g. 10 megohm between send pin and receive pin
* Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
* Receive pin is the sensor pin - try different amounts of foil/metal on this pin
* Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
*/
long SensorValues[10];
long STDValues[10];
CapacitiveSensor cs[10];
void setup()
{

	//cs = CapacitiveSensor[10];
	for (int i = 2; i<13; i++)
	{
		cs[i] = CapacitiveSensor(A0, i);
	}
	double ValuesToNormalize;
	for (int i = 0; i < 10; i++)
	{

	
	for (int z = 0; z < 100;z++)
	{
		ValuesToNormalize= ValuesToNormalize+cs[i].capacitiveSensor(30)/100;
	}
	STDValues[i] = ValuesToNormalize ;
	}
	// turn off autocalibrate on channel 1 - just as an example
	Serial.begin(9600);

}

void loop()
{
	for (int i =0; i<10; i++)
	{
		SensorValues[i]=cs[i].capacitiveSensor(30);
		
	}

	for (int i = 0; i<10; i++)
	{
		if (SensorValues[i] < STDValues[i] - 100 || SensorValues[i] > STDValues[i] + 100)
		{
			//action


		}
	}
	delay(10);                             // arbitrary delay to limit data to serial port 
}
