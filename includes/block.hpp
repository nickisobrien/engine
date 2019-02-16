#pragma once

class Block
{
public:
	inline Block() {};
	inline ~Block() {};

	inline bool isActive(void) { return (!(this->type == Blocktype::AIR_BLOCK || this->type == Blocktype::WATER_BLOCK)); }
	inline uint8_t getType(void) { return this->type; }
	inline void setType(uint8_t t) { this->type = t; }
private:
	uint8_t type = AIR_BLOCK; //need to change to smaller data size ie. char/short// 1==grass, 0==air, 2==sand, 3==snow 
};