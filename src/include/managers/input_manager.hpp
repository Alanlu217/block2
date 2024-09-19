#pragma once

#include "imgui.h"
#include <map>

struct KeyState {
  bool pressed = false;
  bool pressedRepeat = false;
  bool released = false;
};

enum KeyStateMode { Update, Render };

class Input {
public:
  Input() = delete;

  static void initialise();

  static bool isKeyPressed(int key);
  static bool isKeyPressedRepeat(int key);
  static bool isKeyDown(int key);
  static bool isKeyReleased(int key);
  static bool isKeyUp(int key);

  static bool isMouseButtonPressed(int button);
  static bool isMouseButtonReleased(int button);
  static bool isMouseButtonDown(int button);
  static bool isMouseButtonUp(int button);

  static float getMouseWheelMove();

private:
  static std::map<int, std::map<KeyStateMode, KeyState>> keys;

  static ImGuiIO *io;

  static KeyStateMode mode;

  static void update();
  static void startUpdate();
  static void startRender();
  static void resetUpdate();
  static void resetRender();

  friend class App;
};
