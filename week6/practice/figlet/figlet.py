# python figlet.py (ARGS here)
# if run immediately, the font will be randomly chosen
import sys
from pyfiglet import Figlet
import random as rd

fig = Figlet()

# Exit the program if the usage is incorrect
if len(sys.argv) not in [1, 3]:
    sys.exit("Usage: python figlet.py [-f or --font] [font_name]")

# Choose a random font
if len(sys.argv) == 1:
    text = input("Input: ")
    _font = rd.choice(fig.getFonts())
    fig.setFont(font=_font)
    print(f"Output: {fig.renderText(text)}")

# Print the text with the choosen font
else:
    if sys.argv[1] == '-f' or sys.argv[1] == '--font':
        if sys.argv[2] in fig.getFonts():
            text = input("Input: ")
            fig.setFont(font=sys.argv[2])
            print(f"Output: {fig.renderText(text)}")
        else:
            sys.exit("No such font detected!")
    else:
        print(f"Invalid command line argument: {sys.argv[1]}")
