#pragma once

#include "chunk.h"

#define MAP(VAL,A,B,AA,BB) ((VAL - A) / (B - A)) * (BB - AA) + AA;

float noise(float x, float y);

class Terrain
{
public:
private:
};