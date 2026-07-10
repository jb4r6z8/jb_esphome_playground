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

int32_t HDDatasource::get_data_point_by_offset(int32_t offset) {
  return data_[ ( ptr_ - offset ) % data_size ];
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

std::string HDChartSeries::get_entity() {
    return entity_;
}

uint16_t HDChartSeries::get_granularity() {
    return granularity_;
}

uint32_t HDChartSeries::get_data_size() {
  return data_size_;
}

void HDChartSeries::update_settings(HDChartSeriesType seriestype, std::string entity, uint16_t granularity) {
  seriestype_=seriestype;
  entity_ = entity;
  granularity_ = granularity_;
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
        ESP_LOGD("JB", "Chart Update");
        if (hdcs_[series]->get_data_size() > 0) {
          ESP_LOGD("JB", "Chart Update, CS Data Size valid");
          ESP_LOGD("JB", "Chart Update, CS Data Size valid: entity: %s", hdcs_[series]->get_entity().c_str());
          if ( hdds_.contains(hdcs_[series]->get_entity()) ) {
            ESP_LOGD("JB", "Chart Update, Entity exists");
          }
          
          if (hdds_.contains(hdcs_[series]->get_entity()) and hdds_[hdcs_[series]->get_entity()].contains(hdcs_[series]->get_granularity())) {
            ESP_LOGD("JB", "Chart Update, Datasource Found");
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

}  // namespace helper_display
}  // namespace esphome
