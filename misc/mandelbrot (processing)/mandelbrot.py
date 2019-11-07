
def func(c,z):
	return z*z + c
iterations = 100
def ismandel(c):
    #for mandel, z = 0. for julia set, z = c.
    z = c
    # a = z.real
    # b = z.imag
    for i in range(iterations):
        #for mandel, const = c. for julia set, const = (set constant).
        const = -0.8 + 0.156j
        
        #my way 
        z = func(const,z)
        mag = (func(const,z).real)**2 + (func(const,z).imag)**2
        #print(mag)
        
        #codetrain way
        # aa = a*a
        # bb = b*b
        # twoab = 2.0 * a * b
        # a =  aa - bb + z.real
        # b = twoab + z.imag
        
        # mag = aa*aa + bb*bb
        
        if mag > 4:
            #a = False
            break
        else:
            #a = True
            continue
    return i
	
#resolution = w
w = 4
#initialize matrix
imag, real = 1500/w,2000/w
grid = [[0 for x in range(real)] for y in range(imag)] 

#grid[imag][real]
#


def setup():
    size(2000,1500)
   
initreal, initimag = -2, 1.1

currentreal, currentimag = initreal, initimag

zoomfactor = 1

def draw():
    realpart, imagpart = currentreal, currentimag
    x,y = 0,0
    for row in grid:
        for col in row:
            z = (realpart) + (imagpart)*1j
            
            if ismandel(z) == iterations-1:
                fill(0,0,0)
            else:
                if (ismandel(z) < 25):
                    colour1 = 255
                    colour2 = (ismandel(z)*10)
                    colour3 = 0
                if ((ismandel(z) > 24) and (ismandel(z) < 50)):
                    colour1 = 250 - ((ismandel(z)-24)*10)
                    colour2 = 250
                    colour3 = 0
                if ((ismandel(z) > 49) and (ismandel(z) < 75)):
                    colour1 = 0
                    colour2 = 250
                    colour3 = ((ismandel(z)-49)*10)
                if ((ismandel(z) > 74) and (ismandel(z) < 99)):
                    colour1 = ((ismandel(z)-74)*10)
                    colour2 = 250 - ((ismandel(z)-74)*10)
                    colour3 = 250
                    
                fill(colour1, colour2, colour3)
                
            rect(x,y,w,w)
            x = x + w
            realpart += 0.0015*w/zoomfactor
        y = y + w
        imagpart -= 0.0015*w/zoomfactor
        x = 0
        realpart = currentreal
        
def mouseClicked(): 
    #print(mouseX)
    #print(mouseY)
    zoom(mouseX, mouseY)
    
def zoom(x,y):
    global zoomfactor
    global currentreal
    global currentimag
    global iterations

    cornerx, cornery = (x-500/(1)), (y-(375/(1)))

    currentreal += (0.0015/zoomfactor)*(cornerx/1)
    currentimag -= (0.0015/zoomfactor)*(cornery/1)
    
    #print(currentreal)
    #print(currentimag)
    zoomfactor = zoomfactor*2
    print('zoom = ' + str(zoomfactor * 100) + '%')
    
