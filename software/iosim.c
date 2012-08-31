#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

int8_t rotary = 0;
int8_t buttons = 0;



int main() {
  	initscr();
	raw();
	noecho();

  
  int end = 0;
  int last_key = 0;
  
  int fd = open("io.sim", O_RDWR | O_CREAT | O_TRUNC, 0777);
  if(fd == -1) {
	perror("Error opening file 'io.sim' for writing");
	return -1;
  }
  
  write(fd, "\0\0\0\0", 4); // write 4 0-byte to file
  uint32_t* map = mmap(0, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(map == MAP_FAILED) {
    perror("Error mapping file 'io.sim'");
	return -1;
  }

    
  printf(" - I/O Simulator - \r\n");
  printf("a/d...Rotary left/right     s...Rotary click\r\n");
  printf("1-8...Button 1-8            q...Quit\r\n");
  refresh();
  
  // react to user input
  while(!end) {
	last_key = getchar();
	if(last_key == 'q') end = 1;
	// check last key
	if(last_key == 'd') {
		*map = *map + 0x010000;
	}
	if(last_key == 'a') {
		*map = *map - 0x010000;
	}
	if(last_key == 's') {
		*map = *map | 1; 
	}
	if(last_key >= '1' && last_key <= '7') {
		*map = *map | (1 << (last_key - '1' + 1));
	}
	printf("%c", last_key);
	refresh();
	//refresh();
  }
  
  // unmap file
  munmap(map, 4);
  close(fd);
  endwin();
  return 0;
}
