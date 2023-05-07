import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader: csv.DictReader) -> dict[str, list[int]]:
    new: dict[str, list[int]] = {}
    prev: dict[str, list[int]] = {}
    day_count = 0
    
    # Write in the prev dictionary the most 15 recent days
    for row in reversed(list(reader)):
        # Read data from 56 states in 15 days
        day_count += 1
        if day_count > 15 * 56: 
            break
        if row["state"] in prev:
            prev[row["state"]].append(int(row["cases"]))
        else:
            prev[row["state"]] = [int(row["cases"])]
    
    # Calculate the difference and write into the new dict.
    for state, data in prev.items():
        for i in range(14):
            if state in new:
                new[state].append(data[13 - i] - data[14 - i])
            else:
                new[state] = [data[13 - i] - data[14 - i]]
    return new


# Calculate and print out seven day average for given state
def comparative_averages(new_cases: dict[str, list[int]], states: list[str]) -> None:
    for state in states:
        this = 0
        prev = 0
        for i in range(14):
            if i < 7:
                prev += new_cases[state][i]
            else:
                this += new_cases[state][i]
        try:
            perc = (this - prev) / prev
        except ZeroDivisionError:
            print(f"This week {state} has a 7-day average of {this} new cases!")
        if perc >= 0:
            print(f"{state} has a 7-day average of {this} and an increase of {perc:.2f}%.")
        else:
            print(f"{state} has a 7-day average of {this} and a decrease of {-perc:.2f}%.")


if __name__ == "__main__":
    main()
