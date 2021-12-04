#include <cs50.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    //Prompt user input for a positive integer between 1 and 8
    int input;

    //Use a "Do While" Loop
    //Prompts the user for a number between 1 and 8
    //If the user enters an invalid number the action under do will continue
    // until a correct number is entered (condition is false)

    // Does something first: prompts user for input
    do {
       input = get_int("Height:");
    }
    //Checks the condition: A positive int between 1 and 8 inclusive
    //And repeats prompt until the condition is no longer true
    while (input < 1 || input > 8);


    //Create index to track the hashes in each row
    int index = 0;
    int space = input - 1;

    // Print input number of hashes
    for(int i = 0; i < input; i++){
        // Add an inner for loop to create a matrix of
        // hashes to construct the pyramid by printing
        // the input number of hashes in rows

        // Print spaces to format the left pyramid
        for(int s = 0; s < space; s++){
            printf(" ");
        }

        // Create left pyramid
        for(int j = 0; j <= index; j++){
            printf("#");
        }

        // Print spaces between the right and left pyramid
        printf("  ");

        // Create right pyramid
       for(int n = 0; n <= index; n++){
            printf("#");
        }

        //Print a new line after each row
        printf("\n");

        // Increase the number of hashes by 1
        index++;
        // Decrease the number of spaces
        space--;


    }


}
