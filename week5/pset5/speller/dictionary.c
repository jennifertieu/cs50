// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26; // letters of the alphabet, zero indexed

// Hash table
node *table[N-1];

// Hash table size
unsigned int hash_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
    unsigned int hash_value = hash(word);
    // Point cursor to linked list
    node *cursor = table[hash_value];

    // Access linked list at that index in the hash table
    while (cursor != NULL)
    {
        // Traverse linked list, looking for the word (strcasecmp)
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }


        // Update cursor
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 hash function
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned int hash = 5381;
    int c;

    while ((c = *word++)) // word character incremented until null, word dereferenced
    {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Hash function: takes a word as input, outputs a number corresponding to which "bucket"
    // to store the word in

    // Open a dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file != NULL)
    {
        char word[LENGTH + 1];
        // Read strings from file one at a time
        while (fscanf(file, "%s", word) != EOF) // Will return EOF for end of file
        {
            // Create a new node for each word
            // Use malloc to allocate memory for the node
            // check for return value
            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                return false;
            }

            // Copy word into node using strcpy
            strcpy(new_node->word, word);
            // Set new node next pointer to NULL
            new_node->next = NULL;
            // Hash word to obtain hash value
            unsigned int hash_value = hash(word);
            // Set next pointer to the first element in the linked list
            new_node->next = table[hash_value];
            // Reset linked list to point to new node
            table[hash_value] = new_node;
            // Update hash size
            hash_size++;

        }

        // Close file
        fclose(file);
        // Return true if successful
        return true;

    }

    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return hash_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Set cursor and temp to the same node
        node *cursor = table[i];
        while (cursor != NULL)
        {
            // Set temp to point as the same node as pointer
            node *tmp = cursor;
            // Set cursor to the next element in the linked list
            cursor = cursor->next;
            // Free temp
            free(tmp);
        }

        // If last element in the hash table and
        // cursor is NULL
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}
