class Jar:
    def __init__(self, capacity: int = 12):
        self.capacity = capacity
        # A new jar is empty
        self._size = 0

    def __str__(self):
        return "🍪" * self.size

    def deposit(self, n: int):
        if n < 0:
            raise ValueError("Not a valid number!")
        self.size += n

    def withdraw(self, n: int):
        if n < 0:
            raise ValueError("Not a valid number!")
        self.size -= n


    @property
    def capacity(self):
        return self._capacity

    @capacity.setter
    def capacity(self, capacity: int):
        if int(capacity) <= 0:
            raise ValueError("Not a valid capacity!")
        self._capacity = capacity

    @property
    def size(self):
        return self._size

    @size.setter
    def size(self, size: int = 0):
        if (size < 0) or (size > self.capacity): # Check for valid number of cookies in the jar
            raise ValueError("Not a valid number!")
        self._size = size