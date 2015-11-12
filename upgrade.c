#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "cJSON.h"
#include "conf.h"

#define LOCALCONFPATH "./upgrade.json"
#define URLCONFPATH "./upgrade.json.download"
#define PAGESIZE 512

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
	char buf[PAGESIZE];
	size_t n = 0;

	while (n != EOF) {
		n = fread(buf, 1, PAGESIZE, in_fd);
		if (n == 0)
			break;
		fwrite(buf, 1, n, out_fd);
	}
	fclose(in_fd);
	fclose(out_fd);
}

int main(){
	if(access(URLCONFPATH, F_OK) != -1){
		remove(URLCONFPATH);
	}
	cJSON * localjson = loadconffile(LOCALCONFPATH);
	struct conf * localconf = loadconf(localjson);
	int rt  = system(localconf->upgradecmd);
	if(rt == 0){
		cJSON * urljson = loadconffile(URLCONFPATH);
		struct conf * urlconf = loadconf(urljson);
		cJSON_Delete(localjson);
		cJSON_Delete(urljson);

		if (0 != compareconf(localconf, urlconf) ){
			copy(URLCONFPATH, LOCALCONFPATH);
			remove(URLCONFPATH); 
			system("./upgrade");
			exit(1);
		}else{
			fprintf(stdout, "i am in 2\n");
		}

	}else{
	}
	system("ls");
}
