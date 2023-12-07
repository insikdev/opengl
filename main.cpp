#include "pch.h"
#include "app.h"

int main(void)
{
    SPDLOG_INFO("Start OpenGL Tutorial");
    std::make_unique<App>()->Run();
    SPDLOG_INFO("End OpenGL Tutorial");
    return 0;
}