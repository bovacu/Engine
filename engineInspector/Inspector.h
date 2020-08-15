#pragma once

#ifndef ENGINE_INSPECTOR_H
#define ENGINE_INSPECTOR_H

#include <engine/main/Engine.h>

class Inspector : public engine::Layer {

    public:
        struct InspectorLogger {
        ImGuiTextBuffer     buf;
        ImGuiTextFilter     filter;
        ImVector<int>       lineOffsets;
        bool                autoScroll;

        InspectorLogger() {
            this->autoScroll = true;
            this->clear();
        }
        void clear() {
            this->buf.clear();
            this->lineOffsets.clear();
            this->lineOffsets.push_back(0);
        }
        void addLog(const char* _log, ...) {
            int _oldSize = this->buf.size();
            va_list _args;
                    va_start(_args, _log);
            this->buf.appendfv(_log, _args);
                    va_end(_args);
            for (int new_size = this->buf.size(); _oldSize < new_size; _oldSize++)
                if (this->buf[_oldSize] == '\n')
                    this->lineOffsets.push_back(_oldSize + 1);
        }
        void draw(const char* _title) {
            ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

//            if (clear)
//                Clear();
//            if (copy)
//                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* _buf = this->buf.begin();
            const char* _bufEnd = this->buf.end();
//            if (Filter.IsActive()) {
//                // In this example we don't use the clipper when Filter is enabled.
//                // This is because we don't have a random access on the result on our filter.
//                // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
//                // especially if the filtering function is not trivial (e.g. reg-exp).
//                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
//                {
//                    const char* line_start = buf + LineOffsets[line_no];
//                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
//                    if (Filter.PassFilter(line_start, line_end))
//                        ImGui::TextUnformatted(line_start, line_end);
//                }
//            }
//            else
//            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
                // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
                // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
                // both of which we can handle since we an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
                // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
                ImGuiListClipper _clipper;
                _clipper.Begin(this->lineOffsets.Size);
                while (_clipper.Step()) {
                    for (int _lineNo = _clipper.DisplayStart; _lineNo < _clipper.DisplayEnd; _lineNo++) {
                        const char* _lineStart = _buf + this->lineOffsets[_lineNo];
                        const char* _lineEnd = (_lineNo + 1 < this->lineOffsets.Size) ? (_buf + this->lineOffsets[_lineNo + 1] - 1) : _bufEnd;
                        ImGui::TextUnformatted(_lineStart, _lineEnd);
                    }
                }
            _clipper.End();
//            }
            ImGui::PopStyleVar();

            if (this->autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
        }
    };

    private:
        engine::OrthographicCameraController    cameraController;
        engine::FrameBufferPtr                  frameBufferPtr  = nullptr;
        engine::Vec2f                           viewportSize    = {0.0f, 0.0f};

        engine::ScenePtr                        scene;
        engine::GameObject                      squareGbj;
        engine::GameObject                      camera;

        bool                                    viewportFocused = false, viewportHovered = false;
        bool                                    playGame = false;
        entt::entity                            gameObjectSelectedInHierarchy = entt::null;

    public:
    #if defined(USING_INSPECTOR)
        static InspectorLogger                  logger;
    #endif

    private:
        void imGuiHierarchy();
        void imGuiComponents();
        void imGuiAssets();
        void imGuiScene();
        void imGuiConsole();
        void imGuiMenuBar();
        void imGuiActionButtonsBar();
        void imGuiColorPicker(engine::Color& _color);

    public:
        Inspector();
        ~Inspector()                                override = default;

        void onInit()                               override;
        void onEvent(engine::Event& _e)             override;
        void onUpdate(engine::Delta _dt)            override;
        void onFixedUpdate(engine::Delta _dt)       override;
        void onRender(engine::Delta _dt)            override;
        void onImGuiRender(engine::Delta _dt)       override;
        void onEnd()                                override;



};

#endif //ENGINE_INSPECTOR_H
