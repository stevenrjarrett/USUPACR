# We Are Worms! (a Nibbles clone)
# By Al Sweigart al@inventwithpython.com
# http://inventwithpython.com/pygame
# Released under a "Simplified BSD" license
#
# Modified by Steven Jarrett

import random, pygame, sys
from pygame.locals import *
import xboxOne

FPS = 5
WINDOWWIDTH = 1080
WINDOWHEIGHT = 800
CELLSIZE = 10
assert WINDOWWIDTH % CELLSIZE == 0, "Window width must be a multiple of cell size."
assert WINDOWHEIGHT % CELLSIZE == 0, "Window height must be a multiple of cell size."
CELLWIDTH = int(WINDOWWIDTH / CELLSIZE)
CELLHEIGHT = int(WINDOWHEIGHT / CELLSIZE)

#             R    G    B
WHITE     = (255, 255, 255)
BLACK     = (  0,   0,   0)
RED       = (255,   0,   0)
GREEN     = (  0, 255,   0)
DARKGREEN = (  0, 155,   0)
BLUE      = (  0,   0, 255)
DARKBLUE  = (  0,   0, 155)
GRAY      = (150, 150, 150)
MIDGRAY   = (100, 100, 100)
DARKGRAY  = ( 40,  40,  40)
BGCOLOR = DARKGRAY

# Game name
gameName = 'We Are Worms!'
NUMWORMS = 1
NUMAPPLES = 20
LENWORMS = 10

UP = 'up'
DOWN = 'down'
LEFT = 'left'
RIGHT = 'right'

COORD_X = 0
COORD_Y = 1

HEAD = 0 # syntactic sugar: index of the worm's head

def main():
    global FPSCLOCK, DISPLAYSURF, BASICFONT

    pygame.init()
    FPSCLOCK = pygame.time.Clock()
    DISPLAYSURF = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT))
    BASICFONT = pygame.font.Font('freesansbold.ttf', 18)
    pygame.display.set_caption(gameName)

    showStartScreen()
    while True:
        runGame()
        showGameOverScreen()


def runGame():
    # Set a random start point.
    class worm:

        def __init__(self, startx, starty):
            self.wormCoords = []
            for i in range(0, LENWORMS):
                self.wormCoords.append({'x': startx - i, 'y': starty})
            self.direction = RIGHT
            self.shoot = False

    class coordinate:

        def __init__(self, x,y):
            self.x = x
            self.y = y
        def __eq__(self,other):
            return (self.x == other.wormCoords[HEAD]['x'] and self.y == other.wormCoords[HEAD]['y'])

    worms = [worm(random.randint(5, CELLWIDTH - 6), random.randint(5, CELLHEIGHT - 6))]
    for i in range(1,NUMWORMS):
        worms.append(worm(random.randint(5, CELLWIDTH - 6), random.randint(5, CELLHEIGHT - 6)))

    # Start the apple in a random place.
#    apple = getRandomLocation()
    apples = [getRandomLocation()]
    for i in range(1,NUMAPPLES):
        apples.append(getRandomLocation())

    # initialize my rocks
    rocks = [coordinate(random.randint(0, CELLWIDTH - 1), random.randint(0, CELLHEIGHT - 1))]
#    for i in range(0,4):
#        rocks.append(rock(random.randint(0, CELLWIDTH - 1), random.randint(0, CELLHEIGHT - 1)))

    # Start the xbox controller
    gamepad = xboxOne.InputDevice('/dev/input/event8')


    while True: # main game loop
        for event in pygame.event.get(): # event handling loop
            if event.type == QUIT:
                terminate()
            elif event.type == KEYDOWN:
                if (event.key == K_LEFT or event.key == K_KP4) and (worms[0]).direction != RIGHT:
                    (worms[0]).direction = LEFT
                elif (event.key == K_RIGHT or event.key == K_KP6) and (worms[0]).direction != LEFT:
                    (worms[0]).direction = RIGHT
                elif (event.key == K_UP or event.key == K_KP8) and (worms[0]).direction != DOWN:
                    (worms[0]).direction = UP
                elif (event.key == K_DOWN or event.key == K_KP2) and (worms[0]).direction != UP:
                    (worms[0]).direction = DOWN
                if (event.key == K_KP0):
                    (worms[0]).shoot = True
                if len(worms) == 2:
                    if (event.key == K_a or event.key == K_KP4) and (worms[1]).direction != RIGHT:
                        (worms[1]).direction = LEFT
                    elif (event.key == K_d or event.key == K_KP6) and (worms[1]).direction != LEFT:
                        (worms[1]).direction = RIGHT
                    elif (event.key == K_w or event.key == K_KP8) and (worms[1]).direction != DOWN:
                        (worms[1]).direction = UP
                    elif (event.key == K_s or event.key == K_KP2) and (worms[1]).direction != UP:
                        (worms[1]).direction = DOWN
                    if (event.key == K_SPACE):
                        (worms[1]).shoot = True
                if event.key == K_ESCAPE:
                    terminate()

        # Get xbox input
        while True:
            xevent = gamepad.read_one()
            if xevent == None:
                break
            if (xevent.type == 03) and (xevent.code == 16) and (xevent.value == -1) and (worms[0]).direction != RIGHT:
                (worms[0]).direction = LEFT
            elif (xevent.type == 03) and (xevent.code == 16) and (xevent.value == 01) and (worms[0]).direction != LEFT:
                (worms[0]).direction = RIGHT
            elif (xevent.type == 03) and (xevent.code == 17) and (xevent.value == -1) and (worms[0]).direction != DOWN:
                (worms[0]).direction = UP
            elif (xevent.type == 03) and (xevent.code == 17) and (xevent.value == 01) and (worms[0]).direction != UP:
                (worms[0]).direction = DOWN

        # Draw background for next time
        DISPLAYSURF.fill(BGCOLOR)
        drawGrid()

        # update and draw worms

        for i in range(0,len(worms)):

            # check if the worm has hit the edge
            if (worms[i]).wormCoords[HEAD]['x'] == -1 or (worms[i]).wormCoords[HEAD]['x'] == CELLWIDTH or (worms[i]).wormCoords[HEAD]['y'] == -1 or (worms[i]).wormCoords[HEAD]['y'] == CELLHEIGHT:
                return # game over
            # or if it's hit another worm
            for j in range(0,len(worms)):
#                for wormBody in (worms[j]).wormCoords[0:]:
#                    if j == i and wormBody['x'] == (worms[i]).wormCoords[HEAD]['x'] and wormBody['y'] == (worms[i]).wormCoords[HEAD]['y']: # don't go over the head of the current snake
#                        continue
#                    if wormBody['x'] == (worms[i]).wormCoords[HEAD]['x'] and wormBody['y'] == (worms[i]).wormCoords[HEAD]['y']:
#                        return # game over
                for k in range(0,len((worms[j]).wormCoords)):
                    if j == i and k == HEAD:
                        continue
                    if (worms[j]).wormCoords[k]['x'] == (worms[i]).wormCoords[HEAD]['x'] and (worms[j]).wormCoords[k]['y'] == (worms[i]).wormCoords[HEAD]['y']:
                        return # game over

            # Check if the worm has hit a rock
            for rockn in rocks:
                if rockn == worms[i]:
                    return # game over

            # check if worm has eaten an apple
            delEnd = True
            for j in range(0,len(apples)):
                if (worms[i]).wormCoords[HEAD]['x'] == apples[j]['x'] and (worms[i]).wormCoords[HEAD]['y'] == apples[j]['y']:
                    # don't remove worm's tail segment
                    apples[j] = getRandomLocation() # set a new apple somewhere
                    delEnd = False
            if delEnd == True:
                del (worms[i]).wormCoords[-1] # remove worm's tail segment

            # move the worm by adding a segment in the direction it is moving
            if (worms[i]).direction == UP:
                newHead = {'x': (worms[i]).wormCoords[HEAD]['x'], 'y': (worms[i]).wormCoords[HEAD]['y'] - 1}
            elif (worms[i]).direction == DOWN:
                newHead = {'x': (worms[i]).wormCoords[HEAD]['x'], 'y': (worms[i]).wormCoords[HEAD]['y'] + 1}
            elif (worms[i]).direction == LEFT:
                newHead = {'x': (worms[i]).wormCoords[HEAD]['x'] - 1, 'y': (worms[i]).wormCoords[HEAD]['y']}
            elif (worms[i]).direction == RIGHT:
                newHead = {'x': (worms[i]).wormCoords[HEAD]['x'] + 1, 'y': (worms[i]).wormCoords[HEAD]['y']}

            # Update worm and draw
            (worms[i]).wormCoords.insert(0, newHead)


            # Check if the worm has shot a laser
            if worms[i].shoot == True:
                worms[i].shoot = False
#                Set laser parameters
                if worms[i].direction == UP:
                    xMin = worms[i].wormCoords[HEAD]['x']
                    xMax = xMin
                    yMin = 0
                    yMax = worms[i].wormCoords[HEAD]['y']-1
                elif worms[i].direction == DOWN:
                    xMin = worms[i].wormCoords[HEAD]['x']
                    xMax = xMin
                    yMin = worms[i].wormCoords[HEAD]['y']+1
                    yMax = CELLHEIGHT
                elif worms[i].direction == LEFT:
                    xMin = 0
                    xMax = worms[i].wormCoords[HEAD]['x']-1
                    yMin = worms[i].wormCoords[HEAD]['y']
                    yMax = yMin
                else: # RIGHT
                    xMin = worms[i].wormCoords[HEAD]['x']+1
                    xMax = CELLWIDTH
                    yMin = worms[i].wormCoords[HEAD]['y']
                    yMax = yMin
                drawLaser(xMin, xMax, yMin, yMax)
                for wormInd in range(0, len(worms)):
#                    myworm = worms[wormInd]
                    for segInd in range(0, len(worms[wormInd].wormCoords)):
#                        segment = worms[wormInd].wormCoords[segInd]
                        if worms[wormInd].wormCoords[segInd]['x'] >= xMin and worms[wormInd].wormCoords[segInd]['x'] <= xMax and worms[wormInd].wormCoords[segInd]['y'] >= yMin and worms[wormInd].wormCoords[segInd]['y'] <= yMax:
                            if segInd == HEAD:
                                return # game over
                            for dyingInd in range(segInd, len(worms[wormInd].wormCoords)):
                                rocks.append(coordinate(worms[wormInd].wormCoords[dyingInd]['x'], worms[wormInd].wormCoords[dyingInd]['y']))
                            worms[wormInd].wormCoords = worms[wormInd].wormCoords[:segInd]
#                            drawRocks(rocks)
                            break

            drawWorm((worms[i]).wormCoords)

        drawRocks(rocks)
        drawApples(apples)
        drawScores(worms)
        pygame.display.update()
        FPSCLOCK.tick(FPS)

def drawPressKeyMsg():
    pressKeySurf = BASICFONT.render('Press a key to play.', True, DARKGRAY)
    pressKeyRect = pressKeySurf.get_rect()
    pressKeyRect.topleft = (WINDOWWIDTH - 200, WINDOWHEIGHT - 30)
    DISPLAYSURF.blit(pressKeySurf, pressKeyRect)


def checkForKeyPress():
    if len(pygame.event.get(QUIT)) > 0:
        terminate()

    keyUpEvents = pygame.event.get(KEYUP)
    if len(keyUpEvents) == 0:
        return None
    if keyUpEvents[0].key == K_ESCAPE:
        terminate()
    return keyUpEvents[0].key


def showStartScreen():
    titleFont = pygame.font.Font('freesansbold.ttf', 80)
    titleSurf1 = titleFont.render(gameName, True, WHITE, DARKBLUE)
    titleSurf2 = titleFont.render(gameName, True, BLUE)

    degrees1 = 0
    degrees2 = 0
    while True:
        DISPLAYSURF.fill(BGCOLOR)
        rotatedSurf1 = pygame.transform.rotate(titleSurf1, degrees1)
        rotatedRect1 = rotatedSurf1.get_rect()
        rotatedRect1.center = (WINDOWWIDTH / 2, WINDOWHEIGHT / 2)
        DISPLAYSURF.blit(rotatedSurf1, rotatedRect1)

        rotatedSurf2 = pygame.transform.rotate(titleSurf2, degrees2)
        rotatedRect2 = rotatedSurf2.get_rect()
        rotatedRect2.center = (WINDOWWIDTH / 2, WINDOWHEIGHT / 2)
        DISPLAYSURF.blit(rotatedSurf2, rotatedRect2)

        drawPressKeyMsg()

        if checkForKeyPress():
            pygame.event.get() # clear event queue
            return
        pygame.display.update()
        FPSCLOCK.tick(FPS)
        degrees1 += 3 # rotate by 3 degrees each frame
        degrees2 += 7 # rotate by 7 degrees each frame


def terminate():
    pygame.quit()
    sys.exit()


def getRandomLocation():
    return {'x': random.randint(0, CELLWIDTH - 1), 'y': random.randint(0, CELLHEIGHT - 1)}


def showGameOverScreen():
    gameOverFont = pygame.font.Font('freesansbold.ttf', 150)
    gameSurf = gameOverFont.render('Game', True, WHITE)
    overSurf = gameOverFont.render('Over', True, WHITE)
    gameRect = gameSurf.get_rect()
    overRect = overSurf.get_rect()
    gameRect.midtop = (WINDOWWIDTH / 2, 10)
    overRect.midtop = (WINDOWWIDTH / 2, gameRect.height + 10 + 25)

    DISPLAYSURF.blit(gameSurf, gameRect)
    DISPLAYSURF.blit(overSurf, overRect)
    drawPressKeyMsg()
    pygame.display.update()
    pygame.time.wait(500)
    checkForKeyPress() # clear out any key presses in the event queue

    while True:
        if checkForKeyPress():
            pygame.event.get() # clear event queue
            return

def drawScore(score):
    scoreSurf = BASICFONT.render('Score: %s' % (score), True, WHITE)
    scoreRect = scoreSurf.get_rect()
    scoreRect.topleft = (WINDOWWIDTH - 120, 10)
    DISPLAYSURF.blit(scoreSurf, scoreRect)

def drawScores(worms):
    for i in range(0, len(worms)):
        scoreSurf = BASICFONT.render('Score: %s' % (len(worms[i].wormCoords) - 3), True, WHITE)
        scoreRect = scoreSurf.get_rect()
        scoreRect.topleft = (WINDOWWIDTH - 120, i*20)
        DISPLAYSURF.blit(scoreSurf, scoreRect)

def drawWorm(wormCoords):
    for coord in wormCoords:
        x = coord['x'] * CELLSIZE
        y = coord['y'] * CELLSIZE
        wormSegmentRect = pygame.Rect(x, y, CELLSIZE, CELLSIZE)
        pygame.draw.rect(DISPLAYSURF, DARKGREEN, wormSegmentRect)
        wormInnerSegmentRect = pygame.Rect(x + 4, y + 4, CELLSIZE - 8, CELLSIZE - 8)
        pygame.draw.rect(DISPLAYSURF, GREEN, wormInnerSegmentRect)

def drawRocks(rocks):
    for myrock in rocks:
        drawRock(myrock)

def drawRock(myrock):
    x = myrock.x * CELLSIZE
    y = myrock.y * CELLSIZE
    rockRect = pygame.Rect(x, y, CELLSIZE, CELLSIZE)
    pygame.draw.rect(DISPLAYSURF, GRAY, rockRect)

def drawApples(apples):
    for apple in apples:
        drawApple(apple)

def drawApple(coord):
    x = coord['x'] * CELLSIZE
    y = coord['y'] * CELLSIZE
    appleRect = pygame.Rect(x, y, CELLSIZE, CELLSIZE)
    pygame.draw.rect(DISPLAYSURF, RED, appleRect)

def drawLaser(xMin, xMax, yMin, yMax):
    x = xMin * CELLSIZE + CELLSIZE / 4
    y = yMin * CELLSIZE + CELLSIZE / 4
    width = (xMax-xMin)*CELLSIZE + CELLSIZE / 2
    height = (yMax-yMin)*CELLSIZE + CELLSIZE / 2
    laserRect = pygame.Rect(x, y, width, height)
    pygame.draw.rect(DISPLAYSURF, RED, laserRect)


def drawGrid():
    for x in range(0, WINDOWWIDTH, CELLSIZE): # draw vertical lines
        pygame.draw.line(DISPLAYSURF, DARKGRAY, (x, 0), (x, WINDOWHEIGHT))
    for y in range(0, WINDOWHEIGHT, CELLSIZE): # draw horizontal lines
        pygame.draw.line(DISPLAYSURF, DARKGRAY, (0, y), (WINDOWWIDTH, y))


if __name__ == '__main__':
    main()
