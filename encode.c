/*  Name:Reddy Vasanth Narasimha Reddy
    Batch No:26015B
    ID NO.26012_093
    Date of submission : 07/10/26

    Project Description

            This project implements " Steganography in C "", a technique used to hide secret 
        information inside a BMP image without visibly changing the image. The project uses the 
        LSB (Least Significant Bit) technique to encode the secret data into the image. 
        It supports both " encoding and decoding "" operations. During encoding, the program takes 
        a source BMP image, a secret file, and an output image as input. The secret file extension, 
        file size, and file contents are embedded into the image. During decoding, the hidden information 
        is extracted from the stego image and the original secret file is recovered. The project demonstrates 
        important C programming concepts such as **file handling, bit manipulation, pointers, structures, 
        functions, and command-line arguments.

Sample Input

Encoding:

./a.out -e beautiful.bmp secret.txt stego.bmp

Decoding:

./a.out -d stego.bmp

Sample Output

Encoding:

INFO: Opening source image file
INFO: Checking capacity of source image
INFO: Encoding magic string
INFO: Encoding secret file extension
INFO: Encoding secret file size
INFO: Encoding secret file data
INFO: Encoding completed successfully

Decoding:

INFO: Opening stego image file
INFO: Decoding magic string
INFO: Decoding secret file extension
INFO: Decoding secret file size
INFO: Decoding secret file data
INFO: Decoding completed successfully
Secret file extracted successfully


*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

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

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    // return secret file size
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // step 1 -> check argv[2] is having .bmp or not
        //yes -> store the file name into encInfo -> src_image_fname = argv[2]
        //no - > return e_failure

    // step 2 -> check argv[3] is having extn there or not
        // yes - > store the file name into encInfo -> secret_fname = argv[3]
        // no -> return e_failure
    
    // step 3 -> check argv[4] is NULL or not
        //yes -> check argv[4] is having .bmp or not
                //yes -> store the file name into encInfo -> stego_image_fname = argv[4]
                //no - > return e_failure
        
        //no -> store default name the encInfo -> stego_image_fname = "stego.bmp"


    // return e_success;
    if (strstr(argv[2], ".bmp") != NULL)
        encInfo->src_image_fname = argv[2];
    else
        return e_failure;

    if (strchr(argv[3], '.') != NULL)
    {
    encInfo->secret_fname = argv[3];

    char *dot = strrchr(argv[3], '.');
    strcpy(encInfo->extn_secret_file, dot);
    }
    else
    {
    return e_failure;
    }
    if (argv[4] != NULL)
    {
        if (strstr(argv[4], ".bmp") != NULL)
            encInfo->stego_image_fname = argv[4];
        else
            return e_failure;
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
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

Status check_capacity(EncodeInfo *encInfo)
{
    //step 1 -> encInfo -> image_capacity = call get_image_size_for_bmp(encoInfo -> fptr_src_image)

    //step 2 -> encInfo -> size_secret_file = call get_file_size(encInfo -> fptr_secret)

    //step 3 -> check encInfo -> image_capacity > 16 + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)
                //t -> return e_success;
                //f -> return e_failure
                encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    int required_size = 16 + 32 + (strlen(encInfo->extn_secret_file) * 8)
                        + 32 + (encInfo->size_secret_file * 8);

    if (encInfo->image_capacity > required_size)
        return e_success;
    else
        return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // step 1 -> rewind source file pointer

    // step 2 -> read 54 bytes from source file

    // step 3 -> write 54 bytes to dest file

    // return e_success
    char header[54];
    rewind(fptr_src_image);
    fread(header, 54, 1, fptr_src_image);
    fwrite(header, 54, 1, fptr_dest_image);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(magic_string[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(magic_string) times from step 1
        char buffer[8];

    // Loop through each character of the magic string
    for (int i = 0; i < strlen(magic_string); i++)
    {
        // Step 1: Read 8 bytes from source image
        fread(buffer, 8, 1, encInfo->fptr_src_image);

        // Step 2: Encode this character into the 8 bytes
        encode_byte_to_lsb(magic_string[i], buffer);

        // Step 3: Write the modified buffer into the stego image
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }

    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(size, buffer)

    //step 3 -> write the buffer into dest file
    char buffer[32];

    // Step 1: Read 32 bytes from source image
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        fprintf(stderr, "ERROR: Unable to read 32 bytes from source image\n");
        return e_failure;
    }

    // Step 2: Encode the extension size into the 32 bytes
    if (encode_size_to_lsb(size, buffer) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode extension size into LSBs\n");
        return e_failure;
    }

    // Step 3: Write the modified buffer into the stego image
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        fprintf(stderr, "ERROR: Unable to write 32 bytes into stego image\n");
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    // char buffer[8];

    //step 1 -> read 8 bytes from source file

    //step 2 -> call encode_byte_to_lsb(file_extn[i], buffer)

    //step 3 -> write the buffer into dest file

    // repeat this for strlen(file_extn) times from step 1
    char buffer[8];

    // Loop through each character of the file extension
    for (int i = 0; i < strlen(file_extn); i++)
    {
        // Step 1: Read 8 bytes from source image
        if (fread(buffer, 8, 1, encInfo->fptr_src_image) != 1)
        {
            fprintf(stderr, "ERROR: Unable to read 8 bytes from source image\n");
            return e_failure;
        }

        // Step 2: Encode this character into the 8 bytes
        if (encode_byte_to_lsb(file_extn[i], buffer) == e_failure)
        {
            fprintf(stderr, "ERROR: Failed to encode file extension character\n");
            return e_failure;
        }

        // Step 3: Write the modified buffer into the stego image
        if (fwrite(buffer, 8, 1, encInfo->fptr_stego_image) != 1)
        {
            fprintf(stderr, "ERROR: Unable to write 8 bytes into stego image\n");
            return e_failure;
            }
    }
    return e_success;


}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    // char buffer[32];

    //step 1 -> read 32 bytes from source file

    //step 2 -> call encode_size_to_lsb(file_size, buffer)

    //step 3 -> write the buffer into dest file
        char buffer[32];

    // Step 1: Read 32 bytes from source image
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        fprintf(stderr, "ERROR: Unable to read 32 bytes from source image\n");
        return e_failure;
    }

    // Step 2: Encode the file size into the 32 bytes
    if (encode_size_to_lsb((int)file_size, buffer) == e_failure)
    {
        fprintf(stderr, "ERROR: Failed to encode file size into LSBs\n");
        return e_failure;
    }

    // Step 3: Write the modified buffer into the stego image
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        fprintf(stderr, "ERROR: Unable to write 32 bytes into stego image\n");
        return e_failure;
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //step 1 -> read secret data from file and store it into encInfo -> secret_data

    // char buffer[8];

    //step 2 -> read 8 bytes from source file

    //step 3 -> call encode_byte_to_lsb(encInfo -> secret_data[i], buffer)

    //step 4 -> write the buffer into dest file

    // repeat this for strlen(encInfo -> size_secret_file) times from step 2
    char ch;
    char buffer[8];

    // Loop through each byte of the secret file
    for (long i = 0; i < encInfo->size_secret_file; i++)
    {
        // Step 1: Read one byte of secret data
        if (fread(&ch, 1, 1, encInfo->fptr_secret) != 1)
        {
            fprintf(stderr, "ERROR: Unable to read secret file byte\n");
            return e_failure;
        }

        // Step 2: Read 8 bytes from source image
        if (fread(buffer, 8, 1, encInfo->fptr_src_image) != 1)
        {
            fprintf(stderr, "ERROR: Unable to read 8 bytes from source image\n");
            return e_failure;
        }

        // Step 3: Encode the secret byte into the 8 bytes
        if (encode_byte_to_lsb(ch, buffer) == e_failure)
        {
            fprintf(stderr, "ERROR: Failed to encode secret byte into LSBs\n");
            return e_failure;
            }

        // Step 4: Write the modified buffer into the stego image
        if (fwrite(buffer, 8, 1, encInfo->fptr_stego_image) != 1)
        {
            fprintf(stderr, "ERROR: Unable to write 8 bytes into stego image\n");
            return e_failure;
        }
    }

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // write logic for copy remaining data

    // return e_success
    char buffer[1024];
    size_t bytes_read;

    // Copy all remaining bytes from source image to destination
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr_src)) > 0)
    {
        if (fwrite(buffer, 1, bytes_read, fptr_dest) != bytes_read)
        {
            fprintf(stderr, "ERROR: Unable to write remaining data into stego image\n");
            return e_failure;
        }
    }

    return e_success;  
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // write logic to encode the char
    for (int i = 0; i < 8; i++)
    {
        // Clear the LSB of the current image byte
        image_buffer[i] &= 0xFE;

        // Extract the bit from 'data' (starting from MSB) and set it as LSB
        image_buffer[i] |= ((data >> (7 - i)) & 1);
    }

    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
    // write logic to encode the int
    for (int i = 0; i < 32; i++)
    {
        // Clear the LSB of the current image byte
        imageBuffer[i] &= 0xFE;

        // Extract the bit from 'size' (starting from MSB) and set it as LSB
        imageBuffer[i] |= ((size >> (31 - i)) & 1);
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    //step 1 -> check open_files(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 2 -> check check_capacity(encInfo) is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 3 -> call copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success and goto next step

    //step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)

    //step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)

    //step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)

    //step 7 -> call encode_secret_file_size(encInfo ->  size_secret_file, encInfo)

    //step 8 -> call encode_secret_file_data(encInfo)

    //step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)is returning e_success or not
            //no -> print error return e_failure
            //yes -> print success


    // return e_success
    
    // Step 1: Open files
    if (open_files(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Opening files failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Files opened successfully\n");
    }

    // Step 2: Check capacity
    if (check_capacity(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Image does not have enough capacity\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Image has enough capacity\n");
    }

    // Step 3: Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "ERROR: Copying BMP header failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: BMP header copied successfully\n");
    }

    // Step 4: Encode magic string
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Encoding magic string failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Magic string encoded successfully\n");
    }

    // Step 5: Encode secret file extension size
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Encoding secret file extension size failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Secret file extension size encoded successfully\n");
    }

    // Step 6: Encode secret file extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Encoding secret file extension failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Secret file extension encoded successfully\n");
    }

    // Step 7: Encode secret file size
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Encoding secret file size failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Secret file size encoded successfully\n");
    }

    // Step 8: Encode secret file data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: Encoding secret file data failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Secret file data encoded successfully\n");
    }
    // Step 9: Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "ERROR: Copying remaining image data failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Remaining image data copied successfully\n");
    }
    // If all steps succeed
    return e_success;
}