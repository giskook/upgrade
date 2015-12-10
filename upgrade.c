#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "conf.h"
#include "list.h"

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

void execmajor(void){

}

int main(){
	if(atexit(execmajor)){
		fprintf(stdout, "register at exit fail\n");
	}
	
	//stage 1. read the conf file and get the remote conf file
	//stage 2. commpare the two conf files
	//stage 3. if is equal then exit if not equal excute the new conf
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
		struct list_head * head = getconf(localconf, urlconf); 
		if(head && !list_empty(head)){
			downloadprogram(head);
		}
		free(head);
		copy(URLCONFPATH, LOCALCONFPATH);
		remove(URLCONFPATH); 

		runprograms(urlconf);
		destroyconf(urlconf);
	}else{ 
		runprograms(localconf);
	}
	destroyconf(localconf);
}
