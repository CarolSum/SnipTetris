#pragma once

#include <string>

#include "cocos2d.h"
USING_NS_CC;

using std::string;

class GameOver
{
public:
    GameOver(Layer *scene);

public:
    void show(bool visible);

private:
    Label *_ttfGameOver;
    Label *_ttfRestart;
    RepeatForever *_repeatAction;
};

class DebugUI
{
public:
    DebugUI(Layer *scene);

public:
    void show(bool visible);
    void changeText(const string& text);

private:
    Label *_ttfBlockPool;
};