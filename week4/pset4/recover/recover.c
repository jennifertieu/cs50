#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    
    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    
    // Open file and read image memory card
    if (file != NULL) 
    {
        BYTE buffer[512];
        char filename[8];
        int jpeg_file_num = -1;
        FILE *img;
        // Read memory card into buffer array of bytes in 512 byte chunks
        // Your program should number the files it outputs by naming each ###.jpg, 
        // where ### is three-digit decimal number from 000 on up. (Befriend sprintf.)
        // JPEG file or the end of the memory card
        while (fread(buffer, 1, 512, file))
        {
            // Iterate through memory card and look for JPEG images using
            // the first four bytes: 0xff 0xd8 0xff and 0xe0...(first four bits are 1110)
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                (buffer[3] & 0xff) == 0xe0) // Bitwise arithmetic to set the remaining four bits to zero
            {

                // If its the first jpeg file
                // Then read that JPEG image in chunks of 512 bytes until you reach another
                // storing each as a separate file in your current working directory.
                if (jpeg_file_num == -1)
                {
                    // Set jpeg file num to 0 for the first file
                    jpeg_file_num += 1;
                    sprintf(filename, "%03i.jpg", jpeg_file_num);
                    img = fopen(filename, "w");
                    if (img != NULL)
                    {
                        fwrite(buffer, sizeof(unsigned char), 512, img);
                    }

                }
                // Add bytes to new jpeg file
                else
                {
                    // Update jpeg file num
                    jpeg_file_num += 1;
                    sprintf(filename, "%03i.jpg", jpeg_file_num);
                    img = fopen(filename, "w");
                    if (img != NULL)
                    {
                        fwrite(buffer, sizeof(unsigned char), 512, img);
                    }
                }
  

            }
            // If already a jpeg file, then continue reading bytes into jpeg file
            else if (jpeg_file_num >= 0)
            {
                if (img != NULL)
                {
                    fwrite(buffer, sizeof(unsigned char), 512, img);
                }
                
            }

        }
        
        // Close files
        fclose(file);
        fclose(img);
        
    }
    else
    {
        printf("Unable to open file\n");
        return 1;
    }
    

    
    
}
