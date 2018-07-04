#include "headers.h"
#include "GameOver.h"

GameOver::GameOver(Layer * scene)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _ttfGameOver = Label::createWithTTF("G A M E O V E R", "fonts/Fredoka.ttf", 50);
    _ttfGameOver->setPositionX(visibleSize.width * 0.5F);
    _ttfGameOver->setPositionY(visibleSize.height * 0.55F);
    
    scene->addChild(_ttfGameOver, 1);

    _ttfRestart = Label::createWithTTF("press 'R' key to restart. . .", "fonts/BreeSerif.ttf", 20);
    _ttfRestart->setPositionX(visibleSize.width * 0.5F);
    _ttfRestart->setPositionY(visibleSize.height * 0.40F);
    scene->addChild(_ttfRestart, 1);

    auto fout = FadeOut::create(1.5f);
    auto fin = FadeIn::create(1.f);
    auto seq = Sequence::create(fout, fin, NULL);
    _repeatAction = RepeatForever::create(seq);

    _ttfRestart->runAction(_repeatAction);
    show(false);
}

void GameOver::show(bool visible)
{
    _ttfGameOver->setVisible(visible);
    _ttfRestart->setVisible(visible);
    
    if (visible) 
        Director::getInstance()->getActionManager()->resumeTarget(_ttfRestart);
    else
        Director::getInstance()->getActionManager()->pauseTarget(_ttfRestart);
}

DebugUI::DebugUI(Layer * scene)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _ttfBlockPool = Label::createWithTTF("BLOCK: ", "fonts/Acme.ttf", 30);

    static const float mx = BLOCK_SIZE * MAX_COL;

	// 向右偏移120单位
    static const float x = mx + (visibleSize.width - mx) * 0.5F + 120;

    _ttfBlockPool->setPositionX(x);

    _ttfBlockPool->setPositionY(visibleSize.height * 0.2F);
    _ttfBlockPool->setColor(Color3B::GREEN);
    scene->addChild(_ttfBlockPool, 1);
}

void DebugUI::show(bool visible)
{
    _ttfBlockPool->setVisible(visible);
}

void DebugUI::changeText(const string & text)
{
    _ttfBlockPool->setString(text);
}
