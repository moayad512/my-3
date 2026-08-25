



# 
import sqlite3


def get_all_data():
    try:
        db = sqlite3.connect ("app.db")
        print(" connected To Database Successfully")
        
            #  Setting The Cursor
        cr = db.cursor()
        # Fetch Data From Database
        cr.execute (" select * from users ")
        results = cr.fetchall()
        print(results)
        # print Numper of rose
        print (f" Database Has {len(results)} Rows. ")
          # print massage 
        print("shwing Data : ")
            # Loop On Results 
        for row in results :
            print(f"UserID => {row[0]}" , end=" ")
            
            
            print(f"Username => {row[1]}")
            
    except sqlite3.Error as er:
        print("Error Reading Data {er}")
    finally:
        if (db):
            # close Database connection
            
            db.close()
            print("connection To Database Is Closed")
            
get_all_data() 