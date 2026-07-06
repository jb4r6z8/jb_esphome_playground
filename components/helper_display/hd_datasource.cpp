#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

void HD_DataSource::set_entity() {
    ESP_LOGD("JBDS","entity set");
    entity_ = "Test";
}


}  // namespace helper_display
}  // namespace esphome    