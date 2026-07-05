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

    size_t alloc_size = 1024; // 1 KB

    // Allocate memory from external PSRAM
    void *ext_ram_ptr = heap_caps_malloc(alloc_size, MALLOC_CAP_SPIRAM);

    if (ext_ram_ptr == NULL) {
        printf("Failed to allocate %zu bytes from external RAM.\n", alloc_size);
        return;
    }

    printf("Successfully allocated %zu bytes from external RAM at address %p\n",
           alloc_size, ext_ram_ptr);

    // Example usage: fill with data
    for (size_t i = 0; i < alloc_size; i++) {
        ((uint8_t *)ext_ram_ptr)[i] = (uint8_t)(i & 0xFF);
    }

    // Verify data
    printf("First byte: %u, Last byte: %u\n",
           ((uint8_t *)ext_ram_ptr)[0],
           ((uint8_t *)ext_ram_ptr)[alloc_size - 1]);

    // Free memory when done
    free(ext_ram_ptr);
    printf("External RAM memory freed.\n");



    
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
