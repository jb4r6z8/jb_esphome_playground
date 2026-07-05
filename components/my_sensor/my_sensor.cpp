#include "esphome.h"

namespace esphome {
namespace my_sensor {

class MySensor : public PollingComponent, public sensor::Sensor {
 public:
  MySensor() : PollingComponent(60000) {} // default 60s

  void setup() override {
    // Initialization code here
  }

  void update() override {
    // Example: generate a fake temperature value
    float temperature = 20.0 + (rand() % 100) / 10.0;
    publish_state(temperature);
  }
};

}  // namespace my_sensor
}  // namespace esphome
