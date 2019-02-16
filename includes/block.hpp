#pragma once

class Block
{
public:
	inline Block() {};
	~Block();

	bool isActive(void);
	int getType(void);
	void setType(int t);
private:
	uint8_t type = AIR_BLOCK; //need to change to smaller data size ie. char/short// 1==grass, 0==air, 2==sand, 3==snow 
};