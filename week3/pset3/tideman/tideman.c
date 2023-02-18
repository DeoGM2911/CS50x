#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
// Number of "edges" for MAX "nodes"
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        // Update the preferences for each voter
        record_preferences(ranks);
        // For the sake of beautiful CLI
        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Linear search to check the name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate over each candidate then compare each candidate with others
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // ranks[i] beats all ranks[j] where j > i
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Find the winner
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Compare then choose a winner in a pair
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;  // Increment the arr_index in pairs
            }
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;  // Increment the arr_index in pairs
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sort using the Selection Sort algorithm
    int largest;
    pair pairholder;
    int arr_index;
    // The declarations here are for readability
    int x;
    int y;
    for (int i = 0; i <= pair_count; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            x = preferences[pairs[i + j].winner][pairs[i + j].loser];
            y = preferences[pairs[i + j].loser][pairs[i + j].winner];
            if (j == 0)
            {
                largest = x - y;
                continue;
            }
            if ((x - y) > largest)
            {
                largest = x - y;
                pairholder = pairs[i + j];
                arr_index = i + j;
            }
            // Swap pairs
            pairs[arr_index] = pairs[i];
            pairs[i] = pairholder;
        }   
    }
    return;
}

// Check if the pair is in a cycle
bool cycle(int winner, int loser)
{
    while ((winner != -1) && (winner != loser))
    {
        bool potential_loop = false;
        // Check until i is the loser's index. If i != loser, no cycle exists for the pair.
        // This if (or the cycle) only exists when at least 2 lines are drawn.
        for (int i = 0; i < candidate_count; i++)
        {
            // Check whether candidate i and winner and loser create a cycle
            // This can only be a loop if n - 1 lines has been drawn
            if (locked[i][winner]) 
            {
                potential_loop = true;
                // If there is a cycle, then there is a pair in which winner will become the loser, and loser is the winner
                winner = i;
            }
        }
        // if no cycle found
        if (!potential_loop)
        {
            // Break condition and make sure winner != loser
            winner = -1;
        }
    }
    // Ignore the main diagonal (self-matchup) and create a cycle
    if (winner == loser)
    {
        return true;
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // The winner is the person whose "loser column" only contains false
    for (int i = 0; i < candidate_count; i++)
    {
        bool val = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                val = false;
                break;
            }
        }
        // Print out the winner if he/she has all of the columns true (except for the matchup "between himself/herself")
        if (val)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}