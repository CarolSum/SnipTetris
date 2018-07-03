#pragma once

class TetrisGameScene;
class TetrisMap;
class TetrominoFactory;
class TetrisManager;

class TetrominoAction : public TetrominoOrderer
{
public:
    ~TetrominoAction();
    TetrominoAction(const shared_ptr<TetrisManager>& manager);

public:
    void init(TetrisGameScene *scene);
    bool fall();
    void move(const int&);
    void rotate();

    bool hardDrop();

    const float& wrap(const float& x, const float& low, const float& high) { return x < low ? high : x; }

    bool rotationSimulate(array<TCoord, 4>&);
    void getLocationCorrectionValue(const array<TCoord, 4>& _simulArray, TCoord& _correctionVal);

    void tetrominoUpdate(const shared_ptr<Tetromino>& tetromino) override
    {
        _tetromino = tetromino;
    }

    void setManager(const shared_ptr<TetrisManager>& manager);

private:
    void requestUpdate();
    bool mapCheck(const int& _dir);

private:
    shared_ptr<Tetromino> _tetromino;
    shared_ptr<TetrisMap> _tetrisMap;
    weak_ptr<TetrisManager> _manager;
};