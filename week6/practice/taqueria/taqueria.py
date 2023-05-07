menu = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}

bill = 0

while True:
    try:
        order = input("Item: ").title()
    except EOFError:
        print("An error has occurred!")
        break
    except KeyboardInterrupt as e:
        print(e)
        break
    
    # convert the input in to title cased form
    if order in menu:
        bill += menu[order]
        print(f'Total: ${bill:.2f}')
        continue
    else:
        continue