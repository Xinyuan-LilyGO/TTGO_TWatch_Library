// By Ponticelli Domenico.
// https://github.com/pcelli85/M5Stack_FlappyBird_game

#include <TTGO.h>
#include <EEPROM.h>

TTGOClass *ttgo;

#define TFTW            240     // screen width
#define TFTH            240     // screen height
#define TFTW2           160     // half screen width
#define TFTH2           120     // half screen height
// game constant
#define SPEED             1
#define GRAVITY         9.8
#define JUMP_FORCE     2.15
#define SKIP_TICKS     20.0     // 1000 / 50fps
#define MAX_FRAMESKIP     5
// bird size
#define BIRDW             16     // bird width
#define BIRDH             16     // bird height
#define BIRDW2            8     // half width
#define BIRDH2            8     // half height
// pipe size
#define PIPEW            24     // pipe width
#define GAPHEIGHT        42     // pipe gap height
// floor size
#define FLOORH           30     // floor height (from bottom of the screen)
// grass size
#define GRASSH            4     // grass height (inside floor, starts at floor y)

#define COLOR565(r,g,b)  ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

int maxScore = 0;
const int buttonPin = 2;
// background
const unsigned int BCKGRDCOL = COLOR565(138, 235, 244);
// bird
const unsigned int BIRDCOL = COLOR565(255, 254, 174);
// pipe
const unsigned int PIPECOL  = COLOR565(99, 255, 78);
// pipe highlight
const unsigned int PIPEHIGHCOL  = COLOR565(250, 255, 250);
// pipe seam
const unsigned int PIPESEAMCOL  = COLOR565(0, 0, 0);
// floor
const unsigned int FLOORCOL = COLOR565(246, 240, 163);
// grass (col2 is the stripe color)
const unsigned int GRASSCOL  = COLOR565(141, 225, 87);
const unsigned int GRASSCOL2 = COLOR565(156, 239, 88);

// bird sprite
// bird sprite colors (Cx name for values to keep the array readable)
#define C0 BCKGRDCOL
#define C1 COLOR565(195,165,75)
#define C2 BIRDCOL
#define C3 TFT_WHITE
#define C4 TFT_RED
#define C5 COLOR565(251,216,114)

static unsigned int birdcol[] = {
    C0, C0, C1, C1, C1, C1, C1, C0, C0, C0, C1, C1, C1, C1, C1, C0,
    C0, C1, C2, C2, C2, C1, C3, C1, C0, C1, C2, C2, C2, C1, C3, C1,
    C0, C2, C2, C2, C2, C1, C3, C1, C0, C2, C2, C2, C2, C1, C3, C1,
    C1, C1, C1, C2, C2, C3, C1, C1, C1, C1, C1, C2, C2, C3, C1, C1,
    C1, C2, C2, C2, C2, C2, C4, C4, C1, C2, C2, C2, C2, C2, C4, C4,
    C1, C2, C2, C2, C1, C5, C4, C0, C1, C2, C2, C2, C1, C5, C4, C0,
    C0, C1, C2, C1, C5, C5, C5, C0, C0, C1, C2, C1, C5, C5, C5, C0,
    C0, C0, C1, C5, C5, C5, C0, C0, C0, C0, C1, C5, C5, C5, C0, C0
};

// bird structure
static struct BIRD {
    long x, y, old_y;
    long col;
    float vel_y;
} bird;

// pipe structure
static struct PIPES {
    long x, gap_y;
    long col;
} pipes;

// score
int score;
// temporary x and y var
static short tmpx, tmpy;

// ---------------
// draw pixel
// ---------------
// faster drawPixel method by inlining calls and using setAddrWindow and pushColor
// using macro to force inlining
#define drawPixel(a, b, c) ttgo->eTFT->setAddrWindow(a, b, a, b); ttgo->eTFT->pushColor(c)


void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    resetMaxScore();
}

void loop()
{
    // put your main code here, to run repeatedly:
    game_start();
    game_loop();
    game_over();
}



// ---------------
// game loop
// ---------------
void game_loop()
{
    // ===============
    // prepare game variables
    // draw floor
    // ===============
    // instead of calculating the distance of the floor from the screen height each time store it in a variable
    unsigned char GAMEH = TFTH - FLOORH;
    // draw the floor once, we will not overwrite on this area in-game
    // black line
    ttgo->eTFT->drawFastHLine(0, GAMEH, TFTW, TFT_BLACK);
    // grass and stripe
    ttgo->eTFT->fillRect(0, GAMEH + 1, TFTW2, GRASSH, GRASSCOL);
    ttgo->eTFT->fillRect(TFTW2, GAMEH + 1, TFTW2, GRASSH, GRASSCOL2);
    // black line
    ttgo->eTFT->drawFastHLine(0, GAMEH + GRASSH, TFTW, TFT_BLACK);
    // mud
    ttgo->eTFT->fillRect(0, GAMEH + GRASSH + 1, TFTW, FLOORH - GRASSH, FLOORCOL);
    // grass x position (for stripe animation)
    long grassx = TFTW;
    // game loop time variables
    double delta, old_time, next_game_tick, current_time;
    next_game_tick = current_time = millis();
    int loops;
    // passed pipe flag to count score
    bool passed_pipe = false;
    // temp var for setAddrWindow
    unsigned char px;

    while (1) {
        loops = 0;
        while ( millis() > next_game_tick && loops < MAX_FRAMESKIP) {
            // ===============
            // input
            // ===============
            if (ttgo->button->isPressed()) {
                if (bird.y > BIRDH2 * 0.5) bird.vel_y = -JUMP_FORCE;
                else bird.vel_y = 0;
            }
            ttgo->button->loop();

            // ===============
            // update
            // ===============
            // calculate delta time
            // ---------------
            old_time = current_time;
            current_time = millis();
            delta = (current_time - old_time) / 1000;

            // bird
            // ---------------
            bird.vel_y += GRAVITY * delta;
            bird.y += bird.vel_y;

            // pipe
            // ---------------
            pipes.x -= SPEED;
            // if pipe reached edge of the screen reset its position and gap
            if (pipes.x < -PIPEW) {
                pipes.x = TFTW;
                pipes.gap_y = random(10, GAMEH - (10 + GAPHEIGHT));
            }

            // ---------------
            next_game_tick += SKIP_TICKS;
            loops++;
        }

        // ===============
        // draw
        // ===============
        // pipe
        // ---------------
        // we save cycles if we avoid drawing the pipe when outside the screen
        if (pipes.x >= 0 && pipes.x < TFTW) {
            // pipe color
            ttgo->eTFT->drawFastVLine(pipes.x + 3, 0, pipes.gap_y, PIPECOL);
            ttgo->eTFT->drawFastVLine(pipes.x + 3, pipes.gap_y + GAPHEIGHT + 1, GAMEH - (pipes.gap_y + GAPHEIGHT + 1), PIPECOL);
            // highlight
            ttgo->eTFT->drawFastVLine(pipes.x, 0, pipes.gap_y, PIPEHIGHCOL);
            ttgo->eTFT->drawFastVLine(pipes.x, pipes.gap_y + GAPHEIGHT + 1, GAMEH - (pipes.gap_y + GAPHEIGHT + 1), PIPEHIGHCOL);
            // bottom and top border of pipe
            drawPixel(pipes.x, pipes.gap_y, PIPESEAMCOL);
            drawPixel(pipes.x, pipes.gap_y + GAPHEIGHT, PIPESEAMCOL);
            // pipe seam
            drawPixel(pipes.x, pipes.gap_y - 6, PIPESEAMCOL);
            drawPixel(pipes.x, pipes.gap_y + GAPHEIGHT + 6, PIPESEAMCOL);
            drawPixel(pipes.x + 3, pipes.gap_y - 6, PIPESEAMCOL);
            drawPixel(pipes.x + 3, pipes.gap_y + GAPHEIGHT + 6, PIPESEAMCOL);
        }
        // erase behind pipe
        if (pipes.x <= TFTW) ttgo->eTFT->drawFastVLine(pipes.x + PIPEW, 0, GAMEH, BCKGRDCOL);

        // bird
        // ---------------
        tmpx = BIRDW - 1;
        do {
            px = bird.x + tmpx + BIRDW;
            // clear bird at previous position stored in old_y
            // we can't just erase the pixels before and after current position
            // because of the non-linear bird movement (it would leave 'dirty' pixels)
            tmpy = BIRDH - 1;
            do {
                drawPixel(px, bird.old_y + tmpy, BCKGRDCOL);
            } while (tmpy--);
            // draw bird sprite at new position
            tmpy = BIRDH - 1;
            do {
                drawPixel(px, bird.y + tmpy, birdcol[tmpx + (tmpy * BIRDW)]);
            } while (tmpy--);
        } while (tmpx--);
        // save position to erase bird on next draw
        bird.old_y = bird.y;

        // grass stripes
        // ---------------
        grassx -= SPEED;
        if (grassx < 0) grassx = TFTW;
        ttgo->eTFT->drawFastVLine( grassx    % TFTW, GAMEH + 1, GRASSH - 1, GRASSCOL);
        ttgo->eTFT->drawFastVLine((grassx + 64) % TFTW, GAMEH + 1, GRASSH - 1, GRASSCOL2);

        // ===============
        // collision
        // ===============
        // if the bird hit the ground game over
        if (bird.y > GAMEH - BIRDH) break;
        // checking for bird collision with pipe
        if (bird.x + BIRDW >= pipes.x - BIRDW2 && bird.x <= pipes.x + PIPEW - BIRDW) {
            // bird entered a pipe, check for collision
            if (bird.y < pipes.gap_y || bird.y + BIRDH > pipes.gap_y + GAPHEIGHT) break;
            else passed_pipe = true;
        }
        // if bird has passed the pipe increase score
        else if (bird.x > pipes.x + PIPEW - BIRDW && passed_pipe) {
            passed_pipe = false;
            // erase score with background color
            ttgo->eTFT->setTextColor(BCKGRDCOL);
            ttgo->eTFT->setCursor( TFTW2, 4);
            ttgo->eTFT->print(score);
            // set text color back to white for new score
            ttgo->eTFT->setTextColor(TFT_WHITE);
            // increase score since we successfully passed a pipe
            score++;
        }

        // update score
        // ---------------
        ttgo->eTFT->setCursor( TFTW2, 4);
        ttgo->eTFT->print(score);
    }

    // add a small delay to show how the player lost
    delay(1200);
}


// ---------------
// game start
// ---------------
void game_start()
{
    ttgo->eTFT->setTextFont(1);
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->fillRect(10, TFTH2 - 20, TFTW - 20, 1, TFT_WHITE);
    ttgo->eTFT->fillRect(10, TFTH2 + 32, TFTW - 20, 1, TFT_WHITE);
    ttgo->eTFT->setTextColor(TFT_WHITE);
    ttgo->eTFT->setTextSize(3);
    // half width - num char * char width in pixels
    ttgo->eTFT->setCursor( TFTW2 - (6 * 9), TFTH2 - 16);
    ttgo->eTFT->println("FLAPPY");
    ttgo->eTFT->setTextSize(3);
    ttgo->eTFT->setCursor( TFTW2 - (6 * 9), TFTH2 + 8);
    ttgo->eTFT->println("-BIRD-");
    ttgo->eTFT->setTextSize(2);
    ttgo->eTFT->setCursor( 10, TFTH2 - 36);
    ttgo->eTFT->println("T-Watch");
    ttgo->eTFT->setCursor( TFTW2 - (17 * 9), TFTH2 + 36);
    ttgo->eTFT->println("Premi il bottone centrale");
    while (1) {
        delay(1000);
        // wait for push button
        if (ttgo->button->isPressed()) {
            break;
        }
        ttgo->button->loop();

    }
    // init game settings
    game_init();
}

void game_init()
{
    // clear screen
    ttgo->eTFT->fillScreen(BCKGRDCOL);
    // reset score
    score = 0;
    // init bird
    bird.x = 144;
    bird.y = bird.old_y = TFTH2 - BIRDH;
    bird.vel_y = -JUMP_FORCE;
    tmpx = tmpy = 0;
    // generate new random seed for the pipe gape
    randomSeed(analogRead(0));
    // init pipe
    pipes.x = 0;
    pipes.gap_y = random(20, TFTH - 60);
}


// ---------------
// game over
// ---------------
void game_over()
{
    ttgo->eTFT->fillScreen(TFT_BLACK);
    EEPROM_Read(&maxScore, 0);

    if (score > maxScore) {
        EEPROM_Write(&score, 0);
        maxScore = score;
        ttgo->eTFT->setTextColor(TFT_RED);
        ttgo->eTFT->setTextSize(2);
        ttgo->eTFT->setCursor( TFTW2 - (13 * 6), TFTH2 - 26);
        ttgo->eTFT->println("NEW HIGHSCORE");
    }

    ttgo->eTFT->setTextColor(TFT_WHITE);
    ttgo->eTFT->setTextSize(3);
    // half width - num char * char width in pixels
    ttgo->eTFT->setCursor( TFTW2 - (9 * 9), TFTH2 - 6);
    ttgo->eTFT->println("GAME OVER");
    ttgo->eTFT->setTextSize(2);
    ttgo->eTFT->setCursor( 10, 10);
    ttgo->eTFT->print("score: ");
    ttgo->eTFT->print(score);
    ttgo->eTFT->setCursor( TFTW2 - (12 * 6), TFTH2 + 18);
    ttgo->eTFT->println("press button");
    ttgo->eTFT->setCursor( 10, 28);
    ttgo->eTFT->print("Max Score:");
    ttgo->eTFT->print(maxScore);
    while (1) {
        // wait for push button
        if (ttgo->button->isPressed()) {
            break;
        }
        ttgo->button->loop();
    }
}

void resetMaxScore()
{
    EEPROM_Write(&maxScore, 0);
}



void EEPROM_Write(int *num, int MemPos)
{
    byte ByteArray[2];
    memcpy(ByteArray, num, 2);
    for (int x = 0; x < 2; x++) {
        EEPROM.write((MemPos * 2) + x, ByteArray[x]);
    }
}



void EEPROM_Read(int *num, int MemPos)
{
    byte ByteArray[2];
    for (int x = 0; x < 2; x++) {
        ByteArray[x] = EEPROM.read((MemPos * 2) + x);
    }
    memcpy(num, ByteArray, 2);
}


