#pragma once

#include "shader.h"
#include "mesh.h"

unsigned int TextureFromFile(const char *path, const string &directory);

class Model 
{
public:
    // Constructor
    inline Model(string path) { loadModel(path); }
    void Draw(Shader shader);
private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded; 
    /*  Functions   */
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
