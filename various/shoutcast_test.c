#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <libxml/parser.h>

typedef struct {
  char* name;
  char* url;
} ShoutcastStation;

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

int Download_Get(const char* url, const char* destination) {
  CURL* curl;
  FILE* fp;
  CURLcode res;
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
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    return 1;
  }
  return 0;
}


void
parseStation (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *uri = xmlGetProp(cur, "name");
	printf("name: %s\n", uri);
	xmlFree(uri);
	
	uri = xmlGetProp(cur, "id");
	printf("id: %s\n", uri);
	xmlFree(uri);
}

static void
parseDoc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "stationlist")) {
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"station"))){
			parseStation (doc, cur);
		}
		 
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int main() {
  //Download_Get("http://api.shoutcast.com/legacy/Top500?k=so1N15vhCB78Z6k4&limit=25&mt=audio/mpeg", "station.xml");
  //http://yp.shoutcast.com/sbin/tunein-station.pls?id=
  parseDoc("station.xml");
}

