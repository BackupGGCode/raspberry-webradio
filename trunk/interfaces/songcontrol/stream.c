#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT //
#endif


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

long pipe_id = 0;

FILE* open_stream_pipe(char* player) {
  FILE *fpout;

  if ((fpout = popen_tid(player, "w", &pipe_id)) == NULL)
    printf("can't open player '%s'", player);

  return fpout;
}

void close_stream_pipe(FILE* pipe) {
  if (pclose(pipe) == -1)
    printf(" ");
}

size_t send_stream_pipe(char* data, size_t size, size_t len, FILE* pipe) {
  return fwrite(data, sizeof(char), len, pipe);
}

int is_in_header = 1;
int next_is_meta = 0;
long metaint = 0;
long total_recv_byte = 0;
char title[128];

volatile int stop = 0;

int cleanup(void) {
 printf("stopping...\n");
 stop = 1;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written;
  size_t req_size = size * nmemb;
  
  if(stop) {
    printf("abort\n");
    exit(5);
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
   while(header_end < req_size) {
     int rest_bytes = req_size - header_end;
     
     if(rest_bytes >= 4 && strncmp((char*)ptr + header_end, "\r\n\r\n", 4) == 0) {
       is_in_header = 0; 
       header_end += 4;
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
  // we've received meta data
  if(total_recv_byte > metaint) {
    DEBUG_PRINT("meta! (got %lu)\n", total_recv_byte);
    int metapos = req_size - (total_recv_byte - metaint);
    int metalen = (unsigned char)(*((char*)ptr + metapos)) * 16;
    
    DEBUG_PRINT("metalen: %d\n", metalen);
    // --- debug ---
#ifdef DEBUG
    FILE* f = fopen("stream.txt", "a");
    fwrite((char*)ptr + metapos + 1, metalen, 1, f);
    fprintf(f, "\n-----------------------------\n");
    fclose(f);
#endif
    // --- debug end ---
    
    // parse title
    if(metalen > 0) {
      int start_title = metapos + 1;
      int end_title;
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
      FILE* song_file = fopen("song.txt", "w");
      fprintf(song_file, "%s\n", title);
      fclose(song_file);
    }
    
    // play everything that is not metadata
    total_recv_byte = req_size - metapos - metalen - 1;
    send_stream_pipe(ptr, size, metapos, stream);
    send_stream_pipe(ptr + metapos + metalen + 1, size, total_recv_byte, stream);
    return req_size;
  } else {
    // no metadata, just music
    written = send_stream_pipe(ptr, size, nmemb, stream);
    return req_size;
  }
}

FILE* mp3;
void start_stream(char* url, char* player) {
  
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

    slist = curl_slist_append(slist, "Icy-MetaData:1");  
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    
    res = curl_easy_perform(curl);
    
    printf("done\n");
    
    curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
  }

  close_stream_pipe(mp3);  
  
}

void exit_func() {
  char kill[64];
  sprintf(kill, "kill -9 %ld\n", pipe_id);
  system(kill);
}

int main(int argc, char* argv[]) {
  char* url = "http://173.192.207.51:8022"; //"http://bertjuhh66.ftpaccess.cc:8000"; //"http://relay.181.fm:8072"; //"http://relay.181.fm:8062"; //"http://he-srv5.defjay.de:80";
  
  signal(SIGINT, (sig_t)cleanup);
  atexit(exit_func);
  
  //start_stream(argv[1], "mpg123 - 2>&1 | cat > /dev/null");
  start_stream(argv[1], "mpg123");
}



