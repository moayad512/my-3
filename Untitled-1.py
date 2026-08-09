# # # # # # print(5//2)
# # # # # nums = [1, 2, 3, 4]
# # # # # result = [x**2 for x in nums if x % 2 == 0]
# # # # # print(result)
# # # # try:
# # # # result = 10 / 0
# # # # except ZeroDivisionError:
# # # # result = "Error"
# # # # finally:
# # # # print("Done")
# # # # print(result)
# # # from curses import wrapper


# # # def decorator(func):
# # #     def wrapper():
# # #        return func() * 2 
# # #  return wrapper

# # # @decorator
# # # def say_hello():
# # #     return "Hello"

# # # print(say_hello())
# # import re
# # text = "Python 3.9"
# # result = re.findall(r"d", text)
# # print(result)
# text = "Python"
# print(text[::-1])
# text = "Python"
# print(text[::-1])
# x = 5
# y = 10
# x, y = y, x
# print(x, y)
numbers = [1, 2, 3]
numbers.append(4)
print(numbers)