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
  std::vector<std::array<int32_t,1024*256>> data_;
    

};


}  // namespace empty_component
}  // namespace esphome
