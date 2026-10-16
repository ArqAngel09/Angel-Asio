# Architecture and realistic scope

## Target data path
AKG Lyra USB capture -> Windows audio endpoint / WASAPI -> DAW audio engine -> Auto-Tune Pro -> playback endpoint.

UltraASIO Lab currently configures/inspects Windows audio endpoints through JUCE. It does not sit in FL Studio's live signal path and does not create a new driver.

## Why a virtual ASIO driver is not the first milestone
A true ASIO driver is a separate software component that implements Steinberg's ASIO interface and bridges it to a real-time-capable backend. It must handle device discovery, buffer lifecycle, sample-rate changes, channel formats, thread priority, device removal, error recovery, host compatibility, and installer/uninstaller behavior. Driver signing and SDK licensing also need review.

## Milestones
1. Build this device control prototype and validate device enumeration on the target PC.
2. Add latency diagnostics and repeatable loopback tests; record round-trip measurements for each buffer size.
3. Test the AKG Lyra with FL Studio ASIO and ASIO4ALL under the same project and plugin chain.
4. Only then decide whether a licensed ASIO host component or a virtual driver is justified.

## Performance rules
- Do not infer round-trip latency from buffer size alone. One 128-sample buffer at 48 kHz is 2.67 ms, but input/output buffering, converters, USB, scheduling and plugins add more.
- Do not use aggressive system-wide tweaks as defaults.
- Prefer stable audio over a buffer setting that produces dropouts.
- Never install a kernel driver without signing, uninstall support and test coverage.
