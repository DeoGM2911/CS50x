def main():
    # Prompting the user's input
    while True:
        card: str = input("Number: ").strip()
        try:
            if int(card) > 0:
                break
        except ValueError:
            pass
        except EOFError:
            pass

    # Check and print the card
    print(validate(card))


def validate(card: str) -> str:
    length: int = len(card)
    tot: int = 0

    # Calculate the total according to the Luhn's Algorithm
    for i in range(1, length + 1):
        if i % 2 == 0:
            tot += sum(divmod(2 * int(card[-i]), 10))
        else:
            tot += int(card[-i])

    # Check the legitimacy of the card
    if tot % 10 != 0:
        return "INVALID"

    # Decide what the credit card is
    if (length == 15) and (card[:2] in ["34", "37"]):
        return "AMEX"
    if (length == 16) and (51 <= int(card[:2]) <= 55):
        return "MASTERCARD"
    if (length in [13, 16]) and card[0] == "4":
        return "VISA"
    else:
        return "INVALID"


if __name__ == "__main__":
    main()