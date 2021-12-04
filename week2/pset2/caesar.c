#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, string argv[])
{
    /* Take in a single command line argument, k,
    a non-negative integer*/
    int k;

    // printf("%s\n", argv[1]);

    /* If the program is executed without any command line arguments
    or more than one command line argument, the program should print an error
    message of your choice (with printf) and return from "main" a value of 1
    which tends to signify an error) immediately */
    if (argc != 2)
    {
        printf("Please enter one integer.\n");
        return 1;
    }
    /* If any of the characters of the command-line argument is not a decimal digit
    your program should print the message "Usage: ./caesar key" and return from main
    a value of 1*/
    else if (argc == 2)
    {
        // Iterate through the string
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            // If character is not a digit
            if (!isdigit(argv[1][i]))
            {
                printf("Usage: ./caesar key\n");
                return 1;

            }
            else
            {
                // Convert string to digit
                k = atoi(argv[1]);
                printf("Key: %i\n", k);
            }
        }
        // If command line argument is a digit, assign key value and print key
    }
    else
    {
        printf("Please re-enter command line argument.\n");
    }


    /* Do not assume that "k" will be less than or equal to 26. Program should work
    for all non-negative integral values of "k" less than 2^31-26*/

    /* Program must output "plaintext:" (without a newline) and then prompt the user
    for a "string" of plaintext (using get_string)*/
    string user_input = get_string("plaintext:");
    string cipher_text = user_input; // Declare cipher text char array length

    /* Program must preserve case: capitalized letters, though rotated, must remain
    capitalized letters; lowercase letters, though rotated, must remain lowercase letters*/
    // Change user input text to cipher text
    for (int i = 0; i < strlen(user_input); i++)
    {
        // Check if the input is an alpha character
        if (isalpha(user_input[i]) && isupper(user_input[i]))
        {
            // Declare starting character for upper case alphabet
            char A = 65;
            char cipher = A + (((user_input[i] - A) + k) % 26);
            cipher_text[i] = cipher;

            // Check if the character is lower case
        }
        else if (isalpha(user_input[i]) && islower(user_input[i]))
        {
            // Declare starting character for lower case alphabet
            char a = 97;
            char cipher = a + (((user_input[i] - a) + k) % 26);
            cipher_text[i] = cipher;

            // Do not change if not an alpha character
        }
        else
        {
            cipher_text[i] = user_input[i];
        }

    }

    /* Program must output "cipher text:" (wuthout a newline) followed by the plaintext's
    corresponding ciphertext, with each alphabetical characterin the plaintext "rotated"
    by k positions: non-alphabetical characters should be outputted unchanged. */
    printf("ciphertext: %s\n", cipher_text);

    /* After outputting ciphertext, print a newline. Program should exit by returning
    0 from main */
    return 0;


}