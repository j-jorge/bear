#pragma once

#include "visual/class_export.hpp"

#include <claw/image.hpp>

#include <boost/function.hpp>
#include <boost/signals2/connection.hpp>

namespace bear
{
  namespace visual
  {
    class VISUAL_EXPORT base_capture
    {
    public:
      typedef
        boost::function< void( const claw::graphic::image& ) >
        capture_ready;

    public:
      base_capture() {}
      virtual base_capture* clone() const = 0;
      
      virtual ~base_capture() {}
      virtual boost::signals2::connection render( const capture_ready& f ) = 0;
    };
  }
}
