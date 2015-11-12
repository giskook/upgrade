struct program{
	char * name;
	unsigned int version;
	char * downloadname;
	char * updatecmd;
	char * runcmd;
};

struct conf{
	char * upgradecmd;
	int programcount;
	struct program * programs;
};

struct conf * loadconf(cJSON *json);
int compareconf(struct conf * conflocal, struct conf * confurl);
void destroyconf(struct conf * conf); 
