#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        // Get the user's choice
        string name = get_string("Vote: ");
        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Linear search as the candidate list is not sorted
    for (int i = 0; i < MAX; i++)
    {
        // Stop the iteration at the end of the list
        if (strcasecmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
        if (candidates[i + 1].name == NULL)
        {
            break;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Find the person (people) that has the highest count
    int largest;
    for (int i = 0; i < MAX; i++)
    {
        // Set default value for largest
        if (i == 0)
        {
            largest = candidates[0].votes;
        }
        if (largest < candidates[i].votes)
        {
            largest = candidates[i].votes;
        }
        // Stop the iteration at the end of the list
        if (candidates[i + 1].name == NULL)
        {
            break;
        }
    }
    for (int i = 0; i < MAX; i++)
    {
        if (candidates[i].votes == largest)
        {
            printf("%s\n", candidates[i].name);
        }
        // Stop the iteration at the end of the list
        if (candidates[i + 1].name == NULL)
        {
            break;
        }
    }
    return;
}