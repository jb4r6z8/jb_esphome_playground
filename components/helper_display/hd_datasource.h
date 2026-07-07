#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

static const uint16_t data_size = 512;

class HDDatasource {
  public:
    void set_entity(std::string entity);

  protected:
    bool initialized_;
    std::string entity_;
    int32_t granularity_;
    int32_t *data_ = (int32_t *) heap_caps_malloc(data_size * sizeof(int32_t), MALLOC_CAP_SPIRAM);


};

}  // namespace helper_display
}  // namespace esphome