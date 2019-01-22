/* Name: Deval Kaku*/
/* ID: 933342527 */
/* CS344 Program2- Buildrooms*/

#include <dirent.h>
#include <pwd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


#define MIN_CONN 3
#define MAX_CONN 6
#define NUM_ROOMS 7
#define NUM_ROOM_NAMES 10

const char *roomname[NUM_ROOM_NAMES] = {
	"Real Madrid",
	"Manchester United",
	"Bayern Munich",
	"Barcelona",
	"Chelsea",
	"Arsenal",
	"PSG",
	"Juventus",
	"Liverpool",
	"AC Milan"
};

enum room_type {
    START_ROOM,
    END_ROOM,
    MID_ROOM
};

struct room {
    const char *name;
    int cap_conns;
    int num_conns;
    struct room *connections[NUM_ROOMS];
    enum room_type type;
};

struct room roomlist[NUM_ROOMS];
struct room *createroom(void);
void setroomdir(struct room roomlist[NUM_ROOMS]);
int connected( int room1,  int room2, struct room rooms_list[NUM_ROOMS]);
int already_connect( int room1,  int room2);

/* We created a list of rooms and we will use only 7 out of the 10. */
/* We will create 7 rooms and connect all the information from roomlist to the files. */
int main() {
    srand((unsigned)time(NULL));
    createroom();
    setroomdir(roomlist);
    
    return 0;
}
struct room *createroom() {
    int i, j;
    int names[NUM_ROOMS];
    for(i=0;i<NUM_ROOMS;i++){
        names[i]=0;
    }
    for (i = 0; i < NUM_ROOMS; i++) {
        roomlist[i].num_conns = 0;
        int cap = rand() % 3+3;
        roomlist[i].cap_conns = cap;
        
        while (true) {
            /* It will pick a random room*/
            int room_index = rand() % NUM_ROOM_NAMES;
            if (!names[room_index]) {
                names[room_index] = 1;
                roomlist[i].name = roomname[room_index];
                break;
            }
        }
        roomlist[i].type = MID_ROOM;
    }
    /* A random connection number is assigned to the room. */
    for (i = 0; i < NUM_ROOMS; i++) {
        for (j = 0; j < roomlist[i].cap_conns; j++) {
            int random_room = rand() % 5 + 1;
            while (i==random_room||!connected(i, random_room, roomlist)) {
                random_room += 1;
                if (random_room == NUM_ROOMS)
                    random_room = 0;
            }
        }
    }
    roomlist[0].type = START_ROOM;
    roomlist[NUM_ROOMS - 1].type = END_ROOM;
    return roomlist;
}
/* Check whether the two rooms are the same or not. */
/* Check whether the two rooms have already been connected or not. */
int connected( int room1,  int room2, struct room roomlist[NUM_ROOMS]) {
    struct room *r1 = &roomlist[room1];
    struct room *r2 = &roomlist[room2];
    if (r1->num_conns == MAX_CONN) {
        return 1;
    }
    if (already_connect(room1, room2)) {
        return 0;
    }
    if (r1->num_conns >= MAX_CONN || r2->num_conns >= MAX_CONN) {
        return 0;
    }
    assert(r1 != NULL);
    assert(r2 != NULL);
    /* Connect the two rooms and save their connection location in every connect list. */
    r1->connections[r1->num_conns] = r2;
    r2->connections[r2->num_conns] = r1;
    r1->num_conns++;
    r2->num_conns++;
    assert(r1->connections[r1->num_conns-1] != NULL);
    assert(r2->connections[r2->num_conns-1] != NULL);
    return 1;
}
/* Check if the two rooms have already been connected. */
int already_connect( int room1,  int room2) {
    int i;
    if (room1 == room2) {
        return 1;
    }
    for (i = 0; i < roomlist[room1].num_conns; i++) {
        if (roomlist[room1].connections[i] == &roomlist[room2] &&
            roomlist[room1].connections[i] != NULL) {
            return 1;
        }
    }
    return 0;
}
void setroomdir(struct room roomlist[NUM_ROOMS]) {
    int i,j;
    int pid = getpid();
    int max_len = strlen("kakud")+strlen(".rooms.")+10;
    char *dir_name = malloc(max_len * sizeof(char));
    assert(dir_name != NULL);
    sprintf(dir_name, "%s.rooms.%d", "kakud", pid);
    /* Create a file and assign a proccess id to it. */
    mkdir(dir_name, 0777);
    chdir(dir_name);
    /* build file for each room and connect the room information to it. */
    for (i = 0; i < NUM_ROOMS; i++) {
        FILE *fp = fopen(roomlist[i].name, "w");
        fprintf(fp, "ROOM NAME: %s\n", roomlist[i].name);
        for (j = 0; j < roomlist[i].num_conns; j++) {
            struct room *connectroom=roomlist[i].connections[j];
            fprintf(fp, "CONNECTION %d: %s\n", j + 1, connectroom->name);
        }
        if(roomlist[i].type==START_ROOM){
            fprintf(fp, "ROOM TYPE: START_ROOM\n");
        }
        else if(roomlist[i].type==END_ROOM){
            fprintf(fp, "ROOM TYPE: END_ROOM\n");
        }
        else{
            fprintf(fp, "ROOM TYPE: MID_ROOM\n");
        }
        fclose(fp);
    }
    chdir("..");
    free(dir_name);
}