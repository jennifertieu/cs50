from cs50 import get_int

height = get_int("Height: ")

# Reprompt user if input is not a positive integer not greater than 8
while (height < 1 or height > 8):
    height = get_int("Height: ")

hashes = 1
spaces = height - 1
for i in range(height):
    # Use end parameter to remove new line after print 
    for s in range(spaces):
        print(" ", end="")
        
    for j in range(hashes):
        print("#", end="")
        
    # Decrease spaces    
    spaces -= 1
    # Increase hashes
    hashes += 1
    # Add new line
    print()