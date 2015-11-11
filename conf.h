struct conf;

struct conf * loadconf(cJSON *json);
int compareconf(struct conf * conflocal, struct conf * confurl);
void destroyconf(struct conf * conf); 
