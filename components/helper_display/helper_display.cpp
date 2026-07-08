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

bool HelperDisplay::ds_exist(std::string entity, uint16_t granularity) {
  if ( !hdds_.contains(entity) ) {
    return false;
  }
  else {
    if ( !hdds_[entity].contains(granularity) ) {
      return false;
    }
  }
  return true;
}

void HelperDisplay::ds_register(std::string entity, uint16_t granularity) {
  if ( !hdds_.contains(entity) ) {
    hdds_[entity][granularity] = HDDatasource(entity,granularity);
  }
  else {
    if ( !hdds_[entity].contains(granularity) ) {
      hdds_[entity][granularity] = HDDatasource(entity,granularity);
    }
  }
}

void HelperDisplay::ds_update(std::string entity, uint16_t granularity, int32_t value, bool force_append = false) {
  ds_register(entity,granularity);
  hdds_[entity][granularity].update(value, force_append);
}

void HelperDisplay::ds_update_current(std::string entity, int32_t value) {
  if (hdds_.contains(entity)) {
    for (const auto& [granulariy, ds] : hdds_[entity]) {
      ds.update(value);
    }
  }
}

void HelperDisplay::adddata(){
  HDDatasource v_hdds("Test",2);
  v_hdds.update(1, true);
  v_hdds.update(2, true);
  v_hdds.update(3, true);
  v_hdds.update(4, false);
  v_hdds.update(5);
  hdds[v_hdds.get_entity()][v_hdds.get_granularity()] = v_hdds;
  
}



}  // namespace helper_display
}  // namespace esphome
