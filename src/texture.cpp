#include "pktx/texture.h"
#include <string>
namespace pktx
{
    ezr::result<Texture, ktx_error_code_e> Texture::CreateFromNamedFile(std::string_view file_name, ktxTextureCreateFlags flags)
    {
        Texture t;
        auto e = ktxTexture_CreateFromNamedFile(std::string(file_name).c_str(), flags, &t.texture);
        if(e != KTX_SUCCESS) return ezr::err(e);
        return t;
    }
    ezr::result<Texture, ktx_error_code_e> Texture::CreateFromMemory(uint8_t* memory, size_t size, ktxTextureCreateFlags flags)
    {
        Texture t;
        auto e = ktxTexture_CreateFromMemory(memory, size, flags, &t.texture);
        if(e != KTX_SUCCESS) return ezr::err(e);
        return t;
    }
    bool Texture::IsArray()
    {
        return texture->isArray;
    }
    bool Texture::IsCubemap()
    {
        return texture->isCubemap;
    }
    uint32_t Texture::BaseWidth()
    {
        return texture->baseWidth;
    }
    uint32_t Texture::BaseHeight()
    {
        return texture->baseHeight;
    }
    uint32_t Texture::BaseDepth()
    {
        return texture->baseDepth;
    }
    uint32_t Texture::NumDimensions()
    {
        return texture->numDimensions;
    }
    uint32_t Texture::NumLevels()
    {
        return texture->numLevels;
    }
    size_t Texture::DataSize()
    {
        return texture->dataSize;
    }
    size_t Texture::ElementSize()
    {
        return ktxTexture_GetElementSize(texture);
    }
    ezr::result<uint8_t*, ktx_error_code_e> Texture::Data(uint32_t level, uint32_t layer, uint32_t face)
    {
        size_t off;
        auto e = ktxTexture_GetImageOffset(texture, level, layer, face, &off);
        if(e != KTX_SUCCESS) return ezr::err(e);
        return ktxTexture_GetData(texture) + off;
    }
    ktxTexture* Texture::RawTexture()
    {
        return texture;
    }
    bool Texture::NeedsTranscode()
    {
        return ktxTexture_NeedsTranscoding(texture);
    }
    Version Texture::GetVersion()
    {
        return texture->classId == ktxTexture1_c ? Version::One : Version::Two;
    }
}