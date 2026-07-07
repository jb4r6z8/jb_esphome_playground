#include "esphome/core/component.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

class HD_Datasource;

class HelperDisplay : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void adddata();
 protected:
  std::vector<int32_t *> data_;
  //std::vector<HD_DataSource> hdds_;
  helper_display::HD_Datasource hdds_;
  




};



}  // namespace helper_display
}  // namespace esphome