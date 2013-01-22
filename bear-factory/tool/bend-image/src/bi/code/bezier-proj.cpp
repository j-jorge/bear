#include <claw/image.hpp>
#include <claw/png.hpp>
#include <claw/curve.hpp>
#include <claw/string_algorithm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

#include <cassert>

typedef claw::math::vector_2d<double> point_type;
typedef claw::math::curve<point_type> curve;

void help( const std::string& n )
{
  std::cerr << "usage:\n" << n
            << " width height source_image\n"
            << "\nPoints are read from stdin in the following format:\n"
            << "p.x p.y [in.x in.y out.x out.y]"
            << std::endl;
} // help()

typedef std::vector< std::vector<double> > channel_map;

channel_map create_channel_map( std::size_t w, std::size_t h )
{
  return channel_map( h, std::vector<double>( w, 0 ) );
}

curve read_curve( std::istream& is )
{
  curve result;
  std::string line;
  std::size_t line_number(0);

  while ( claw::text::getline( is, line ) )
    {
      ++line_number;
      claw::text::trim( line );

      if ( !line.empty() && (line[0] != '#') )
        {
          std::istringstream iss(line);

          point_type p;

          if ( !(iss >> p.x >> p.y) )
            std::cerr << line_number << ": invalid format" << std::endl;
          else
            {
              point_type in;
              point_type out;

              if ( iss >> in.x >> in.y >> out.x >> out.y )
                result.push_back( curve::control_point(p, in, out) );
              else
                result.push_back( curve::control_point(p) );
            }
        }
    }
      
  return result;
} // read_curve()

int main( int argc, char* argv[])
{
  if ( argc != 4 )
    {
      help(argv[0]);
      return 1;
    }

  std::size_t width, height;
  std::istringstream iss( std::string(argv[1]) + ' ' + argv[2] );
  std::ifstream f(argv[3]);

  if ( !f || !(iss >> width >> height) )
    {
      help(argv[0]);
      return 1;
    }

  curve bezier( read_curve(std::cin) );
  const claw::graphic::image source(f);

  claw::graphic::image dest(width, height);
  std::fill( dest.begin(), dest.end(), claw::graphic::transparent_pixel );

  channel_map red = create_channel_map(width, height);
  channel_map green = create_channel_map(width, height);
  channel_map blue = create_channel_map(width, height);
  channel_map alpha = create_channel_map(width, height);
  channel_map count = create_channel_map(width, height);

  point_type prev_p( bezier.begin()->get_position() );
  double source_distance(0);

  for ( std::size_t x=0; x!=width; ++x )
    {
      const curve::section::resolved_point p = bezier.get_point_at_x( x )[0];
      const point_type dp(p.get_position());
      point_type tangent( p.get_section().get_tangent_at(p.get_date()) );

      const point_type norm( tangent.get_orthonormal_clockwise() );

      source_distance += dp.distance(prev_p);
      prev_p = dp;

      const int sx
        ( (unsigned int)(source_distance + 0.5) % source.width() );

      for ( std::size_t dx(0); dx != source.height(); ++dx )
        for ( std::size_t dy(0); dy != source.height(); ++dy )
          {
            const double dist = point_type(dx, dy).length();

            if ( dist <= source.height() - 1 )
              {
                const int sy(dist + 0.5);
                const int tx( dp.x - dx );
                const int ty( dp.y + dy );
                
                const point_type tp( tx, ty );
                const point_type v( dp, tp );

                const double a =
                  std::acos
                  ( norm.dot_product( v ) / ( v.length() * norm.length() ) );

                double r =
                  (dp == tp) ? 1 :
                  1
                  - point_type(tp, dp).length()
                  * std::abs(std::sin(a)) / source.height();

                r = std::min( 1.0, std::max( 0.0, r ) );

                if ( (tx >= 0) && ((unsigned)tx < width)
                     && (ty >= 0) && ((unsigned)ty < height)
                     && (count[ty][tx] < r) )
                  {
                    const claw::graphic::rgba_pixel pxl( source[sy][sx] );
                    red[ty][tx] = (double)pxl.components.red;
                    green[ty][tx] = (double)pxl.components.green;
                    blue[ty][tx] = (double)pxl.components.blue;
                    alpha[ty][tx] = (double)pxl.components.alpha;
                    count[ty][tx] = r;
                  }
              }
          }
    }

  for ( std::size_t y=0; y!=height; ++y )
    for ( std::size_t x=0; x!=width; ++x )
      {
        const double c( count[y][x] );

        if ( c != 0 )
          {
            dest[y][x].components.red = (unsigned int)(red[y][x]);
            dest[y][x].components.green = (unsigned int)(green[y][x]);
            dest[y][x].components.blue = (unsigned int)(blue[y][x]);
            dest[y][x].components.alpha = (unsigned int)(alpha[y][x]);
          }
      }

  std::ofstream of("output.png");
  claw::graphic::png::writer writer(dest, of);

  return 0;
} // main()
