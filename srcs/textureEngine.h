#pragma once

class TextureEngine
{
public:
	inline TextureEngine() {};
	unsigned int loadTexture(char const *path);
	unsigned int loadCubemap(vector<std::string> faces);
	unsigned int TextureFromFile(const char *path, const string &directory);
};