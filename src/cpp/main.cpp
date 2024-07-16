#include "app.hpp"

int main(void) {
  App app;

  while (app.isOpen()) {
    app.run();
  }
}