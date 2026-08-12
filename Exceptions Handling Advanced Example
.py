the_file = None
the_tris = 5
 
while the_tris > 0:
    try:
        print("Enter The File Name With Absolute Path To Open")
        print(f" You Have {the_tris} Tries Left")
        print("Example: D:\Python\Files\yourfile.extension")
        file_name_and_path = input ("File Name => : ").strip()
        the_file = open ( file_name_and_path , 'r')
        print (the_file.read())
        
        break
    except FileNotFoundError:
        print("File Not Found Please Be Sure The Name is Valid")
        the_tris -= 1
        

    except:
        print("Error Happens")
    finally:
        if the_file is not None:
            the_file.close()
            print("File Closed.")
else:
    print("All Tries Is Done")