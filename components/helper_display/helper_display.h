#pragma once

#include <map>
#include <ArduinoJson.h>
#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

static const uint16_t data_size = 512;

class HDDatasource {
  public:
    
    HDDatasource(std::string entity, uint16_t granularity);
    HDDatasource();
    std::string get_entity();
    uint16_t get_granularity();
    void update(int32_t value, bool force_append);
    void init_by_json(JsonObjectConst json);

  protected:
    std::string entity_ = "";
    uint16_t granularity_ = 0;
    int32_t *data_ = (int32_t *) heap_caps_malloc(data_size * sizeof(int32_t), MALLOC_CAP_SPIRAM);
    uint16_t ptr_ = 0;
    int32_t ts_ = 0;

};


enum class HDChartSeriesType : uint8_t {
  DISABLED = 0x00,
  STANDARD = 0x01,
}; 

class HDChartSeries {
  public:
    
  HDChartSeries(std::string series, HDChartSeriesType seriestype, std::string entity, uint16_t granularity, int32_t * data);
  HDChartSeries(std::string series, int32_t * data);
  HDChartSeries();

  void update_settings(HDChartSeriesType seriestype, std::string entity, uint16_t granularity);
  
  std::string get_series();
  HDChartSeriesType get_seriestype();
  std::string get_entity();
  uint16_t get_granularity();
  uint16_t get_data_size();


  void test();
    

  protected:
    std::string series_ = "";
    HDChartSeriesType seriestype_ = HDChartSeriesType::STANDARD;
    std::string entity_ = "";
    uint16_t granularity_ = 0;
    int32_t * data_ ;
    uint16_t data_size_ = 0;

};



class HelperDisplay : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  bool ds_exist(std::string entity, uint16_t granularity);
  void ds_register(std::string entity, uint16_t granularity);
  void ds_update(std::string entity, uint16_t granularity, int32_t value, bool force_append);
  void ds_update_current(std::string entity, int32_t value);
  void ds_init_by_json(JsonObjectConst json);

  void cs_register(std::string series, int32_t * data );
  void cs_update_settings(std::string series, HDChartSeriesType seriestype, std::string entity, uint16_t granularity);

  void adddata();
  void test();



 protected:
  std::map<std::string,std::map<uint16_t,HDDatasource*>> hdds_;
  std::map<std::string,HDChartSeries*> hdcs_;
  




};



}  // namespace helper_display
}  // namespace esphome