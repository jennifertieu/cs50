#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void){

    // Declare user input
    float input;
    // Declare coin count
    int count = 0;
    // Check if user input is a
    // non-negative number
    do {
        input = get_float("Change owed:");
    }
    while(input < 0);
    // round input to the nearest cent
    int cents = round(input * 100);

    // Determine the minimum number of coins
    // that the change can be made
    do{
        if (cents >= 25){
            cents -= 25;
            count++;
        } else if (cents >= 10){
            cents -= 10;
            count++;
        } else if (cents >= 5){
            cents -= 5;
            count++;
        } else if (cents >= 1) {
            cents -= 1;
            count++;
        }
    } 
    while(cents != 0);
    printf("Number of coins: %i\n", count);

    
}


