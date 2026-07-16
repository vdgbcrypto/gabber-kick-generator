# Gabber Kick Generator

JUCE 8 / C++ VST3 **synth** that generates hardcore / gabber kicks from scratch — no samples.

## What it does

Triggered by a **MIDI note-on**, it plays a sine oscillator whose pitch is envelope-swept from a high "punch" frequency down to a low sub, with an amplitude decay envelope and a short transient ("click") layer on top.

DSP chain:
1. **Pitch envelope** — exponential one-pole from `PITCH_START` → `BASE_NOTE`, tau = `PITCH_DECAY` (very fast, ~2 ms → locks to sub in a few ms).
2. **Amplitude envelope** — exponential decay, tau = `AMP_DECAY` (instant attack).
3. **Transient layer** — LCG white-noise burst / saw impuls / exponential transient, own short envelope × `CLICK_MIX`.
4. Summed, mono-upmixed to stereo.

## Parameters

| Param | Range | Default | Notes |
|---|---|---|---|
| `PITCH_START` | 120 – 400 Hz | 180 | Punch start freq (sine sweeps down) |
| `PITCH_DECAY` | 1 – 8 ms | 2.0 | Pitch fall speed to sub |
| `BASE_NOTE` | 40 – 60 Hz | 50 | Final sub (tune to root: A1=55, G1=49, F#1=46.25, F1=43.7) |
| `AMP_DECAY` | 120 – 400 ms | 200 | Body length |
| `CLICK_TYPE` | 0 = White, 1 = Saw, 2 = Exp | 0 | Transient character |
| `CLICK_DECAY` | 1 – 6 ms | 2.0 | Transient length |
| `CLICK_MIX` | 0 – 0.5 | 0.20 | Transient level |

## Build (Windows / MSVC)

```bat
:: Requires JUCE 8 at C:\JUCE and Visual Studio 2022+ (cl 14.x)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target GabberKickGenerator_VST3
```

Copy the resulting `.vst3` to your VST3 folder, e.g.:
`C:\Users\<user>\AppData\Local\Programs\Common\VST3`

## Verification

`dsp_test.cpp` is an offline harness (no audio-thread, no GUI): injects a MIDI note-on
and measures output RMS + amp-decay tau. Run it to confirm the DSP numerically.

## License

MIT — see [LICENSE](LICENSE).

## Acknowledgements

Built with the Hermes VST Architect protocol: real MSVC compile + `dsp_test` numerical verification.
