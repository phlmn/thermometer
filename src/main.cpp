#include <OneWire.h>
#include <DallasTemperature.h>
#include <TFT.h>
#include <SPI.h>

#define ONE_WIRE_BUS  10

#define SCREEN_CS     9
#define SCREEN_DC     7
#define SCREEN_RESET  8

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

TFT screen = TFT(SCREEN_CS, SCREEN_DC, SCREEN_RESET);

class Text
{
  private:
    String lastText;
    int posX;
    int posY;
    float textSize;

  public:
    Text(int posX, int posY, float textSize):
      lastText(""), posX(posX), posY(posY), textSize(textSize) { }

    void update(String newText)
    {
      // prevent unnecessary updates
      if (newText == lastText) return;

      screen.setTextSize(textSize);

      // clear old text
      screen.stroke(0, 0, 0);
      screen.text(lastText.c_str(), posX, posY);

      // write new text
      screen.stroke(255, 255, 255);
      screen.text(newText.c_str(), posX, posY);

      lastText = newText;
    }
};

float temp1Min = +INFINITY;
float temp1Max = -INFINITY;

float temp2Min = +INFINITY;
float temp2Max = -INFINITY;

Text temp1Text(35, 20, 3.0);
Text temp1MinText(13, 50, 1.0);
Text temp1MaxText(87, 50, 1.0);

Text temp2Text(35, 70, 3.0);
Text temp2MinText(13, 100, 1.0);
Text temp2MaxText(87, 100, 1.0);

void setup(void)
{ 
  Serial.begin(9600); 

  sensors.begin(); 

  screen.begin();
  screen.background(0, 0, 0);
  screen.setTextSize(3);
}

void loop(void)
{ 
  sensors.requestTemperatures();
  float temp1 = sensors.getTempCByIndex(0);
  float temp2 = sensors.getTempCByIndex(1);

  temp1Min = min(temp1Min, temp1);
  temp1Max = max(temp1Max, temp1);

  temp2Min = min(temp2Min, temp2);
  temp2Max = max(temp2Max, temp2);
  
  Serial.println(String("") + millis() + "," + temp1 + "," + temp2);

  temp1Text.update(String("") + temp1);
  temp1MinText.update(String("Min: ") + temp1Min);
  temp1MaxText.update(String("Max: ") + temp1Max);

  temp2Text.update(String("") + temp2);
  temp2MinText.update(String("Min: ") + temp2Min);
  temp2MaxText.update(String("Max: ") + temp2Max);

  delay(500); 
}
