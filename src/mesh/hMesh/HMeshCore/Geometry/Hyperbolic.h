/*!
*      \file Hyperbolic.h
*      \brief Hyperbolic Geometric Functions
*	   \author David Gu
*      \date 11/14/2010
*
*/

#ifndef _HMESHLIB_HYPERBOLIC_H_
#define _HMESHLIB_HYPEROBLIC_H_
#include <cmath>

namespace HMeshLib{

inline double _sinh( double x )
{
	double e = std::exp( x );
	return (e - 1.0/e)/2.0;
};

inline double _cosh( double x )
{
	double e = std::exp( x );
	return (e + 1.0/e)/2.0;
};

inline double _tanh( double x )
{
	double e = std::exp( x );
	return ( e-1.0/e )/( e + 1.0/e );
};

inline double _asinh( double x )
{
	return std::log( x + std::sqrt( 1 + x*x));
};

inline double _acosh( double x )
{
	return 2 * std::log (std::sqrt((x+1)/2) + std::sqrt((x-1)/2));
};

inline double _atanh( double x )
{
	return (std::log  (1+x) - std::log (1-x))/2;
};

//Calculate corner angle
inline double _hyperbolic_cosine_law( double a, double b, double c )
{
	double C;
	C = std::acos( (std::cosh(a) * std::cosh(b)-std::cosh(c))/(std::sinh(a)*std::sinh(b)) );
	return C;
};


inline double _hyperbolic_inverse_cosine_law( double a, double b, double C )
{
  double c;
  c = std::cosh(a) * std::cosh(b) - std::sinh(a)*std::sinh(b) * std::cos(C) ;
  c = std::log( c+std::sqrt(c*c-1) );
  return c;
}


}; //namespace

#endif
