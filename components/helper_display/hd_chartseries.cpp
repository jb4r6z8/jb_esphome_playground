#include "esphome/core/log.h"
#include "hd_chartseries.h"

namespace esphome {
namespace helper_display {

HDChartSeries::HDChartSeries(std::string series, HDChartSeriesType seriestype, std::string entity,
              uint16_t granularity, int32_t * data) {
    series_ = series;
    seriestype_ = seriestype;
    entity_ = entity;
    granularity_ = granularity;
    data_ = data;
}

HDChartSeries::HDChartSeries() : HDChartSeries::HDChartSeries("",HDChartSeriesType::STANDARD,"",0,nullptr) {
}

std::string HDChartSeries::get_entity() {
    return entity_;
}

uint16_t HDChartSeries::get_granularity() {
    return granularity_;
}


}  // namespace helper_display
}  // namespace esphome    