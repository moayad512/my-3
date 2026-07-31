from dataclasses import dataclass,field
# class Person ():
#     def __init__(self , name="moayad" , age=23, height=1.83 ,email="moayad.com" ):
#         self.name = name 
#         self.age = age
#         self. height = height
#         self.email = email
#     def __repr__(self):
#         return f" {self.__class__.__name__} (name ={self.name} age={self.age} height = { self.height} emial={self.email})"
    
#     def __eq__( self , othrr):
#         return (self. name , self. age ,self. height , self. email)==( othrr. name , othrr.age , othrr. height,othrr.email)
# ahmad = Person("ahmad" , 25 , 1.80 ," ahmad.com")
# ahmad2 = Person("ahmad" , 25 , 1.80 ," ahmad.com")
# print(ahmad==ahmad2)
   
""" لو اني بدي استخدم الكلاس العادي مطر اكتب الحكي السابق وهي من فوائد ال dataclasses اي وهيك
# """
# @dataclass
# class Person():
#     name:  str =" moayad "
#     age: int = 23
#     height: float = 1.83
#     email: str = "moayad.com"
#     def __eq__(self , other):
        # return self.age == other. age
    
    # def __repr__(self):
        # return f" this is a person called { self . name }"
# moayad = Person()
# print(moayad)
# print(moayad.age)
# ahmad = Person("ahmad" , 25 , 1.80 ," ahmad.com")
# ahmad2 = Person("ahmad" , 25 , 1.80 ," ahmad.com")
# print(ahmad==ahmad2)
# print(id(ahmad2))
# print(id(ahmad))
# print(f"your data person 1 {ahmad} and  person 2 your data is {moayad}" )
# """" ما بينفع اعطي قيم افتراضية سابقة ويكون عندي متغير بعدها وما يكون الو قيمة """
# """ يمكن ان اعطي قيمة لاخر متغير ويسبقه متغير بلا قيمة """

# """
# lt< 
# le <=
# gt  >
# ge >=
# # """
# @dataclass(order=True)
# class Person:
#     sort_indix: int = field(init=False, repr=  False)
#     name:  str =" moayad "
#     age: int = 23
#     height: float = 1.83
#     email: str = "moayad.com"
#     def __post_init__(self):
#         self . sort_indix =self.age
        
# ahmad = Person("ahmad" , 40 , 1.80 ," ahmad.com")
# ahmad2 = Person("ahmad" , 25 , 1.80 ," ahmad.com")
# # print(ahmad==ahmad2)
# # print(id(ahmad2))
# # print(id(ahmad))

# print(ahmad> ahmad2)   
""" مقارنة حسب الطول  بدلنا لفولت  مكان الانتجر  """
   
# @dataclass(order=True)
# class Person:
#     sort_indix: float = field(init=False, repr=  False)
#     name:  str =" moayad "
#     age: int = 23
#     height: float = 1.83
#     email: str = "moayad.com"
#     def __post_init__(self):
#         self . sort_indix =self.height
        
# ahmad = Person("ahmad" , 40 , 1.80 ," ahmad.com")
# ahmad2 = Person("ahmad" , 25 , 1.90 ," ahmad.com")
# # print(ahmad==ahmad2)
# # print(id(ahmad2))
# # print(id(ahmad))

# print(ahmad< ahmad2)   
""" مقارنة حسب الاسم  sort_inddix : = str, \n self . name  بدل self . height """
# @dataclass(order=True)
# class Person:
#     sort_indix: str = field(init=False, repr=  False)
#     name:  str =" moayad "
#     age: int = 23
#     height: float = 1.83
#     email: str = "moayad.com"
#     def __post_init__(self):
#         self . sort_indix =self.name
        
# ahmad = Person("ahmad" , 40 , 1.80 ," ahmad.com")
# ahmad2 = Person("moayad" , 25 , 1.90 ," ahmad.com")
# # print(ahmad==ahmad2)
# # print(id(ahmad2))
# # print(id(ahmad))

# print(ahmad > ahmad2)   
""" frozen هي بشكل عام بالداتا كلاس بتكون فالس بس اذا فعلناها وخليناها ترو ما منعود منحسن نغير على اي بارميتر ضمن المثود اي بتمنع التغير  """
@dataclass(order=True)
class Person:
    sort_indix: int = field(init=False, repr=  False)
    name:  str =" moayad "
    age: int = 23
    height: float = 1.83
    email: str = "moayad.com"
    def __post_init__(self):
        # self . sort_indix =self.name
        object.__setattr__(self, 'sort_indix', self .age)
        # """ من خلال السطر الي في الاعلى مع sort_indix الاول بعد الكلاس  نستطيع التحايل على الفورزن """
        """ انتبه الغلط الي صلك ساعة عم اتدور عليه هوه مسطرة زيادة انتبه للمسافى وانو لازم يكون التعريفات متكابقة تماما  """
# Person1 = Person("ahmad" , 50 , 1.80 ," ahmad.com")
# Person2 = Person("moayad" , 25 , 1.90 ," ahmad.com") 
# Person1. age= "50 اما بالنسبة لهل السطر غير ممكن مع الفيرزون 
""" اما اذا اردنا ان نغير بغير العمر فكل ما علينا ان نغير ال العمر بما نريد ان نغير  ونطع ما يناسبه في السطر 105  """

# print(Person1 > Person2)   
@dataclass
class Employee(Person):
    salrey :int = 3000
    dept : str = "mika engineering"
emplyee1 = Employee()
print(emplyee1)

emplyee2 =Employee("bulal", 25 , 174, "bulal.com",3500)
print(emplyee2)
# dasjdkj
# cldjkjasdhdksjahdkjs
# sad,ahafsdhfnas