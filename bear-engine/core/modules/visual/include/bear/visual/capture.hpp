#pragma once

#include "bear/visual/class_export.hpp"

#include <claw/image.hpp>

#include <boost/function.hpp>
#include <boost/signals2/connection.hpp>

namespace bear
{
  namespace visual
  {
    class base_capture;
    
    class VISUAL_EXPORT capture
    {
    public:
      typedef
        boost::function< void( const claw::graphic::image& ) >
        capture_ready;
      typedef boost::function< void( double ) > capture_progress;

    public:
      capture();
      capture( const base_capture& c );
      capture( const capture& that );
      capture( capture&& that );
      ~capture();

      capture& operator=( const capture& that );
      capture& operator=( capture&& that );

      boost::signals2::connection render
        ( const capture_ready& ready, const capture_progress& progress );

    private:
      base_capture* m_impl;

    };
  }
}
