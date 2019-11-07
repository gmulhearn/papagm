import tkinter as tk
import math
import random
import time

class CanvasApp(object):


    SIZE = 500 # The size of the screen.
    RADIUS = 50 # The radius of the circle
    MOVE = 100 # The amount to move on each step
    def __init__(self, master):
        master.title("2048 by George")
         
        self._canvas = tk.Canvas(master, bg="white", width=self.SIZE, height=self.SIZE)
        self._canvas.pack(side=tk.TOP, expand=True, fill=tk.BOTH)
         
        frame = tk.Frame(master)

        upBtn = tk.Button(frame, text="UP", command=self._move_up)
        upBtn.pack(side=tk.LEFT)

        LeftBtn = tk.Button(frame, text="LEFT", command=self._move_left)
        LeftBtn.pack(side=tk.LEFT)

        RightBtn = tk.Button(frame, text="RIGHT", command=self._move_right)
        RightBtn.pack(side=tk.LEFT)

        DownBtn = tk.Button(frame, text="DOWN", command=self._move_down)
        DownBtn.pack(side=tk.LEFT)

        dltBtn = tk.Button(frame, text="DELETE", command=self._delete)
        dltBtn.pack(side=tk.LEFT)

        GenBtn = tk.Button(frame, text='GENERATE', command=self._random_gen)
        GenBtn.pack(side=tk.LEFT)

        frame.pack(side=tk.TOP)

        #STARTING MATRIX
        self._row1 = [0,0,0,0,0]
        self._row2 = [0,0,0,0,0]
        self._row3 = [0,0,0,0,0]
        self._row4 = [0,0,0,0,0]
        self._row5 = [0,0,0,0,0]

        self._random_gen()
        self._random_gen()
        self._redraw()

           
    def _random_gen(self):
        placed = False
        while placed == False:
            rownum = random.randint(1,5)
            if rownum == 1:
                row = self._row1
            if rownum == 2:
                row = self._row2
            if rownum == 3:
                row = self._row3
            if rownum == 4:
                row = self._row4
            if rownum == 5:
                row = self._row5

            index = random.randint(0,4)

            if row[index] == 0:
                row[index] = 1
                placed = True
        self._redraw()


    def _redraw(self):
        self._canvas.delete(tk.ALL)


        self._make_cells(1)
        self._make_cells(2)
        self._make_cells(3)
        self._make_cells(4)
        self._make_cells(5)

        #GRID VERTICAL
        self._canvas.create_line(0,0,0,500)
        self._canvas.create_line(100,0,100,500)
        self._canvas.create_line(200,0,200,500)
        self._canvas.create_line(300,0,300,500)
        self._canvas.create_line(400,0,400,500)
        self._canvas.create_line(500,0,500,500)

        #GRID HORIZONTAL
        self._canvas.create_line(0, 0, 500, 0)
        self._canvas.create_line(0, 100, 500, 100)
        self._canvas.create_line(0, 200, 500, 200)
        self._canvas.create_line(0, 300, 500, 300)
        self._canvas.create_line(0, 400, 500, 400)
        self._canvas.create_line(0, 500, 500, 500)


    def _make_cells(self, rownum):

        row =[]

        if rownum == 1:
            row = self._row1
        elif rownum == 2:
            row = self._row2
        elif rownum == 3:
            row = self._row3
        elif rownum == 4:
            row = self._row4
        elif rownum == 5:
            row = self._row5
        
        row_index=0
        for col in row:

            if col == 1:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '2')

            if col == 2:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '4')

            if col == 3:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '8')

            if col == 4:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '16')

            if col == 5:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '32')

            if col == 6:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '64')

            if col == 7:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '128')

            if col == 8:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '256')

            if col == 9:
                coords = (row_index*100, (rownum-1)*100, (100*(row_index +1)), 100*(rownum))
                self._canvas.create_rectangle(coords, fill="#FFFDD0")
                self._canvas.create_text(row_index*100+50, (rownum-1)*100+50,font=("Helvetica Bold", "32"), text = '512')

            row_index += 1

        


    def _move_up(self):

        ##################ROW1 UP MOVE################
        self._move_up_row1()

        self._redraw()

        #################ROW2 UP MOVE##################
        self._move_up_row2()

        #RECHECK ROW1 
        self._move_up_row1()

        self._redraw()

        ##############ROW3 UP MOVE################
        self._move_up_row3()

        #RECHECK ROW 2
        self._move_up_row2()

        #RECHECK ROW1 
        self._move_up_row1()

        self._redraw()

        ################ROW4 UP MOVE################
        self._move_up_row4()
        
        #RECHECK ROW3
        self._move_up_row3()
       

        #RECHECK ROW 2
        self._move_up_row2()


        #RECHECK ROW1 
        self._move_up_row1()

        self._redraw()
        
        self._random_gen()


    def _move_up_row1(self):
        row_index = 0
        for col in self._row1:
            if (col == self._row2[row_index]) and (col != 0) :
                self._row1[row_index] += 1
                self._row2[row_index] = 0

            if col == 0:
                self._row1[row_index] = self._row2[row_index]
                self._row2[row_index] = 0

            row_index +=1

    def _move_up_row2(self):
        row_index = 0
        for col in self._row2:
            if (col == self._row3[row_index]) and (col != 0) :
                self._row2[row_index] += 1
                self._row3[row_index] = 0

            if col == 0:
                self._row2[row_index] = self._row3[row_index]
                self._row3[row_index] = 0

            row_index +=1

    def _move_up_row3(self):
        row_index = 0
        for col in self._row3:
            if (col == self._row4[row_index]) and (col != 0) :
                self._row3[row_index] += 1
                self._row4[row_index] = 0

            if col == 0:
                self._row3[row_index] = self._row4[row_index]
                self._row4[row_index] = 0

            row_index +=1

    def _move_up_row4(self):
        row_index = 0
        for col in self._row4:
            if (col == self._row5[row_index]) and (col != 0) :
                self._row4[row_index] += 1
                self._row5[row_index] = 0

            if col == 0:
                self._row4[row_index] = self._row5[row_index]
                self._row5[row_index] = 0

            row_index +=1
    
        



    def _move_down(self):

        ###########ROW5 MOVE DOWN################

        self._move_down_row5()
        self._redraw()

        ############ROW4 MOVE DOWN#################
        self._move_down_row4()

        #RECHECK ROW5
        self._move_down_row5()
        self._redraw()

        ################ROW3 MOVE DOWN##############
        self._move_down_row3()

        #RECHECK ROW4
        self._move_down_row4()

        #RECHECK ROW5
        self._move_down_row5()
        self._redraw()

        ###########ROW2 MOVE DOWN###############
        self._move_down_row2()

        #RECHECK ROW3
        self._move_down_row3()

        #RECHECK ROW4
        self._move_down_row4()

        #RECHECK ROW5
        self._move_down_row5()
        self._redraw()

        self._random_gen()



    def _move_down_row5(self):
        row_index = 0
        for col in self._row5:
            if (col == self._row4[row_index]) and (col != 0) :
                self._row5[row_index] += 1
                self._row4[row_index] = 0

            if col == 0:
                self._row5[row_index] = self._row4[row_index]
                self._row4[row_index] = 0

            row_index +=1

    def _move_down_row4(self):
        row_index = 0
        for col in self._row4:
            if (col == self._row3[row_index]) and (col != 0) :
                self._row4[row_index] += 1
                self._row3[row_index] = 0

            if col == 0:
                self._row4[row_index] = self._row3[row_index]
                self._row3[row_index] = 0

            row_index +=1

    def _move_down_row3(self):
        row_index = 0
        for col in self._row3:
            if (col == self._row2[row_index]) and (col != 0) :
                self._row3[row_index] += 1
                self._row2[row_index] = 0

            if col == 0:
                self._row3[row_index] = self._row2[row_index]
                self._row2[row_index] = 0

            row_index +=1

    def _move_down_row2(self):
        row_index = 0
        for col in self._row2:
            if (col == self._row1[row_index]) and (col != 0) :
                self._row2[row_index] += 1
                self._row1[row_index] = 0

            if col == 0:
                self._row2[row_index] = self._row1[row_index]
                self._row1[row_index] = 0
            row_index +=1


        

    def _move_left(self):
        for rownum in range(5):

            if rownum + 1 == 1:
                row = self._row1
            elif rownum + 1 == 2:
                row = self._row2
            elif rownum + 1 == 3:
                row = self._row3
            elif rownum + 1 == 4:
                row = self._row4
            elif rownum + 1 == 5:
                row = self._row5


            #### COLUMN1 ######
            if (row[0] == row[1]) and  (row[0] != 0):
                row[0] += 1
                row[1] = 0
            if row[0] == 0:
                row[0] = row[1]
                row[1] = 0
            self._redraw()

            ##### COLUMN2 #######
            if (row[1] == row[2]) and  (row[1] != 0):
                row[1] += 1
                row[2] = 0
            if row[1] == 0:
                row[1] = row[2]
                row[2] = 0

            #recheck column 1
            if (row[0] == row[1]) and  (row[0] != 0):
                row[0] += 1
                row[1] = 0
            if row[0] == 0:
                row[0] = row[1]
                row[1] = 0
            self._redraw()

            #### COLUMN 3 ######
            if (row[2] == row[3]) and  (row[2] != 0):
                row[2] += 1
                row[3] = 0
            if row[2] == 0:
                row[2] = row[3]
                row[3] = 0

            #recheck column 2
            if (row[1] == row[2]) and  (row[1] != 0):
                row[1] += 1
                row[2] = 0
            if row[1] == 0:
                row[1] = row[2]
                row[2] = 0

            #recheck column 1
            if (row[0] == row[1]) and  (row[0] != 0):
                row[0] += 1
                row[1] = 0
            if row[0] == 0:
                row[0] = row[1]
                row[1] = 0
            self._redraw()

            #### COLUMN 4 #####
            if (row[3] == row[4]) and  (row[3] != 0):
                row[3] += 1
                row[4] = 0
            if row[3] == 0:
                row[3] = row[4]
                row[4] = 0

            #recheck column 3\
            if (row[2] == row[3]) and  (row[2] != 0):
                row[2] += 1
                row[3] = 0
            if row[2] == 0:
                row[2] = row[3]
                row[3] = 0

            #recheck column 2
            if (row[1] == row[2]) and  (row[1] != 0):
                row[1] += 1
                row[2] = 0
            if row[1] == 0:
                row[1] = row[2]
                row[2] = 0

            #recheck column 1
            if (row[0] == row[1]) and  (row[0] != 0):
                row[0] += 1
                row[1] = 0
            if row[0] == 0:
                row[0] = row[1]
                row[1] = 0
            self._redraw()

        self._random_gen()



  

    def _move_right(self):
        for rownum in range(5):

            if rownum + 1 == 1:
                row = self._row1
            elif rownum + 1 == 2:
                row = self._row2
            elif rownum + 1 == 3:
                row = self._row3
            elif rownum + 1 == 4:
                row = self._row4
            elif rownum + 1 == 5:
                row = self._row5

            ##### COLUMN 5 #####
            if (row[4] == row[3]) and  (row[4] != 0):
                row[4] += 1
                row[3] = 0
            if row[4] == 0:
                row[4] = row[3]
                row[3] = 0
            self._redraw()

            #### COLUMN 4 #####
            if (row[3] == row[2]) and  (row[3] != 0):
                row[3] += 1
                row[2] = 0
            if row[3] == 0:
                row[3] = row[2]
                row[2] = 0

            #recheck column 5
            if (row[4] == row[3]) and  (row[4] != 0):
                row[4] += 1
                row[3] = 0
            if row[4] == 0:
                row[4] = row[3]
                row[3] = 0
                self._redraw()

            ##### COLUMN 3 #####
            if (row[2] == row[1]) and  (row[2] != 0):
                row[2] += 1
                row[1] = 0
            if row[2] == 0:
                row[2] = row[1]
                row[1] = 0

            #recheck column 4
            if (row[3] == row[2]) and  (row[3] != 0):
                row[3] += 1
                row[2] = 0
            if row[3] == 0:
                row[3] = row[2]
                row[2] = 0

            #recheck column 5
            if (row[4] == row[3]) and  (row[4] != 0):
                row[4] += 1
                row[3] = 0
            if row[4] == 0:
                row[4] = row[3]
                row[3] = 0
            self._redraw()

            #### COLUMN 2 #####
            if (row[1] == row[0]) and  (row[1] != 0):
                row[1] += 1
                row[0] = 0
            if row[1] == 0:
                row[1] = row[0]
                row[0] = 0

            #recheck column 3
            if (row[2] == row[1]) and  (row[2] != 0):
                row[2] += 1
                row[1] = 0
            if row[2] == 0:
                row[2] = row[1]
                row[1] = 0

            #recheck column 4
            if (row[3] == row[2]) and  (row[3] != 0):
                row[3] += 1
                row[2] = 0
            if row[3] == 0:
                row[3] = row[2]
                row[2] = 0

            #recheck column 5
            if (row[4] == row[3]) and  (row[4] != 0):
                row[4] += 1
                row[3] = 0
            if row[4] == 0:
                row[4] = row[3]
                row[3] = 0
            self._redraw()

        self._random_gen()






        
       

    def _delete(self):
        self._canvas.delete(tk.ALL)

        

root = tk.Tk()
app = CanvasApp(root)

root.mainloop()
