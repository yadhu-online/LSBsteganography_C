#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)  //function for validate valid file and argument are passed
{
    if(strcmp(&argv[2][strlen(argv[2])-4],".bmp")==0) //check if 3rd argument vector is a bmp file or not
    {
        decInfo->steg_image_fname=argv[2];
    }
    else {
        printf("bmp file type error\n");
        return e_failure;}
    
    if(argv[3])
    {
        char *token=strtok(argv[3],"."); //if 4th argument vector is given, then saving the name without extention
        //decInfo->decode_fname=token;
        strcpy(decInfo->decode_fname,token);
    }
    else   //else simple create a file with name "decode"
    {
        //decInfo->decode_fname="decode";
        strcpy(decInfo->decode_fname,"decode");
    }
    printf("read and validate complete! ✔\n");
    //printf("%s\n",decInfo->decode_fname);
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo) // where all function for decoding is called
{
    if(open_files_dec(decInfo)==e_success)
    {
        printf("File open successfully ✔\n");
    }
    else{
        printf("Failure: File open unsuccessfully\n");
        return e_failure; }
    if(decode_magic_string(MAGIC_STRING,decInfo)==e_success) //calling fun for decoding magic string
    { //printf("Decoded Magic string!\n");
        }
    else{
       // printf("Magic string Decoding Failed");
        return e_failure;
    }

    if(decode_secret_file_extn_size(decInfo)==e_success){
        printf("Decoded secrect file extention size ✔\n");}
    else{printf("Failed file extension size decoding\n"); return e_failure;}

    if(decode_secret_file_extn(decInfo)==e_success){
        printf("Decoded secrect file extention ✔\n");}
    else{printf("Failed file extension decoding\n"); return e_failure;}

    if(decode_secret_file_size(decInfo)==e_success){
        printf("Decoded secrect file size ✔\n");}
    else{printf("Failed file size decoding\n"); return e_failure;}

    if(decode_secret_file_data(decInfo)==e_success){
        printf("Decoded secret data ✔\n");}
    else{printf("Failed Secret data decoding!\n"); return e_failure;}

    return e_success;
}

Status open_files_dec(DecodeInfo *decInfo) //open file function definition
{
    decInfo->fptr_steg_image = fopen(decInfo->steg_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_steg_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->steg_image_fname);

    	return e_failure;
    }
    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo) //function for decode magic string
{
    fseek(decInfo->fptr_steg_image,54,SEEK_SET); //set pointer to 54pos where RGB data starts
    char* magstr=decode_data_to_image(strlen(magic_string),decInfo->fptr_steg_image); 


    if(strcmp(magstr,MAGIC_STRING)==0) //validating magic string
    {
        printf("Magic string verified! ✔\n");
        //return e_success;
        free(magstr);
    }
    else{
        printf("Magic string Not match!\n");
        return e_failure;
    }
    return e_success;
}

//char* decode_data_to_image(char *data, int size, FILE *fptr_steg_image)
char* decode_data_to_image(int size, FILE *fptr_steg_image)
{
    char buf[8];char *c=malloc((size+1)*sizeof(char));int i=0; //function for decode data from encoded file
    for(i=0;i<size;i++)
    {
         fread(buf,8,1,fptr_steg_image); //read 1Byte from encoded file and kept in buffer
         c[i]=decode_byte_to_lsb(buf); //passed to function and collected the decoded char data
    }
    c[i]='\0';  //adding null to last
    return c;
}

char decode_byte_to_lsb(char *data)
{
    int ch=0;
    for(int j=7;j>=0;j--) {        /*for(int i=0;i<8;i++){ch=((data[i]&0x01)<<i | ch);}*/
    ch=(ch<<1)|(data[j]&1);  //logic for decoding LSB from each Byte of data and saving it in variable;
    }
    return ch;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo) //size=4;
{
    char buff[32]; //char array for 4 Byte data
    fread(buff,32,1,decInfo->fptr_steg_image);
    int extn_size=decode_lsb_to_size(buff); //passing to above function to decode file extention size
    decInfo->extn_steg_size=extn_size; //storing in structure, so that later can be used.

    return e_success;
}

int decode_lsb_to_size(char* str) //logic for decoding 4 Byte data
{
    int n;
    for(int j=32;j>=0;j--) {
    n=(n<<1)|(str[j]&1);
    }
    return n;
}

Status decode_secret_file_extn( DecodeInfo *decInfo) //find secret file extention
{
    char buff[8];int i=0;
    char c_ext[(decInfo->extn_steg_size)+1];
    for(i=0;i<decInfo->extn_steg_size;i++)
    {
        fread(buff,8,1,decInfo->fptr_steg_image);
        c_ext[i]=decode_byte_to_lsb(buff);
    }
    c_ext[i]='\0';

    strcat(decInfo->decode_fname,c_ext); //concatanation of extention and filename given at initially
    return e_success;
} 

Status decode_secret_file_size(DecodeInfo *decInfo) //function for decoding secret file size
{
    char buff[32]; // 4 Byte data
    fread(buff,32,1,decInfo->fptr_steg_image);
    int secdata_size=decode_lsb_to_size(buff); //passed to above function
    decInfo->sec_data_size=secdata_size;

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo) //decode secret file data from encoded file
{
    decInfo->secret_data=decode_data_to_image(decInfo->sec_data_size,decInfo->fptr_steg_image); //passing to above function to decode
        decInfo->fptr_secret=fopen(decInfo->decode_fname,"w");

    if (decInfo->fptr_secret== NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->decode_fname);

    	return e_failure;
    }
    fwrite(decInfo->secret_data,decInfo->sec_data_size,1,decInfo->fptr_secret); //write the decoded data to new file

    return e_success;
}



