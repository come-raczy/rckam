/**
 ** rckam
 ** Copyright (c) 2020-2021 Come Raczy
 ** All rights reserved.
 **
 ** This software is provided under the terms and conditions of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 **
 ** You should have received a copy of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 ** along with this program. If not, see
 ** <https://fsf.org/>
 **/

#ifndef RCKAM_COMMON_RCKAM_HPP
#define RCKAM_COMMON_RCKAM_HPP

namespace rckam
{
namespace common
{

constexpr unsigned dataPort = 12345;

#define TYP_INIT 0 
#define TYP_SMLE 1 
#define TYP_BIGE 2 

/// implements byte reordering for 64 bits
unsigned long long htonll(unsigned long long src)
{ 
  static int typ = TYP_INIT; 
  unsigned char c; 
  union
  { 
    unsigned long long ull; 
    unsigned char c[8]; 
  } x; 
  if (typ == TYP_INIT) { 
    x.ull = 0x01; 
    typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE; 
  } 
  if (typ == TYP_BIGE) 
    return src; 
  x.ull = src; 
  c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c; 
  c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c; 
  c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c; 
  c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c; 
  return x.ull; 
}

} // namespace common
} // namespace rckam

#endif // #ifndef RCKAM_COMMON_RCKAM_HPP
