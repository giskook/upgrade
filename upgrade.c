#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include "conf.h"

#define LOCALCONFPATH "./upgrade.json"
#define URLCONFPATH "./upgrade.json.download"

cJSON * loadconffile(char * path){ 
	FILE *f;long len;char *data;
	f=fopen(path,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);data[len]='\0';fclose(f); 
	cJSON *json;
	json=cJSON_Parse(data);
	free(data); 
	return json;
}
void copy(char * src, char * dst){
	FILE *in_fd = fopen(src,"rb"); 
	FILE *out_fd = fopen(dst,"w"); 
	char buf[512];
	int n = 0;

	while (n != EOF) {
		n = fread(buffer, 1, 512, in_fd);
		if (n == 0)
			break;
		fwrite(buffer, 1, n, out_fd);
	}

}

int main(){
	cJSON * localjson = loadconffile(LOCALCONFPATH);
	cJSON * urljson = loadconffile(URLCONFPATH);
	struct conf * localconf = loadconf(localjson);
	struct conf * urlconf = loadconf(urljson);
	cJSON_Delete(localjson);
	cJSON_Delete(urljson);
	if (0 != compareconf(localconf, urlconf) ){
		copy(URLCONFPATH, LOCALCONFPATH);
	}
}
