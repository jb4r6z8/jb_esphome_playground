#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace helper_display {


class HD_Datasource {
  public:
    static void set_entity();

  protected:
    static std::string entity_;
};

}  // namespace helper_display
}  // namespace esphome