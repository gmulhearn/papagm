##blackjack

import numpy as np


suits = ['spades', 'hearts', 'diamonds', 'clubs']

numbers = ['ace', 2, 3, 4, 5, 6, 7, 8, 9, 10, 'jack', 'king', 'queen']


def gencard():
    suit = suits[np.random.randint(len(suits))]

    number = numbers[np.random.randint(len(numbers))]

    return str(number) + ' of ' + str(suit)
          

def value(card):

    val = card.split(' ')[0]

    val1 = 0
    val2 = val
    
    if (val == 'jack') or (val == 'king') or (val == 'queen'):
        val2 = 10
    elif val == ('ace'):
        val1 = 1
        val2 = 11
    
    

    return (val1, int(val2))


card1 = 0
card2 = 0
card3 = 0
card4 = 0
card5 = 0


def hit():
    
    global card3
    global card4
    global card5

    if card3 == '0 of nothing':
        card3 = gencard()
        return
    elif card4 == '0 of nothing':
        card4 = gencard()
        return
    elif card5 == '0 of nothing':
        card5 = gencard()
        return
        
def summ():
    #for card in (card1, card2, card3, card4, card5):
        #if value(card)[0] == 1:
           # pass
    tot = value(card1)[1] + value(card2)[1] + value(card3)[1] + value(card4)[1] + value(card5)[1]
    
    return tot
    
dlrcard1 = gencard()
dlrcard2 = gencard()
dlrcard3 = 0
dlrcard4 = 0
dlrcard5 = 0
dlrwin = False

def dealerturn(score):
    dlrcard3 = 0
    dlrcard4 = 0
    dlrcard5 = 0
    dlrcards = (dlrcard1, dlrcard2, dlrcard3, dlrcard4, dlrcard5)
    dlrnumcards = 2
    print(' ')
    print('it is now the dealers turn')
    print(' ')
    print(' ')
    print(' ')
    print('their cards are: ')
    print(' ')
    for i in range(dlrnumcards):
            print(dlrcards[i])
            print(' ')
    
    

    print(' ')
    print('giving them a value of: ' + str(value(dlrcard1)[1]
                                           + value(dlrcard2)[1]))

    if ((int(value(dlrcard1)[1]) + int(value(dlrcard2)[1])) > (int(score)-1)):
        dlrwin = True
        return dlrwin




    input('type anything to continue...    ')
    
    dlrcard3 = gencard()

    dlrcards = (dlrcard1, dlrcard2, dlrcard3, dlrcard4, dlrcard5)
    
    dlrnumcards += 1
    print('the dealer hits')
    print(' ')
    print(' ')
    print(' ')
    print('their cards are now: ')
    print(' ')
    for i in range(dlrnumcards):
            print(dlrcards[i])
            print(' ')

    print(' ')
    print('giving them a value of: ' + str(value(dlrcard1)[1] + value(dlrcard2)[1]
                                           + value(dlrcard3)[1]))
    

    if ((int(value(dlrcard1)[1]) + int(value(dlrcard2)[1])
         + int(value(dlrcard3)[1])) > 21):
          dlrwin = False
          return dlrwin

    elif ((int(value(dlrcard1)[1]) + int(value(dlrcard2)[1])
         + int(value(dlrcard3)[1])) > (int(score)-1)):
          dlrwin = True
          return dlrwin




    input('type anything to continue...    ')
    
    dlrcard4 = gencard()

    dlrcards = (dlrcard1, dlrcard2, dlrcard3, dlrcard4, dlrcard5)
    
    dlrnumcards += 1
    print('the dealer hits again')
    print(' ')
    print(' ')
    print(' ')
    print('their cards are now: ')
    print(' ')
    for i in range(dlrnumcards):
            print(dlrcards[i])
            print(' ')
    



##def dlrwin(score):
##    pass



def game():

    for i in range(30):
                print(' ')
                
    global card1 
    global card2
    global card3 
    global card4 
    global card5

    card1 = gencard()
    card2 = gencard()
    card3 = '0 of nothing'
    card4 = '0 of nothing'
    card5 = '0 of nothing'

##    global dlrcard1 
##    global dlrcard2
##    global dlrcard3 
##    global dlrcard4 
##    global dlrcard5
##
##    dlrcard1 = gencard()
##    dlrcard2 = gencard()
##    dlrcard3 = '0 of nothing'
##    dlrcard4 = '0 of nothing'
##    dlrcard5 = '0 of nothing'

    
    bust = False
    print(' ')
    numcards = 2
    
    print('Welcome to blackjack ')
    print(' ')
    print(' ')
    print(' ')
    
    while bust == False:
        cards = (card1, card2, card3, card4, card5)
        print('your cards are: ')
        print(' ')
        for i in range(numcards):
            print(cards[i])
            print(' ')
        summ()
        print('value: ' + str(summ()))
        
        if summ() > 21:
            print('you busted!!')
            bust = True
            print(' ')
            input('play again? ')
            game()
            
        else: 
            ans = input('would you like to hit?  ("y"/"n") ')
            if ans == 'y':
                hit()
            if ans == 'n':
                
                for i in range(10):
                    print(' ')
                
                if dealerturn(summ()) == True:
                    print('u lose!')
                    print(' ')
                    input('play again? ')
                    game()
                else:
                    print('the dealer busted! u win' )
                    print(' ')
                    input('play again? ')
                    game()
                    
                break
            elif ans == 'exit':
                break
            numcards += 1

            



start = input('Play blackjack? ')
if start == 'y':
    game()
if start == 'n':
    game()
    


    
