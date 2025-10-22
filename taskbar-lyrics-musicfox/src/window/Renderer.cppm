#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <wrl/client.h>

export module window.Renderer;

export class Renderer {
private:
    Microsoft::WRL::ComPtr<ID2D1Factory> d2dFactory{};
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget{};
    Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory{};
    Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat{};
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> textBrush{};
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> backgroundBrush{};

    HWND hwnd = nullptr;

public:
    auto initialize(HWND hwnd) -> bool {
        this->hwnd = hwnd;

        HRESULT hr = S_OK;

        // Create D2D factory
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, this->d2dFactory.GetAddressOf());
        if (FAILED(hr)) return false;

        // Create DWrite factory
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(this->writeFactory.GetAddressOf())
        );
        if (FAILED(hr)) return false;

        // Create text format
        hr = this->writeFactory->CreateTextFormat(
            L"Microsoft YaHei",  // Font family
            nullptr,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            24.0f,  // Font size
            L"zh-cn",
            this->textFormat.GetAddressOf()
        );
        if (FAILED(hr)) return false;

        // Center alignment
        this->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        this->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

        // Get client rect
        RECT rc;
        GetClientRect(hwnd, &rc);

        // Create render target
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        hr = this->d2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            this->renderTarget.GetAddressOf()
        );
        if (FAILED(hr)) return false;

        // Create brushes
        this->renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            this->textBrush.GetAddressOf()
        );

        this->renderTarget->CreateSolidColorBrush(
            D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f),  // Semi-transparent black
            this->backgroundBrush.GetAddressOf()
        );

        return true;
    }

    auto resize(UINT width, UINT height) -> void {
        if (this->renderTarget) {
            this->renderTarget->Resize(D2D1::SizeU(width, height));
        }
    }

    auto render(const std::wstring& lyric) -> void {
        if (!this->renderTarget) return;

        this->renderTarget->BeginDraw();

        // Clear with transparent background
        this->renderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

        // Get size
        auto size = this->renderTarget->GetSize();
        D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width, size.height);

        // Draw semi-transparent background
        this->renderTarget->FillRectangle(rect, this->backgroundBrush.Get());

        // Draw text
        this->renderTarget->DrawText(
            lyric.c_str(),
            static_cast<UINT32>(lyric.length()),
            this->textFormat.Get(),
            rect,
            this->textBrush.Get()
        );

        this->renderTarget->EndDraw();
    }

    auto cleanup() -> void {
        this->textBrush.Reset();
        this->backgroundBrush.Reset();
        this->textFormat.Reset();
        this->writeFactory.Reset();
        this->renderTarget.Reset();
        this->d2dFactory.Reset();
    }
};

