#pragma once
#define CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL 1
#include <bitset>
#include "HelperClasses.h"
#include "cocos2d.h"
#include "MapLayer.h"

class MapLayer;

// PlayerSprite, inherit from cocos2d::Sprite,
// represent the player, npc, animal and anything that can move, interact
class PlayerSprite : public cocos2d::Sprite
{
public:
    // Movements enum class
    enum class MOVEMENT : char
    {
        UP, DOWN, LEFT, RIGHT, //  stand direction
        W_UP, W_DOWN, W_LEFT, W_RIGHT, //  walk direction
        I_UP, I_DOWN, I_LEFT, I_RIGHT, //  interact direction
        STAY, //  stay
        NONE, ALL //  represent no movement and all movement
    };

    // static factory method
    // doc is the json file of PlayerSprite, which should contain movements, frame_format and plist file path
    // start_gird_pos is the start position, size is the size of the sprite, always_run represents speed of moving
    static PlayerSprite* create(const rapidjson::Document* doc, const Vec<int>& start_grid_pos, const Vec<int>& size, bool always_run = false);

    // move method, move_e can be W_UP, W_DOWN, W_LEFT, W_RIGHT, STAY
    // for W_**, length is the grid you walk, e_move is the direction you move towards
    // for STAY, length is the time you display the stay action
    void move(MOVEMENT move_e, int length = -1);

    // stop method, stop the player, move_e can be W_UP, W_DOWN, W_LEFT, W_RIGHT, ALL
    // ALL will stop all movement
    void stop(MOVEMENT move_e);

    // interact method, interact with the object at the pos
    // pos is relative to the player, so (0,0) means the object is at the player's position
    // When pos == (* , -1), the player will display I_DOWN action
    // When pos == (-1, 0) or (-1, 1), the player will display I_LEFT action
    // When pos == (1, 0) or (1, 1), the player will display I_RIGHT action
    // When pos == (0, 1), the player will display I_UP action
    // When pos == (0, 0), the player will display the action corresponding to the stand direction
    void interact(const Vec<int>& pos);

    // change Speed method, change the speed of the player, it will change is_run_ variable
    void changeSpeed();

    // override update method, update camera position and calculate collision, destination
    virtual void update(float delta) override;

    // setParentMapLayer method, set the parent MapLayer of this PlayerSprite
    // this method should be called before any movement and interaction
    void setParentMapLayer(MapLayer* parent) { parent_ = parent; }

private:
    // constructor and destructor
    PlayerSprite(const rapidjson::Document* doc,const Vec<int>& start_grid_pos, const Vec<int>& size, bool always_run);
    ~PlayerSprite() override;

    // Helper function, convert "DOWN" into MOVEMENT::DOWN
    static MOVEMENT getMovementFromString(const std::string& name);

    const static std::vector<Vec<int>> directions_; // directions of movement

    std::vector<std::vector<int>> movements_ = std::vector(13, std::vector<int>()); //  Sprite movements frames
    std::string frame_format_; //  the format of frame name
    MapLayer* parent_ = nullptr; //  the MapLayer which this PlayerSprite belongs to
    cocos2d::ActionInterval* repeat_action_ = nullptr; //  current repeat action of the player
    cocos2d::Vec2 destination = { -1,-1 }; //  the destination of the player
    Vec<int> grid_pos_ = { -1,-1 }; //  the grid position of the player
    Vec<int> size_ = { 1, 2 };
    MOVEMENT stand_direction_ = MOVEMENT::DOWN; //  the direction the player is facing
    bool run_ = false; //  if the player is running, changeSpeed Method will change this
    bool is_moving = false; //  if the player is moving, stay movements is considered as moving
};

