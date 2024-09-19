#pragma once
#include "result.hpp"
#include <ktx.h>
#include <string_view>
#include <fstream>
namespace pktx
{
    enum class Version
    {
        One, Two
    };
    void ktxStreamFromFile(std::ifstream& file, ktxStream* out, std::istream::pos_type& pos);
    class Texture
    {
    public:
        Texture() = default;
        Texture(const Texture&) = delete;
        Texture(Texture&& other) { texture = other.texture; other.texture = nullptr; }
        void operator=(Texture&& other) { texture = other.texture; other.texture = nullptr; }
        ~Texture() { if(texture) ktxTexture_Destroy(texture); }
    public:
        [[nodiscard]] static ezr::result<Texture, ktx_error_code_e> CreateFromNamedFile(std::string_view file_name, ktxTextureCreateFlags flags);
        [[nodiscard]] static ezr::result<Texture, ktx_error_code_e> CreateFromMemory(uint8_t* memory, size_t size, ktxTextureCreateFlags flags);
        [[nodiscard]] static ezr::result<Texture, ktx_error_code_e> CreateFromStream(ktxStream* file, ktxTextureCreateFlags flags);
        
        [[nodiscard]] bool IsArray() const;
        [[nodiscard]] bool IsCubemap() const;

        [[nodiscard]] uint32_t BaseWidth() const;
        [[nodiscard]] uint32_t BaseHeight() const;
        [[nodiscard]] uint32_t BaseDepth() const;

        [[nodiscard]] uint32_t NumDimensions() const;
        [[nodiscard]] uint32_t NumLevels() const;
        [[nodiscard]] uint32_t NumFaces() const;
        [[nodiscard]] uint32_t NumLayers() const;

        [[nodiscard]] size_t DataSize() const;
        [[nodiscard]] size_t ElementSize() const;
        [[nodiscard]] size_t ImageSize(uint32_t level) const;

        [[nodiscard]] ezr::result<uint8_t*, ktx_error_code_e> Data(uint32_t level = 0, uint32_t layer = 0, uint32_t face = 0) const;
        [[nodiscard]] ktxTexture* RawTexture() const;

        [[nodiscard]] bool NeedsTranscode() const;
        
        [[nodiscard]] Version GetVersion() const;
        [[nodiscard]] int GetVkFormat() const;
        [[nodiscard]] ktx_error_code_e Transcode(ktx_transcode_fmt_e fmt, ktx_transcode_flags flags);

    private:
        ktxTexture* texture = nullptr;
    };
}