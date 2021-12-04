#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Initialize the functions
int count_letters(string user_input);
int count_words(string user_input);
int count_sentences(string user_input);

int main(void)
{
    // Implement a program that computes the approximate grade level needed to comprehend some text. 
    
    // Prompt the user for a string of text (using get_string)
    string user_input = get_string("Text: ");
    
    // Count the letters, words, and sentences of the user text input
    int letters = count_letters(user_input);
    int words = count_words(user_input);
    int sentences = count_sentences(user_input);
    
    // Output grade rounded to the nearest integer
    float L = ((float)letters / (float)words) * 100;
    float S = ((float)sentences / (float)words) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    
    // If the resulting integer is 16 or higher
    if (round(index) > 16)
    {
        printf("Grade 16+\n");
        
    }
    // If the resulting integer is lower than 1
    else if (round(index) < 1)
    {
        printf("Before Grade 1\n");
        
    }
    else
    {
        printf("Grade %.0f\n", round(index));
    }
    

}

// Count the number of letters
int count_letters(string user_input)
{
    int count = 0;
    for (int i = 0; i < strlen(user_input); i++)
    {
        // Checl if character is an alphabetical
        if (isalpha(user_input[i]))
        {
            count += 1;
        }
    }
    return count;
}

// Count the number of words
int count_words(string user_input)
{
    int count = 1; // Count the last word
    for (int i = 0; i <= strlen(user_input); i++)
    {
        // Check for spaces
        if (isspace(user_input[i]))
        {
            count += 1;
        }
    }
    return count;
}

// Count the number of sentences
int count_sentences(string user_input)
{
    int count = 0;
    for (int i = 0; i <= strlen(user_input); i++)
    {
        // Check for spaces
        if (user_input[i] == '.' || user_input[i] == '!' || user_input[i] == '?')
        {
            count += 1;
        }
    }
    return count;
}