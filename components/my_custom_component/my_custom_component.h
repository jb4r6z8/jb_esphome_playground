#include "esphome.h"

namespace esphome {
namespace my_custom_component {

class MyCustomSensor : public PollingComponent, public sensor::Sensor {
 public:
  MyCustomSensor() : PollingComponent(5000) {} // default 5s

  void setup() override {
    // Initialization code here
  }

  void update() override {
    // Example: generate a fake temperature value
    float temperature = 20.0 + (rand() % 100) / 10.0f;
    publish_state(temperature);
  }
};

}  // namespace my_custom_component
}  // namespace esphome
