#include "managers/input_manager.hpp"
#include "imgui.h"
#include "raylib.h"

#include <map>

ImGuiIO *Input::io;

std::map<int, std::map<KeyStateMode, KeyState>> Input::keys;
KeyStateMode Input::mode;

void Input::initialise() { io = &ImGui::GetIO(); }

bool Input::isKeyPressed(int key) {
  if (io->WantCaptureKeyboard) {
    return false;
  }

  return keys[key][mode].pressed;
}
bool Input::isKeyPressedRepeat(int key) {
  if (io->WantCaptureKeyboard) {
    return false;
  }
  return keys[key][mode].pressedRepeat;
}
bool Input::isKeyDown(int key) {
  if (io->WantCaptureKeyboard) {
    return false;
  }
  return IsKeyDown(key);
}
bool Input::isKeyReleased(int key) {
  if (io->WantCaptureKeyboard) {
    return false;
  }
  return keys[key][mode].released;
}
bool Input::isKeyUp(int key) {
  if (io->WantCaptureKeyboard) {
    return false;
  }
  return isKeyUp(key);
}

bool Input::isMouseButtonPressed(int button) {
  if (io->WantCaptureMouse) {
    return false;
  }
  return IsMouseButtonPressed(button);
}

bool Input::isMouseButtonReleased(int button) {

  if (io->WantCaptureMouse) {
    return false;
  }
  return IsMouseButtonReleased(button);
}

bool Input::isMouseButtonDown(int button) {
  if (io->WantCaptureMouse) {
    return false;
  }
  return IsMouseButtonDown(button);
}

bool Input::isMouseButtonUp(int button) {
  if (io->WantCaptureMouse) {
    return false;
  }
  return IsMouseButtonUp(button);
}

void Input::update() {
  int key = GetKeyPressed();
  while (key != 0 && !keys.contains(key)) {
    std::map<KeyStateMode, KeyState> key_mode;
    key_mode[Update] = {
        .pressed = false, .pressedRepeat = false, .released = false};
    keys[key] = key_mode;
  }

  for (auto &[key, modes] : keys) {
    if (IsKeyPressed(key)) {
      modes[Update].pressed = true;
      modes[Render].pressed = true;
    }
    if (IsKeyPressedRepeat(key)) {
      modes[Update].pressedRepeat = true;
      modes[Render].pressedRepeat = true;
    }
    if (IsKeyReleased(key)) {
      modes[Update].released = true;
      modes[Render].released = true;
    }
  }
}

void Input::startUpdate() { mode = Update; }
void Input::startRender() { mode = Render; }

void Input::resetUpdate() {
  for (auto &[key, modes] : keys) {
    modes[Update] = KeyState{};
  }
}

void Input::resetRender() {
  for (auto &[key, modes] : keys) {
    modes[Render] = KeyState{};
  }
}
