#pragma once

class TetrisGameScene;
class TetrisMap;
class TetrominoFactory;
class GameManager;

class TetrominoAction : public TetrominoOrderer
{
public:
    ~TetrominoAction();
    TetrominoAction(const shared_ptr<GameManager>& manager);

public:
    void init(TetrisGameScene *scene);
    bool fall();
    void move(const int&);
    void rotate();

    bool hardDrop();

    const float& wrap(const float& x, const float& low, const float& high) { return x < low ? high : x; }

    bool rotationSimulate(array<coord_t, 4>&);
    void getLocationCorrectionValue(const array<coord_t, 4>& _simulArray, coord_t& _correctionVal);

    void tetrominoUpdate(const shared_ptr<Tetromino>& tetromino) override
    {
        _tetromino = tetromino;
    }

    void setManager(const shared_ptr<GameManager>& manager);

private:
    void requestUpdate();
    bool mapCheck(const int& _dir);

private:
    shared_ptr<Tetromino> _tetromino;
    shared_ptr<TetrisMap> _tetrisMap;
    weak_ptr<GameManager> _manager;
};