/*
 * Star Wars code from https://github.com/AbhishekGhosh/Arduino-Buzzer-Tone-Codes/blob/master/star-wars.ino
 * LED code from "strandtest" NeoPixel Example
 */

/*
 * NOTES
 * The program purports to hold a tone for 'duration' microseconds.
 *  Lies lies lies! It holds for at least 'duration' microseconds, _plus_
 *  any overhead created by incremeting elapsed_time (could be in excess of 
 *  3K microseconds) _plus_ overhead of looping and two digitalWrites()
 *  
 * As a result, a tone of 'duration' plays much more slowly than a rest
 *  of 'duration.' rest_count creates a loop variable to bring 'rest' beats 
 *  in line with 'tone' beats of the same length. 
 * 
 * rest_count will be affected by chip architecture and speed, as well as 
 *  overhead from any program mods. Past behavior is no guarantee of future 
 *  performance. Your mileage may vary. Light fuse and get away.
 */
 
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

const int buzzerPin = 4;
const int pixelPin = 3;
int counter = 0;

int melody = 2; // Switch songs

// TONES
// Defining the relationship between note, period & frequency. 
// period is in microsecond so P = 1/f * (1E6)
 
#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517    // 659 Hz
#define  f5    1433    // 698 Hz
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955

// A special note, 'R', to represent a rest
#define  R     0

// MELODIES and TIMING //
// melody[] is an array of notes, accompanied by beats[], 
// which sets each note's relative length (higher #, longer note) 
 
// Melody 1: Star Wars Imperial March
int melody1[] = { a4, R,  a4, R,  a4, R,  f4, R, c5, R,  a4, R,  f4, R, c5, R, a4, R,  e5, R,  e5, R,  e5, R,  f5, R, c5, R, g5, R,  f5, R,  c5, R, a4, R};
int beats1[]  = { 50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 50, 20, 50, 20, 40, 5, 20, 5, 60, 10, 40, 5,  20, 5, 60, 40};
 
// Melody 2: Star Wars Theme
int melody2[] = { f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s, c5};
int beats2[]  = { 21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128 };

long tempo = 10000;
int pause = 1000;
int rest_count = 50;
 
int toneM = 0;
int beat = 0;
long duration  = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, pixelPin, NEO_GRB + NEO_KHZ800);
uint32_t red = strip.Color( 255, 0, 0 );
uint32_t green = strip.Color( 0, 255, 0 );
uint32_t blue = strip.Color( 0, 0, 255 );
uint32_t eyeColor[] = { red, green };

void setup() {
  strip.begin();
  pinMode( buzzerPin, OUTPUT );

  strip.setPixelColor( 0, blue );
  strip.setPixelColor( 1, blue );
  strip.show();

  if ( melody == 1 ) {  
    // Set up a counter to pull from melody1[] and beats1[]
    for (int i = 0; i < sizeof( melody1 ) / 2; i++ ) {
      toneM = melody1[i];
      beat = beats1[i];
      duration = beat * tempo;
   
      playTone();
      delayMicroseconds( pause );
    }
  } else {
    for ( int i = 0; i < sizeof( melody2 ) / 2; i++ ) {
      toneM = melody2[i];
      beat = beats2[i];
      duration = beat * tempo;
   
      playTone();
      delayMicroseconds( pause );
    }
  }
}


void loop() {
  rainbowCycle(20);
}

// PLAY TONE  //
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if ( toneM > 0 ) {
    // Play different LED depending on value of 'counter'
    strip.setPixelColor( counter % 2, eyeColor[ counter % 2 ] );
    strip.show();

    // if this isn't a Rest beat, while the tone has 
    // played less long than 'duration', pulse speaker HIGH and LOW
    while ( elapsed_time < duration ) {
 
      digitalWrite( buzzerPin, HIGH );
      delayMicroseconds( toneM / 2 );
 
      // DOWN
      digitalWrite( buzzerPin, LOW );
      delayMicroseconds( toneM / 2 );
 
      // Keep track of how long we pulsed
      elapsed_time += ( toneM );
    }

    strip.setPixelColor( counter % 2, blue );
    strip.show();
    counter++;
  } else {
    // Rest beat; loop times delay
    for ( int j = 0; j < rest_count; j++ ) {
      delayMicroseconds( duration );
    }
  }
}

void rainbowCycle( uint8_t wait ) {
  uint16_t i, j;

  for ( j = 0; j < 256 * 5; j++ ) {
    for( i = 0; i < strip.numPixels(); i++ ) {
      strip.setPixelColor( i, Wheel( ( ( i * 256 / strip.numPixels() ) + j ) & 255 ) );
    }

    strip.show();
    delay( wait );
  }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel( byte WheelPos ) {
  uint32_t color;
  
  WheelPos = 255 - WheelPos;
  if ( WheelPos < 85 ) {
    color = strip.Color( 255 - WheelPos * 3, 0, WheelPos * 3 );
  } else if ( WheelPos < 170 ) {
    WheelPos -= 85;
    color = strip.Color( 0, WheelPos * 3, 255 - WheelPos * 3 );
  } else {
    WheelPos -= 170;
    color = strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  return color;
}
