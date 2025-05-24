#include "App.h"

int main(int argc, char **argv) {
    Logger::Info("BilliardShow started.");
    App app;
    app.Run();
    Logger::Info("BilliardShow exited.");
    return 0;
}