from copy import copy, deepcopy
from random import randint, shuffle

BOARD_SIZE  = 4
DEPTH_LIMIT = 4
MAX_INT     = float("inf")
MIN_INT     = float("-inf")
UP          = "up"
DOWN        = "down"
LEFT        = "left"
RIGHT       = "right"
UPLEFT      = "upleft"
UPRIGHT     = "upright"
DOWNLEFT    = "downleft"
DOWNRIGHT   = "downright"
DIRECTIONS  = [UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT]
RATIONAL    = "rational agent"
RANDOM      = "random agent"

def cellIsAvailable(cell, user):
    return cell[0] == 0 or cell[1] == user

def printBoard(board):
       print("Board")
       for y in range(0,BOARD_SIZE):
               for x in range(0,BOARD_SIZE):
                    if board[y][x][1] == RATIONAL:
                        print("A:", board[y][x][0], "  ", end="")
                    elif board[y][x][1] == RANDOM:
                        print("B:", board[y][x][0], "  ", end="")
                    else:   
                        print("0:", board[y][x][0], "  ", end="")
               print("")


def moveX(direction, x, value):
    if("left" in direction): return x - value
    if("right" in direction): return x + value
    return x

def moveY(direction, y, value):
    if("up" in direction): return y - value
    if("down" in direction): return y + value
    return y

def move(user, direction, board, x, y):
    if board[y][x][0] == 0 or board[y][x][1] != user: return None
    
    stonesLeft = board[y][x][0]

    newX1       = moveX(direction, x, 1)
    newY1       = moveY(direction, y, 1)
    newValue1   = None
    newX2       = moveX(direction, x, 2)
    newY2       = moveY(direction, y, 2)
    newValue2   = None
    newX3       = moveX(direction, x, 3)
    newY3       = moveY(direction, y, 3)
    newValue3   = None

    if(0 <= newX1 < BOARD_SIZE and 0 <= newY1 < BOARD_SIZE and cellIsAvailable(board[newY1][newX1], user)):
        newValue1 = board[newY1][newX1][0]
        if(0 <= newX2 < BOARD_SIZE and 0 <= newY2 < BOARD_SIZE and cellIsAvailable(board[newY2][newX2], user)):
            newValue2 = board[newY2][newX2][0]
            if(0 <= newX3 < BOARD_SIZE and 0 <= newY3 < BOARD_SIZE and cellIsAvailable(board[newY3][newX3], user)):
                newValue3 = board[newY3][newX3][0]
                
                newValue1   += 1
                stonesLeft  -= 1
                newValue2   +=  min(stonesLeft, 2)
                stonesLeft  -= min(stonesLeft, 2)
                newValue3   += stonesLeft
            else:
                newValue1   += 1
                stonesLeft  -= 1
                newValue2   += stonesLeft
        else:
            newValue1+= stonesLeft

    # print(direction, newValue1, newValue2, newValue3)

    temp = [list(i) for i in board]
    temp[y][x] = (0, "")
    if newValue1 != None:
        temp[newY1][newX1] = (newValue1, user)
    if newValue2 != None:
        temp[newY2][newX2] = (newValue2, user)
    if newValue3 != None:
        temp[newY3][newX3] = (newValue3, user)

    return tuple(tuple(i) for i in temp) if newValue1 != None else None   
    
def getChildren(board, user):
    moves = []
    shuffle(DIRECTIONS)
    for y in range(0,BOARD_SIZE):
        for x in range(0,BOARD_SIZE):
            if(board[y][x][1] != user): continue
            for direction in DIRECTIONS:
                moves.append(move(user, direction, board, x, y))

    ret = [x for x in moves if x is not None]

    return ret

# def getNumberOfChildren(board, user):
#     total = 0
#     for y in range(0,BOARD_SIZE):
#         for x in range(0,BOARD_SIZE):
#             if(board[y][x][1] != user): continue
#             for direction in DIRECTIONS:
#                 newX = moveX(direction, x, 1)
#                 newY = moveY(direction, y, 1)
#                 if(0 <= newX < BOARD_SIZE and 0 <= newY < BOARD_SIZE and cellIsAvailable(board[newY][newX], user)): total += 1
#     return total

# def evaluate1(board):
#     return getNumberOfChildren(board, RATIONAL) - getNumberOfChildren(board, RANDOM)

def rationalMove(board, isRational, alpha, beta, depth):
    rationalChildren = getChildren(board, RATIONAL)
    randomChildren = getChildren(board, RANDOM)

    if len(rationalChildren) == 0 or len(randomChildren) == 0 or depth == DEPTH_LIMIT:
        # print("Leaf", len(rationalChildren)- len(randomChildren))
        return (len(rationalChildren)- len(randomChildren), board)

    children = getChildren(board, RATIONAL) if isRational else getChildren(board, RANDOM)

    bestChild = children[0]
    if isRational:
        maxVal = MIN_INT
        for child in children:
            # if depth != DEPTH_LIMIT - 1: 
                # print("CHILD", isRational, depth)
                # printBoard(child)
            
            childVal = rationalMove(child, not isRational, alpha, beta, depth + 1)[0]

            if childVal > maxVal:
                bestChild = child

            maxVal = max(childVal, maxVal)
            alpha = max(alpha, maxVal)    

            # if depth != DEPTH_LIMIT - 1: print("alpha", alpha)
            if(beta < alpha): 
                # print("prune", alpha, beta)
                break;
        return (maxVal, bestChild)
    else:
        minVal = MAX_INT
        for child in children:
            # if depth != DEPTH_LIMIT - 1: 
                # print("CHILD", isRational, depth)
                # printBoard(child)
            
            childVal = rationalMove(child, not isRational, alpha, beta, depth + 1)[0]

            if childVal < minVal:
                bestChild = child

            minVal = min(childVal, minVal)
            beta = min(beta, minVal)    

            # if depth != DEPTH_LIMIT - 1: print("beta", beta)
            if(beta < alpha): 
                # print("prune", alpha, beta)
                break;
        return (minVal, bestChild)        


def randomAgent(board):
    children = getChildren(board, RANDOM)
    return children[randint(0, len(children) - 1)]

def rationalAgent(board):
    return rationalMove(board, True, MIN_INT, MAX_INT, 0)


if __name__ == '__main__':
    board = (
        ((10, RATIONAL), (0, ""), (0, ""), (0, "")),
        ((0, ""), (0, ""), (0, ""), (0, "")),
        ((0, ""), (0, ""), (0, ""), (0, "")),
        ((0, ""), (0, ""), (0, ""), (10, RANDOM))
    )

    # board = rationalAgent(board)
    turns = 0
    for i in range(0,1):
        while True:
            turns += 1
            if(len(getChildren(board, RATIONAL)) > 0):
                print("Rational player turn", turns)
                board = rationalAgent(board)[1]
                printBoard(board)
            else:
                print("Random player wins")
                printBoard(board)
                break
            if(len(getChildren(board, RANDOM)) > 0):
                print("Random player turn", turns)
                board = randomAgent(board)
                printBoard(board)
            else:
                print("Rational player wins")
                printBoard(board)
                break


