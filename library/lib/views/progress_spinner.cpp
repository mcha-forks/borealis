/*
    Copyright 2019-2021 natinusala
    Copyright 2021 XITRIX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <borealis/views/progress_spinner.hpp>
#include <borealis/core/application.hpp>

namespace brls
{

ProgressSpinner::ProgressSpinner() { }

void ProgressSpinner::restartAnimation()
{
    Style style = Application::getStyle();

    
    this->animationValue.reset(0);
    this->animationValue.stop();
    this->animationValue.setEndCallback([this](bool done){
        if (done)
            this->restartAnimation();
    });
    this->animationValue.addStep(8.0f, style["brls/spinner/animation_duration"], EasingFunction::linear);
    this->animationValue.start();
}

void ProgressSpinner::draw(NVGcontext* vg, float x, float y, float width, float height, Style style, FrameContext* ctx)
{
    Theme theme = Application::getTheme();
    NVGcolor barColor = a(theme["brls/spinner/bar_color"]);

    // Each bar of the spinner
    for (int i = 0 + animationValue; i < 8 + animationValue; i++)
    {
        barColor.a = fmax((i - animationValue) / 8.0f, a(theme["brls/spinner/bar_color"]).a);
        nvgSave(vg);
        nvgTranslate(vg, x + width / 2, y + height / 2);
        nvgRotate(vg, nvgDegToRad(i * 45)); // Internal angle of octagon
        nvgBeginPath(vg);
        nvgMoveTo(vg, height * style["brls/spinner/center_gap_multiplier"], 0);
        nvgLineTo(vg, height / 2 - height * style["brls/spinner/center_gap_multiplier"], 0);
        nvgStrokeColor(vg, barColor);
        nvgStrokeWidth(vg, height * style["brls/spinner/bar_width_multiplier"]);
        nvgLineCap(vg, NVG_SQUARE);
        nvgStroke(vg);
        nvgRestore(vg);
    }
}

void ProgressSpinner::willAppear(bool resetState)
{
    this->restartAnimation();
}

void ProgressSpinner::willDisappear(bool resetState)
{
    this->animationValue.stop();
}

brls::View* ProgressSpinner::create()
{
    return new ProgressSpinner();
}

} // namespace brls
