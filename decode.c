#include "decode.h"
#include <string.h>


char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data = (data << 1) | (image_buffer[i] & 1);
    }
    return data;
}

int decode_size_from_lsb(char *image_buffer)
{
    int size = 0;
    for (int i = 0; i < 32; i++)
    {
        size = (size << 1) | (image_buffer[i] & 1);
    }
    return size;
}

Status decode_magic_string(FILE *fptr_stego_image, const char *expected_magic)
{
    char buffer[8];
    char decoded_char;

    for (int i = 0; i < strlen(expected_magic); i++)
    {
        fread(buffer, 8, 1, fptr_stego_image);
        decoded_char = decode_byte_from_lsb(buffer);

        if (decoded_char != expected_magic[i])
        {
            fprintf(stderr, "ERROR: Magic string mismatch\n");
            return e_failure;
        }
    }
    return e_success;
}

char *decode_secret_file_extn(FILE *fptr_stego_image, int extn_size)
{
    char buffer[8];
    char *extn = malloc(extn_size + 1);

    for (int i = 0; i < extn_size; i++)
    {
        fread(buffer, 8, 1, fptr_stego_image);
        extn[i] = decode_byte_from_lsb(buffer);
    }
    extn[extn_size] = '\0';
    return extn;
}

long decode_secret_file_size(FILE *fptr_stego_image)
{
    char buffer[32];
    fread(buffer, 32, 1, fptr_stego_image);
    return decode_size_from_lsb(buffer);
}

Status decode_secret_file_data(FILE *fptr_stego_image, FILE *fptr_output, long size)
{
    char buffer[8];
    char ch;

    for (long i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_stego_image);
        ch = decode_byte_from_lsb(buffer);
        fwrite(&ch, 1, 1, fptr_output);
    }
    return e_success;
}

Status do_decoding(const char *stego_fname, const char *output_fname,int argc)
{
    FILE *fptr_stego = fopen(stego_fname, "rb");
    FILE *fptr_output;
    if (!fptr_stego)
    {
        fprintf(stderr, "ERROR: Unable to open stego image\n");
        return e_failure;
    }

    // Skip BMP header (54 bytes)
    fseek(fptr_stego, 54, SEEK_SET);

    // Step 1: Decode magic string
    if (decode_magic_string(fptr_stego, MAGIC_STRING) == e_failure)
    {
        fclose(fptr_stego);
        return e_failure;
    }

    // Step 2: Decode extension size
    char buffer[32];
    fread(buffer, 32, 1, fptr_stego);
    int extn_size = decode_size_from_lsb(buffer);

    // Step 3: Decode extension
    char *extn = decode_secret_file_extn(fptr_stego, extn_size);

    // Step 4: Decode file size
    long secret_size = decode_secret_file_size(fptr_stego);
    if(argc==3)
    {
        char file[100]="Output";
        strcat(file,extn);
        fptr_output=fopen(file,"wb");
        if(fptr_output==NULL)
        {
            printf("Error : to open file\n");
            return e_failure;
        }
    }
    else
    {
        if(strstr(output_fname,extn)==NULL)
        {
            printf("Input file extension and stego extension are not match.\n");
            printf("Found Extens -> %s\n",extn);
            return e_failure;
        }
        fptr_output = fopen(output_fname, "wb");
        if (!fptr_output)
        {
            fprintf(stderr, "ERROR: Unable to create output file\n");
            fclose(fptr_stego);
            return e_failure;
        }
    }
    // Step 5: Decode file data
    decode_secret_file_data(fptr_stego, fptr_output, secret_size);

    printf("INFO: Decoding completed successfully. Secret file recovered as %s%s\n",
           output_fname, extn);

    free(extn);
    fclose(fptr_stego);
    fclose(fptr_output);

    return e_success;
}
