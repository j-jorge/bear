#pragma once

#include "visual/base_capture.hpp"
#include "visual/gl_state.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    class VISUAL_EXPORT gl_capture:
      public base_capture
    {
    public:
      explicit gl_capture( const std::vector< gl_state >& states );

      gl_capture* clone() const override;
      boost::signals2::connection render( const capture_ready& f ) override;

    private:
      const std::vector< gl_state > m_states;
    };
  }
}
