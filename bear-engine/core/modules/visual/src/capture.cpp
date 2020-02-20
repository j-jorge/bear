#include "bear/visual/capture.hpp"

#include "bear/visual/base_capture.hpp"

bear::visual::capture::capture()
  : m_impl( nullptr )
{

}
    
bear::visual::capture::capture( const base_capture& c )
  : m_impl( c.clone() )
{

}

bear::visual::capture::capture( const capture& that )
  : m_impl( that.m_impl->clone() )
{

}

bear::visual::capture::capture( capture&& that )
  : m_impl( that.m_impl )
{
  that.m_impl = nullptr;
}

bear::visual::capture::~capture()
{
  delete m_impl;
}

bear::visual::capture& bear::visual::capture::operator=( const capture& that )
{
  if ( this == &that )
    return *this;

  delete m_impl;

  if ( that.m_impl == nullptr )
    m_impl = nullptr;
  else
    m_impl = that.m_impl->clone();

  return *this;
}

bear::visual::capture& bear::visual::capture::operator=( capture&& that )
{
  std::swap( m_impl, that.m_impl );
  return *this;
}

boost::signals2::connection
bear::visual::capture::render
( const capture_ready& ready, const capture_progress& progress )
{
  assert( m_impl != nullptr );
  return m_impl->render( ready, progress );
}

