/* Name: Deval Kaku */
/* ID: 933342527 */
/* CS344 Program2 - Adventure */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>

#define NUM_ROOMS 7
#define NUM_ROOM_NAMES 10

struct room {
    char name[16];
    int num_conns;
    char connect[6][32];
    char type[16];
};
struct path {
    int path;
    char pathroom[32];
};
/*This function gets the game room dirname which we are looking for now.*/
void getdirname(char *dir){
    
    struct stat filebuffer;
    time_t lastfile = 0;
    DIR *d = opendir(".");
    struct dirent *dp;
    int count = 0;
    char tmp[256] = "./";
    
    if (d)
    {
        while ((dp = readdir(d)) != NULL)
        {
            if (strstr(dp->d_name, "kakud.rooms") == NULL)
                continue;
            
            stat(dp->d_name, &filebuffer);
            /*find the lastfile*/
            if (count != 0 && lastfile >= filebuffer.st_mtime)
                continue;
            
            lastfile = filebuffer.st_mtime;
            strcpy(dir, dp->d_name);
            count += 1;
        }
        closedir(d);
    }
    strcat(dir, "/");
    strcat(tmp, dir);
    strcpy(dir, tmp);
}

/* This function creates a room structure to save all the information that we just build. */
struct room *createNewRoom(char* roomname, char* roomtype) {
    struct room *rooms = malloc(sizeof(struct room)*7);
    
    memset(rooms->name, '\0', sizeof(rooms->name));
    strcpy(rooms->name, roomname);
    
    rooms->num_conns = 0;
    
    memset(rooms->type, '\0', sizeof(rooms->type));
    strcpy(rooms->type, roomtype);
    int i = 0;
    for (i = 0; i < 6; i++) {
        memset(rooms->connect[i], '\0', sizeof(rooms->connect[i]));
        strcpy(rooms->connect[i], "None");
    }
    return rooms;
}
/* Connection is added from the current room to the another room.*/
void addconnect(struct room* room1, char* roomConnection) {
    strcpy(room1->connect[room1->num_conns], roomConnection);
    room1->num_conns++;
}
/*This function is used to set the room type of the given room. */
void setType(struct room* room1, char* newRoomType) {
    memset(room1->type, '\0', sizeof(room1->type));
    strcpy(room1->type, newRoomType);
}
/* The game file is accessed and the the information is copied from each room file to the room struct. */
struct room *downloadFile(struct room *rooms){
    char *newestDirName=malloc(sizeof(char) * 256);
    getdirname(newestDirName);
    DIR *d = opendir(newestDirName);
    FILE* curRoomFile;
    char filebuf[512];
    char filenameBuffer[128];
    char currentname[32];
    char con_name[32];
    char current_type[32];
    struct dirent *dp;
    int i = 0;
    
    while ((dp = readdir(d)) != NULL)
    {
        if (dp->d_type == DT_REG) {
            /* Resets buffer. */
            memset(filebuf, '\0', sizeof(filebuf));
            memset(filenameBuffer, '\0', sizeof(filenameBuffer));
            memset(currentname, '\0', sizeof(currentname));
            memset(con_name, '\0', sizeof(con_name));
            memset(current_type, '\0', sizeof(current_type));
            
            /* Look and open room file. */
            strcpy(filenameBuffer, newestDirName);
            strcat(filenameBuffer, "/");
            strcat(filenameBuffer, dp->d_name);
            curRoomFile = fopen(filenameBuffer, "r");
            if (!curRoomFile) {
                fprintf(stderr, "Could not open room file\n");
                perror("Error opening file");
                exit(1);
            }
            
            /* Copies the room name. */
            fgets(filebuf, sizeof(filebuf), curRoomFile);
            filebuf[strlen(filebuf) - 1] = '\0';
            strcpy(currentname, &filebuf[11]);
            
            /* Creates new room struct with load room name*/
            struct room *newRoom = malloc(sizeof(struct room)*7);
            newRoom= createNewRoom(currentname, "MID_ROOM");
            rooms[i] = *newRoom;
            free(newRoom);
            
            /* Reads the connections and room type. */
            while (fgets(filebuf, sizeof(filebuf), curRoomFile) != NULL) {
                if (strstr(filebuf, "CONNECTION") != NULL) {
                    filebuf[strlen(filebuf) - 1] = '\0';
                    strcpy(con_name, &filebuf[14]);
                    addconnect(&rooms[i], con_name);
                }
                else {
                    filebuf[strlen(filebuf) - 1] = '\0';
                    strcpy(current_type, &filebuf[11]);
                    setType(&rooms[i], current_type);
                }
            }
            /* Closes current file. */
            fclose(curRoomFile);
            i++;
        }
    }
    closedir(d);
    return rooms;
}
/* Gets the current time. */
void* setTime(void* myMutex) {
    /*locks*/
    pthread_mutex_lock(myMutex);
    
    FILE* timeFile = fopen("currentTime.txt", "w");
    time_t curTime;
    char* timeStr;
    
    /* Get the current time. */
    curTime = time(0);
    timeStr = ctime(&curTime);
    
    /* Put the time into file. */
    fputs(timeStr, timeFile);
    fclose(timeFile);
    
    /* Unlocks and goes back to the main thread. */
    pthread_mutex_unlock(myMutex);
    return NULL;
}
/* Prints current time.*/
void* getTime() {
    FILE* timeFile = fopen("currentTime.txt", "r");
    char timestamp[64];
    fgets(timestamp, sizeof(timestamp), timeFile);
    printf("\n%s\n", timestamp);
    fclose(timeFile);
    return NULL;
}
/* In this part, we start to run the game. */
void GetStartRm(struct room *rooms){
    pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&myMutex);
    int curRoom=0,i;
    /* Looks for the starting room. */
    for (i = 0; i < 7; i++)
    {
        if(strcmp(rooms[i].type,"START_ROOM")==0){
            curRoom=i;
            break;
        }
    }
    size_t bufferSize = 0;
    pthread_t timeThread;
    char* userInput = NULL;
    int charsEntered,result_code;
    int breakLoop = 0;
    /* Builds the path table*/
    struct path gamePath;
    gamePath.path=0;
    for (i = 0; i < 32; i++) {
        gamePath.pathroom[i] = -1;
    }
    
    /* Start running the loop. If the room is end room, then leave the loop*/
    while (strcmp(rooms[curRoom].type, "END_ROOM") !=0) {
        breakLoop = 0;
        printf("CURRENT LOCATION: %s\n", rooms[curRoom].name);
        printf("POSSIBLE CONNECTIONS: ");
        int i = 0;
        for (i = 0; i < rooms[curRoom].num_conns; i++) {
            printf("%s", rooms[curRoom].connect[i]);
            if (i != (rooms[curRoom].num_conns - 1)) {
                printf(", ");
            }
        }
        printf(".\n");
        /*get input*/
        while (breakLoop == 0) {
            printf("WHERE TO? >");
            charsEntered = getline(&userInput, &bufferSize, stdin);
            userInput[charsEntered - 1] = '\0';
            /* Checks whetehr the room is in the correct list or not. */
            for (i = 0; i < rooms[curRoom].num_conns; i++) {
                if (strcmp(rooms[curRoom].connect[i],userInput)==0) {
                    printf("\n");
                    bufferSize = 0;
                    breakLoop = 1;
                    for (i = 0; i < NUM_ROOMS; i++)
                    {
                        if(strcmp(rooms[i].name,userInput)==0){
                            curRoom=i;
                            break;
                        }
                    }
                    userInput = NULL;
                    free(userInput);
                    break;
                }
                /* If the input is time, display the current time. */
                if(strcmp("time",userInput)==0){
                    pthread_mutex_unlock(&myMutex);
                    result_code = pthread_create(&timeThread, NULL, setTime, &myMutex);
                    pthread_join(timeThread, NULL);
                    pthread_mutex_lock(&myMutex);
                    getTime();
                    free(userInput);
                    bufferSize = 0;
                    userInput = NULL;
                    break;
                }
                if(i+1==rooms[curRoom].num_conns){
                    printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
                    free(userInput);
                    bufferSize = 0;
                    userInput = NULL;
                }
            }
        }
        gamePath.pathroom[gamePath.path] = curRoom;
        gamePath.path++;
    }
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS. ", gamePath.path);
    printf("YOUR PATH TO VICTORY WAS:\n");
    
    for (i = 0; i < gamePath.path; i++) {
        printf("%s\n", rooms[gamePath.pathroom[i]].name);
    }
    /* Frees the memory which was allocated dynamically. */
    free(userInput);
}
int main() {
    /* In the begining, we created a roompointer to point the 7 rooms which we will make later.*/
    struct room* rooms= malloc(sizeof(struct room) * NUM_ROOMS);
    int i;
    for (i = 0; i < NUM_ROOMS; i++)
    {
        rooms[i].num_conns = 0;
    }
    
    /*In this function, we will go to the game file first and then copy the information from each room file to the room struct.*/
    downloadFile(rooms);
    
    /*In this part, we start to run the game.*/
    GetStartRm(rooms);
    
    /* Frees the memory which was allocated dynamically. */
    free(rooms);
    return 0;
}