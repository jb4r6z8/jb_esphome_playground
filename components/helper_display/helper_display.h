#include <map>
#include "esphome/core/component.h"
#include "hd_datasource.h"

namespace esphome {
namespace helper_display {

class HelperDisplay : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  bool ds_exist(std::string entity, uint16_t granularity);
  void ds_register(std::string entity, uint16_t granularity);
  void ds_update(std::string entity, uint16_t granularity, int32_t value, bool force_append);
  void ds_update_current(std::string entity, int32_t value);

  void adddata();
 protected:
  std::map<std::string,std::map<uint16_t,HDDatasource>> hdds_;
  




};



}  // namespace helper_display
}  // namespace esphome