module;

#include <dwrite.h>
#include <string>
#include <unordered_map>
#include <Windows.h>

export module plugin.Config;

auto stringToWString(const std::string &str) {
    std::wstring result(MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0), 0);
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, result.data(), result.size());
    return result;
}

export enum TASKBAR_WINDOW_ALIGNMENT {
    TASKBAR_WINDOW_ALIGNMENT_AUTO,
    TASKBAR_WINDOW_ALIGNMENT_LEFT,
    TASKBAR_WINDOW_ALIGNMENT_CENTER,
    TASKBAR_WINDOW_ALIGNMENT_RIGHT
};

export struct Config {
    // 歌词内容
    std::wstring lyric_primary = L" ";
    std::wstring lyric_secondary = L" ";
    // 上次设置的歌词（用于去重）
    std::wstring prev_lyric_primary = L" ";
    std::wstring prev_lyric_secondary = L" ";
    // 标记：主歌词更新计数（用于检测是否应该清除副歌词）
    int primary_update_count_since_secondary = 0;
    // 通用设置
    std::wstring font_family = L"Microsoft YaHei UI";
    int margin_left = 0;
    int margin_right = 0;
    int line_spacing = 4;
    TASKBAR_WINDOW_ALIGNMENT window_alignment = TASKBAR_WINDOW_ALIGNMENT::TASKBAR_WINDOW_ALIGNMENT_AUTO;
    // 主要歌词设置
    unsigned int color_primary = 0xFFFFFFFF;
    int size_primary = 14;
    bool underline_primary = false;
    bool strikethrough_primary = false;
    DWRITE_FONT_WEIGHT weight_primary = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
    DWRITE_FONT_STYLE slope_primary = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
    DWRITE_TEXT_ALIGNMENT align_primary = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
    // 次要歌词设置
    unsigned int color_secondary = 0xFFFFFFFF;
    int size_secondary = 14;
    bool underline_secondary = false;
    bool strikethrough_secondary = false;
    DWRITE_FONT_WEIGHT weight_secondary = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
    DWRITE_FONT_STYLE slope_secondary = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
    DWRITE_TEXT_ALIGNMENT align_secondary = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
} config;

export auto setConfig(const std::string &key, const std::string &value) -> bool {
    static const auto setters = std::unordered_map<std::string, bool(*)(const std::string &)>{
        // 歌词内容 - 返回 true 如果值发生变化
        {"lyric_primary", [](const std::string &str) -> bool {
            auto newVal = stringToWString(str);
            if (newVal == config.prev_lyric_primary) return false;
            config.prev_lyric_primary = config.lyric_primary;
            config.lyric_primary = newVal;

            // 主歌词更新计数+1
            config.primary_update_count_since_secondary++;

            // 当主歌词连续更新2次以上，而副歌词未更新时，清除副歌词
            // 这表示从双行歌词模式切换到了单行歌词模式
            if (config.primary_update_count_since_secondary >= 2 &&
                (config.lyric_secondary != L" " && !config.lyric_secondary.empty())) {
                config.lyric_secondary = L" ";
                config.prev_lyric_secondary = L" ";
            }

            return true;
        }},
        {"lyric_secondary", [](const std::string &str) -> bool {
            auto newVal = stringToWString(str);
            if (newVal == config.prev_lyric_secondary) return false;
            config.prev_lyric_secondary = config.lyric_secondary;
            config.lyric_secondary = newVal;

            // 副歌词更新时，重置主歌词更新计数
            config.primary_update_count_since_secondary = 0;

            return true;
        }},
        // 通用设置 - 总是返回 true（需要刷新）
        {"font_family", [](const std::string &str) -> bool { config.font_family = stringToWString(str); return true; }},
        {"margin_left", [](const std::string &str) -> bool { config.margin_left = std::stoi(str); return true; }},
        {"margin_right", [](const std::string &str) -> bool { config.margin_right = std::stoi(str); return true; }},
        {"line_spacing", [](const std::string &str) -> bool { config.line_spacing = std::stoi(str); return true; }},
        {"window_alignment", [](const std::string &str) -> bool { config.window_alignment = static_cast<TASKBAR_WINDOW_ALIGNMENT>(std::stoi(str)); return true; }},
        // 主要歌词设置
        {"size_primary", [](const std::string &str) -> bool { config.size_primary = std::stoi(str); return true; }},
        {"color_primary", [](const std::string &str) -> bool { config.color_primary = std::stoul(str, nullptr, 0); return true; }},
        {"underline_primary", [](const std::string &str) -> bool { config.underline_primary = (str == "true"); return true; }},
        {"strikethrough_primary", [](const std::string &str) -> bool { config.strikethrough_primary = (str == "true"); return true; }},
        {"weight_primary", [](const std::string &str) -> bool { config.weight_primary = static_cast<DWRITE_FONT_WEIGHT>(std::stoi(str)); return true; }},
        {"slope_primary", [](const std::string &str) -> bool { config.slope_primary = static_cast<DWRITE_FONT_STYLE>(std::stoi(str)); return true; }},
        {"align_primary", [](const std::string &str) -> bool { config.align_primary = static_cast<DWRITE_TEXT_ALIGNMENT>(std::stoi(str)); return true; }},
        // 次要歌词设置
        {"size_secondary", [](const std::string &str) -> bool { config.size_secondary = std::stoi(str); return true; }},
        {"color_secondary", [](const std::string &str) -> bool { config.color_secondary = std::stoul(str, nullptr, 0); return true; }},
        {"underline_secondary", [](const std::string &str) -> bool { config.underline_secondary = (str == "true"); return true; }},
        {"strikethrough_secondary", [](const std::string &str) -> bool { config.strikethrough_secondary = (str == "true"); return true; }},
        {"weight_secondary", [](const std::string &str) -> bool { config.weight_secondary = static_cast<DWRITE_FONT_WEIGHT>(std::stoi(str)); return true; }},
        {"slope_secondary", [](const std::string &str) -> bool { config.slope_secondary = static_cast<DWRITE_FONT_STYLE>(std::stoi(str)); return true; }},
        {"align_secondary", [](const std::string &str) -> bool { config.align_secondary = static_cast<DWRITE_TEXT_ALIGNMENT>(std::stoi(str)); return true; }},
    };
    if (const auto it = setters.find(key); it != setters.end()) {
        return it->second(value);
    }
    return false; // 未知的 key，不需要刷新
}
