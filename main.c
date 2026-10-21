#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    int op_type=check_operation_type(argv[1]);
    if (op_type == e_encode)
    {
        EncodeInfo encInfo;

        if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            fprintf(stderr, "ERROR: Invalid arguments for encoding\n");
            return 1;
        }
        else
        {
            printf("INFO: Arguments validated successfully\n");
        }

        if (do_encoding(&encInfo) == e_failure)
        {
            fprintf(stderr, "ERROR: Encoding failed\n");
            return 1;
        }
        else
        {
            printf("INFO: Encoding completed successfully\n");
        }
    }
    else if (op_type == e_decode)
    {
        if (argc < 2)
        {
            fprintf(stderr, "ERROR: Missing stego image\n");
            fprintf(stderr, "Usage: %s -d <stego_image.bmp> [output_file]\n", argv[1]);
            return 1;
        }

        char *output_file = NULL;

        if (argc >= 4)
        {
            output_file = argv[3];
        }
        else{

        }
        if (do_decoding(argv[2], output_file,argc) == e_failure)
        {
            fprintf(stderr, "ERROR: Decoding failed\n");
            return 1;
        }
        else
        {
            printf("INFO: Decoding completed successfully\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Unsupported operation. Use -e for encode or -d for decode.\n");
        return 1;
    }

    return 0;
}

OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-e") == 0)
        return e_encode;
    else if (strcmp(symbol, "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}
