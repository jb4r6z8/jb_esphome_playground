#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {

enum class HDChartSeriesType : uint8_t {
  STANDARD = 0x00,
}; 

class HDChartSeries {
  public:
    
  HDChartSeries(std::string series, HDChartSeriesType seriestype, std::string entity, uint16_t granularity, int32_t * data);
  HDChartSeries();

  std::string get_entity();
  uint16_t get_granularity();
    

  protected:
    std::string series_ = "";
    HDChartSeriesType seriestype_ = HDChartSeriesType::STANDARD;
    std::string entity_ = "";
    uint16_t granularity_ = 0;
    int32_t * data_ ;

};

}  // namespace helper_display
}  // namespace esphome