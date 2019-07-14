
/*
 * Author: Dan Allen
 * Credits:
 *  Adafruit NeoPixel Examples -> buttoncycler
 *     https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels
 *     
 *  Arduino Forum: https://forum.arduino.cc/index.php?topic=45000.0
 *    Button Debounce idea, specifically mikalhart's comment
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define BUTTON_PIN   3  //attiny85 physical pin 2
#define PIXEL_PIN    4  //Attiny85 physical pin 3
#define PIXEL_COUNT 60  // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

int     mode     = 0;    // Currently-active animation mode, 0-9

volatile boolean buttonWasPressed = false;
static unsigned long last_interrupt_time = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'

  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00001000;    // turn on interrupts on PB3
  sei();                 // enables interrupts
}

void loop() {

  //Button press is handled by the interrupt routine
  if(buttonWasPressed == true){
    buttonWasPressed = false;  
    mode++;
    if(mode > 8) {
       mode = 0; // Advance to next mode, wrap around after #8               
    }    
  }

  //Every function called has a tight coupling to the button press state and needs to check to see prior to any delay being honored.   
  switch(mode) {           
    case 0:
      colorWipe(strip.Color(0,   0,   0), 150);    // Black/off
      break;
    case 1:
      pong(strip.Color(0,   50,   0), 50);
      //colorWipe(strip.Color(255,   0,   0), 50);    // Red
      break;
    case 2:
      pong(strip.Color(100,   100,   100), 10);
      //colorWipe(strip.Color(  0, 255,   0), 50);    // Green
      break;
    case 3:
      pong(strip.Color(0,   0,   255), 50);
      //colorWipe(strip.Color(  0,   0, 255), 50);    // Blue
      break;
    case 4:
      theaterChase(strip.Color(10, 10, 10), 50); // White
      break;
    case 5:
      theaterChase(strip.Color(10,   0,   0), 50); // Red
      break;
    case 6:
      theaterChase(strip.Color(  0,   0, 5), 50); // Blue
      break;
    case 7:
      rainbow(10);
      break;
    case 8:
      theaterChaseRainbow(50);
      break;
    }   
    
}

//Interrupt Service Routine, handles the button debounce using a reading across 250 miliseconds
ISR(PCINT0_vect){
  unsigned long interrupt_time = millis();
  if(interrupt_time - last_interrupt_time > 250){
    buttonWasPressed = true;
  }
  last_interrupt_time = interrupt_time;  
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);                           //  Pause for a moment
    }
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    // 'c' counts up from 'b' to end of strip in steps of 3...
    for(int c=b; c<strip.numPixels(); c += 3) {
      strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
    }
    strip.show(); // Update strip with new contents
    
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);  // Pause for a moment
    }
  }  
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);                           //  Pause for a moment
    } 
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)  
  for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    // 'c' counts up from 'b' to end of strip in increments of 3...
    for(int c=b; c<strip.numPixels(); c += 3) {
      // hue of pixel 'c' is offset by an amount to make one full
      // revolution of the color wheel (range 65536) along the length
      // of the strip (strip.numPixels() steps):
      int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
      uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
      strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
    }
    strip.show();                // Update strip with new contents
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);                           //  Pause for a moment
    }                  // Pause for a moment
    firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
  }  
}

void pong(uint32_t color, int wait){
  for(int j=0; j<PIXEL_COUNT; j++){
    for(int i=0; i<PIXEL_COUNT; i++){
      if(j==i){
        strip.setPixelColor(i, color);      
      }else{
        strip.setPixelColor(i, strip.Color(  0,   0,   0));
      }      
    }
    
    strip.show();
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);                          
    }    
  }

  for(int j=PIXEL_COUNT; j>=0; j--){
    for(int i=PIXEL_COUNT; i>=0; i--){
      if(j==i){
        strip.setPixelColor(i, color);      
      }else{
        strip.setPixelColor(i, strip.Color(  0,   0,   0));
      }      
    }
    strip.show();
    
    if(buttonWasPressed){
      return;
    }else{
      delay(wait);                          
    } 
  }
}
