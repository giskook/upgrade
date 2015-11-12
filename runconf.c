#include "conf.h"

int runprogram(struct program * program){ 
	return system(program->runcmd)
}

int updateprogram(struct program * program){ 
	if(access(program->downloadname, F_OK) != -1){
		remove(program->downloadname);
	}
	int rt = system(program->updatecmd);
	if (rt == 0){
		remove(program->name);
		rename(program->downloadname, program->name);

		return 0;
	}else{ 
		return 1;
	}
}
