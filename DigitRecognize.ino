#include "neuton.h"
#include <M5GFX.h>

M5GFX display;

const uint8_t touchBufferSize = 255;
float* inputBuff = (float*) calloc(touchBufferSize, sizeof(float));   // allocate memory for pixel buffer with 0s
uint32_t prev = 0;
uint16_t counter = 0;

void setup()
{
  // Initialize the M5Stack object
  display.init();
  Serial.begin(115200);

  if (!inputBuff)
  {
    Serial.println("Failed to allocate memory");
  }

  // Check if the display is present
  if (!display.touch())
  {
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("Touch not found.", display.width() / 2, display.height() / 2);
  }

  display.startWrite();

  delay(1000);
}

void loop()
{
  static bool drawed = false;
  lgfx::touch_point_t tp[3];

  uint8_t nums = display.getTouchRaw(tp, 3);

  if (nums)
  {
    //display.setFont(&fonts::Font0);
    display.convertRawXY(tp, nums);

    for (uint8_t i = 0; i < nums; ++i)
    {
      //display.setCursor(0, 0);
      // print X, Y and location(L = i*width + j)
      //display.printf("Convert X:%03d  Y:%03d L:%03d", tp[i].x, tp[i].y, (tp[i].y * 320 + tp[i].x));

      // ensure no duplication of touch points
      if ((tp[i].y * 320 + tp[i].x) != prev && counter < 255)
      {
        inputBuff[counter] = tp[i].y * 320 + tp[i].x;
        prev = (tp[i].y * 320 + tp[i].x);
        counter++;
      }
    }
    //display.display();

    // draw the points on the screen
    display.setColor(TFT_GREEN);
    for (uint8_t i = 0; i < nums; ++i)
    {
      uint8_t s = tp[i].size + 4;
      switch (tp[i].id)
      {
        case 0:
          display.fillCircle(tp[i].x, tp[i].y, s);
          break;
        case 1:
          display.drawLine(tp[i].x - s, tp[i].y - s, tp[i].x + s, tp[i].y + s);
          display.drawLine(tp[i].x - s, tp[i].y + s, tp[i].x + s, tp[i].y - s);
          break;
        default:
          break;
      }
      display.display();
    }
    drawed = true;
  }

  // clear the screen and go to default state once the sample is drawn
  else if (drawed)
  {
    drawed = false;

    display.waitDisplay();
    display.clear();
    display.display();

    // counter threshold ensures that the buffer is not empty or any accidental screen touch/taps
    if (neuton_model_set_inputs(inputBuff) == 0 && counter > 5)
    {
      uint16_t index;
      float* outputs;

      if (neuton_model_run_inference(&index, &outputs) == 0)
      {
        // code for handling prediction result
        // Serial.println("Prediction: " + String(index));
        display.setFont(&fonts::Font4);



        if (index == 0)
        {
          Serial.println("0");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Zero : 0", display.width() / 2, display.height() / 2);
        }
        if (index == 1)
        {
          Serial.println("1");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("One : 1", display.width() / 2, display.height() / 2);
        }
        if (index == 2)
        {
          Serial.println("2");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Two : 2", display.width() / 2, display.height() / 2);
        }
        if (index == 3)
        {
          Serial.println("3");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Three : 3", display.width() / 2, display.height() / 2);
        }
        if (index == 4)
        {
          Serial.println("4");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Four : 4", display.width() / 2, display.height() / 2);
        }
        if (index == 5)
        {
          Serial.println("5");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Five : 5", display.width() / 2, display.height() / 2);
        }
        if (index == 6)
        {
          Serial.println("6");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Six : 6", display.width() / 2, display.height() / 2);
        }
        if (index == 7)
        {
          Serial.println("7");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Seven : 7", display.width() / 2, display.height() / 2);
        }
        if (index == 8)
        {
          Serial.println("8");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Eight : 8", display.width() / 2, display.height() / 2);
        }
        if (index == 9)
        {
          Serial.println("9");
          display.setTextDatum(textdatum_t::middle_center);
          display.drawString("Nine : 9", display.width() / 2, display.height() / 2);
        }
      }
    }

    counter = 0;
    prev = 0;

    neuton_model_reset_inputs();
    free(inputBuff); // free the memory
    inputBuff = (float*) calloc(touchBufferSize, sizeof(float));

    if (!inputBuff)
    {
      Serial.println("Failed to allocate memory");
    }

    vTaskDelay(2000);
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("Ready :)", display.width() / 2, display.height() / 2);

    display.waitDisplay();
    display.clear();
    display.display();
  }
  vTaskDelay(1);
}
