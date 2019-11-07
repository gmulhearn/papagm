import tkinter as tk
import math

class CanvasApp(object):


    SIZE = 500 # The size of the screen.
    RADIUS = 50 # The radius of the circle
    MOVE = 100 # The amount to move on each step
    def __init__(self, master):
        master.title("2048 by George")
         
        self._canvas = tk.Canvas(master, bg="white", width=self.SIZE, height=self.SIZE)
        self._canvas.pack(side=tk.TOP, expand=True, fill=tk.BOTH)
         
        frame = tk.Frame(master)

        upBtn = tk.Button(frame, text="UP", command=self.move_up)
        upBtn.pack(side=tk.LEFT)

        LeftBtn = tk.Button(frame, text="LEFT", command=self.move_left)
        LeftBtn.pack(side=tk.LEFT)

        RightBtn = tk.Button(frame, text="RIGHT", command=self.move_right)
        RightBtn.pack(side=tk.LEFT)

        DownBtn = tk.Button(frame, text="DOWN", command=self.move_down)
        DownBtn.pack(side=tk.LEFT)

        dltBtn = tk.Button(frame, text="Delete", command=self.delete)
        dltBtn.pack(side=tk.LEFT)

        frame.pack(side=tk.TOP)

        self._x, self._y = (self.SIZE / 2, self.SIZE / 2)
        self._path = [(self._x, self._y)]
        self._redraw()
           
        
    def _redraw(self):
        self._canvas.delete(tk.ALL)
        coords = (self._x - self.RADIUS, 
            self._y - self.RADIUS, 
            self._x + self.RADIUS, 
            self._y + self.RADIUS)
        object2 = (self._canvas.create_rectangle(coords, fill="#FFFDD0")
        + self._canvas.create_text(self._x, self._y, fill="Gray",font=("Helvetica Bold", "32"), text="2"))

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


    def _move(self, dx, dy):

        self._x += dx
        self._y += dy
        self._redraw()
        
    def move_up(self):
        
        if self._y < 100:
            pass
        else:
            self._move(0, -self.MOVE)

    def move_down(self):
        if self._y > 400:
            pass
        else:
            self._move(0, self.MOVE)

    def move_left(self):
        if self._x < 100:
            pass
        else:
            self._move(-self.MOVE, 0)

    def move_right(self):
        if self._x > 400:
            pass
        else:
            self._move(self.MOVE, 0)
       

    def delete(self):
        self._canvas.delete(tk.ALL)

        

root = tk.Tk()
app = CanvasApp(root)
root.mainloop()
