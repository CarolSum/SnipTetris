#pragma once
#include "TetrisParticleManager.h"

/* define */
class TetrisGameScene;
typedef std::array<std::array<bool, MAX_COL>, MAX_ROW> conceptual_map;
typedef std::array<std::array<shared_ptr<Block>, MAX_COL>, MAX_ROW> concrete_map;

class GameManager;

class TetrisMap : public TetrominoOrderer
{
public:
    TetrisMap(const shared_ptr<GameManager>& manager);
    ~TetrisMap();

private:
    void findBingo();
    void deleteLine(const int& line);
    void fall(const int& line);
	int rowIndex(const int& i) const;
	int colIndex(const int& i) const;

public:
    void init(TetrisGameScene *scene);
    bool update();
    bool isAccessible(const coord_t& _coord);
    void setTetromino(const shared_ptr<Tetromino>& tetromino) { _tetromino = tetromino; }
    void reset();

    void tetrominoUpdate(const shared_ptr<Tetromino>& tetromino) override
    {
        _tetromino = tetromino;
    }

    void setManager(const shared_ptr<GameManager>& manager);

private:
    concrete_map _concreteMap;
    conceptual_map _conceptualMap;
    shared_ptr<Tetromino> _tetromino;
    weak_ptr<GameManager> _manager;
    unique_ptr<TetrisParticleManager> _particleManager;

    function<void(const shared_ptr<Block> &)> _collector;
};
