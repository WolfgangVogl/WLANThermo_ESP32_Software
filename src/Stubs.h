#include "Arduino.h"

typedef uint32_t TickType_t;
#define xTaskGetTickCount() millis()
#define vTaskDelayUntil(a, b) delay(b)
#define xTaskCreatePinnedToCore(a, b, c, d, e, f, g) millis()
#define vTaskDelete(a) millis()
#define dacWrite(a, b) millis()
#define ledcDetachPin(a) millis()
#define ledcSetup(a, b, c) millis()
#define ledcAttachPin(a, b) millis()
#define ledcWrite(a, b) millis()
#define ledcRead(a) millis()
#define ledcWriteTone(a, b) millis()
#define esp_timer_get_time() micros()