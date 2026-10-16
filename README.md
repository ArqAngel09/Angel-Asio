# UltraASIO Lab (Windows 11)

**Experimental low-latency audio control/diagnostic app — not an ASIO driver.**

This is a JUCE-based Windows desktop prototype for inspecting audio devices and selecting sample-rate/buffer settings where supported. It does not install a kernel driver, expose a new ASIO driver to FL Studio, or promise zero latency.

## Architecture
- Phase 1: device diagnostics/control app using JUCE AudioDeviceManager.
- Phase 2: add loopback/round-trip latency measurement, glitch counters and repeatable tests.
- Phase 3: optionally integrate a properly licensed ASIO host SDK and test installed vendor drivers.
- Phase 4: investigate a separate virtual driver only if needed; driver signing, SDK licensing and extensive DAW/hardware testing are required.

## AKG Lyra + FL Studio 24
Start by testing the Lyra's native Windows endpoint and the current FL Studio ASIO / ASIO4ALL options. USB mic input plus a separate playback device can cause clock drift, resampling or instability. Test 48 kHz at 256 samples first, then 128 if stable. Raise the buffer if you hear crackles. Plugin processing, USB buffering, Windows scheduling and playback hardware all contribute to latency.

## Build on Windows 11
Prerequisites: Visual Studio 2022 with Desktop development with C++, CMake 3.24+, Git and internet access.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The executable location depends on the generator, commonly `build\Release\UltraASIOLab.exe`.

## Limitations
- No zero-latency guarantee; all real systems have finite capture, processing and playback latency.
- This prototype is not currently a DAW plugin or ASIO driver and will not appear in FL Studio's driver list.
- No system-wide audio settings are changed without user action.
- Do not install unsigned kernel/audio drivers from unknown sources.
