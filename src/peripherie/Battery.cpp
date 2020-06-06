/*************************************************** 
    Copyright (C) 2016  Steffen Ochs
    Copyright (C) 2019  Martin Koerner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    HISTORY: Please refer Github History

    SOURCE: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/adc.html
    
****************************************************/
#include "Battery.h"
#include "system/SystemBase.h"
#include "display/DisplayBase.h"
#include "Settings.h"

#if defined(ESP8266)
#elif defined(ESP32)
#include <esp_adc_cal.h>
#else
	#error "Only for ESP8266 or ESP32"
#endif

#define BATTERY_ADC_IO 39u
#define BATTERTY_CHARGE_IO 35u
#define BATTERY_USB_IO 34u
#define MEDIAN_SIZE 30u
#define PERCENTAGE_UPDATE_CYCLE 30u // 30 seconds
#define BATTMIN 3550        // MINIMUM BATTERY VOLTAGE in mV
#define BATTMAX 4180        // MAXIMUM BATTERY VOLTAGE in mV
#define BATTDIV 5.7F        // VOLTAGE DIVIDER
#define CORRECTIONTIME 60000
#define BATTERYSTARTUP 20000
#define REF_VOLTAGE_DEFAULT 1120

Battery::Battery()
{
  pinMode(BATTERTY_CHARGE_IO, INPUT);
  updatePowerMode();
}

void Battery::update()
{
}

void Battery::updatePowerPercentage()
{
}

void Battery::updatePowerMode()
{
}

void Battery::setReference()
{
  switch (this->powerMode)
  {
  case PowerMode::Charging:
    break;
  case PowerMode::USB:
  case PowerMode::Standby:
    break;
  }
}

void Battery::saveConfig()
{
  DynamicJsonBuffer jsonBuffer(Settings::jsonBufferSize);
  JsonObject &json = jsonBuffer.createObject();
  json["batmax"]  = this->max;
  json["batmin"]  = this->min;
  json["batfull"] = this->setreference;
  json["batref"]  = this->refvoltage;
  Settings::write(kBattery, json);
}

void Battery::loadConfig()
{
  DynamicJsonBuffer jsonBuffer(Settings::jsonBufferSize);
  JsonObject &json = Settings::read(kBattery, &jsonBuffer);

  if (json.success())
  {
    if (json.containsKey("batmax"))
      this->max = json["batmax"];
    if (json.containsKey("batmin"))
      this->min = json["batmin"];
    if (json.containsKey("batfull"))
      this->setreference = json["batfull"];    
    if (json.containsKey("batref"))
      this->refvoltage = json["batref"];
  }
}