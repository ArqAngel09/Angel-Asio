#pragma once
#include <juce_audio_utils/juce_audio_utils.h>

class MainComponent final : public juce::Component,
                            private juce::AudioIODeviceCallback,
                            private juce::Timer,
                            private juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData, int numInputChannels,
                                          float* const* outputChannelData, int numOutputChannels,
                                          int numSamples, const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void buttonClicked(juce::Button* button) override;
    void refreshDevices();
    void applySelection();
    void setStatus(const juce::String& text);

    juce::AudioDeviceManager deviceManager;
    juce::Label title, subtitle, deviceLabel, rateLabel, bufferLabel, statusLabel, disclaimer;
    juce::ComboBox inputDevice, outputDevice, sampleRate, bufferSize;
    juce::TextButton refreshButton { "Refresh devices" }, applyButton { "Apply settings" };
    juce::String statusText { "Ready. Choose a device, then apply." };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
