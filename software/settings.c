///////////////////////////////////////////////////////////
//
// Settings Manager
//
// 21.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include "settings.h"

typedef struct {
	char* key;
	char* value;
} SettingsSetting;

typedef struct {
	char* name;
	int vals;
	SettingsSetting* values;
} SettingsCat;

typedef struct {
  int cats;
  SettingsCat* category;
} SettingsFile;

SettingsFile* settings = NULL;

// ---------------------------------------------------------------------------
int isWhitespace(char c) {
	if(c == ' ' || c == '\t' || c == '\r' || c == '\n') return 1;
	else return 0;
}

// ---------------------------------------------------------------------------
char* skipWhitespace(char* str) {
	while(isWhitespace(*str) && *str != 0) str++;
	return str;
}


// ---------------------------------------------------------------------------
char* trim(char* str) {
	char* ptr;
	str = skipWhitespace(str);
	ptr = str;
	while(!isWhitespace(*ptr) && *ptr != 0) ptr++;
	*ptr = 0;
	return str;
}

// ---------------------------------------------------------------------------
void addCategory(char* name) {
    settings->category = (SettingsCat*)realloc(settings->category, sizeof(SettingsCat) * (settings->cats + 1));
    settings->category[settings->cats].vals = 0;
    settings->category[settings->cats].values = NULL;
    settings->category[settings->cats].name = name; //(char*)malloc(sizeof(char) * (1 + strlen(name)));
    //strcpy(settings->category[settings->cats].name, name);
    settings->cats++;
}

// ---------------------------------------------------------------------------
void addValue(int current_cat, char* key, char* value) {
			// allocate memory for new key-value pair
			settings->category[current_cat].values = (SettingsSetting*)realloc(settings->category[current_cat].values,
					sizeof(SettingsSetting) * (1 + settings->category[current_cat].vals));

			// save key and value
			settings->category[current_cat].values[settings->category[current_cat].vals].key = key;
			settings->category[current_cat].values[settings->category[current_cat].vals].value = value;

			settings->category[current_cat].vals++;
}

// ---------------------------------------------------------------------------
int Settings_Load(char* file) {
	if(settings != NULL) Settings_Unload();
	settings = (SettingsFile*)malloc(sizeof(SettingsFile));
	settings->category = NULL;
	settings->cats = 0;

	FILE* f = fopen(file, "r");
	if(f == NULL) return 0;

	char buffer[256];
	char* line;
	int current_cat = -1;
	while(fgets(buffer, 256, f) != NULL) {
		line = trim(&buffer[0]);
		if(strlen(line) == 0) continue; // blank line
		if(line[0] == '#') continue; // just a comment
		// new category
		if(line[0] == '[') {
			line++;
			line[strlen(line) - 1] = 0;
			char* cat_name = (char*)malloc(sizeof(char) * (1 + strlen(line)));
            strcpy(cat_name, line);
			addCategory(cat_name);
            current_cat = settings->cats - 1;
            //printf("Cat: %s\r\n", line);
		}
		// new value
		else {
			// value without category -> error
			if(current_cat == -1) {
				fclose(f);
				return 0;
			}

			// get key and value
			char* ptr = strtok(line, "=");
			if(ptr == NULL) {
				fclose(f);
				return 0;
			}
			ptr = skipWhitespace(ptr);
			char* key = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
			strcpy(key, ptr);
			key = trim(key);

			ptr = strtok(NULL, "=");
			if(ptr == NULL) {
				fclose(f);
				return 0;
			}
			ptr = skipWhitespace(ptr);
			char* value = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
			strcpy(value, ptr);
			value = trim(value);

            addValue(current_cat, key, value);
            //printf(" -> %s: %s\r\n", key, value);
		}
	}
	fclose(f);
	return 1;
}

// ---------------------------------------------------------------------------
int Settings_Unload() {
	int cat, val;
	for(cat = 0; cat < settings->cats; cat++) {
	  for(val = 0; val < settings->category[cat].vals; val++) {
	    free(settings->category[cat].values[val].key);
	    free(settings->category[cat].values[val].value);
	  }
	  free(settings->category[cat].values);
	}
	free(settings->category);
	free(settings);
	settings = NULL;
	return 1;
}


// ---------------------------------------------------------------------------
char* Settings_Get(char* category, char* key) {
	int cat, val;
	for(cat = 0; cat < settings->cats; cat++) {
	  if(strcmp(category, settings->category[cat].name) == 0) {
	     for(val = 0; val < settings->category[cat].vals; val++) {
	       if(strcmp(settings->category[cat].values[val].key, key) == 0) {
		  return settings->category[cat].values[val].value;
	       }
	     }
	     return NULL;
	  }
	}
	return NULL;
}

// ---------------------------------------------------------------------------
int Settings_Add(char* category, char* key, char* value) {
    int cat, cat_found = -1;
    char *s_cat, *s_key, *s_val;
    s_cat = (char*)malloc((1 + strlen(category)) * sizeof(char));
    s_key = (char*)malloc((1 + strlen(key)) * sizeof(char));
    s_val = (char*)malloc((1 + strlen(value)) * sizeof(char));
    strcpy(s_cat, category);
    strcpy(s_key, key);
    strcpy(s_val, value);
    
    for(cat = 0; cat < settings->cats; cat++) {
        if(strcmp(category, settings->category[cat].name) == 0) {
            cat_found = cat;
            break;
        }
    }
    
    int found = 0;
    
    // need to add this category
    if(cat_found == -1) {
        addCategory(s_cat);
        cat_found = settings->cats - 1;
        //printf("add %s\r\n", s_cat);
    } else {
      // check if this key already exists
      int val;
      for(val = 0; val < settings->category[cat_found].vals; val++) {
	if(strcmp(settings->category[cat_found].values[val].key, s_key) == 0) {
	  found = 1;
	  free(settings->category[cat_found].values[val].value);
	  free(s_key);
	  free(s_cat);
	  settings->category[cat_found].values[val].value = s_val;
	  break;
	}
      }
    }
    
    // add key-value pair
    if(!found) addValue(cat_found, s_key, s_val);
    return 1;
}

// ---------------------------------------------------------------------------
int Settings_Save(char* file) {
	int cat, val;
    FILE* f = fopen(file, "w");
    if(f == NULL) return 0;

	for(cat = 0; cat < settings->cats; cat++) {
	  fprintf(f, "[%s]\r\n", settings->category[cat].name);
	     for(val = 0; val < settings->category[cat].vals; val++) {
	       fprintf(f, "%s=%s\r\n", settings->category[cat].values[val].key,
                settings->category[cat].values[val].value);
	     }
	     fprintf(f, "\r\n");
	}
    fclose(f);
    return 1;
}

