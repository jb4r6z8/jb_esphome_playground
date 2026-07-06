#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

class HelperDisplay : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void adddata();
 protected:
  std::vector<int32_t *> data_;
  //std::vector<HD_DataSource> hdds_;
  HD_DataSource hdds_;
  



};


class HD_DataSource {
  protected:
    std::string entity_;
};



}  // namespace helper_display
}  // namespace esphome