#pragma once

#include "esphome/core/log.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

void HD_Datasource::set_entity(std::string entity) {
    ESP_LOGD("JBDS","entity set");
    entity_ = entity;
}


}  // namespace helper_display
}  // namespace esphome    