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
      ts_ = now_ts - (now_ts % granularity_);
    }
    data_[ptr_] = value;
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
      }
      data_[r] = v_value_i;
    }
  }
  if (entity_ == "sensor.pvfrnsac_power" and granularity_ == 120 ) {
    for (int i == 0; i < data_size; i++ ) {
      ESP_LOGD("JB","i: %i value: %i",i, data_[i]);
    }

  }

}

int32_t HDDatasource::get_data_point_by_offset(int32_t offset) {
  return data_[ ( ptr_ + data_size + offset ) % data_size ];
}

///////////HDChartSeries
HDChartSeries::HDChartSeries(std::string series, HDChartSeriesType seriestype, std::string entity,
                             uint16_t granularity, int32_t * data, uint32_t data_size) {
    series_ = series;
    seriestype_ = seriestype;
    entity_ = entity;
    granularity_ = granularity;
    data_ = data;
    data_size_ = data_size;
}

HDChartSeries::HDChartSeries(std::string series, int32_t * data, uint32_t data_size) : HDChartSeries::HDChartSeries(series,HDChartSeriesType::DISABLED,"",0,data, data_size) {
}

HDChartSeries::HDChartSeries() : HDChartSeries::HDChartSeries("",HDChartSeriesType::DISABLED,"",0,nullptr,0) {
}

std::string HDChartSeries::get_series() {
    return series_;
}

HDChartSeriesType HDChartSeries::get_seriestype() {
    return seriestype_;
}

void HDChartSeries::set_seriestype(HDChartSeriesType seriestype) {
    seriestype_ = seriestype;
}

std::string HDChartSeries::get_entity() {
    return entity_;
}

uint16_t HDChartSeries::get_granularity() {
    return granularity_;
}

void HDChartSeries::set_granularity(uint16_t granularity) {
    granularity_ = granularity;
}

uint32_t HDChartSeries::get_data_size() {
  return data_size_;
}

uint32_t HDChartSeries::get_data_min() {
  if (data_size_ == 0) {
    return 0;
  }
  else {
    int32_t data_min = INT32_MAX;
    for (uint32_t i = 0; i < data_size_; i++) {
      if (data_min > data_[i]) {
        data_min = data_[i];
      }
    }
    return data_min;
  }
}

uint32_t HDChartSeries::get_data_max() {
  if (data_size_ == 0) {
    return 0;
  }
  else {
    int32_t data_max = INT32_MIN;
    for (uint32_t i = 0; i < data_size_; i++) {
      if (data_max < data_[i] and data_[i] != INT32_MAX ) {
        data_max = data_[i];
      }
    }
    return data_max;
  }
}


void HDChartSeries::update_settings(HDChartSeriesType seriestype, std::string entity, uint16_t granularity) {
  seriestype_=seriestype;
  entity_ = entity;
  granularity_ = granularity;
}

void HDChartSeries::update_data_point(uint32_t point, int32_t value) {
  if (point < data_size_) {
    data_[point] = value;
  }
}

///////////HelperDisplay
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

void HelperDisplay::cs_register(std::string series, int32_t * data, uint32_t data_size ) {
  if (!hdcs_.contains(series)) {
    hdcs_[series] = new HDChartSeries(series, data, data_size);
  }
}

void HelperDisplay::cs_update_settings(std::string series, HDChartSeriesType seriestype, std::string entity, uint16_t granularity) {
  if (hdcs_.contains(series)) {
    hdcs_[series]->update_settings(seriestype, entity, granularity);
  }
}

HDChartSeriesType HelperDisplay::cs_get_seriestype(std::string series) {
  if (hdcs_.contains(series)) {
    return hdcs_[series]->get_seriestype();
  }
  else {
    return HDChartSeriesType::DISABLED;
  }
}

void HelperDisplay::cs_set_seriestype(std::string series, HDChartSeriesType seriestype) {
  if (hdcs_.contains(series)) {
    hdcs_[series]->set_seriestype(seriestype);
  }
}

uint16_t HelperDisplay::cs_get_granularity(std::string series) {
  if (hdcs_.contains(series)) {
    return hdcs_[series]->get_granularity();
  }
  else {
    return 0;
  }
}

void HelperDisplay::cs_set_granularity(std::string series, uint16_t granularity) {
  if (hdcs_.contains(series)) {
    hdcs_[series]->set_granularity(granularity);
  }
}

void HelperDisplay::cs_update_data(std::string series) {
  if (hdcs_.contains(series)) {
    switch (hdcs_[series]->get_seriestype()) {
      case HDChartSeriesType::DISABLED:
        if (hdcs_[series]->get_data_size() > 0) {
          for (uint32_t i = 0; i < hdcs_[series]->get_data_size(); i++) {
            hdcs_[series]->update_data_point(i,INT32_MAX);
          }
        }
        break;
      case HDChartSeriesType::STANDARD:
        if (hdcs_[series]->get_data_size() > 0) {
          if (hdds_.contains(hdcs_[series]->get_entity()) and hdds_[hdcs_[series]->get_entity()].contains(hdcs_[series]->get_granularity())) {
            for (uint32_t i = 0; i < hdcs_[series]->get_data_size(); i++) {
              int32_t value = hdds_[hdcs_[series]->get_entity()][hdcs_[series]->get_granularity()]->get_data_point_by_offset(i + 1 - hdcs_[series]->get_data_size());
              hdcs_[series]->update_data_point(i, value );
            }
          }
          else {
            for (uint32_t i = 0; i < hdcs_[series]->get_data_size(); i++) {
              hdcs_[series]->update_data_point(i,INT32_MAX);
            }
          }
        }
        break;
    }
  }
}

void HelperDisplay::cs_update_data_all() {
  for (const auto& [series, cs] : hdcs_) {
    cs_update_data(series);
  }
}

int32_t HelperDisplay::cs_get_data_min(std::string series) {
  if (hdcs_.contains(series)) {
    return hdcs_[series]->get_data_min();
  }
  else {
    return INT32_MAX;
  }
}

int32_t HelperDisplay::cs_get_data_max(std::string series) {
  if (hdcs_.contains(series)) {
    return hdcs_[series]->get_data_max();
  }
  else {
    return INT32_MIN;
  }
}


}  // namespace helper_display
}  // namespace esphome
