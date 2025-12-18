#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

OperationType check_operation_type(char *argv[])  //function for checking decode or encode 
{
    if(strcmp(argv[1],"-e")==0) 
    {
        return e_encode;
    }
    if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr) //function for getting file size
{
    fseek(fptr,0,SEEK_END);  //points to file ending
    return ftell(fptr);  //return current file pointer(size of file)
    rewind(fptr);   //points to starting
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)  //function for opening file
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)  //validate file name are valid
{  
        if(strcmp(&argv[2][strlen(argv[2])-4],".bmp")==0) //validate 3rd argvector should be .bmp file 
        {
            encInfo->src_image_fname=argv[2];
        }
        else {
        printf("bmp file type error\n");
        return e_failure;}
        if(argv[3] && strcmp(&argv[3][strlen(argv[3])-4],".txt")==0) //strcmp(&argv[3][strlen(argv[3])-4],".txt")==0 
        {                                                       //validate 4th argvector should be .txt file
            encInfo->secret_fname=argv[3];
            strcpy(encInfo->extn_secret_file,".txt");
        }
        else{
        printf("text file type error\n");
        return e_failure;}
    //if(argv[4] && strstr(argv[4],".bmp")!=NULL)  //strcmp(&argv[4][strlen(argv[4])-4],".bmp")==0
    if(argv[4] && strcmp(&argv[4][strlen(argv[4])-4],".bmp")==0)  //if 5th argument is present,it should be file name for encoded file
    {
        encInfo->stego_image_fname=argv[4];
        //if user passes,validate it
    }
    else    //else create a default file 
    {
        encInfo->stego_image_fname="stego.bmp";
    }
    printf("read and validate complete! ✔\n");
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo) //full decoding function is contained in this function
{
    if(open_files(encInfo)==e_success) //function call for opening file 
    {
        printf("File open successfully ✔\n");
    }
    else{
        printf("Failure: File open unsuccessfully\n");
        return e_failure;
    }

    if(check_capacity(encInfo)==e_success)  //function call for check capacity 
    {
        printf("capacity within limits ✔\n");
    }
    else{
        printf("capacity exceeds limit");
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success) //function call for coping header from source file to encoding file
    {
        printf("header copying done! ✔\n");
    }else{printf("Failed coping header\n");return e_failure;}

    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success) //enoding magic string to encoding file
    {
        printf("Magic string encoded ✔\n");
    }else{printf("Failed encoding magic string\n");return e_failure;}

    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success) //encoding secret file extension size to encoding file
    {
        printf("encoded secret file extention size ✔\n");
    }else{printf("Failed encoding extention size.\n");return e_failure;}


    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)  //encoding secret file extension to encoding file
    {
        printf("encoded secret file extention ✔\n");
    }else{printf("Failed encoding extention.\n");return e_failure;}

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success) //encoding secret file size to encoding file
    {
        printf("encoded secret file size ✔\n");
    }else{printf("Failed encoding file size.\n");return e_failure;}

    if(encode_secret_file_data(encInfo)==e_success)  //encoding secret file size to encodingfile
    {
        printf("encoded secret file data ✔\n");
    }else{printf("Failed encoding file data.\n");return e_failure;}

    //encode_secret_file_size(encInfo->size_secret_file,encInfo);

    //encode_secret_file_data(encInfo);

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success) //coping remaining data from source file to encoding file
    {
        printf("copyng remaining data. ✔\n");
    }
    else{ printf("failed coping remaining data.\n");return e_failure; }
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)   
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image); //getting image size
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret); //get secret file size
    if(encInfo->image_capacity>16+32+32+32+(encInfo->size_secret_file*8)) //checking image capacity to ensure data can be encoded within limit
    {
        return e_success;
    }
    return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image) //coping exact header so the image basic data(width,height,size) is not changed
{
    char buff[54];  //header file is from 0-54th bit,so taking char array of size 54
    fseek(fptr_src_image,0,SEEK_SET); //points to starting of source file
    fread(buff,54,1,fptr_src_image); //copying char from source to array
    fwrite(buff,54,1,fptr_stego_image); //write from array to encoding file
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)  //function for encoding magicstring
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[8]; //char array of size 8
    for(int i=0;i<size;i++) //size of magic string (2 Byte, 2 character)
    {
        fread(str,8,1,fptr_src_image); //read 8 byte from source
        encode_byte_to_lsb(data[i],str); //passing function to encode data to LSB
        fwrite(str,8,1,fptr_stego_image); //write the encoded data to encoding file
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer) 
{
    for(int i=0;i<8;i++) 
    {
        image_buffer[i]=(image_buffer[i]&0xFE) | ((data>>i)&1); //encoding data to LSB
    }
    return e_success;
}

Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo) //function to encode secret file extn size
{
    char str[32]; //char array of 4 Bytes (integer value)
    fread(str,32,1,encInfo->fptr_src_image); //read from source and store it in array
    encode_size_to_lsb(size,str); //Passing to the function
      /*for(int i=0;i<32;i++)
        {
            str[i]=(str[i]&0xFE) | ((strlen(data)>>i)&1);
        } */
    fwrite(str,32,1,encInfo->fptr_stego_image ); //write to encoding file

    return e_success;
}

Status encode_size_to_lsb(int size,char* str)
{
    for(int i=0;i<32;i++) // for 4 Byte
    {
        str[i]=(str[i]&0xFE) | ((size>>i)&1);  //encode data to LBS
    }
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image); //function to encode data 

    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo) 
{
    encode_secret_file_extn_size(encInfo->size_secret_file,encInfo); //passed to above function (same logic)

    return e_success;
} 

Status encode_secret_file_data(EncodeInfo *encInfo) 
{
    rewind(encInfo->fptr_secret); //setting the pointer to starting of secret file
    char secdata[encInfo->size_secret_file];//secdata[encInfo->size_secret_file]='\0';  //create character array of secret data size
    fread(secdata,encInfo->size_secret_file,1,encInfo->fptr_secret);
    //printf("%s\n",secdata);
    encode_data_to_image(secdata,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image); //passing to function to encode data

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) 
{
    int ch;
    while((ch=fgetc(fptr_src))!=EOF) //get each charater from source and print it into encoding file
    {
        putc(ch,fptr_dest);
    }
    return e_success;
} 






