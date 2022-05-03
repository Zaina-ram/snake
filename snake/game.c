#include <stdint.h>
#include <pic32mx.h> 
#include "header.h" 

// initilize all variables needed for the game
int sw1;
int BTN1,BTN2,BTN3,BTN4;
int score = 0;

//We didn't know how to get the rand() and srand() functions from the c library to work 
//So we created the apple's x and y positions as arrays with prefilled values. 
int appleX[] = {42,95,39,88,21,68,13,100};
int appleY[] = {8,20,15,9,18,13,17,13};
// An array with all the hexadecimal numbers representing the LEDs lighting up one after another, until
// all our lighten up.
int lights[] = {0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};


void moveSnake() { 
    //Store all the buttons
    BTN1 = getbtn1(); 
    BTN2 = getbtns234() & 0x1;
    BTN3 = getbtns234() & 0x2;
    BTN4 = getbtns234() & 0x4;

    // for loop that will make the body of the snake follow the head. 
    //ex: snake.x[9] and snake.y[9] at x = 6 and y = 3 will be moved to snake.x[8] and snake.y[8] at x = 7 and y = 3
    //this will be done for each "part" of the snake, making all the parts follow each other
    int i;
    for(i = snake.length - 1; i > 0; i--) { 
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    
    //change the coordinates according to what direction it is
    if(snake.dir == 'R') { 
        snake.x[0] += 1;
    }
    if(snake.dir == 'D') { 
        snake.y[0] += 1;
    }
    if(snake.dir == 'U') { 
        snake.y[0] -= 1;
    }
    if(snake.dir == 'L') { 
        snake.x[0] -= 1;
    }

    // We use this to ensure that the direction is saved even after the button is released
    if(BTN1 && snake.dir != 'L') {
        snake.dir = 'R';  
    }
    if(BTN2 && snake.dir != 'U') {
        snake.dir = 'D';
    }
    if(BTN3 && snake.dir != 'D') {
        snake.dir = 'U';
    }
    if(BTN4 && snake.dir != 'R') {
        snake.dir = 'L';
    }
}
//This function is called when a player wins by eating 8 apples.
//the score will be displayed and the LEDs will light upp.
void winner() {
    sw1= getsw1();
    clear();
    // This will loop intil switch 1 is flipped and the game restarts
    while(!sw1) {
        PORTE = PORTE + 2;
        display_string(0, "YOU WON! SCORE:");
        display_string(1, itoaconv(score));
        display_string(2, "RESTART: SW1");
        display_update();
        sw1 = getsw1();
        delay(1000);
    }
    //reset the score for the next game, turn off the LEDs, clear the display and 
    //restart the game in main
    score = 0;
    PORTE = 0x0;
    clear();
    main();  
}

//When the snake collides with itself or a wall, the palyer loses 
//the score is displayed 
void loser() {
    PORTE = 0x0;
    sw1 = getsw1();
    clear();
    //loop until switch 1 is filpped to restart
    while(!sw1) { 
        display_string(0, "YOU LOST! SCORE:");
        display_string(1, itoaconv(score));
        display_string(2, "RESTART: SW1");
        display_update();
        sw1 = getsw1();
    }
    score = 0;
    clear();
    main(); 
}

// function to draw the snake 
void drawSnake() { 
    int i, x, y;
    for(i = 0; i < snake.length; i++) {
        x = snake.x[i];
        y = snake.y[i];
        //light a pixel at the x-y coordinates for the snake
        display[y][x] = 1; 
    }
}

//function to draw an apple
//technically we are drawing 4 apples by lighting 4 pixels next to eachother, 
//we did that so the apple will be bigger and easier to "eat"
//The apples position will be picked from the arrays with prefilled values based on the current score
void drawApple() {
    int i, j;
    for(i = 0; i < 2; i++) { 
        for(j = 0; j < 2; j++) {
            display[appleY[score]+j][appleX[score]+i] = 1;
        }
    }
}

//function to get status of btns 2,3 and 4 at bit indexes 5,6 and 7 in PORTD
int getbtns234(void) { 
    int temp;
    temp = (PORTD >> 5) & 0x7;
    return temp;
}

//function to get status of btn 1 at bit index 1 in PORTF
int getbtn1(void) {
    int temp;
    temp = (PORTF >> 1) & 0x1;
    return temp;
}

//function to get status of sw 1 at bit index 8 in PORTF
int getsw1(void) {
    int temp;
    temp = (PORTD >> 8) & 0x1;
    return temp;
}

void game() { 
    //call the snake movments
    moveSnake(); 
    //draw the snake
    drawSnake(); 
    // draw the apple
    drawApple(); 
    
    // here we check if the snake ate an apple and in that case we increase the length by 2, increase the score
    // by 1, and light up the LED accordingly
    //checking the collision with the apple is done by checking that the snake's head cooordinates are equal to an apple
    int i, j;
    for(i = 0; i < 2; i++) { 
        for(j = 0; j < 2; j++) {
            if(snake.y[0] == appleY[score]+j && snake.x[0] == appleX[score]+i) {
                snake.length = snake.length + 2; 
                score++;
                PORTE = lights[score-1];
                if(score == 8) {
                    winner();
                }
                break;
            }
        }
    }
    
    //here we check if the snake has hit a wall. (we know that the display has the dimensions 32 X 128)
    if((snake.y[0] == 32 || snake.y[0] == 0) || (snake.x[0] == 128 || snake.x[0] == 0)) { //If we collide with wall
        loser();
    }

    //here we check if the head of the snake collides with the body
    int k;
    for(k = snake.length - 1; k > 0; k--) { 
        if(snake.x[0] == snake.x[k] && snake.y[0] == snake.y[k]) {
            loser();
        }
    }

    delay(1000);
}