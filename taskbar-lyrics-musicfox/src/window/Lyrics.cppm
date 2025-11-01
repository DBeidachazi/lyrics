module;

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>

export module window.Lyrics;

import plugin.Config;

export class Lyrics {
private:
    ID2D1RenderTarget *renderTarget = nullptr;
    IDWriteFactory *writeFactory = nullptr;

    auto createTextFormat(const std::wstring &fontFamily, int fontSize, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_TEXT_ALIGNMENT alignment) -> Microsoft::WRL::ComPtr<IDWriteTextFormat> {
        Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
        if (this->writeFactory) {
            this->writeFactory->CreateTextFormat(
                fontFamily.c_str(),
                nullptr,
                weight,
                style,
                DWRITE_FONT_STRETCH_NORMAL,
                static_cast<FLOAT>(fontSize),
                L"zh-cn",
                &textFormat
            );
            if (textFormat) {
                textFormat->SetTextAlignment(alignment);
                textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            }
        }
        return textFormat;
    }

    auto getColorFromARGB(unsigned int argb) -> D2D1_COLOR_F {
        float a = static_cast<float>((argb >> 24) & 0xFF) / 255.0f;
        float r = static_cast<float>((argb >> 16) & 0xFF) / 255.0f;
        float g = static_cast<float>((argb >> 8) & 0xFF) / 255.0f;
        float b = static_cast<float>(argb & 0xFF) / 255.0f;
        return D2D1::ColorF(r, g, b, a);
    }

public:
    Lyrics(ID2D1RenderTarget *target, IDWriteFactory *factory) 
        : renderTarget(target), writeFactory(factory) {}

    auto onDraw() -> void {
        if (!this->renderTarget || !this->writeFactory) {
            return;
        }

        auto size = this->renderTarget->GetSize();

        // 判断是否有副歌词
        bool hasSecondary = !config.lyric_secondary.empty() && config.lyric_secondary != L" ";

        if (hasSecondary) {
            // 双行模式：计算总高度，使两行歌词整体居中
            float totalHeight = config.size_primary + config.line_spacing + config.size_secondary;
            float startY = (size.height - totalHeight) / 2.0f;

            // 绘制主歌词
            if (!config.lyric_primary.empty() && config.lyric_primary != L" ") {
                D2D1_RECT_F rectPrimary = D2D1::RectF(0, startY, size.width, startY + config.size_primary);

                auto textFormat = this->createTextFormat(
                    config.font_family,
                    config.size_primary,
                    config.weight_primary,
                    config.slope_primary,
                    config.align_primary
                );

                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
                this->renderTarget->CreateSolidColorBrush(
                    this->getColorFromARGB(config.color_primary),
                    &brush
                );

                if (textFormat && brush) {
                    textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                    this->renderTarget->DrawText(
                        config.lyric_primary.c_str(),
                        static_cast<UINT32>(config.lyric_primary.length()),
                        textFormat.Get(),
                        rectPrimary,
                        brush.Get()
                    );
                }
            }

            // 绘制副歌词
            float secondaryY = startY + config.size_primary + config.line_spacing;
            D2D1_RECT_F rectSecondary = D2D1::RectF(0, secondaryY, size.width, secondaryY + config.size_secondary);

            auto textFormat = this->createTextFormat(
                config.font_family,
                config.size_secondary,
                config.weight_secondary,
                config.slope_secondary,
                config.align_secondary
            );

            Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
            this->renderTarget->CreateSolidColorBrush(
                this->getColorFromARGB(config.color_secondary),
                &brush
            );

            if (textFormat && brush) {
                textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                this->renderTarget->DrawText(
                    config.lyric_secondary.c_str(),
                    static_cast<UINT32>(config.lyric_secondary.length()),
                    textFormat.Get(),
                    rectSecondary,
                    brush.Get()
                );
            }
        } else {
            // 单行模式：只有主歌词，垂直居中
            if (!config.lyric_primary.empty() && config.lyric_primary != L" ") {
                D2D1_RECT_F rect = D2D1::RectF(0, 0, size.width, size.height);

                auto textFormat = this->createTextFormat(
                    config.font_family,
                    config.size_primary,
                    config.weight_primary,
                    config.slope_primary,
                    config.align_primary
                );

                Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
                this->renderTarget->CreateSolidColorBrush(
                    this->getColorFromARGB(config.color_primary),
                    &brush
                );

                if (textFormat && brush) {
                    this->renderTarget->DrawText(
                        config.lyric_primary.c_str(),
                        static_cast<UINT32>(config.lyric_primary.length()),
                        textFormat.Get(),
                        rect,
                        brush.Get()
                    );
                }
            }
        }
    }
};
