# Editor setup — TODO

Checklist for wiring game modes and player state in Unreal Editor. Aligns with **Design.md** §3 (lobby vs run) and §8.2 (Hub).

C++ game modes live under `Source/EC_FireGun/GameMode/` (`AEC_FireGunGameModeBase`, `AEC_FireGunGameMode`, `AEC_HubGameMode`, `AShooterGameMode`, `AHorrorGameMode`).

## Game modes per map

- [ ] **Hub map(s)** — *World Settings → Game Mode → GameMode Override* → `AEC_HubGameMode` or a Blueprint child (e.g. `BP_HubGameMode`).
- [ ] **Run / gameplay map(s)** — same → a **subclass** of `AEC_FireGunGameMode` (e.g. `BP_FireGunGameMode`). `AEC_FireGunGameMode` is abstract in C++; use a BP child unless you change it to non-abstract.

## Project defaults

- [ ] **Edit → Project Settings → Maps & Modes** — set **Default GameMode** to the mode that matches your usual startup map (hub or test run).

## Blueprint game modes

- [ ] If a Blueprint game mode (e.g. `BP_FirstPersonGameMode`) does **not** inherit from `AEC_FireGunGameModeBase` or a derived C++ class, open that BP and set **Classes → Player State Class** to **`AEC_PlayerState`** so GAS on PlayerState works.

## Optional verification

- [ ] Listen server + 2 clients (PIE): confirm health/UI and respawn after assigning the correct `PlayerStateClass` and modes.

---

*Add more editor-only TODOs here as needed.*
