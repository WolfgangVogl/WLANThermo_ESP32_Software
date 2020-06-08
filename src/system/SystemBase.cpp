/*************************************************** 
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
    
****************************************************/

#include <SPI.h>

#if defined(ESP8266)
  #include "Stubs.h"
  #include <ESP8266WiFi.h> 
  // #include <spiffs/spiffs.h> // implicity included... avoid typedef conflict
  //#include <rtctime.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <SPIFFS.h>
  #include <rom/rtc.h>
#else
	#error "Only for ESP8266 or ESP32"
#endif

#include "SystemBase.h"
#include "Constants.h"
#include "RecoveryMode.h"

#define STRINGIFY(s) #s

char SystemBase::serialNumber[13] = "";

SystemBase::SystemBase()
{
  buzzer = NULL;
  battery = NULL;
  sdCard = NULL;
  deviceName = "undefined";
  cpuName = "esp32";
  language = "de";
  hardwareVersion = 1u;
  pitmasterProfileCount = 0u;
  powerSaveModeSupport = false;
  powerSaveModeEnabled = false;
  damperSupport = false;
  initDone = false;
}

void SystemBase::init()
{
}

void SystemBase::hwInit()
{
}

void SystemBase::run()
{
  xTaskCreatePinnedToCore(SystemBase::task, "SystemBase::task", 5000, this, 2, NULL, 1);
}

void SystemBase::task(void *parameter)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  SystemBase *system = (SystemBase *)parameter;

  for (;;)
  {
    uint32_t time = esp_timer_get_time();
    system->update();
    time = esp_timer_get_time() - time;
    //printf("Performance: %dus\n", time);

    // Wait for the next cycle.
    vTaskDelayUntil(&xLastWakeTime, 1000);
  }
}

PitmasterProfile *SystemBase::getPitmasterProfile(uint8_t index)
{
  PitmasterProfile *prof = NULL;

  if (index < this->pitmasterProfileCount)
  {
    prof = this->profile[index];
  }

  return prof;
}

uint8_t SystemBase::getPitmasterProfileCount()
{
  return this->pitmasterProfileCount;
}

void SystemBase::update()
{
  boolean buzzerAlarm = false;

  if (battery != NULL)
  {
    battery->update();

    if (battery->requestsStandby())
    {
      esp_sleep_enable_timer_wakeup(10);
      esp_deep_sleep_start();
    }

    if (powerSaveModeSupport)
    {
      setPowerSaveMode(!battery->isUsbPowered());
      pitmasters.enable(battery->isUsbPowered());
    }
  }

  this->wireLock();
  temperatures.update();
  this->wireRelease();
  pitmasters.update();

  for (uint8_t i = 0; i < temperatures.count(); i++)
  {
    if (temperatures[i] != NULL)
    {
      this->notification.check(temperatures[i]);
      AlarmStatus alarmStatus = temperatures[i]->getAlarmStatus();
      AlarmSetting alarmSetting = temperatures[i]->getAlarmSetting();
      boolean acknowledged = temperatures[i]->isAlarmAcknowledged();

      if ((alarmStatus != NoAlarm) && (false == acknowledged) && ((AlarmViaSummer == alarmSetting) || (AlarmAll == alarmSetting)))
        buzzerAlarm = true;
    }
  }

  if (buzzer != NULL)
  {
    if (buzzerAlarm)
      buzzer->enable();
    else
      buzzer->disable();

    buzzer->update();
  }
}

void SystemBase::resetConfig()
{
  for (uint8_t i = 0; i < temperatures.count(); i++)
  {
    if (temperatures[i] != NULL)
    {
      temperatures[i]->loadDefaultValues();
    }
  }

  wlan.setHostName(DEFAULT_HOSTNAME + String(this->serialNumber));
  wlan.setAccessPointName(DEFAULT_APNAME);
}

void SystemBase::saveConfig()
{
}

void SystemBase::loadConfig()
{
  SPIFFS.begin();
  cloud.loadConfig();
  mqtt.loadConfig();
  notification.loadConfig();
  otaUpdate.loadConfig();
}

boolean SystemBase::isInitDone()
{
  return this->initDone;
}

void SystemBase::restart()
{
  WiFi.disconnect();
  delay(500);
  yield();
  RecoveryMode::zeroResetCounter();
  ESP.restart();
}

void SystemBase::wireLock()
{
  // ESP8266 - NOP
}

void SystemBase::wireRelease()
{
  // ESP8266 - NOP
}

String SystemBase::getDeviceName()
{
  return this->deviceName;
}

String SystemBase::getCpuName()
{
  return this->cpuName;
}

size_t SystemBase::getFlashSize()
{
  return ESP.getFlashChipSize();
}

String SystemBase::getSerialNumber()
{
  if (strlen(serialNumber) == 0u)
  {    
// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
    return SystemBase_serialNumber;
    //String(ESP.getChipId(), HEX);
  }

  return serialNumber;
}

String SystemBase::getLanguage()
{
  return this->language;
}

void SystemBase::setLanguage(String language)
{
  this->language = language;
}

uint8_t SystemBase::getHardwareVersion()
{
  return this->hardwareVersion;
}

void SystemBase::setPowerSaveMode(boolean enabled)
{
  if ((enabled == powerSaveModeEnabled) || (false == powerSaveModeSupport))
    return;

// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
}

String SystemBase::getResetReason(uint8_t cpuId)
{
  
// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
// ESP8266 DUMMY - TODO TODO TODO TODO
  return ESP.getResetReason();
  
  /*
  String resetString;

  switch ( reason)
  {
    case NO_MEAN: resetString = STRINGIFY(NO_MEAN); break;
    case POWERON_RESET: resetString = STRINGIFY(POWERON_RESET); break;
    case SW_RESET: resetString = STRINGIFY(SW_RESET); break;
    case OWDT_RESET: resetString = STRINGIFY(OWDT_RESET); break;
    case DEEPSLEEP_RESET: resetString = STRINGIFY(DEEPSLEEP_RESET); break;
    case SDIO_RESET: resetString = STRINGIFY(SDIO_RESET); break;
    case TG0WDT_SYS_RESET: resetString = STRINGIFY(TG0WDT_SYS_RESET); break;
    case TG1WDT_SYS_RESET: resetString = STRINGIFY(TG1WDT_SYS_RESET); break;
    case RTCWDT_SYS_RESET: resetString = STRINGIFY(RTCWDT_SYS_RESET); break;
    case INTRUSION_RESET: resetString = STRINGIFY(INTRUSION_RESET); break;
    case TGWDT_CPU_RESET: resetString = STRINGIFY(TGWDT_CPU_RESET); break;
    case SW_CPU_RESET: resetString = STRINGIFY(SW_CPU_RESET); break;
    case RTCWDT_CPU_RESET: resetString = STRINGIFY(RTCWDT_CPU_RESET); break;
    case EXT_CPU_RESET: resetString = STRINGIFY(EXT_CPU_RESET); break;
    case RTCWDT_BROWN_OUT_RESET: resetString = STRINGIFY(RTCWDT_BROWN_OUT_RESET); break;
    case RTCWDT_RTC_RESET: resetString = STRINGIFY(RTCWDT_RTC_RESET); break;
    default: resetString = "UNKNOWN";
  }

  return resetString;
  */
}

boolean SystemBase::getSupportDamper()
{
  return this->damperSupport;
}