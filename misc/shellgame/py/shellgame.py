import time

bowls = [7,7,7,7,7,7,7,7,7,7,7,7,7,7]

turn = 0

pits = [0,0]

def printBoard():
    s = ""
    b = ""
    for i in range(1,8):
        s+=str(i) + " "
        b+=str(bowls[6+i]) + " "
    print(s)
    print(b)
    s = ""
    b = ""
    for i in range(7,0,-1):
        s+=str(i) + " "
        b+=str(bowls[i-1]) + " "
    print(b)
    print(s)

    print("player {0}'s turn".format(turn))


def move(pos):
    currBowl = pos + turn*7
    hand = bowls[currBowl]
    bowls[currBowl] = 0

    while (hand>0):
        currBowl = (currBowl+1)%14

        if ((currBowl + 7*turn)%14 == 7):
            pits[turn] += 1
            hand-=1
            if (hand == 0):
                #finished on pit
                #handle exception
                pos = int(input("landed on pit, pick a new bowl: "))
                move(pos-1)
                break
            else:
                pass


        bowls[currBowl]+= 1
        hand-=1
        print("turn in progress...")
        printBoard()


        if (hand == 0):
            if (bowls[currBowl] > 1):
                hand = bowls[currBowl]
                bowls[currBowl] = 0
                print("bruh {0}".format(hand))
                
            else:
                if (currBowl > 0+7*turn and currBowl < 7 + 7*turn): #player side
                    oppoBowl = (currBowl + 2*(7 - currBowl%7) - 1)%14
                    pits[turn]+= bowls[oppoBowl]
                    bowls[oppoBowl] = 0
                    print("finished turn on your side")
                    break
                else:
                    print("finished turn on opponent side")
                    break

        time.sleep(1)




def nextTurn():
    pass


def game():
    while True:
        printBoard()
        try:
            pos = int(input("select a bowl to pick up: "))
            if (pos == 69):
                print("quiting..")
                break;
        except:
            print("invalid input, try again")
            continue

        if (pos > 7 or pos < 1 or bowls[pos-1] == 0):
            print("invalid selection, try again")
            continue

        move(pos-1)

        nextTurn()


game()
