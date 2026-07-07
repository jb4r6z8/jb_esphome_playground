#include "esphome/core/log.h"
#include "helper_display.h"
#include "hd_datasource.cpp"

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
  int32_t *arr = (int32_t *) heap_caps_malloc(1024 * sizeof(int32_t), MALLOC_CAP_SPIRAM);
  for (int i = 0; i < 1024; i++) {
    arr[i] = i * 2;
  }
    
  data_.push_back(arr);
  ESP_LOGD("JBMEM","0,0: %i",data_[0][0]);
  std::string v_str = "Test2";
  HDDatasource v_hdds("Test",2);
  v_hdds.update(1, true);
  v_hdds.update(2, true);
  v_hdds.update(3, true);
  v_hdds.update(4, false);
  v_hdds.update(5);
  hdds_.push_back(v_hdds);
  
}



}  // namespace helper_display
}  // namespace esphome
