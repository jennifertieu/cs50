from cs50 import get_float

change_input = get_float("Changed owed: ")

# Reprompt user if input is a non-negative number
while (change_input < 0):
    change_input = get_float("Changed owed: ")
    
cents = round(change_input * 100)
coins = 0
while (cents != 0):
    # Starting with the largest coin
    coins += cents // 25;
    cents = cents % 25;
    
    coins += cents // 10;
    cents = cents % 10;
    
    coins += cents // 5;
    cents = cents % 5;
    
    coins += cents // 1;
    cents = cents % 1;

print(coins)
    