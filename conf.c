#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "conf.h"

#define UPGRADECMD "upgradecmd"
#define PROGRAMS "programs"
#define NAME "name"
#define VERSION "version"
#define DOWNLOADNAME "downloadname"
#define UPDATECMD "updatecmd"
#define RUNCMD "runcmd"

struct conf * loadconf(cJSON *json){
	if(!json){
		return NULL;
	}
	cJSON *jprograms= cJSON_GetObjectItem(json,PROGRAMS);
	int programcount = cJSON_GetArraySize(jprograms); 

	struct program * programs = (struct program *)malloc(sizeof(struct program)*programcount);
	memset(programs, 0, sizeof(struct program) * programcount); 

	cJSON * name;
	cJSON * version;
	cJSON * downloadname;
	cJSON * updatecmd;
	cJSON * runcmd;

	int i = 0;
	for(;i<programcount; i++){ 
		cJSON * subitem = cJSON_GetArrayItem(jprograms, i);
		name = cJSON_GetObjectItem(subitem, NAME);
		if(name && name->type == cJSON_String){
			programs[i].name = strdup(name->valuestring);
		}
		version = cJSON_GetObjectItem(subitem, VERSION);
		if(version && version->type == cJSON_String){
			programs[i].version = atoi(version->valuestring);
		}
		downloadname = cJSON_GetObjectItem(subitem, DOWNLOADNAME);
		if(downloadname && downloadname->type == cJSON_String){
			programs[i].downloadname = strdup(downloadname->valuestring);
		}
		updatecmd  = cJSON_GetObjectItem(subitem, UPDATECMD);
		if(updatecmd && updatecmd->type == cJSON_String){
			programs[i].updatecmd = strdup(updatecmd->valuestring);
		}
		runcmd = cJSON_GetObjectItem(subitem, RUNCMD);
		if(runcmd && runcmd->type == cJSON_String){
			programs[i].runcmd = atoi(runcmd->valuestring);
		}
	}

	cJSON * jupgradecmd = cJSON_GetObjectItem(json, UPGRADECMD);
	struct conf * conf = (struct conf *) malloc(sizeof(struct conf));
	memset(conf, 0, sizeof(struct conf));
	if(jupgradecmd && jupgradecmd->type == cJSON_String){
		conf->upgradecmd = strdup(jupgradecmd->valuestring);
	}else{
		return NULL;
	}
	conf->programcount = programcount;
	conf->programs = programs;

	return conf;
}


int compareconf(struct conf * conflocal, struct conf * confurl){ 
	if(conflocal == NULL || confurl == NULL){
		return 1;
	}

	return 0;
}

void destroyprogram( struct program * program ){
	if(program){
		free(program->srcurl);
		free(program->dst);
		free(program->parameters);
	}
}

void destroyconf(struct conf * conf){ 
	if(conf){
		int i = 0;
		for(; i < conf->programcount; i++){
			destroyprogram(&conf->programs[i]);
		}
		free(conf->upgradecmd);
		free(conf->programs);
	}
	free(conf);
}
