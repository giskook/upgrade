
INCLUDE = -I$(PROJ_DIR)./

CC=gcc

CFLAGS= -c -Wall -g -std=gnu99
LIBS = -lm
DEFS +=

all: upgrade.bin broadcast.bin

upgrade.bin: upgrade.o conf.o cJSON.o
	$(CC) upgrade.o conf.o cJSON.o $(LIBS) -o upgrade.bin

# rule for file "upgrade.o".
main.o: upgrade.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)upgrade.c

# rule for file "conf.o".
conf.o: conf.h conf.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)conf.c

# rule for file "cJSON.o".
cJSON.o: cJSON.h cJSON.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)cJSON.c


broadcast.bin: broadcast.o
	$(CC) broadcast.o $(LIBS) -o broadcast.bin

# rule for file "broadcast.o".
broadcast.o: broadcast.c
	$(CC) $(CFLAGS) $(INCLUDE) $(DEFS) $(PROJ_DIR)broadcast.c


# rule for cleaning files generated during compilations.
clean:
	/bin/rm -f *.bin *.o
