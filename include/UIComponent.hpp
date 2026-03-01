#pragma once
#include <string_view>

class UIComponent {

public:
    UIComponent();
    ~UIComponent();

    std::string_view name_;

    virtual void render() = 0;

};