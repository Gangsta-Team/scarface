#include "main.hpp"
#include "logger.hpp"
#include "renderer.hpp"
#include "main_window.hpp"

int main(int argc, char *argv[])
{

    using namespace gangsta;

    vega::g_Renderer.Initialize();
    
    while(vega::g_Renderer.running)
    {
        vega::g_Renderer.BeginRender();
        RenderMainWindow();
        vega::g_Renderer.EndRender();
    }

    vega::g_Renderer.Shutdown();

    return 0;
}