#include <chrono>
#include "esphome/core/log.h"
#include "helper_display.h"

namespace esphome {
namespace helper_display {

static const char *TAG = "helper_display.component";

///////////HDDataSource
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

///////////HDChartSeries
HDChartSeries::HDChartSeries(std::string series, HDChartSeriesType seriestype, std::string entity,
                             uint16_t granularity, int32_t * data) {
    series_ = series;
    seriestype_ = seriestype;
    entity_ = entity;
    granularity_ = granularity;
    data_ = data;
}

HDChartSeries::HDChartSeries(std::string series, int32_t * data) : HDChartSeries::HDChartSeries(series,HDChartSeriesType::DISABLED,"",0,data) {
}

HDChartSeries::HDChartSeries() : HDChartSeries::HDChartSeries("",HDChartSeriesType::DISABLED,"",0,nullptr) {
}

std::string HDChartSeries::get_entity() {
    return entity_;
}

uint16_t HDChartSeries::get_granularity() {
    return granularity_;
}

void HDChartSeries::test() {
  data_[0] = 0;
  data_[1] = 10;
  data_[2] = 20;
  data_[3] = 30;

}
///////////HekperDisplay
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
    hdds_[entity][granularity] = new HDDatasource(entity,granularity);
  }
  else {
    if ( !hdds_[entity].contains(granularity) ) {
      hdds_[entity][granularity] = new HDDatasource(entity,granularity);
    }
  }
}

void HelperDisplay::ds_update(std::string entity, uint16_t granularity, int32_t value, bool force_append = false) {
  ds_register(entity,granularity);
  hdds_[entity][granularity]->update(value, force_append);
}

void HelperDisplay::ds_update_current(std::string entity, int32_t value) {
  if (hdds_.contains(entity)) {
    for (const auto& [granulariy, ds] : hdds_[entity]) {
      ds->update(value);
    }
  }
}

void HelperDisplay::ds_init_by_json(JsonObjectConst json) {
  std::string entity = json["response"]["entity"];
  uint16_t granularity = json["response"]["seconds"];
  ds_register(entity,granularity);
  hdds_[entity][granularity]->init_by_json(json);

}

void HelperDisplay::cs_register(std::string series, int32_t * data ) {
  if (hdcs_.contains(series)) {

  }
  else {
    
    hdcs_[series] = new HDChartSeries(series, data);
  }

}

void HelperDisplay::test() {
  hdcs_["lv_page001tl_chart001_series001"]->test();
}


void HelperDisplay::adddata(){
  HDDatasource v_hdds("Test",2);
  v_hdds.update(1, true);
  v_hdds.update(2, true);
  v_hdds.update(3, true);
  v_hdds.update(4, false);
  v_hdds.update(5);
  hdds_[v_hdds.get_entity()][v_hdds.get_granularity()] = &v_hdds;
  
}



}  // namespace helper_display
}  // namespace esphome
