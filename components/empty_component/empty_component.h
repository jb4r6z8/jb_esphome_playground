#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace empty_component {

class EmptyComponent : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void adddata();
 protected:
  std::vector<std::vector<int32_t>> data_ heap_caps_malloc(100000, MALLOC_CAP_SPIRAM);

};


}  // namespace empty_component
}  // namespace esphome
