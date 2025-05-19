/*
 * utils.h
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

// power related defines
#define DEF_POWER_SUPPLY 24                 // default power supply voltage
#define DEF_MOT_VOLT_LIM (DEF_POWER_SUPPLY) // default motor voltage limit
#define DEF_DRV_VOLT_LIM (DEF_POWER_SUPPLY) // default driver voltage limit
#define DEF_MOT_CURR_LIM 2                  // default current limit
#define DEF_MOT_VEL_LIM  5                  // default velocity limit

// velocity PID controller params
#define DEF_PID_VEL_P 0.5f                  // default PID controller P value
#define DEF_PID_VEL_I 10.0f                 // default PID controller I value
#define DEF_PID_VEL_D 0.0f                  // default PID controller D value
#define DEF_PID_VEL_RAMP 1000.0f            // default PID controller voltage ramp value
#define DEF_PID_VEL_LIM (DEF_POWER_SUPPLY)  // default PID controller voltage limit

// angle PID controller params
#define DEF_PID_ANG_P 20.0f                 // default angle PID controller P value
#define DEF_PID_ANG_I 0.0f                  // default angle PID controller I value
#define DEF_PID_ANG_D 0.0f                  // default angle PID controller D value
#define DEF_PID_ANG_RAMP 0.0f               // default angle PID controller voltage ramp value
#define DEF_PID_ANG_LIM 20.0f               // angle velocity limit default

// utility defines
#define _2_SQRT3 1.15470053838f
#define _SQRT3 1.73205080757f
#define _1_SQRT3 0.57735026919f
#define _SQRT3_2 0.86602540378f
#define _SQRT2 1.41421356237f
#define _120_D2R 2.09439510239f
#define _PI 3.14159265359f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _2PI 6.28318530718f
#define _3PI_2 4.71238898038f
#define _PI_6 0.52359877559f
#define _RPM_TO_RADS 0.10471975512f

// for input argument parsing (if set or not)
#define NOT_SET -12345.0f

// some functions
#define _sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
#define _round(x) ((x)>=0?(long)((x)+0.5f):(long)((x)-0.5f))
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _sqrt(a) (_sqrtApprox(a))
#define _isset(a) ( (a) != (NOT_SET) )
#define _powtwo(x) (1 << (x))

__attribute__((weak)) float _sin(float a){
  // 16bit integer array for sine lookup. interpolation is used for better precision
  // 16 bit precision on sine value, 8 bit fractional value for interpolation, 6bit LUT size
  // resulting precision compared to stdlib sine is 0.00006480 (RMS difference in range -PI,PI for 3217 steps)
  static uint16_t sine_array[65] = {0,804,1608,2411,3212,4011,4808,5602,6393,7180,7962,8740,9512,10279,11039,11793,12540,13279,14010,14733,15447,16151,16846,17531,18205,18868,19520,20160,20788,21403,22006,22595,23170,23732,24279,24812,25330,25833,26320,26791,27246,27684,28106,28511,28899,29269,29622,29957,30274,30572,30853,31114,31357,31581,31786,31972,32138,32286,32413,32522,32610,32679,32729,32758,32768};
  int32_t t1, t2;
  unsigned int i = (unsigned int)(a * (64*4*256.0f/_2PI));
  int frac = i & 0xff;
  i = (i >> 8) & 0xff;
  if (i < 64) {
    t1 = (int32_t)sine_array[i]; t2 = (int32_t)sine_array[i+1];
  }
  else if(i < 128) {
    t1 = (int32_t)sine_array[128 - i]; t2 = (int32_t)sine_array[127 - i];
  }
  else if(i < 192) {
    t1 = -(int32_t)sine_array[-128 + i]; t2 = -(int32_t)sine_array[-127 + i];
  }
  else {
    t1 = -(int32_t)sine_array[256 - i]; t2 = -(int32_t)sine_array[255 - i];
  }
  return (1.0f/32768.0f) * (t1 + (((t2 - t1) * frac) >> 8));
}

__attribute__((weak)) float _cos(float a){
  float a_sin = a + _PI_2;
  a_sin = a_sin > _2PI ? a_sin - _2PI : a_sin;
  return _sin(a_sin);
}

__attribute__((weak)) void _sincos(float a, float* s, float* c){
  *s = _sin(a);
  *c = _cos(a);
}

__attribute__((weak)) float _sqrtApprox(float number) {//low in fat
  union {
    float    f;
    uint32_t i;
  } y = { .f = number };
  y.i = 0x5f375a86 - ( y.i >> 1 );
  return number * y.f;
}

#endif /* UTILS_UTILS_H_ */
