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
  std::array<std::array<int32_t,90>,${gc_series}> data_;

};


}  // namespace empty_component
}  // namespace esphome
