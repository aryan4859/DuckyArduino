#include <HID-Project.h>
#include <HID-Settings.h>

// Configuration constants
const int CMD_DELAY = 500;
const int LONG_DELAY = 1500;
const int SHORT_DELAY = 50;

// Utility function to simulate key press and release
void typeKey(int key) {
  Keyboard.press(key);
  delay(SHORT_DELAY);
  Keyboard.release(key);
}

// Utility function to execute a command with Enter key
void executeCommand(const char* command) {
  Keyboard.print(command);
  typeKey(KEY_RETURN);
  delay(CMD_DELAY);
}

// Utility function to open a terminal (assuming a common Linux desktop environment)
void openTerminal() {
  // Open terminal using Ctrl+Alt+T (common shortcut in many Linux DEs)
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(LONG_DELAY);
}

void setup() {
  // Initialize HID devices
  Keyboard.begin();

  // Open terminal
  openTerminal();

  // Execute shutdown command (immediate shutdown)
  executeCommand("sudo shutdown -h now");

  // Terminate HID devices
  Keyboard.end();
}

void loop() {
  // Empty loop as payload runs once
}