#pragma once

#include "esphome/core/log.h"
#include "helper_display.h"

namespace esphome {
namespace helper_display {

static const char *TAG = "helper_display.component";

void HelperDisplay::setup() {

}

void HelperDisplay::loop() {

}

void HelperDisplay::dump_config(){
    ESP_LOGCONFIG(TAG, "Helper Display");
}

void HelperDisplay::adddata(){
  int32_t *arr = (int32_t *) heap_caps_malloc(1024 * 256 * sizeof(int32_t), MALLOC_CAP_SPIRAM);
  for (int i = 0; i < 1024 * 256; i++) {
    arr[i] = i * 2;
  }
    
  data_.push_back(arr);
  ESP_LOGD("JBMEM","0,0: %i",data_[0][0]);
  std::string v_str = "Test";
  //HD_Datasource hdds;
  //hdds.set_entity(v_str);
  hdds_.set_entity(v_str);
}



}  // namespace helper_display
}  // namespace esphome
