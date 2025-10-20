import random
import time
bank_acc=None

def check_bank_acc():
    if bank_acc is None:
        print("Please Register First")
        reg()

def display_menu():
    print("Banking Menu")
    print("..............")
    print("1. Registering Bank Account")
    print("2. Deposit")
    print("3. Transfer")
    print("4. Withdraw")
    print("5. TopUp")
    print("6. Profile")
    print("Q: Quit")

def reg():
    print("Registering")
    print("............")
    d1,d2,d3,d4=random.randint(1000,9999),random.randint(1000,9999),random.randint(1000,9999),random.randint(1000,9999)
    acc_no= f"{d1} {d2} {d3} {d4}"
    print("Account Number: ",acc_no)
    Name =input("Name: ")
    Balance = int(input("Enter Amount: "))
    while Balance <=0:
        print("Invalid Amount. Please enter valid amount.")
        Balance = int(input("Enter Amount: "))
    global bank_acc
    bank_acc ={
        "acc_no": acc_no,
        "name": Name,
        "balance": Balance
    }
    print("Registration is successful")

def deposit():
    check_bank_acc()
    print("Deposit")
    print("..........")
    amount = int(input("Enter Amount: "))
    while amount <= 0:
        print("Invalid Amount. Please enter valid amount.")
        amount = int(input("Enter Amount: "))
    print(f"You have deposited {amount} MMK.")
    bank_acc["balance"] += amount
    print(f'Your Total is {bank_acc["balance"]} MMK.')

def transfer():
    check_bank_acc()
    print("Transfer")
    print("..........")
    other_acc = int(input("Enter other account number: "))
    amount = int(input("Enter Amount: "))
    while amount <= 0 or amount > bank_acc["balance"]:
        print("Invalid Amount. Please enter valid amount.")
        amount = int(input("Enter Amount: "))
    bank_acc["balance"] -= amount
    print(f"You have transferred {amount} MMK to {other_acc}")
    print(f'Your Total Left: {bank_acc["balance"]} MMK.')

def withdraw():
    check_bank_acc()
    print("Withdraw")
    print("...........")
    amount = int(input("Enter amount: "))
    while amount <=0 or amount > bank_acc["balance"]:
        print("Invalid Amount. Please Enter VALID Amount.")
        amount = int(input("Enter amount: "))
    print(f"You just withdraw {amount} MMK.")
    bank_acc["balance"] -= amount
    print(f'Your Total Left: {bank_acc["balance"]} MMK.')

def top_up():
    check_bank_acc()
    print("Top Up")
    print("........")
    phone=int(input("Enter Phone Number: "))
    print("1. Billing")
    print("2. Data Package")
    num = (input("Enter a number: "))
    while num not in ['1','2']:
        print("Invalid Number.Try VALID Number.")
        num = (input("Enter a number: "))
    if num == "1":
        print("Billing")
        print(".........")
        print("1. 1000 MMK",end='\n')
        print("2. 2000 MMK",end='\n')
        print("3. 3000 MMK",end='\n')
        print("4. 4000 MMK",end='\n')
        print("5. 5000 MMK",end='\n')
        no = (input("Enter a number: "))
        while no not in ['1','2','3','4','5']:
            print("Invalid Number.Try VALID Number.")
            no = (input("Enter a number: "))
        billing_dict = {
            '1': 1000,
            '2': 2000,
            '3': 3000,
            '4': 4000,
            '5': 5000
        }
        amount = billing_dict[no]
        bank_acc["balance"] -= amount
        print(f'You have successfully purchased bill of {amount}')
        print(f'Your Total Left: {bank_acc["balance"]} MMK.')
    else:
        print("Data Package")
        print("..............")
        print("1. 215MB(30Days) 799MMK")
        print("2. 303MB(30Days) 999MMK")
        print("3. 500MB(30Days) 1600MMK")
        ans = (input("Enter a number: "))
        while ans not in ['1','2','3']:
            print("Invalid Number.Try VALID Number.")
            ans = (input("Enter a number: "))
        data_dict={
            '1' : 799,
            '2' : 999,
            '3' : 1600
        }
        info_dict={
            '1' : "215MB(30Days)",
            '2' : "303MB(30Days)",
            '3' : "500MB(30Days)",
        }
        amount = data_dict[ans]
        bank_acc["balance"] -= amount
        print(f'You have successfully purchased {amount} for {info_dict[ans]}')
        print(f'Your Total Left: {bank_acc["balance"]} MMK.')

def profile():
    check_bank_acc()
    print("Profile")
    print(".........")
    print(f'Name: {bank_acc["name"]}')
    print(f'Account No: {bank_acc["acc_no"]}')
    print(f'Balance Amount: {bank_acc["balance"]}')

def main():
    while True:
        display_menu()
        ans = (input("Enter a Number: "))
        while ans not in ['1','2','3','4','5','6','Q']:
            print("Invalid Number. Try Again with VALID NUmber")
            ans = (input("Enter a Number: "))
        if ans == "1":
            reg()
        elif ans == "2":
            deposit()
        elif ans == "3":
            transfer()
        elif ans == "4":
            withdraw()
        elif ans == "5":
            top_up()
        elif ans == "6":
            profile()
        else:
            break
main()
