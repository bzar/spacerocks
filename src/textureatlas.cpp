#include "textureatlas.h"

TextureAtlas::TextureAtlas() :
  texture(nullptr), transforms()
{
}

TextureAtlas::TextureAtlas(TextureAtlas const& other) :
  texture(glhckTextureRef(other.texture)), transforms(other.transforms)
{
}

TextureAtlas::TextureAtlas(std::vector<std::string> const& filenames) :
  texture(nullptr), transforms()
{
  glhckAtlas *atlas = glhckAtlasNew();
  for(std::string const filename : filenames)
  {
    glhckTexture* tex = glhckTextureNewFromFile(filename.data(), nullptr, glhckTextureDefaultSpriteParameters());
    glhckAtlasInsertTexture(atlas, tex);
    glhckTextureFree(tex);
    transforms.push_back(TransformData());
  }

  glhckAtlasPack(atlas, GLHCK_RGBA, true, true, nullptr);

  for(int i = 0; i < transforms.size(); ++i)
  {
    TransformData& t = transforms.at(i);
    glhckAtlasGetTransform(atlas, glhckAtlasGetTextureByIndex(atlas, i), &t.transform, &t.degree);
  }

  texture = glhckTextureRef(glhckAtlasGetTexture(atlas));
  glhckAtlasFree(atlas);
}

TextureAtlas& TextureAtlas::operator=(TextureAtlas const& other)
{
  if(&other != this)
  {
    if(texture)
      glhckTextureFree(texture);

    texture = other.texture;
    if(texture)
      texture = glhckTextureRef(texture);

    transforms = other.transforms;
  }

  return *this;
}

TextureAtlas::~TextureAtlas()
{
  if(texture != nullptr)
    glhckTextureFree(texture);
}

glhckTexture* TextureAtlas::getTexture() const
{
  return texture;
}

TextureAtlas::TransformData const& TextureAtlas::getTransform(int const index)
{
  return transforms.at(index);
}
