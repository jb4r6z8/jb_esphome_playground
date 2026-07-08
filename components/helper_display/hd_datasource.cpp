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
  auto now = std::chrono::system_clock::now();
  auto now_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  ts_ = now_ts - (now_ts % granularity_);
  ptr_ = 0;
  for (int i = 0; i < data_size; i++) {
    data_[i] = 0;
  }
  JsonArrayConst v_result = json["response"]["result"].as<JsonArrayConst>();
  if (!v_result.isNull()) {
    size_t v_result_rows = v_result.size();
    int v_result_i = v_result_rows;
    int v_seq_i = json["response"]["rows"].as<int>();
    int v_value_i = 0;
    ptr_ = v_seq_i - 1;
    for (int r = v_seq_i - 1; r > 0; r--) {
      if (v_result_i > 0 and r < v_seq_i ) {
        v_result_i--;
        v_seq_i = v_result[v_result_i]["seq"].as<int>();
        v_value_i = round(v_result[v_result_i]["state_last"].as<float>());
        ESP_LOGD("JBDSI", "Seq: %i Value: %i", v_seq_i, v_value_i);
      }
      data_[r] = v_value_i;
    }
  }
}


}  // namespace helper_display
}  // namespace esphome    