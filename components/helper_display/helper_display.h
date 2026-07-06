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
  //HD_Datasource hdds_;
  
class ClassB {
public:
};


class ClassA {
public:
    helper_display::ClassB obj;
};




};



}  // namespace helper_display
}  // namespace esphome