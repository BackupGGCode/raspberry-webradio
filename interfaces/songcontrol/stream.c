///////////////////////////////////////////////////////////
//
// Shoutcast Stream Player
//
// 04.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "../../software/settings.h"

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT //
#endif

// enclose function in this to avaid warn_unused_result warning
#define ignore_result(x) ({ typeof(x) z = x; (void)sizeof z; })

FILE* mp3;
long pipe_id = 0;
int is_in_header = 1;
int next_is_meta = 0;
unsigned long metaint = 0;
unsigned long total_recv_byte = 0;
char title[256];
char* player =  NULL;
char* url = NULL;
char* path = NULL;
char current_song[256];
long buffer_size = 8192;
volatile int stop = 0;

// ---------------------------------------------------------------------------
FILE* popen_tid(char* args, const char* type, long* tid) { 
  int p[2]; 
  FILE* fp; 
  
  char* arguments[3];
  arguments[0] = args;
  arguments[1] = "-";
  arguments[2] = NULL;

  if (*type != 'r' && *type != 'w') 
    return NULL; 

  if (pipe(p) < 0) 
    return NULL; 

  if ((*tid = fork()) > 0) { /* then we are the parent */ 
    if (*type == 'r') { 
      close(p[1]); 
      fp = fdopen(p[0], type); 
    } else { 
      close(p[0]); 
      fp = fdopen(p[1], type); 
    } 
    return fp; 
  } else if (*tid == 0) {  /* we're the child */ 
    /* make our thread id the process group leader */ 
    setpgid(0, 0); 

    if (*type == 'r') { 
      fflush(stdout); 
      fflush(stderr);
      close(1); 

      if (dup(p[1]) < 0) 
        perror("dup of write side of pipe failed");
      
      close(2);
      
      if (dup(p[1]) < 0) 
        perror("dup of write side of pipe failed");
    } else { 
      close(0); 
      if (dup(p[0]) < 0) 
        perror("dup of read side of pipe failed"); 
    } 

    close(p[0]); /* close since we dup()'ed what we needed */ 
    close(p[1]); 

    execvp(arguments[0], arguments);

    printf("popen(): execve(%s) failed!\n", args);
  } else {         /* we're having major problems... */ 
    close(p[0]); 
    close(p[1]); 
    printf("popen(): fork() failure!\n");
  } 
  return NULL; 
}


// ---------------------------------------------------------------------------
FILE* open_stream_pipe(char* player) {
  FILE *fpout;

  if ((fpout = popen_tid(player, "w", &pipe_id)) == NULL)
    printf("can't open player '%s'", player);

  return fpout;
}

// ---------------------------------------------------------------------------
void close_stream_pipe(FILE* pipe) {
  if (pclose(pipe) == -1)
    printf(" ");
}

// ---------------------------------------------------------------------------
size_t send_stream_pipe(char* data, size_t size, size_t len, FILE* pipe) {
  return fwrite(data, sizeof(char), len, pipe);
}


// ---------------------------------------------------------------------------
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written;
  size_t req_size = size * nmemb;
  
  if(stop) {
    printf("abort\n");
    return 0;
  }
  
  if(is_in_header) {
   // --- debug ---
#ifdef DEBUG
   FILE* f = fopen("stream.txt", "a");
   fwrite(ptr, size, nmemb, f);
   fprintf(f, "-----------------------------\n");
   fclose(f);
#endif
   // --- debug end ---
   
   // parse metaint from header
   int header_end = 0;
   
   // search for end of header and icy data
   while(header_end < req_size) {
     int rest_bytes = req_size - header_end;
     
     if((rest_bytes >= 4 && strncmp((char*)ptr + header_end, "\r\n\r\n", 4) == 0)) {
       is_in_header = 0; 
       header_end += 4;
       break;
     }
     
     if(req_size == 2) {
       is_in_header = 0;
       header_end += 2;
       break;
     }
     
     if(rest_bytes >= 12 && strncmp((char*)ptr + header_end, "icy-metaint:", 12) == 0) {
       metaint = strtoul((char*)ptr + header_end + 12, NULL, 0);
       DEBUG_PRINT("Metaint: %lu\n", metaint);
     }
     header_end++;
   }
   
   // if header contains music too start the music
   if(header_end < req_size - 4) {
     total_recv_byte += req_size - header_end;
     send_stream_pipe(ptr + header_end, size, total_recv_byte, stream);
   }
   return req_size;
  }
  
  
  total_recv_byte += size * nmemb;
  //printf("%lu / %lu\r\n", total_recv_byte, metaint);
  
  int was_meta = 0;

  if(req_size > 2*metaint) DEBUG_PRINT("Warning! Received %lu bytes!\r\n", size * nmemb);
  
  // we've received meta data
  if(total_recv_byte > metaint) {
    was_meta = 1;
    DEBUG_PRINT("meta! (got %lu)\n", total_recv_byte);
    long metapos = req_size - (total_recv_byte - metaint);
    int metalen = (unsigned char)(*((char*)ptr + metapos)) * 16;
    
    DEBUG_PRINT("metalen: %d\n", metalen);
    // --- debug ---
#ifdef DEBUG
    FILE* f = fopen("stream.txt", "a");
    fwrite((char*)ptr + metapos + 1, metalen, 1, f);
    //fprintf(f, "\n-----------------------------\n");
    fclose(f);
#endif
    // --- debug end ---
    
    // parse title
    if(metalen > 0) {
      long start_title = metapos + 1;
      long end_title;
      while(*((char*)ptr + start_title) != '\'' && start_title < metapos + metalen) start_title++;
      start_title++;
      end_title = start_title + 1;
      while(!(*((char*)ptr + end_title) == '\'' && *((char*)ptr + end_title + 1) == ';') && end_title < metapos + metalen) {
	end_title++;
      }
      *((char*)ptr + end_title) = 0;
      strcpy(title, (char*)ptr + start_title);
      printf("Song: %s\n", title);
      // save song in file
      FILE* song_file = fopen(current_song, "w");
      fprintf(song_file, "%s\n", title);
      fclose(song_file);
    }
    
    // play everything that is not metadata
    total_recv_byte = req_size - metapos - metalen - 1;
    send_stream_pipe(ptr, size, metapos, stream);
    send_stream_pipe(ptr + metapos + metalen + 1, size, total_recv_byte, stream);
    return req_size;
  } 

  if(!was_meta) {
    // no metadata, just music
    written = send_stream_pipe(ptr, size, nmemb, stream);
    return req_size;
  }
}


// ---------------------------------------------------------------------------
void start_stream() {
  DEBUG_PRINT("Playing '%s' with '%s'\r\n", url, player);
  CURL* curl;
  CURLcode res;
  struct curl_slist* slist = NULL;
  
  curl = curl_easy_init();
  if(curl) {
    mp3 = open_stream_pipe(player);
    if(mp3 == NULL) {
      curl_easy_cleanup(curl);
      return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mp3);
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, buffer_size);
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);

    slist = curl_slist_append(slist, "Icy-MetaData:1");  
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    
    res = curl_easy_perform(curl);
    
    printf("done\n");
    
    curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
  }

  close_stream_pipe(mp3);  
  
}


// ---------------------------------------------------------------------------
int cleanup(void) {
 printf("stopping...\n");
 stop = 1;
}

// ---------------------------------------------------------------------------
void exit_func() {
  char kill[64];
  sprintf(kill, "kill -9 %ld\n", pipe_id);
  system(kill);
}

// ---------------------------------------------------------------------------
void broken_pipe() {
 DEBUG_PRINT("Broken pipe! Restarting...\r\n"); 
 stop = 1;
 char* argv[] = {path, url, NULL};
 execv(path, argv);
}

// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
  // load settings
  Settings_Load("play.conf");

  // initialize signal handlers
  signal(SIGINT, (sig_t)cleanup);
  signal(SIGPIPE, (sig_t)broken_pipe);
  atexit(exit_func);
  
  player = Settings_Get("program", "player");
  strcpy(current_song, Settings_Get("path", "current_song"));
  buffer_size = atol(Settings_Get("program", "buffer"));
  
  // reset current song
  FILE* song_file = fopen(current_song, "w");
  fprintf(song_file, "%s\n", " - ");
  fclose(song_file);
    
  // save program name and url to play
  url = argv[1];
  path = argv[0];
  
  // start the stream
  start_stream();
  
  // if done, cleanup
  Settings_Unload();
  return 0;
}



