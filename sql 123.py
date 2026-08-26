

# Input big Massege
input_massege = """
What Do You Want To Do ?
"s" => Show All Skills 
"a" => Add New Skill 
"d" => Delete A Skill
"u" => Update A Skill
"q" => Quit The App
Choose An Option :
"""

#  input option choise
user_input = input(input_massege).strip().lower()

# print (user_input)

# commend list 
commend_list = ["s", "a", "d" , "u" , "     q" ]

# Define If Commend IS exists
def show_skills():
    print(
        "Showing all skills..."  )
def add_skill():
    print(
        "Adding new skill..."   )
def delete_skill():
    print( "Deleting skill..." )
def update_skill():
    print(        "Updating skill..." )

def update_skills():
    print(
        "Updating all skills...")

# check if commend is exists 
if user_input in commend_list : 
    print (f"commends Found  {user_input}")
    if user_input == "s":
        show_skills()
    elif user_input == "a":
        add_skill()
    elif user_input == "d":
        delete_skill()
    elif user_input == "u":
        update_skill()
    elif user_input == "q":
        update_skills()
    else:
        print("App Is Closed")    
else:
    print (f" Sorry This commend   \"{user_input}\"  is not found")
    