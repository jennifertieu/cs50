from cs50 import get_string




def main():
    text = get_string("Text: ")

    # Count letters, words, and sentences of the user's input text
    num_letters = count_letters(text)
    num_words = count_words(text)
    num_sentences = count_sentences(text)

    # Round output grade to the nearest integer
    L = (num_letters / num_words) * 100
    S = (num_sentences / num_words) * 100
    index = 0.0588 * L - 0.296 * S - 15.8

    if round(index) > 16:
        print("Grade 16+")

    elif round(index) < 1:
        print("Before Grade 1")

    else:
        print(f"Grade {round(index)}")



def count_letters(text):
    """
    Return count of letters

    """
    count = 0
    for i in text:
        # Check if character is alphabetical character
        if i.isalpha():
           count += 1
    return count

def count_words(text):
    """
    Return count of words

    """
    count = 1
    for i in text:
        # Check if character is a space
        if i.isspace():
            count += 1
    return count


def count_sentences(text):
    """
    Return count of sentences

    """
    count = 0
    for i in text:
        # Check for punctuations
        if i == "." or i == "!" or i == "?":
            count += 1

    return count

main()