#include <string>

namespace vizior {
    typedef void (* loop_function)(int32_t);

    int16_t targetFrameRate = 30;

    void Init();

    void createWindow(int32_t w, int32_t h, std::string name);

    void setupLoop(loop_function loopFunc);

    void startLoop();

    void 
}