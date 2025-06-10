#include <HID-Project.h>
#include <HID-Settings.h>
 
void typeKey(uint8_t key) {
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

void setup() {
  Keyboard.begin();
  
  // Start Payload
  delay(2000); 
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(500);

  Keyboard.print("firefox https://example.com");
  typeKey(KEY_RETURN);
  delay(3000); // Wait for Firefox to load

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_PRINT_SCREEN);
  Keyboard.releaseAll();
  delay(1000);

  Keyboard.print("~/screenshot.png");
  typeKey(KEY_RETURN);
  delay(500);

  typeKey(KEY_RETURN);
  delay(500);

  // Close terminal
  Keyboard.print("exit");
  typeKey(KEY_RETURN);

  Keyboard.end();
}

void loop() {}