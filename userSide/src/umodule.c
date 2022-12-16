#include <linux/fs.h>
#include <string.h>
#include <stdio.h> 
#include <stdlib.h>

#define OUTPUT_BUFFER_SIZE 2000
#define INPUT_BUFFER_SIZE 2000
#define KMODULE "/proc/sreader"


struct user_dentry{
   unsigned int  d_flags;
};

void print_dentry(struct user_dentry* direction_entry){
    printf("----DENTRY INFO----\n");
        //printf("d_iname: %s\n", direction_entry->d_iname);
        printf("dflags: %lu\n", direction_entry->d_flags);
        //printf("d_time: %ul\n", direction_entry->d_time);
        //printf("superblock: %ul\n", direction_entry->d_sb->s_root->d_iname);
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
    if(r_file == NULL){
        printf("can't open file\n");
        return 1;
    }
    char isOk;
    if (fread(&isOk, sizeof(char), 1, r_file) == 0){
        printf("can't read structure from file\n");
    } else{
        printf("file found!\n");
        struct user_dentry den;
        unsigned int str;
        if (fread(&den, sizeof(struct user_dentry), 1, r_file) == 0){
            printf("can't read structure from file\n");
        }
        else{
            //printf("d_iname: %u\n", str);
            print_dentry(&den); 
        }
        fclose(r_file);
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
