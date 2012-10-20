#ifndef TEXTUREATLAS_HH
#define TEXTUREATLAS_HH

#include <string>
#include <vector>
#include "glhck/glhck.h"

class TextureAtlas
{
public:
  struct TransformData
  {
    TransformData() : transform{0, 0, 0, 0}, degree{0} {}
    glhckRect transform;
    short degree;
  };

  TextureAtlas();
  TextureAtlas(TextureAtlas const& other);
  TextureAtlas(std::vector<std::string> const& filenames);
  TextureAtlas& operator=(TextureAtlas const& other);
  ~TextureAtlas();
  glhckTexture* getTexture() const;
  TransformData const& getTransform(int const index);

private:
  glhckTexture* texture;
  std::vector<TransformData> transforms;
};

#endif
