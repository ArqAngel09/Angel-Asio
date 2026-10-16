#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(760, 540);
    title.setText("UltraASIO Lab", juce::dontSendNotification);
    title.setFont(juce::Font(30.0f, juce::Font::bold));
    title.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(title);

    subtitle.setText("Low-latency device setup • Windows 11 • Experimental", juce::dontSendNotification);
    subtitle.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(subtitle);

    deviceLabel.setText("Input device", juce::dontSendNotification);
    rateLabel.setText("Sample rate", juce::dontSendNotification);
    bufferLabel.setText("Buffer size", juce::dontSendNotification);
    for (auto* c : { &deviceLabel, &rateLabel, &bufferLabel }) addAndMakeVisible(*c);

    addAndMakeVisible(inputDevice);
    addAndMakeVisible(outputDevice);
    outputDevice.setText("Output device", juce::dontSendNotification);
    addAndMakeVisible(sampleRate);
    addAndMakeVisible(bufferSize);

    sampleRate.addItem("44100 Hz", 1);
    sampleRate.addItem("48000 Hz (recommended starting point)", 2);
    sampleRate.addItem("88200 Hz", 3);
    sampleRate.addItem("96000 Hz", 4);
    sampleRate.setSelectedId(2);

    bufferSize.addItem("64 samples (if supported)", 1);
    bufferSize.addItem("128 samples", 2);
    bufferSize.addItem("256 samples (stable starting point)", 3);
    bufferSize.addItem("512 samples", 4);
    bufferSize.addItem("1024 samples", 5);
    bufferSize.setSelectedId(3);

    refreshButton.addListener(this);
    applyButton.addListener(this);
    addAndMakeVisible(refreshButton);
    addAndMakeVisible(applyButton);

    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    statusLabel.setText(statusText, juce::dontSendNotification);
    addAndMakeVisible(statusLabel);

    disclaimer.setText("This is a device control/diagnostic prototype, NOT an ASIO driver. "
                       "It cannot create zero latency or replace Apollo DSP. Use the DAW's driver settings for FL Studio.",
                       juce::dontSendNotification);
    disclaimer.setColour(juce::Label::textColourId, juce::Colours::orange);
    disclaimer.setJustificationType(juce::Justification::topLeft);
    addAndMakeVisible(disclaimer);

    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addAudioCallback(this);
    refreshDevices();
    startTimerHz(2);
}

MainComponent::~MainComponent()
{
    stopTimer();
    deviceManager.removeAudioCallback(this);
    refreshButton.removeListener(this);
    applyButton.removeListener(this);
}

void MainComponent::refreshDevices()
{
    inputDevice.clear();
    outputDevice.clear();

    auto* type = deviceManager.getCurrentDeviceTypeObject();
    if (type == nullptr)
    {
        setStatus("No audio backend available.");
        return;
    }

    const auto names = type->getDeviceNames();
    for (const auto& name : names)
    {
        inputDevice.addItem(name, inputDevice.getNumItems() + 1);
        outputDevice.addItem(name, outputDevice.getNumItems() + 1);
    }

    if (inputDevice.getNumItems() > 0) inputDevice.setSelectedId(1);
    if (outputDevice.getNumItems() > 0) outputDevice.setSelectedId(1);
    setStatus("Found " + juce::String(names.size()) + " device(s). Select and apply.");
}

void MainComponent::applySelection()
{
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = inputDevice.getText();
    setup.outputDeviceName = outputDevice.getText();
    setup.sampleRate = sampleRate.getSelectedId() == 1 ? 44100.0
                     : sampleRate.getSelectedId() == 3 ? 88200.0
                     : sampleRate.getSelectedId() == 4 ? 96000.0 : 48000.0;
    setup.bufferSize = bufferSize.getSelectedId() == 1 ? 64
                     : bufferSize.getSelectedId() == 2 ? 128
                     : bufferSize.getSelectedId() == 4 ? 512
                     : bufferSize.getSelectedId() == 5 ? 1024 : 256;

    const auto error = deviceManager.setAudioDeviceSetup(setup, true);
    if (error.isNotEmpty())
        setStatus("Could not apply: " + error);
    else if (auto* device = deviceManager.getCurrentAudioDevice())
        setStatus("Applied. " + device->getName()
                  + " • " + juce::String(device->getCurrentSampleRate(), 0)
                  + " Hz • " + juce::String(device->getCurrentBufferSizeSamples())
                  + " samples");
    else
        setStatus("Settings submitted, but no active audio device is available.");
}

void MainComponent::setStatus(const juce::String& text)
{
    statusText = text;
    statusLabel.setText(statusText, juce::dontSendNotification);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &refreshButton) refreshDevices();
    if (button == &applyButton) applySelection();
}

void MainComponent::timerCallback()
{
    if (auto* d = deviceManager.getCurrentAudioDevice())
    {
        const auto rate = d->getCurrentSampleRate();
        const auto block = d->getCurrentBufferSizeSamples();
        const auto ms = rate > 0.0 ? (1000.0 * block / rate) : 0.0;
        statusLabel.setTooltip("Nominal one-buffer duration: " + juce::String(ms, 2) +
                               " ms. This is not round-trip latency.");
    }
}

void MainComponent::audioDeviceIOCallbackWithContext(const float* const*, int,
                                                       float* const* outputChannelData, int numOutputChannels,
                                                       int numSamples, const juce::AudioIODeviceCallbackContext&)
{
    // Safe silence callback only. This prototype does not monitor the microphone.
    for (int ch = 0; ch < numOutputChannels; ++ch)
        if (outputChannelData[ch] != nullptr)
            juce::FloatVectorOperations::clear(outputChannelData[ch], numSamples);
}
void MainComponent::audioDeviceAboutToStart(juce::AudioIODevice*) {}
void MainComponent::audioDeviceStopped() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101820));
    g.setColour(juce::Colour(0xff263746));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(16.0f).withTrimmedTop(70.0f), 12.0f);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(28);
    title.setBounds(area.removeFromTop(42));
    subtitle.setBounds(area.removeFromTop(30));
    area.removeFromTop(22);

    deviceLabel.setBounds(area.removeFromTop(22));
    inputDevice.setBounds(area.removeFromTop(34));
    outputDevice.setBounds(area.removeFromTop(34));
    area.removeFromTop(14);

    rateLabel.setBounds(area.removeFromTop(22));
    sampleRate.setBounds(area.removeFromTop(34));
    bufferLabel.setBounds(area.removeFromTop(22));
    bufferSize.setBounds(area.removeFromTop(34));
    area.removeFromTop(16);

    auto buttons = area.removeFromTop(38);
    refreshButton.setBounds(buttons.removeFromLeft(180));
    buttons.removeFromLeft(12);
    applyButton.setBounds(buttons.removeFromLeft(180));
    area.removeFromTop(14);
    statusLabel.setBounds(area.removeFromTop(42));
    area.removeFromTop(8);
    disclaimer.setBounds(area.removeFromTop(55));
}
