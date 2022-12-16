#include <linux/fs.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>

#define OUTPUT_BUFFER_SIZE 2000
#define INPUT_BUFFER_SIZE 2000
#define KMODULE "/proc/sreader"


struct user_dentry{
    unsigned char d_iname[32];
    unsigned int d_flags;
    unsigned long d_time;
    unsigned long s_blocksize;
    unsigned long inode_ino;
};

void print_dentry(struct user_dentry* direction_entry){
    printf("----DENTRY INFO----\n");
        printf("d_iname: %s\n", direction_entry->d_iname);
        printf("dflags: %lu\n", direction_entry->d_flags);
        printf("d_time: %lu\n", direction_entry->d_time);
        printf("superblock size: %lu\n", direction_entry->s_blocksize);
    printf("-------------\n"); 
}


int main(int args, char **argv) {
    char output[OUTPUT_BUFFER_SIZE];
    char input[INPUT_BUFFER_SIZE];

    if (args != 2) {
        printf("Wrong number of arguments: %d. Requiered: 1\n", args);
        return 1;
    }


    sprintf(input, "%s", argv[1]);

    const char * end_marker = "-------------\n";

    FILE *w_file;
    w_file = fopen(KMODULE, "w");
    if(w_file == NULL){
        printf("can't open file\n");
        return 1;
    }
    fprintf(w_file, "%s", input);


    FILE *r_file;
    r_file = fopen(KMODULE, "r");
    if(r_file == NULL){ printf("can't open file\n");
        return 1;
    }
    char isOk;
    if (fread(&isOk, sizeof(char), 1, r_file) == 0){
        printf("unsucced to read data from proc file\n");
    } else if(isOk == 1){
        printf("file found!\n");
        struct user_dentry den;
        unsigned int str;
        char dname_len;
        if (fread(&dname_len, sizeof(char), 1, r_file) == 0) printf("can't read structure from file\n");
        else{
            if (fread(&den, sizeof(struct user_dentry), 1, r_file) == 0) printf("can't read structure from file\n");
            else print_dentry(&den); 
        }

        fclose(r_file);
    } else{
        printf("unsucced to read structure from file\n");
    }


    //struct inode* ind = (struct inode*) malloc(640);

    //r_file = fopen(KMODULE, "r");
    /*
    if (fread(&den, sizeof(struct user_dentry), 1, r_file) == 0){
        printf("can't read structure from file\n");
    }
    else{
    while (fgets(output, 256, r_file) != NULL) {
        printf("%s", output);
        if(strcmp(output, end_marker) == 0) break;
    }
    */
    /*
    fgets(output, 256, r_file);
    memcpy(ind, output, sizeof(struct user_dentry));
    print_dentry(ind); 
    */
    //fgets(output, 256, r_file);
    return 0;
}
