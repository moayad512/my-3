class Skill:
    def __init__(self):
        self.skill = ["Html", "css", "Js"]
    def __str__(self):
        return f" This is My skill => {self. skill}"
    def __len__(self):
        return len (self .skill)



profil = Skill()
print (profil)
print (len(profil))
profil.skill.append("PHP")

profil.skill.append("MySQL")
print (len(profil))