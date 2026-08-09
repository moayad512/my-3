# ----------------------------------
# -- Date and Time => Format Date --
# ----------------------------------
# https://strftime.org/
# ---------------------

import datetime

myBirthday = datetime.datetime(2003, 7, 1)

print(myBirthday)
print(myBirthday.strftime("%a"))
print(myBirthday.strftime("%A"))
print(myBirthday.strftime("%b"))
print(myBirthday.strftime("%B"))

print(myBirthday.strftime("%d %B %Y"))
print(myBirthday.strftime("%d, %B, %Y"))
print(myBirthday.strftime("%d/%B/%Y"))
print(myBirthday.strftime("%d - %B - %Y"))
print(myBirthday.strftime("%B - %Y"))