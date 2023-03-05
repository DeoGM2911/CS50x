import csv
import sys


def main():
    # Check for command-line usage
    while True:
        if len(sys.argv) != 3:
            print("Invalid command-line argument!")
            return
        else:
            break
    # Read database file into a variable
    people: dict[str, dict[str, int]] = {}
    sub_seqs: list = []
    
    with open(sys.argv[1], "r") as database:
        reader = csv.DictReader(database)
        for person in reader:
            people[person["name"]] = {sub_seq: int(num) for sub_seq, num in person.items() if sub_seq != "name"}
            sub_seqs = [sub_seq for sub_seq in person.keys() if (sub_seq != "name" and sub_seq not in sub_seqs)]
    
    # Read DNA sequence file into a variable
    sequence_list: list = []
    
    with open(sys.argv[2], "r") as file:
        for line in file:
            sequence_list.append(line)
    
    seq: str = "".join(sequence_list)

    # Find longest match of each STR in DNA sequence
    results: dict = {}
    
    for dna_str in sub_seqs:
        results[dna_str] = longest_match(seq, dna_str)
    # Check database for matching profiles
    number: int = len(sub_seqs)
    matched: int = 0
    
    for person in people:
        for sub_seq in people[person]:
            if people[person][sub_seq] == results[sub_seq]:
                matched += 1
        if matched == number:
            print(person)
            return
        else:
            matched = 0
    print("No match")


def longest_match(sequence: str, subsequence: str) -> int:
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run: int = 0
    subsequence_length: int = len(subsequence)
    sequence_length: int = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count: int = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start: int = i + count * subsequence_length
            end: int = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run: int = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
