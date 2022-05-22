from time import sleep
import pygame
from math import sin, cos,pi


#Taille
HAUTEUR = 720
LARGEUR = 1280

#Couleurs
WHITE = (255,255,255)
LIGHTBLUE = (114,159,207)
BLUE = (52,101,164)
GREEN = (0,255,0)
DARKGREEN = (78,154,6)
BLACK = (0,0,0)
RED = (255,0,0)
YELLOW = (255,255,0)
PINK = (168,50,117)
PURPLE = (119,50,168)

class Screen():
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((LARGEUR,HAUTEUR))

    """
    Method to en the pygame session
    @param: None
    @return: None
    """
    def close(self):
        pygame.display.quit()
        pygame.quit()

    """
    Method to update the screen
    @param: None
    @return: None
    """
    def updateScreen(self):
        pygame.display.flip()

    """
    Method to display the background
    @param: screen, the screen
    @return: None
    """
    def drawcircle(self,pos,size,fill):
       pygame.draw.circle(self.screen,YELLOW,(pos[0],pos[1]),size,fill)

    def fondDecran(self):
        self.screen.fill(BLUE)
        pygame.draw.rect(self.screen,LIGHTBLUE,pygame.Rect(30,30,(1280-60),(720-60)))

    def drawsun(self):
        self.drawcircle((LARGEUR//2,HAUTEUR//2),30,0)
        self.drawcircle((LARGEUR//2,HAUTEUR//2),300,1)
    def drawEarth(self,coord):
        self.drawcircle((coord[0],coord[1]),10,0)

def eventQuit(a):
    return(a == pygame.QUIT)

def leapYear(year):
    if (year%4  == 0):
        if (year%100 != 0) :
            return True;
        elif (year%400 == 0) :
            return True;
    return False;

def dayOfYear(day, months,year):
    daysInMonth = [31,28,31,30,31,30,31,31,30,31,30,31];
    if leapYear(year):
        daysInMonth[1] = 29;
    else:
        daysInMonth[1] = 28;

    output = 0
    for i in range(0,months-1):
        output += daysInMonth[i];
    output += day;
    return output;

def get_angle(doy,h):
    x = (doy*24)+h
    return int(((x/8760)*2*pi)*100)

def getXY(angle,r):
    x = int(r*cos(angle/100))
    y = int(r*sin(angle/100))
    return (x,y)

if __name__ == "__main__":  
    print("Start screen...")
    screen = Screen()
    screen.fondDecran()
    screen.updateScreen()
    print("Screen up")
    print(pi)
    running = True

    Vdoy = 0
    h = 0
    screen.drawsun()
    while running:
        # for loop through the event queue  
        for event in pygame.event.get():
            # Check for QUIT event      
            if eventQuit(event.type):
                running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    Vdoy = 0 if(Vdoy>=365) else Vdoy+1
                elif event.key == pygame.K_ESCAPE:
                    running = False

        screen.fondDecran()
        screen.drawsun()
        angle = get_angle(Vdoy,h)
        coord = getXY(angle , 300)
        print("Angle : %d | coord : %d,%d"%(angle,coord[0],coord[1]))
        screen.drawEarth((coord[0]+LARGEUR//2,coord[1]+HAUTEUR//2))
        screen.updateScreen()
        if (h>=24):
            h = 0
            Vdoy += 1
        else:
            h += 1
        sleep(0.001)
    
    print("Closing...")
    screen.close()