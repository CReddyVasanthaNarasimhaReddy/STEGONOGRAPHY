#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// Function prototypes
char decode_byte_from_lsb(char *image_buffer);
int decode_size_from_lsb(char *image_buffer);

Status decode_magic_string(FILE *fptr_stego_image, const char *expected_magic);
char *decode_secret_file_extn(FILE *fptr_stego_image, int extn_size);
long decode_secret_file_size(FILE *fptr_stego_image);
Status decode_secret_file_data(FILE *fptr_stego_image, FILE *fptr_output, long size);

Status do_decoding(const char *stego_fname, const char *output_fname,int);

#endif
