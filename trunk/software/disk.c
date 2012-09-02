///////////////////////////////////////////////////////////
//
// Disk functions
//
// 24.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include "disk.h"

int Disk_GetStat(char* path, long* size, int* used_perc) {
 struct statfs s;
 long blocks_used;
 long blocks_percent_used;
 
 if(statfs(path, &s) != 0) {
  return 0;
 }
 
 if(s.f_blocks > 0) {
  blocks_used = s.f_blocks - s.f_bfree;
  if(blocks_used == 0) {
   blocks_percent_used = 0; 
  } else {
   blocks_percent_used = (long)(blocks_used * 100.0 / (blocks_used + s.f_bavail) + 0.5); 
  }
  
  *size = (long)(((double)(s.f_blocks / 1024.0 / 1024.0) * (double)(s.f_bsize / 1024.0)) + 0.5);
  *used_perc = (int)blocks_percent_used;
 } else {
  return 0; 
 }
 return 1;
}

