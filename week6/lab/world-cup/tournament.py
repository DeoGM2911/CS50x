# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams: list[dict] = []
    try:
        with open(sys.argv[1], "r") as file:
            reader = csv.DictReader(file)
            for row in reader:
                row["rating"] = int(row["rating"])
                teams.append(row)
    except FileNotFoundError:
        sys.exit(f"There is no file named {sys.argv[1]}!")

    counts: dict = {}
    for _ in range(N):
        winner: str = simulate_tournament(teams)
        counts[winner] = counts.get(winner, 0) + 1
        # The equivalent of:
        #if winner in counts:
        #   counts[winner] += 1
        #else:
        #   counts[winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1: dict, team2: dict) -> bool:
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1, rating2 = team1["rating"], team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams: list[dict]) -> list:
    """Simulate a round. Return a list of winning teams."""
    winners: list[dict] = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams: list[dict]) -> str:
    """Simulate a tournament. Return name of winning team."""
    while len(teams) != 1:
        teams = simulate_round(teams)
    return teams[0]["team"]


if __name__ == "__main__":
    main()
