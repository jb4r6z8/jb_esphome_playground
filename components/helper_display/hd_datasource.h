#pragma once

#include <ArduinoJson.h>
#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

static const uint16_t data_size = 512;

class HDDatasource {
  public:
    
    HDDatasource(std::string entity, uint16_t granularity);
    HDDatasource();
    std::string get_entity();
    uint16_t get_granularity();
    void update(int32_t value, bool force_append);
    void init_by_json(JsonObjectConst json);

  protected:
    std::string entity_ = "";
    uint16_t granularity_ = 0;
    int32_t *data_ = (int32_t *) heap_caps_malloc(data_size * sizeof(int32_t), MALLOC_CAP_SPIRAM);
    uint16_t ptr_ = 0;
    int32_t ts_ = 0;

};

}  // namespace helper_display
}  // namespace esphome