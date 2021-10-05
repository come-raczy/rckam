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

enum ServerCommand
{
  ECHO_ONLY = 0, // just checking if server is alive
  GET_SERVER_CONFIG,
  LIST_CAMERAS,
  GET_CAMERA_CONFIG,
  GET_CAMERA_SINGLE_CONFIG,
  LIST_CAMERA_CONFIG,
  SET_CAMERA_CONFIG,
  SET_CAMERA_SINGLE_CONFIG,
  CAMERA_CAPTURE,
  CAMERA_TRIGGER_CAPTURE,
  CAMERA_CAPTURE_PREVIEW,
  CAMERA_SUMMARY,
  CAMERA_ABOUT,
  CAMERA_WAIT_FOR_EVENT,
  PAN,
  TILT,
  LAST_COMMAND
};

enum ResponseCode
{
  NOT_SUPPORTED = 1,
  LAST_CODE
};

constexpr size_t COMMAND_BUFFER_SIZE = 512;
constexpr unsigned dataPort = 12345;
constexpr unsigned controlPort = 12346;

#define TYP_INIT 0 
#define TYP_SMLE 1 
#define TYP_BIGE 2 

/// implements byte reordering for 64 bits
inline unsigned long long htonll(unsigned long long src)
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
