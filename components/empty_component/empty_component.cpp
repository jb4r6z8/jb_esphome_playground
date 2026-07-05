#include "esphome/core/log.h"
#include "empty_component.h"

namespace esphome {
namespace empty_component {

static const char *TAG = "empty_component.component";

void EmptyComponent::setup() {

}

void EmptyComponent::loop() {

}

void EmptyComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty component");
}

void EmptyComponent::adddata(){
  int32_t *arr = (int32_t *) heap_caps_malloc(1024 * 256 * sizeof(int32_t), MALLOC_CAP_SPIRAM);
  for (int i = 0; i < 1024 * 256; i++) {
    arr[i] = i * 2;
  }
    
  data_.push_back(arr);
}


}  // namespace empty_component
}  // namespace esphome
