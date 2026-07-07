#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

HDDatasource::HDDatasource() {
  initialized_ = false;
  entity_ = "";
  granularity_ = 0;
  data_ptr_ = 0;
}

HDDatasource::HDDatasource(std::string entity, uint16_t granularity) {
  HDDatasource();
  entity_ = entity;
  granularity_ = granularity;
}


void HDDatasource::set_entity(std::string entity) {
    ESP_LOGD("JBDS","entity set");
    entity_ = entity;

}


}  // namespace helper_display
}  // namespace esphome    