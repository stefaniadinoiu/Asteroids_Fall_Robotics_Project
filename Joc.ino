#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);


// delay between updates of the display
unsigned long delaytime = 1000;

// to Display the score */
String scoreString;
int score;
int hundreds;
String scoreStr;
String scoreArr[] = {"","","" };

//micro switches for control
int PIN_LEFT = 2;
int PIN_RIGHT = 3;
volatile unsigned long Button_Pressed;
int Button_Delay = 200;  //handle contact bounce

volatile bool Game_Over = false;

//"rythm" of the game(milliseconds)
int tick;
int Tick_Counter = 1;

//timestamp
unsigned long now;

int ship; // Initial ship position
int columns[] = {0,0,0,0,0,0,0,0}; // 8 columns, every int is row 1-8.
int randomInt;


void setup()
{
    //Initializing
    Game_Over = false;
    hundreds = 0;
    scoreArr[0] = "";
    scoreArr[1] = "";
    scoreArr[2] = "";
    score = 0;
    tick = 500;
    Tick_Counter = 1;
    ship = 3;
    now = millis();
    Button_Pressed = millis();
    randomSeed(analogRead(15));

    for (int i = 0; i<8; i++)
        columns[i] = 0;


    //we have to do a wakeup call
    lc.shutdown(0,false);

    //Set the brightness to a medium values
    lc.setIntensity(0,1);

    //clear the display
    lc.clearDisplay(0);

    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_RIGHT, INPUT_PULLUP);

    // attach button press to interrupts
    attachInterrupt(digitalPinToInterrupt(PIN_LEFT), left, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_RIGHT), right, FALLING);

}

// handle left button press

void left()
{
    if (millis() - Button_Pressed > Button_Delay) // handle switch contact bounce
    {
        if (ship != 0)
            ship--;
        else
            ship = 7;

        lc.clearDisplay(0);
        Button_Pressed = millis();
    }

    // after game over, button press will restart game
    if (Game_Over == true)
    {
        Game_Over = false;
        setup();
    }
}


// handle right button press

void right()
{
    if (millis() - Button_Pressed > Button_Delay) // handle switch contact bounce
    {
        if (ship != 7)
            ship++;
        else
            ship = 0;

        lc.clearDisplay(0);
        Button_Pressed = millis();
    }

    // after game over, button press will restart game
    if (Game_Over == true)
    {
        Game_Over = false;
        setup();
    }
}


void loop()
{

    if (millis() - now > tick)   //do every tick
    {

        // score is: how many ticks you survived
        score++;

        now = millis();

        if (Tick_Counter == 1)  //every 10th tick
        {

            // make game faster over time
            tick = tick/1.09;

            // randomly choose column
            randomInt = random(0, 8);

            // if no asteroid exists in column, create one in row 1.
            if (columns[randomInt] == 0)
            {
                columns[randomInt] = 1;
            }
        }

        if (Tick_Counter != 10)
            Tick_Counter++;
        else
            Tick_Counter = 1;

        //for every column
        for (int i = 0; i<8; i++)
        {

            if (columns[i] == 10) // delete asteroids when out of display
                columns[i] = 0;

            if (columns[i] != 0) // make asteroids fall down
                columns[i]++;
        }

        lc.clearDisplay(0);
    }


    // ship
    lc.setLed(0, 7, ship, true);

    // asteroids
    for (int i = 0; i<8; i++)
    {
        if (columns[i] > 0)
            lc.setLed(0, columns[i]-2, i, true);
        lc.setLed(0, columns[i]-3, i, true);
    }

    // detect collision of ship with asteroid
    if (columns[ship] == 10 or columns[ship] == 9)
    {
        lc.clearDisplay(0);

        // animate explosion
        for (int i = 0; i<4; i++)
        {
            lc.setLed(0,7,ship+i,true);
            lc.setLed(0,7,ship-i,true);
            lc.setLed(0,7-i,ship+i,true);
            lc.setLed(0,7-i,ship-i,true);
            lc.setLed(0,7-1.5*i,ship,true);

            lc.clearDisplay(0);
        }

        delay(45);

        // display score

        scoreStr = String(score);

        // get single digits from score int
        scoreArr[0] = scoreStr.charAt(0);
        scoreArr[1] = scoreStr.charAt(1);
        scoreArr[2] = scoreStr.charAt(2);

        // matrix can only display up to 99
        if (score < 100)
        {
            for (int i = 0; i<2; i++)
            {
                if (scoreArr[i] == "0")
                    draw_0(1+i*4);
                if (scoreArr[i] == "1")
                    draw_1(1+i*4);
                if (scoreArr[i] == "2")
                    draw_2(1+i*4);
                if (scoreArr[i] == "3")
                    draw_3(1+i*4);
                if (scoreArr[i] == "4")
                    draw_4(1+i*4);
                if (scoreArr[i] == "5")
                    draw_5(1+i*4);
                if (scoreArr[i] == "6")
                    draw_6(1+i*4);
                if (scoreArr[i] == "7")
                    draw_7(1+i*4);
                if (scoreArr[i] == "8")
                    draw_8(1+i*4);
                if (scoreArr[i] == "9")
                    draw_9(1+i*4);
            }

        }
        // if score is > 100, display only last two digits of score
        else
        {

            for (int i = 1; i<3; i++)
            {
                if (scoreArr[i] == "0")
                    draw_0(1+(i-1)*4);
                if (scoreArr[i] == "1")
                    draw_1(1+(i-1)*4);
                if (scoreArr[i] == "2")
                    draw_2(1+(i-1)*4);
                if (scoreArr[i] == "3")
                    draw_3(1+(i-1)*4);
                if (scoreArr[i] == "4")
                    draw_4(1+(i-1)*4);
                if (scoreArr[i] == "5")
                    draw_5(1+(i-1)*4);
                if (scoreArr[i] == "6")
                    draw_6(1+(i-1)*4);
                if (scoreArr[i] == "7")
                    draw_7(1+(i-1)*4);
                if (scoreArr[i] == "8")
                    draw_8(1+(i-1)*4);
                if (scoreArr[i] == "9")
                    draw_9(1+(i-1)*4);
            }

            // first digit of score ("hundreds") is displayed as dots: One dot for every hundred.
            for (int i = 1; i<10; i++)
            {
                if (scoreArr[0] == String(i))
                    hundreds = i;
            }
            for (int i = 1; i <= hundreds; i++)
            {
                lc.setLed(0,0,i-1,true);
                lc.setLed(0,1,i-1,true);
                delay(200);
            }

        }
        Game_Over = true;
        while (Game_Over == true)
        {
            //wait for button press
        }
    }

}

//Draw score digits

void draw_0(int position)
{

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010001);
    lc.setColumn(0,2+position,B00011111);
}


void draw_1(int position)
{

    lc.setColumn(0,0+position,B00001000);
    lc.setColumn(0,1+position,B00011111);

}

void draw_2(int position)
{

    lc.setColumn(0,0+position,B00010111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011101);
}
void draw_3(int position)
{

    lc.setColumn(0,0+position,B00010001);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);
}
void draw_4(int position)
{

    lc.setColumn(0,0+position,B00011100);
    lc.setColumn(0,1+position,B00000100);
    lc.setColumn(0,2+position,B00011111);
}
void draw_5(int position)
{

    lc.setColumn(0,0+position,B00011101);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00010111);
}
void draw_6(int position)
{

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00010111);
}
void draw_7(int position)
{

    lc.setColumn(0,0+position,B00010000);
    lc.setColumn(0,1+position,B00010011);
    lc.setColumn(0,2+position,B00011100);
}
void draw_8(int position)
{

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);
}

void draw_9(int position)
{

    lc.setColumn(0,0+position,B00011101);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);
}
