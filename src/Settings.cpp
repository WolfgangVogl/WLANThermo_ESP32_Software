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

#include "Settings.h"

#define STRINGIFY(s) #s

typedef struct
{
  const char *keyName;
  boolean exportEnabled;
} NvsKeyConfig_t;

static const NvsKeyConfig_t NvsKeyConfig[] =
        {{STRINGIFY(kWifi),       false},
        {STRINGIFY(kMqtt),        false},
        {STRINGIFY(kCloud),       false},
        {STRINGIFY(kSystem),      true},
        {STRINGIFY(kChannels),    true},
        {STRINGIFY(kPitmasters),  true},
        {STRINGIFY(kPush),        false},
        {STRINGIFY(kDisplay),     true},
        {STRINGIFY(kBattery),     true},
        {STRINGIFY(kOtaUpdate),   true},
        {STRINGIFY(kServer),      true}};

const char *Settings::nvsNamespace = "wlanthermo";
const uint16_t Settings::jsonBufferSize = 3072u;
std::vector<SettingsOnChangeCallback> Settings::registeredCallbacks;

Settings::Settings()
{
}

void Settings::write(SettingsNvsKeys key, JsonObject &json)
{
}

JsonObject &Settings::read(SettingsNvsKeys key, DynamicJsonBuffer *jsonBuffer)
{
  JsonObject &json = jsonBuffer->parseObject("");
  return json;
}

String Settings::exportFile()
{
  String exportString = "";

  return exportString;
}

void Settings::write(String key, String value)
{
}

void Settings::remove(SettingsNvsKeys key)
{
}

void Settings::remove(String key)
{
}

void Settings::clear()
{
}

void Settings::onWrite(SettingsOnChangeCallback cb)
{
  registeredCallbacks.push_back(cb);
}
