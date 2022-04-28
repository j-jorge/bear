#pragma once

#include "bear/visual/class_export.hpp"

#include <claw/graphic/image.hpp>

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
      typedef boost::function< void( double ) > capture_progress;

    public:
      base_capture() {}
      virtual base_capture* clone() const = 0;
      
      virtual ~base_capture() {}
      virtual boost::signals2::connection render
        ( const capture_ready& ready, const capture_progress& progress ) = 0;
    };
  }
}
