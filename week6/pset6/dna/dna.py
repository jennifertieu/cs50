import csv
from sys import argv, exit


# Get the CSV file containing STR counts from first command line argument
# Get the name of text file containg DNA sequences for identify
# from the second command line argument
# Verify that only two command line arguments were inputted
if len(argv) > 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

str_counts = []
dna_seq = {}

# Program should open the csv file and read it into memory
with open(argv[1], "r") as file:
    # Read file into dictionary with first row as columns name
    reader = csv.DictReader(file)
    for data in reader:
        str_counts.append(dict(data))

for STR in str_counts[0].keys():
    if STR != "name":
        dna_seq[STR] = 0

# Open the DNA sequence and read its contents into memory
with open(argv[2], "r") as file:
    for i in file:
        seq = i
        
# Identify the longest run of consecutive sequences
for STR in dna_seq.keys():
    # Set temp DNA seq count
    temp = {STR: 0}
    i = 0
    j = len(STR) + i
    while j < len(seq):
        # If STR found, look for consecutive sequences
        if seq[i:j] == STR:
            temp[STR] += 1
            i += len(STR)
            j = len(STR) + i
        # Update consecutive STR count if temp count is larger
        elif temp[STR] > dna_seq[STR]:
            dna_seq[STR] = temp[STR]
            temp[STR] = 0 # Reset temp count
            i += 1
            j = len(STR) + i
        # Update string index and to try and find STR sequence
        else:
            temp[STR] = 0 # Reset temp count
            i += 1
            j = len(STR) + i
            
match = None
# Determine DNA match
for i in str_counts:
    # print(i)
    if all(int(i[STR]) == count for STR, count in dna_seq.items()):
        match = i["name"]
        break

# Declare match
if match:
    print(match)
else:
    print("No match.")
