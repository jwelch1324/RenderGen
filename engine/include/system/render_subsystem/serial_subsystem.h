#ifndef __SERIAL_SUBSYSTEM_H__
#define __SERIAL_SUBSYSTEM_H__

#include "geometry/point.h"
#include "system/render_subsystem/render_subsystem.h"
namespace rengen::system::render {
class SerialRenderer : public RenderSubSystem {
public:
  SerialRenderer();
  virtual ~SerialRenderer();

  geometry::RGBColor RenderRay(geometry::Ray &ray) const override;
};
} // namespace rengen::system::render
#endif // __SERIAL_SUBSYSTEM_H__