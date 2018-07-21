#include "Tetromino.h"

const int TetrominoI::shape[4][2] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } };
const int TetrominoI::rotationVector2[4][2] = { { 2, -1 },{ 1, 0 },{ 0, 1 },{ -1, 2 } };

const int TetrominoJ::shape[4][2] = { { 0, 1 },{ 0, 0 },{ 1, 0 },{ 2, 0 } };
const int TetrominoJ::rotationVector2[4][2] = { { 2, 0 },{ 1, -1 },{ 0, 0 },{ -1, -1 } };

const int TetrominoL::shape[4][2] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 2, 1 } };
const int TetrominoL::rotationVector2[4][2] = { { 1, -1 },{ 0, 0 },{ -1, -1 },{ 0, -2 } };

const int TetrominoO::shape[4][2] = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
const int TetrominoO::rotationVector2[4][2] = { { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } };

const int TetrominoS::shape[4][2] = { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 2, 1 } };
const int TetrominoS::rotationVector2[4][2] = { { 1, -1 },{ 0, 0 },{ 1, 1 },{ 0, -2 } };

const int TetrominoT::shape[4][2] = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
const int TetrominoT::rotationVector2[4][2] = { { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } };

const int TetrominoZ::shape[4][2] = { { 0, 1 },{ 1, 1 },{ 1, 0 },{ 2, 0 } };
const int TetrominoZ::rotationVector2[4][2] = { { 2, 0 },{ 1, 1 },{ 0, 0 },{ -1, 1 } };
