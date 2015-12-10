#include "list.h"

struct program{
	char * name;
	unsigned int version;
	char * downloadname;
	char * updatecmd;
	char * runcmd;
	struct list_head list;
};

struct conf{
	char * upgradecmd;
	int programcount;
	struct program * programs;
};

struct conf * loadconf(cJSON *json);
struct list_head * getconf(struct conf * conflocal, struct conf * confurl);
void destroyconf(struct conf * conf); 
void downloadprogram(struct list_head * head);
void runprograms(struct conf * urlconf);
void copy(char * src, char * dst);
