#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUMPIXELS 32

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Player
{
    int score;
};

Player p1, p2;

// int timer;

const int potentiometer_pin = A0;
const int btnPin = 3;


int btnState = HIGH;
int lastButtonState;
int buttonPushed;
bool toggle = false;
bool playing = true;
bool reverse = false;

int readPotentiometer;



void allRed() {
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    }
    pixels.show();
}

void gameOver() {
    pixels.clear();
    allRed();
    delay(100);
}


void showScore(int score) {
    pixels.clear();
    for (int i = 0; i < score;i++) {
        pixels.setPixelColor(i, pixels.Color(0,255,0));
    }
    pixels.show();
}

void setup() {
    pixels.clear();
    pinMode(btnPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    pixels.begin();
    p1.score = 0;
    p2.score = 0;
}

void loop() {
    static unsigned long t_val = 0;
    static int i = 0;
    static int timer = 5;
    int read = digitalRead(btnPin);


    // timer = map(analogRead(potentiometer_pin), 0, 1023, 50, 500);


    // digitalWrite(LED_BUILTIN, HIGH);
    // Serial.println(digitalRead(btnPin));
    // delay(100);
    // digitalWrite(LED_BUILTIN, LOW);
    // delay(100);

    if (read != lastButtonState) {
        buttonPushed = millis();
    }

    if (millis() - buttonPushed >= 10) {
        if (btnState != read) {
            btnState = read;
            if (btnState == LOW) {
                toggle = !toggle;
            }
        }
    }

    if (btnState == LOW && i == 31) {
        reverse = true;
        pixels.clear();
        timer = 2;
    } else if (btnState == LOW && i == 0) {
        timer = 2;
        reverse = false;
        pixels.clear();
    }


    if (millis() - t_val >= timer) {
        t_val = millis();
        if(!reverse){
            if (timer > 100) {
                timer = 100;
            } else {
                timer += 1;
            }
        } else {
            if (timer > 100) {
                timer = 100;
            } else {
                timer += 1;
            }
            
        }


        if (reverse && playing) {
            i--;
        } else if (playing) {
            i++;
        }

        if ( i >= 32 && playing) {
            playing = false;
            p1.score++;
            showScore(p1.score);
            delay(1000);
        } else if (i < 0 && playing) {
            playing = false;
            p2.score++;
            showScore(p2.score);
            delay(1000);
        }

        if (i >= 32 || i < 0) {
            gameOver();
            delay(40);
            if (btnState == LOW) {
                pixels.clear();
                playing = true;
                i = 0;
            }
        }
        if (playing) {
            pixels.setPixelColor(i, pixels.Color(0,0,255));
        }
        pixels.show();

    }
    lastButtonState = read;
}