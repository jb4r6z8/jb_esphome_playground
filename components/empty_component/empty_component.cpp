#include "esphome/core/log.h"
#include "empty_component.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiram.h"

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
    
    // Check if PSRAM is available
    if (!esp_spiram_is_initialized()) {
        ESP_LOGE(TAG, "PSRAM not initialized!");
        // Depending on the application, you might want to halt or handle this error
        return;
    }
    
  for (uint16_t i = 0; i < 10; i++ ) {
    std::vector<int32_t> v_vector_inside = {1,2,3,4};
    for (uint16_t j = 0; j < 1000; j++ ) {
       v_vector_inside.push_back(j);
    }
    data_.push_back(v_vector_inside);
  }
}


}  // namespace empty_component
}  // namespace esphome
