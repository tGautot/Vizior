#include "Vizior.hpp"

namespace Vizior {

Window easyWin = Window(1,1,"aaa");
loop_func loop = NULL;
int16_t targetFrameRate = 60;
std::vector<std::shared_ptr<Window>> windows = std::vector<std::shared_ptr<Window>>();

unsigned int registerWindow(std::shared_ptr<Window> win){
    int sz = windows.size();
    for(int i = 0; i < sz; i++){
        if(windows[i] == NULL){
            windows[i] = win;
            return i;
        }
    }

    windows.push_back(win);
    return sz;
}

void setLoopFunc(loop_func f){
    loop = f;
}

bool shoudKeepLooping(){
    int sz = windows.size();
    std::cout << "Found n windows: " << sz << std::endl;
    if(sz == 0) return false;

    bool should = true;
    for(int i = 0; i < sz; i++){
        should = should && !windows[i]->shouldClose();
        std::cout << "Should close after window " << i << !windows[i]->shouldClose() << std::endl;
    }
    return should;
}

void setTargetFramerate(uint16_t target){
    targetFrameRate = target;
}

void Start(){
    int fc = 0;
    while(shoudKeepLooping()){
        std::cout << "Looping " << fc << std::endl;
        loop(fc++);
        usleep(1000000 * 1.0/targetFrameRate);
        for(int i = 0; i < windows.size(); i++){
            if(windows[i] == NULL) continue;
            windows[i]->drawSource();
        }
    }
}



}