#include <chrono>
#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

HDDatasource::HDDatasource() {
  initialized_ = false;
  entity_ = "";
  granularity_ = 0;
  data_ptr_ = 0;
  for (int i = 0; i < data_size; i++) {
    data_[i] = 0;
  }
}

HDDatasource::HDDatasource(std::string entity, uint16_t granularity) {
  HDDatasource();
  entity_ = entity;
  granularity_ = granularity;
}


void HDDatasource::set_entity(std::string entity) {
    ESP_LOGD("JBDS","entity set");
    auto unix_timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    ESP_LOGD("JBDS","TS: i%", unix_timestamp);

    
    entity_ = entity;


}


}  // namespace helper_display
}  // namespace esphome    