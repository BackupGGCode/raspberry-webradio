/************************************************
 *
 * USB Player Module for Raspberry WebRadio
 *
 * Plays local MP3 and M3U files
 *
 * 13.08.2012 - Michael Schwarz
 *
 ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define PLAYER "mpg123"

long player_tid = 0;
int stop = 0;

int is_file(char* file, char* ending) {
	if(strlen(file) >= strlen(ending) && 
	   strncmp(&file[strlen(file) - strlen(ending)], ending, strlen(ending)) == 0) return 1;
	else return 0;
}

int is_playlist(char* file) {
	return is_file(file, ".m3u");
}

int is_mp3(char* file) {
	return is_file(file, ".mp3");
}

void dump_id3tag(char* file) {
	char buffer[512];
	sprintf(buffer, "id3v2 -R \"%s\" | grep TPE1 | cut -b 7- | tr -d \"\\n\" > song.txt", file);
	system(buffer);
	sprintf(buffer, "echo -n \" - \" >> song.txt");
	system(buffer);
	sprintf(buffer, "id3v2 -R \"%s\" | grep TIT2 | cut -b 7- >> song.txt", file);
	system(buffer);
}

void start_player(char* player, char* file) {
	//printf("play '%s'\n", file);
	
	char* arguments[3];
	arguments[0] = player;
	arguments[1] = file;
	arguments[2] = NULL;
	
	dump_id3tag(file);
	
	if((player_tid = fork()) > 0) { // we are parent
		waitpid(player_tid, 0, 0);
	} else {
		execvp(arguments[0], arguments);
		exit(127);
	}
}

int is_whitespace(char c) {
 if(c == ' ' || c == '\t' || c == '\r' || c == '\n') return 1;
 else return 0;
}

char* trim(char* string) {
 while(is_whitespace(*string)) string++;
 while(is_whitespace(string[strlen(string) - 1])) string[strlen(string) - 1] = 0;
 return string;
}

void play_playlist(char* player, char* file) {
	char buffer[512];
	char* line;
	
	FILE* f = fopen(file, "r");
	if(f == NULL) return;
	
	while(fgets(buffer, 512, f) != NULL && !stop) {
		line = buffer;
		line = trim(line);
		if(line[0] == '#') continue; // skip comments and meta information
		// otherwise, check if it is an mp3 and if yes, play it
		if(is_mp3(line)) {
		    //printf("play: %s\n", line);
		    start_player(player, line);
		} else {
		    //printf("no mp3: %s\n", line);
		}
	}
	
	fclose(f);
}

int cleanup(void) {
	stop = 1;
	char kill[32];
	sprintf(kill, "kill -9 %ld\n", player_tid);
	system(kill);
	return 1;
}

int next_song(void) {
  char kill[32];
  sprintf(kill, "kill -9 %ld\n", player_tid);
  system(kill);
  return 1;
}

int main(int argc, char* argv[]) {
	signal(SIGINT, (sig_t)cleanup);
	signal(SIGHUP, (sig_t)next_song);

	// start playing
	if(is_mp3(argv[1])) {
		start_player(PLAYER, argv[1]);
	} else if(is_playlist(argv[1])) {
		//printf("playlist\r\n");
		play_playlist(PLAYER, argv[1]);
	}
	
	// done playing, reset id3 info
	FILE* f = fopen("song.txt", "w");
	fprintf(f, "(no artist) - (no title)\r\n");
	fclose(f);
	
	return 0;
}
