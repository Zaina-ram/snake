import tcod as t
from client import Client
 
screen_width = 40
screen_height = 20
clientNumber = 0
# Peripherals
key = t.Key()
mouse = t.Mouse()

 
MAP = [
    "           ##################### ",
    "############                  ###",
    "#                       T      ##",
    "#    T                          #",
    "#             T         p       #",
    "#                               #",
    "#        T           T          #",
    "##             T                #",
    " ##                        ######",
    "  ##########################     "
]
# Player
class Player:
    def  __init__(self, x, y):
        self.x = x
        self.y = y
        self.player = (x,y)
    
    def collision_R(self):
        if key.vk != t.KEY_RIGHT:
            self.move()
        else:
            self.x +=0
    def collision_L(self):
        if key.vk != t.KEY_LEFT:
            self.move()
        else:
            self.x +=0
    def collision_U(self):
        if key.vk != t.KEY_UP:
            self.move()
        else:
            self.y +=0
    def collision_D(self):
        if key.vk != t.KEY_DOWN:
            self.move()
        else:
            self.y +=0
    def move(self):
        if key.vk == t.KEY_ESCAPE:
                return
        if key.vk == t.KEY_UP and MAP[self.y-1][self.x]== ' ' :
            self.y -= 1
        elif key.vk == t.KEY_DOWN and MAP[self.y+1][self.x]== ' ' :
            self.y += 1
        if key.vk == t.KEY_LEFT and MAP[self.y][self.x-1]== ' ' :
            self.x -= 1
        elif key.vk == t.KEY_RIGHT and MAP[self.y][self.x+1]== ' ' :
            self.x += 1
        if  key.vk == t.KEY_SPACE and MAP[self.y][self.x+1]== 'p':  
            set_map(self.x+1, self.y, ' ')
        if  key.vk == t.KEY_SPACE and MAP[self.y][self.x-1]== 'p':
            set_map(self.x-1, self.y, ' ')
        if key.vk == t.KEY_SPACE and MAP[self.y+1][self.x]== 'p':
            set_map(self.x, self.y+1, ' ')
        if key.vk == t.KEY_SPACE and MAP[self.y-1][self.x]== 'p':
            set_map(self.x, self.y-1, ' ')
        
        self.update()
        
    def update(self):
        self.player = (self.x, self.y)

     
def read_pos(str):
    str = str.split(",")
    return int(str[0]), int(str[1])


def make_pos(tup):
    return str(tup[0]) + "," + str(tup[1])
 
def set_map(x, y, c):
    if y > len(MAP) or y < 0:
        return
    if x > len(MAP[0]) or x < 0:
        return
    MAP[y] = MAP[y][:x] + c + MAP[y][x+1:]
 
def map_draw():
    for y in range(len(MAP)):
        for x in range(len(MAP[0])):
            cchar(x, y, MAP[y][x])
 
# Game - My Game
def game():
    c = Client()
    startPos = read_pos(c.getPos())
    player1 = Player(startPos[0],startPos[1])
    player2 = Player(20,2)
 
    # Console
    t.console_init_root(screen_width, screen_height, "My Game")
    
    while not t.console_is_window_closed():
        t.console_set_default_foreground(0, t.white)
        t.sys_check_for_event(t.EVENT_KEY_PRESS, key, mouse)
 
        # Draw player
        map_draw()
        cchar(player1.x, player1.y, '@')
        cchar(player2.x, player2.y, '&')
 
        t.console_flush()
        cclear()
       
        p2Pos = read_pos(c.send(make_pos((player1.x, player1.y))))
        player2.x = p2Pos[0]
        player2.y = p2Pos[1]
        
        if ( player1.x+1  == player2.x and player1.y  ==  player2.y):
            player1.collision_R()
        elif(player1.x-1== player2.x and player1.y  ==  player2.y):
            player1.collision_L()
        elif(player1.x== player2.x and player1.y+1  ==  player2.y):
            player1.collision_D()
        elif(player1.x== player2.x and player1.y-1  ==  player2.y):
            player1.collision_U()
        else:
            player2.update()
            player1.move()
        
   
     

        
            
     

def cchar(x, y, c):
    t.console_put_char(0, x, y, ord(c))

 
def cprint(x, y, txt):
    t.console_print(0, x, y, txt)
 
def cclear():
    t.console_clear(0)
 
if __name__ == '__main__':
    game()