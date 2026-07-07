#include <chrono>
#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

HDDatasource::HDDatasource() {
  initialized_ = false;
  entity_ = "";
  granularity_ = 0;
  ptr_ = 0;
  for (int i = 0; i < data_size; i++) {
    data_[i] = 0;
  }
}

HDDatasource::HDDatasource(std::string entity, uint16_t granularity) {
  HDDatasource();
  entity_ = entity;
  granularity_ = granularity;
}

void HDDatasource::update(int32_t value, bool force_append = false ) {
    if (force_append) {
      ptr_ = ( ptr_ + 1 ) % data_size;
    } else {
      auto now = std::chrono::system_clock::now();
      auto now_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
      if ( ts_ + granularity_ < now_ts ) {
        ts_ = now_ts - (now_ts % granularity_);
        ptr_ = ( ptr_ + 1 ) % data_size;
      }
    }
    data_[ptr_] = value;
    ESP_LOGD("JBDS","PTR: %i value: %i", ptr_, data_[ptr_]);
}


void HDDatasource::set_entity(std::string entity) {
    ESP_LOGD("JBDS","entity set");
    auto now = std::chrono::system_clock::now();
    auto unix_timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    ESP_LOGD("JBDS","TS: %i", unix_timestamp);

    
    entity_ = entity;


}


}  // namespace helper_display
}  // namespace esphome    