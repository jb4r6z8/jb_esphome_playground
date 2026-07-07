#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

static const uint16_t data_size = 512;

class HDDatasource {
  public:
    
    HDDatasource();
    HDDatasource(std::string entity, uint16_t granularity);
    void set_entity(std::string entity);

  protected:
    bool initialized_;
    std::string entity_;
    uint16_t granularity_;
    int32_t *data_ = (int32_t *) heap_caps_malloc(data_size * sizeof(int32_t), MALLOC_CAP_SPIRAM);
    uint16_t data_ptr_;
    int32_t ts;

};

}  // namespace helper_display
}  // namespace esphome