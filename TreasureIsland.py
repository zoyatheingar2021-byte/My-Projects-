print("Welcome to Treasure Island. Your Mission is to find the tressure!")
print("....................................................................")

print ("Chose a way to continue.")
print ("1:Left, 2,Right")
ans=input("Enter a number: ")
if ans=="1":
    print("You are safe!")
    print ("\nWill you swim or wait?....")
    print ("3:Swim, 4:Wait")
    choice=input("Enter a number: ")
    if choice=="4":
        print ("Good Choice!\n\nNow Which door will you Choose?")
        print ("5:Red Door, 6:Blue Door, 7:Yellow Door")
        door=input("Enter a Number: ")
        if door=="5":
            print("You are BURNED by FIRE.TT Hope you are okay!")
        elif door=="6":
            print ("You are EATEN by BEASTS. GAME OVER!")
        elif door=="7":
            print ("CONGRATULATION! You WIN!!!")
        else:
            print("NOOO you should have chose one from given.")
    elif choice=="3":
        print ("Oh oH, SORRY! you Lose.")
    else:
        print ("Invalid Number")
elif ans=="2":
    print ("OPPS! You fall into a hole. GAME OVER!")
else:
    print ("Invalid Number")
