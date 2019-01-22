// Name: Deval Kaku
// ONID: 933342527

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define KEYGENMIN 65
#define KEYGENMAX 90

/// NAME: GenKey
/// DESC: generates a char* of random chars
char* GenKey(int length)
{
    int i;
    char* Key = malloc(sizeof(char) * length);

    //stop before null char
    for(i = 0;i < length;i++){
        if( (rand() % (10) + 1) == 3 || (rand() % (10) + 1) == 6){
            Key[i] = ' ';
        }
        else{
            Key[i] = (rand() % (KEYGENMAX + 1 - KEYGENMIN) + KEYGENMIN);
        }
    }
    Key[i] = '\0';

    return Key;
}

//validates theres atleast 1 arg in args passed to funcion.
void validArgc(int argc)
{
    //expected ./Keygen ###
    if(argc == 1){
        fprintf(stderr,"KeyGen: Enter a key length.\n");
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    //vars
    int KeyLength;
    char* EncryptionKey;
    validArgc(argc);

    //Begin Prog.
    srand(time(NULL));
    KeyLength = atoi(argv[1]) + 1;//must be + 1 to match test script.

    // gen key.
    EncryptionKey = GenKey(KeyLength);
    //print off key.
    printf("%s",EncryptionKey);

    //free from heap.
    free(EncryptionKey);
    return 0;
}
