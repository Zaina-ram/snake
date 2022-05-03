// TAKEN FROM LAB
/* Declares display-related functions from display.c */
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
char * itoaconv( int num );
void quicksleep(int cyc);
void delay(int); // In assembly

int main(void);
void start();
void clear();
void translateToImage();
void game();

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

//Declare the display based on the uno32 display dimensions(info found in manual)
uint8_t display[32][128]; 

//Declare array to store the display pixels in a way that the computer can process it
uint8_t oled_display[512]; 

//declare arrays for snake
uint8_t snakeX[50];
uint8_t snakeY[50];

//struct to declare all variables needed for the snake 
typedef struct Snake {
    uint8_t *y; 
    uint8_t *x;
    int length;
    string dir;
} Snake;
Snake snake; 