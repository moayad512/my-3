# -----------------------------------
# -- Modules => Create Your Module --
# -----------------------------------
import momo
import sys
# sys.path.append(r"mm:\momo.py")
# print(sys.path)

print(dir(momo))

momo.seyHallo("Ahmed")
momo.seyHallo("Osama")
momo.seyHallo("Mohamed")

momo.seyHawAreYou("Ahmed")
momo.seyHawAreYou("Osama")
momo.seyHawAreYou("Mohamed")

# Alias

import momo as ee

ee.seyHallo("Ahmed")
ee.seyHallo("Osama")
ee.seyHallo("Mohamed")

ee.seyHawAreYou("Ahmed")
ee.seyHawAreYou("Osama")
ee.seyHawAreYou("Mohamed")

# from momo import sayHello

# sayHello("Osama")

# from momo import sayHello as ss

# ss("Osama")