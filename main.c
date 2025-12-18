/*
    Name:Yadhu Krishnan

    Date:05/12/25

    Description: Implemented a steganography Project that encode secret data into BMP images and 
    decode it to the original file.

    Working:In encoding, a BMP image and a text file are given as input, 
    and the secret data is embedded into the image’s RGB values to generate a new encoded BMP file.
    In decoding, the encoded BMP file is input, and the hidden data is extracted and saved back into its original file format.
 */

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

int main(int argc,char *argv[])
{
   EncodeInfo encode;      // structure variable
   DecodeInfo decode;       
   if(argc<3)             //check for command line argument count
   {
    printf("Invalid CLA input\n");
    return 0;
   }
   if(check_operation_type(argv)==e_encode)  //select encode or decode based on user input(argument vector)
   {
    printf("encoding..\n");
    if(read_and_validate_encode_args(argv,&encode)==e_success) //check file's are in correct format
    {
        printf("************started encoding*************\n");
         if(do_encoding(&encode)==e_success)    //encoding function call(where rest of encoding is done)
         {
            printf(GREEN"ENCODED SUCCESFULLY! ✔\n"RESET); //prompt message for succesfull completion of encoding
         }
         else{
             printf(RED"ENCODED FAILED! ✘\n"RESET);  
         }
     }
    }

    else if(check_operation_type(argv)==e_decode) //if user given '-d' as argv[2] then condition become true, decoding condition
    {
     printf("decoding..\n");
     if(read_and_validate_decode_args(argv,&decode)==e_success)   //check file's are in correct format
     {
         printf("************started decoding*************\n");
         if(do_decoding(&decode)==e_success)        //decoding function call(where rest of decoding is done)
         {
             printf(GREEN"DECODED SUCCESFULLY! ✔\n"RESET);
         }
         else{
             printf(RED"DECODING FAILED! ✘\n"RESET);
         }
     }
    }
    else if(check_operation_type(argv)==e_unsupported) //if user entered other than '-e' or '-d' as argv[2],condition become true
    {
    printf(RED"Invalid command! try:\n'-e' : encode\n'-d' : decode\n"RESET); //prompt message showing invalid
    }
    return 0;
}


