def main():
    while True:
        try:
            if (owe := float(input("Change owed: "))) >= 0:
                break
        except ValueError:
            pass
        except EOFError:
            pass
    
    print(least_amt(int(owe * 100)))


def least_amt(owe: int) -> int:
    """Calculate the least amount of coins needed

    Args:
        owe (int): the total money owed

    Returns:
        int: the number of coins needed
    """
    count: int = 0
    # Calculate the amount of coins needed for each currency
    count, owe = owe // 25, owe % 25
    count, owe = count + owe // 10, owe % 10
    count, owe = count + owe // 5, owe % 5
    count, owe = count + owe, owe % 1
    return count # Convert as when assign owe to count, count becomes a float


if __name__ == "__main__":
    main()
