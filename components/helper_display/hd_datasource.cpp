#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

void HDDatasource::set_entity(std::string entity) {
    ESP_LOGD("JBDS","entity set");
    entity_ = entity;
    for (int i = 0; i < 1024 * 256; i++) {
        arr_[i] = i * 2;
    }

}


}  // namespace helper_display
}  // namespace esphome    