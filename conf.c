#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#define UPGRADEURL "upgradeurl"
#define PROGRAMS "programs"
#define VERSION "version"
#define SRCURL "srcurl"
#define DST "dst"
#define RUN "run"
#define PARAMETERS "parameters"

struct program{
	unsigned char version;
	char * srcurl;
	char * dst;
	unsigned char run;
	char * parameters;
};

struct conf{
	char * upgradeurl;
	int programcount;
	struct program * programs;
};

struct conf * loadconf(cJSON *json){ 
	if(!json){
		return NULL;
	}
	cJSON *jprograms= cJSON_GetObjectItem(json,PROGRAMS);
	int programcount = cJSON_GetArraySize(jprograms); 

	struct program * programs = (struct program *)malloc(sizeof(struct program)*programcount);
	memset(programs, 0, sizeof(struct program) * programcount); 

	cJSON * version;
	cJSON * srcurl;
	cJSON * dst;
	cJSON * run;
	cJSON * parameters;

	int i = 0;
	for(;i<programcount; i++){ 
		cJSON * subitem = cJSON_GetArrayItem(jprograms, i);
		version = cJSON_GetObjectItem(subitem, VERSION);
		if(version && version->type == cJSON_String){
			programs[i].version = atoi(version->valuestring);
		}
		srcurl  = cJSON_GetObjectItem(subitem, SRCURL);
		if(srcurl && srcurl->type == cJSON_String){ 
			programs[i].srcurl = strdup(srcurl->valuestring);
		}
		dst = cJSON_GetObjectItem(subitem, DST);
		if(dst && dst->type == cJSON_String){
			programs[i].dst = strdup(dst->valuestring);
		}
		run = cJSON_GetObjectItem(subitem, RUN);
		if(run && run->type == cJSON_String){
			programs[i].run = atoi(run->valuestring);
		}
		parameters = cJSON_GetObjectItem(subitem, PARAMETERS);
		if(parameters && parameters->type == cJSON_String){
			programs[i].parameters = strdup(parameters->valuestring);
		}
	}

	cJSON * jupgradeurl = cJSON_GetObjectItem(json, UPGRADEURL);
	struct conf * conf = (struct conf *) malloc(sizeof(struct conf));
	memset(conf, 0, sizeof(struct conf));
	if(jupgradeurl && jupgradeurl->type == cJSON_String){
		conf->upgradeurl = strdup(jupgradeurl->valuestring);
	}else{
		return NULL;
	}
	conf->programcount = programcount;
	conf->programs = programs;

	return conf;
}


int compareconf(struct conf * conflocal, struct conf * confurl){ 
	if( 0 == strcmp(conflocal->upgradeurl, confurl->upgradeurl)){ 
		if(conflocal->programcount > confurl->programcount){
			return 2;
		}else if(conflocal->programcount < confurl->programcount){
			return 3;
		}else{
			int programcount = conflocal->programcount;
			int i = 0;
			for(;i < programcount; i++){
				if(conflocal->programs[i].version != confurl->programs[i].version ||
						0 != strcmp(conflocal->programs[i].srcurl, confurl->programs[i].srcurl) || 
						0 != strcmp(conflocal->programs[i].dst, confurl->programs[i].dst) || 
						conflocal->programs[i].run!= confurl->programs[i].run || 
						0 != strcmp(conflocal->programs[i].parameters, confurl->programs[i].parameters)
						){
					return i*10 + 4;
				}
			}
		}
	}else{
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
		free(conf->upgradeurl);
		free(conf->programs);
	}
	free(conf);
}
