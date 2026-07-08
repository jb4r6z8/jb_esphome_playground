#include "esphome/core/log.h"
#include "hd_chartseries.h"

namespace esphome {
namespace helper_display {

HDChartSeries::HDChartSeries() {
}

std::string HDChartSeries::get_entity() {
    return entity_;
}

uint16_t HDChartSeries::get_granularity() {
    return granularity_;
}


}  // namespace helper_display
}  // namespace esphome    