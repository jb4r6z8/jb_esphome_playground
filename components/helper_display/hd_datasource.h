#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {


class HDDatasource {
  public:
    void set_entity(std::string entity);

  protected:
    bool initialized_;
    std::string entity_;
    int32_t granularity_;
    int32_t *arr_ = (int32_t *) heap_caps_malloc(1024 * 256 * sizeof(int32_t), MALLOC_CAP_SPIRAM);


};

}  // namespace helper_display
}  // namespace esphome