#include <raylib.h>

#include "Model.h"

Graphics::Graphics(std::string const& filename, std::string const& texturename)
    : mFileName(filename)
    , mTextureName(texturename)
{
    Load();
}

Graphics::Graphics(Graphics const& g)
    : mFileName(g.mFileName)
    , mTextureName(g.mTextureName)
{
    Load();
}

void Graphics::Load()
{
	mResources.model = LoadModel(mFileName.c_str());
    // Load model texture
    mResources.texture = LoadTexture(mTextureName.c_str());
    // Set map diffuse texture
    mResources.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = mResources.texture;
}

void Graphics::Render(Vector3 const& position, float angle, float scale /* = 1.f */) const
{
    Vector3 up = { 0.f, 1.f, 0.f };
    Vector3 vscale = { scale, scale, scale };

    DrawModelEx(mResources.model, position, up, angle, vscale, WHITE);             // Draw 3d model with texture
}

Graphics::~Graphics() {
    UnloadModel(mResources.model);
    UnloadTexture(mResources.texture);
}
