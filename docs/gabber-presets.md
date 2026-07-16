# Gabber Kick Generator — 20 Gabber / Hardcore Presets

## Intro: how a gabber kick is designed

The gabber kick is built on a **909-style sine body whose pitch is swept downward** from a bright click-start (~120–400 Hz) to a deep sub around **45–55 Hz** (F1→A1) — the downward pitch envelope is what gives the "thud-then-rumble" punch. On top of that body the sound is defined by a **high, aggressive transient click** (noise burst, pitched saw impulse, or exponential transient) to cut through the mix, and by **heavy waveshaping distortion/clipping** that flattens and "destroys" the mid/top of the signal. The classic production trick is to **split the kick at a crossover (50–150 Hz)**, keep the subband clean, and only distort the topband, then recombine — so you get a tight, un-muddy low end under a brutal, clipped attack. Hard clip (0) gives the square, punchy edge of Rotterdam/mainroom; soft tanh clip (1) keeps a rounder, more controlled modern sound.

## Presets (Generator + Distortion)

| Preset Name | PITCH_START | PITCH_DECAY | BASE_NOTE | AMP_DECAY | CLICK_TYPE | CLICK_DECAY | CLICK_MIX | DRIVE | CROSSOVER | SUB_MIX | OUT_GAIN | CLIP_MODE | Note |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1. Rotterdam Stamp | 180 | 3 | 55 | 200 | 0 | 3 | 0.30 | 6 | 90 | 0.60 | -9 | 0 | Classic Rotterdam mainroom punch, A1 sub |
| 2. Pure Gabber 909 | 160 | 4 | 55 | 220 | 1 | 2 | 0.25 | 5 | 80 | 0.70 | -7 | 0 | 909-derived body, gentle saw click |
| 3. Neophyte Tribute | 200 | 2 | 49 | 180 | 1 | 2 | 0.35 | 7 | 95 | 0.50 | -10 | 0 | Old-school Neophyte/Bodylotion, G1 sub |
| 4. Mellow Moenie | 150 | 5 | 49 | 240 | 0 | 4 | 0.20 | 4 | 85 | 0.80 | -6 | 1 | Softer early hardcore, round sub |
| 5. French Core Sub | 140 | 6 | 46.25 | 300 | 0 | 2 | 0.15 | 3 | 60 | 0.95 | -4 | 1 | Sub-heavy French core, clean low end |
| 6. French Core Brutal | 170 | 4 | 46.25 | 260 | 1 | 3 | 0.30 | 8 | 70 | 0.70 | -11 | 0 | Distorted French core, tight sub |
| 7. Parisian Rumble | 130 | 5 | 43.7 | 320 | 0 | 2 | 0.12 | 2 | 55 | 1.00 | -3 | 1 | Deepest F1 sub, minimal distortion |
| 8. French Bounce | 160 | 3 | 55 | 280 | 2 | 3 | 0.25 | 6 | 65 | 0.85 | -8 | 0 | Exponential transient, bouncy sub |
| 9. DJ Promo Terror | 260 | 2 | 49 | 160 | 1 | 2 | 0.40 | 10 | 100 | 0.35 | -16 | 0 | Destroyed terrorcore, max drive |
| 10. Industrial Destroy | 300 | 1 | 46.25 | 140 | 1 | 1 | 0.45 | 11 | 110 | 0.30 | -18 | 0 | Absolute destroy, fastest pitch drop |
| 11. Acid Gabber | 240 | 3 | 55 | 190 | 2 | 2 | 0.35 | 9 | 95 | 0.40 | -14 | 0 | Acid-influenced destroyed kick |
| 12. Speedcore Blast | 350 | 1 | 49 | 130 | 0 | 1 | 0.50 | 11 | 120 | 0.25 | -20 | 0 | Fastest/brightest, max distortion |
| 13. Early Hardcore Clean | 150 | 5 | 55 | 250 | 0 | 4 | 0.20 | 3 | 80 | 0.90 | -5 | 1 | Clean 90s early hardcore |
| 14. Happy Hardcore Lite | 170 | 4 | 55 | 270 | 2 | 4 | 0.22 | 4 | 75 | 0.85 | -6 | 1 | Softer, rounded for happy hardcore |
| 15. Thunderdome | 190 | 3 | 46.25 | 230 | 1 | 3 | 0.30 | 6 | 90 | 0.60 | -9 | 0 | Classic Thunderdome mid-hard |
| 16. Submarine 909 | 140 | 6 | 43.7 | 340 | 0 | 3 | 0.15 | 4 | 50 | 1.00 | -5 | 1 | Deepest clean sub, long tail |
| 17. Click Monster | 220 | 3 | 55 | 180 | 1 | 5 | 0.50 | 7 | 100 | 0.40 | -11 | 0 | Click-heavy, long saw transient |
| 18. Noise Attack | 200 | 2 | 49 | 170 | 0 | 6 | 0.45 | 8 | 105 | 0.35 | -12 | 0 | Noise-burst attack dominates |
| 19. Punch Core | 210 | 2 | 55 | 160 | 2 | 2 | 0.40 | 6 | 95 | 0.50 | -9 | 0 | Tight punch, exponential transient |
| 20. Modern Hardcore | 230 | 3 | 46.25 | 210 | 2 | 3 | 0.35 | 8 | 85 | 0.55 | -12 | 1 | Modern distortion, soft clip control |

### Notes on the parameter mapping

- **CLICK_TYPE**: `0` = White Noise Burst, `1` = Pitch Saw Impuls, `2` = Exponential Transient.
- **CLIP_MODE**: `0` = Hard Clip (punchy/square), `1` = Soft Tanh (round/controlled).
- **SUB_MIX** near `1.0` = sub-heavy/clean low end; near `0.25` = click/distortion-dominant.
- **DRIVE** `2–4` = clean-ish; `6–8` = classic distorted; `9–11` = destroyed/terrorcore.
- **BASE_NOTE** follows A1=55, G1=49, F#1=46.25, F1=43.7 (lower = deeper/faster-feeling).
