#pragma once
#include "result.hpp"
#include <ktx.h>
#include <string_view>
namespace pktx
{
    class Texture
    {
    public:
        Texture(const Texture&) = delete;
        Texture(Texture&& other) { texture = other.texture; other.texture = nullptr; }
        ~Texture() { if(texture) ktxTexture_Destroy(texture); }
    public:
        [[nodiscard]] static ezr::result<Texture, ktx_error_code_e> CreateFromNamedFile(std::string_view file_name, ktxTextureCreateFlagBits flags);
        [[nodiscard]] static ezr::result<Texture, ktx_error_code_e> CreateFromMemory(uint8_t memory, ktxTextureCreateFlagBits flags);

        
        [[nodiscard]] bool IsArray();
        [[nodiscard]] bool IsCubemap();

        [[nodiscard]] uint32_t BaseWidth();
        [[nodiscard]] uint32_t BaseHeight();
        [[nodiscard]] uint32_t BaseDepth();

        [[nodiscard]] uint32_t NumDimensions();
        [[nodiscard]] uint32_t NumLevels();

        [[nodiscard]] size_t DataSize();
        [[nodiscard]] size_t ElementSize();

        [[nodiscard]] ezr::result<uint8_t*, ktx_error_code_e> Data(uint32_t level = 0, uint32_t layer = 0, uint32_t face = 0);
        [[nodiscard]] ktxTexture* RawTexture();

    private:
        ktxTexture* texture;
    };
}