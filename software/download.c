///////////////////////////////////////////////////////////
//
// Function to Download a File
//
// 12.09.2012 - Michael Schwarz
//
/////////////////////////////////////////////////////////// 

#include "download.h"

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

int Download_Get(const char* url, const char* destination) {
  CURL* curl;
  FILE* fp;
  curl = curl_easy_init();
  if(curl) {
    fp = fopen(destination, "wb");
    if(fp == NULL) {
      curl_easy_cleanup(curl);
      return 0;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    return 1;
  }
  return 0;
}
