#include "pch.h"
#include "ImageData.h"
#include "Texture.h"
#include "TextureLoader.h"

#include <wincodec.h>

bool CTextureLoader::LoadImage(const std::filesystem::path& path, ImageData& image)
{
    Microsoft::WRL::ComPtr<IWICImagingFactory> factory;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory));

    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;

    hr = factory->CreateDecoderFromFilename(
        path.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &decoder);

    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;

    hr = decoder->GetFrame(0, &frame);

    if (FAILED(hr)) return false;

    UINT width = 0;
    UINT height = 0;

    hr = frame->GetSize(&width, &height);
    if (width == 0 || height == 0) return false;

    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;

    hr = factory->CreateFormatConverter(&converter);

    if (FAILED(hr)) return false;

    hr = converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeCustom);

    if (FAILED(hr)) return false;

    ImageData loadImage{};
    loadImage.width = width;
    loadImage.height = height;
    loadImage.pixels.resize(
        static_cast<std::size_t>(width) *
        static_cast<std::size_t>(height) * 4);

    const auto stride = width * 4;

    hr = converter->CopyPixels(
        nullptr,
        stride,
        static_cast<UINT>(loadImage.pixels.size()),
        loadImage.pixels.data());
    if (FAILED(hr)) return false;

    image = std::move(loadImage);
    return true;
}

std::shared_ptr<CTexture> CTextureLoader::Load(
    ID3D11Device* device,
    const std::filesystem::path& path)
{
    if (!device) return nullptr;

    ImageData image;
    if (!CTextureLoader::LoadImage(path, image)) return nullptr;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = image.width;
    desc.Height = image.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initialData{};
    initialData.pSysMem = image.pixels.data();
    initialData.SysMemPitch = image.width * 4;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    auto hr = device->CreateTexture2D(
        &desc,
        &initialData,
        &texture);

    if (FAILED(hr)) return nullptr;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    hr = device->CreateShaderResourceView(
        texture.Get(),
        nullptr,
        &srv);

    if (FAILED(hr)) return nullptr;

    const auto result = std::make_shared<CTexture>();
    result->m_uWidth = image.width;
    result->m_uHeight = image.height;
    result->m_shaderResourceView = std::move(srv);
    return result;
}
