#include <chrono>
#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

HDDatasource::HDDatasource(std::string entity, uint16_t granularity) {
  entity_ = entity;
  granularity_ = granularity;
  for (int i = 0; i < data_size; i++) {
    data_[i] = 0;
  }
}

HDDatasource::HDDatasource() : HDDatasource("", 0) {
}

std::string HDDatasource::get_entity() {
    return entity_;
}

uint16_t HDDatasource::get_granularity() {
    return granularity_;
}

void HDDatasource::update(int32_t value, bool force_append = false ) {
    auto now = std::chrono::system_clock::now();
    auto now_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    if (force_append or ts_ + granularity_ < now_ts ) {
      ptr_ = ( ptr_ + 1 ) % data_size;
    }
    ts_ = now_ts - (now_ts % granularity_);
    data_[ptr_] = value;
    ESP_LOGD("JBDS","PTR: %i value: %i", ptr_, data_[ptr_]);
}

void HDDatasource::init_by_json(JsonObjectConst json) {
  ptr_ = 0;
  for (int i = 0; i < data_size; i++) {
    data_[i] = 0;
  }
}


}  // namespace helper_display
}  // namespace esphome    