#pragma once

class TetrisGameScene;
class TetrisMap;
class TetrominoAction;
class TetrominoFactory;

class TetrisManager : public enable_shared_from_this<TetrisManager>
{
public:
    

public:
    void init(TetrisGameScene *scene);
    void request(const int& code);

    const shared_ptr<TetrisMap>& getTetrisMap() const;
    const shared_ptr<TetrominoAction>& getTetromino() const;

private:
    TetrisGameScene *_scene;
    shared_ptr<TetrisMap> _gridMap;
    shared_ptr<TetrominoAction> _action;
    unique_ptr<TetrominoFactory> _factory;
};