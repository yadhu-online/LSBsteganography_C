/*#ifndef DECODE_H
#define DECODE_H */

#include "types.h"

/* 
 * Structure to store information required for
 * decoding source bmp image to secret file
 * Info and intermediate data is
 * also stored
 */

typedef struct DecodeInfo
{
    char* steg_image_fname;
    //char* decode_fname;
    char decode_fname[255]; 
    FILE* fptr_steg_image;  //pointer of .bmp file
    int  extn_steg_size;   //extension size
    int  sec_data_size;   //secret data size
    char *secret_data;
    FILE* fptr_secret;
}DecodeInfo; 

//function prototype for decoding 

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo); // not done

Status open_files_dec(DecodeInfo *decInfo);

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

char* decode_data_to_image(int size, FILE *fptr_steg_image);

char decode_byte_to_lsb(char *data);

//Status decode_secret_file_extn_size(FILE *fptr_steg_image);
Status decode_secret_file_extn_size(DecodeInfo *decInfo); //size=4;

int decode_lsb_to_size(char* str);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);


