def main():
    # Prompting until encounter a valid input
    while True:
        try:
            h = int(input("Height: "))
            if 0 < h <= 8:
                break
        except ValueError:
            print("Invalid height.")
    draw(h)
    # draw_recur(h, 2 * h + 2)


def draw(height: int) -> None:
    # Draw the pyramid line by line
    for i in range(1, height + 1):
        print(" " * (height - i) + "#" * i, end="  ")
        print("#" * i)


def draw_recur(lvl: int, h: int) -> None:
    # This function is the recursive version of draw
    if lvl == 0:
        return
    draw_recur(lvl - 1, h)
    print(" " * (h - lvl) + "#" * lvl, end="  ")
    print("#" * lvl)


if __name__ == "__main__":
    main()