#include <juce_gui_extra/juce_gui_extra.h>
#include "MainComponent.h"

class UltraASIOApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "UltraASIO Lab"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }
    void initialise(const juce::String&) override { window.reset(new MainWindow(getApplicationName())); }
    void shutdown() override { window = nullptr; }

    class MainWindow final : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(juce::String name)
            : DocumentWindow(name, juce::Colours::darkslategrey, allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);
            centreWithSize(760, 540);
            setResizable(true, true);
            setVisible(true);
        }
        void closeButtonPressed() override { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }
    };

private:
    std::unique_ptr<MainWindow> window;
};

START_JUCE_APPLICATION(UltraASIOApplication)
