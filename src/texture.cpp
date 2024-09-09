#include "pktx/texture.h"
#include <vulkan/vulkan.h>
#include <ktxvulkan.h>
#include <string>
namespace pktx
{
    void ktxStreamFromFile(std::ifstream &file, ktxStream *out, std::ifstream::pos_type& pos)
    {
        ktxStream& str = *out;
        str.type = eStreamTypeCustom;
        // dont really know the difference btw address and allocator address but for our purposed we use them to store data
        str.data.custom_ptr.address = &file;
        str.data.custom_ptr.allocatorAddress = &pos;
        str.read = [](ktxStream* str, void* dst, const ktx_size_t count){
            std::ifstream& f = *(std::ifstream*)str->data.custom_ptr.address;
            f.read((char*)dst, count);
            return KTX_SUCCESS;
        };
        str.skip = [](ktxStream* str,const ktx_size_t count){
            std::ifstream& f = *(std::ifstream*)str->data.custom_ptr.address;
            f.seekg(count, std::ios::cur);
            return KTX_SUCCESS;
        };
        str.setpos = [](ktxStream* str, const ktx_off_t offset){
            std::ifstream& f = *(std::ifstream*)str->data.custom_ptr.address;
            f.seekg(*(std::ifstream::pos_type*)str->data.custom_ptr.allocatorAddress, std::ios::beg);
            f.seekg(offset, std::ios::cur);
            return KTX_SUCCESS;
        };
        str.destruct = [](ktxStream* str){
        };
    }
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
    ezr::result<Texture, ktx_error_code_e> Texture::CreateFromStream(ktxStream* file, ktxTextureCreateFlags flags)
    {
        ktxStream fstream;
        fstream.type = eStreamTypeCustom;
        fstream.data.custom_ptr.address = &file;
    }
    bool Texture::IsArray() const
    {
        return texture->isArray;
    }
    bool Texture::IsCubemap() const
    {
        return texture->isCubemap;
    }
    uint32_t Texture::BaseWidth() const
    {
        return texture->baseWidth;
    }
    uint32_t Texture::BaseHeight() const
    {
        return texture->baseHeight;
    }
    uint32_t Texture::BaseDepth() const
    {
        return texture->baseDepth;
    }
    uint32_t Texture::NumDimensions() const
    {
        return texture->numDimensions;
    }
    uint32_t Texture::NumFaces() const
    {
        return texture->numFaces;
    }
    uint32_t Texture::NumLayers() const
    {
        return texture->numLayers;
    }
    uint32_t Texture::NumLevels() const
    {
        return texture->numLevels;
    }
    size_t Texture::DataSize() const
    {
        return texture->dataSize;
    }
    size_t Texture::ElementSize() const
    {
        return ktxTexture_GetElementSize(texture);
    }
    ezr::result<uint8_t*, ktx_error_code_e> Texture::Data(uint32_t level, uint32_t layer, uint32_t face) const
    {
        size_t off;
        auto e = ktxTexture_GetImageOffset(texture, level, layer, face, &off);
        if(e != KTX_SUCCESS) return ezr::err(e);
        return ktxTexture_GetData(texture) + off;
    }
    ktxTexture* Texture::RawTexture() const
    {
        return texture;
    }
    bool Texture::NeedsTranscode() const
    {
        return ktxTexture_NeedsTranscoding(texture);
    }
    Version Texture::GetVersion() const
    {
        return texture->classId == ktxTexture1_c ? Version::One : Version::Two;
    }
    int Texture::GetVkFormat() const
    {
        return ktxTexture_GetVkFormat(texture);
    }
    ktx_error_code_e Texture::Transcode(ktx_transcode_fmt_e fmt, ktx_transcode_flags flags)
    {
        if(GetVersion() != Version::Two) return KTX_INVALID_OPERATION;
        return ktxTexture2_TranscodeBasis((ktxTexture2*)texture, fmt, flags);
    }

}