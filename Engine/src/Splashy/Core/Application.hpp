#pragma once
#include "Graphics/Window.hpp"
#include "Layer.hpp"
#include "Render/RendererCommands.hpp"

namespace ant
{
    class Event;

    struct AppSettings
    {
        bool running = true;

        struct //? window properties
        {
            uint32_t width = 1240;
            uint32_t height = 720;
            const char *title = "LearnOpenGL";
        } windowSettings;

        RenderApi renderApi = RenderApi::OpenGl;
    };

    class Application
    {

    public:
        ~Application();

        void Init();
        void Run();

        void Terminate() { m_appdata.running = false; }

        void OnEvent(Event &e);

        inline RenderApi GetRenderApi() { return m_appdata.renderApi; }

        const Window &GetWindow() const { return *m_window; }

        static Application *GetInstance() { return s_instance; }

        static Application *s_instance;

    protected:
        Application() {}

    private:
        AppSettings m_appdata;
        Ref<Window> m_window;

    protected:
        LayerStack m_layerStack;
        std::function<void()> m_appInitFn;
    };

}