#pragma once

class TetrominoShape;

struct Tetromino
{
    cocos2d::Node *axis;
    FourBlocks blocks;
    unique_ptr<TetrominoShape> shape;
};

class TetrominoShape
{
public:
    virtual ~TetrominoShape() = 0 {}
    virtual const int& getColor() = 0;
    virtual const float& getMaxAngle() = 0;
    virtual const int& referToInicoord_tTable(const int& row, const int& col) = 0;
    virtual const int& referToInitLocationTable(const int& row, const int& col) = 0;
    virtual const int& referToRotationTable(const int& row, const int& col) = 0;
};

class IShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_SKYBLUE;
    const float _maxAngle = -90.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::I[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::I[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::I[row][col]; };
};

class JShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_BLUE;
    const float _maxAngle = -270.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::J[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::J[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::J[row][col]; };
};

class LShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_ORANGE;
    const float _maxAngle = -270.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::L[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::L[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::L[row][col]; };
};

class TShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_PINK;
    const float _maxAngle = -270.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::T[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::T[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::T[row][col]; };
};

class SShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_GREEN;
    const float _maxAngle = -90.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::S[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::S[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::S[row][col]; };
};

class ZShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_RED;
    const float _maxAngle = -90.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::Z[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::Z[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::Z[row][col]; };
};

class OShape : public TetrominoShape
{
private:
    const int _color = TETRIS::COLOR_YELLOW;
    const float _maxAngle = 0.F;

public:
    const int& getColor() { return _color; }
    const float& getMaxAngle() { return _maxAngle; }
    const int& referToInicoord_tTable(const int& row, const int& col) { return crd::O[row][col]; }
    const int& referToInitLocationTable(const int& row, const int& col) { return loc::O[row][col]; }
    const int& referToRotationTable(const int& row, const int& col) { return fnt::O[row][col]; };
};