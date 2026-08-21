class Food:
    def __init__(self, name, price):
        self.name = name
        self.price = price
        print(f"{self.name} is created from Food Base Class")

    def eat(self):
        print("Eat Method From Base Class")


class Apple(Food):
    def __init__(self, name, price, amount):
        super().__init__(name, price)
        self.amount = amount
        print(
            f"{self.name} is created from Derived Class "
            f"and price is {self.price} and amount is {self.amount}"
        )

    def get_from_tree(self):
        print("Get From Tree From Derived Class")


food_two = Apple("Pizza", 150, 500)
food_two.eat()
food_two.get_from_tree()