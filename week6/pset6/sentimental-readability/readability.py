def main():
    # Prompt for text
    text: str = input("Text: ").strip()

    # Count the number of letters, words, and sentences
    s: int = text.count(".") + text.count("!") + text.count("?")
    w: int = text.count(" ") + 1
    c: int = 0
    for char in text:
        if char.isalnum():
            c += 1

    # Calculate and print out the result
    index: int = round(0.0588 * 100 * (c / w) - 0.296 * 100 * (s / w) - 15.8)
    if 16 > index >= 1:
        print(f" Grade {index}")
    elif index < 1:
        print("Before Grade 1")
    else:
        print("Grade 16+")


if __name__ == "__main__":
    main()