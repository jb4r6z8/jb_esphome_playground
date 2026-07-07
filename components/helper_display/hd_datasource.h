#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {


class HD_Datasource {
  public:
    void set_entity(std::string entity);

  protected:
    std::string entity_;
};

}  // namespace helper_display
}  // namespace esphome