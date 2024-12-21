#include"FishPond.h"

//改变状态，记录钓鱼时间
void FishPond::stopFishing() 
{
        isFishing = false;
        if (1) {
            
        }
        else {
            
        }
}

// 停止钓鱼
void FishPond::stopFishing() {
    isFishing = true;
};

//开始钓鱼
void FishPond::startFishing() {
    isFishing = false;
};

bool FishPond::isCurrentlyFishing(){
    return isFishing;
}

//判断是否钓到鱼了
bool ifCaught() {

};
