# Editor setup — TODO

Checklist for wiring game modes and player state in Unreal Editor. Aligns with **Design.md** §3 (lobby vs run) and §8.2 (Hub).

C++ game modes live under `Source/EC_FireGun/GameMode/` (`AEC_FireGunGameModeBase`, `AEC_FireGunGameMode`, `AEC_HubGameMode`, `AShooterGameMode`, `AHorrorGameMode`).

## Game modes per map

- [x] **Hub map (`LVL_Hub`)** — Created with `GM_Hub` (BP child of `AEC_HubGameMode`). Verify *World Settings → GameMode Override* is set to `GM_Hub` in the editor.
- [ ] **Run / gameplay map(s)** — Set *World Settings → GameMode Override* to `GM_FireGun` (BP child of `AEC_FireGunGameMode`) on `LVL_StartZone01` and any future run maps.

## Project defaults

- [x] **Edit → Project Settings → Maps & Modes** — Default GameMode set to `GM_Hub`, default map and editor startup map set to `LVL_Hub`.

## Blueprint game modes

- [ ] If a Blueprint game mode (e.g. `BP_FirstPersonGameMode`) does **not** inherit from `AEC_FireGunGameModeBase` or a derived C++ class, open that BP and set **Classes → Player State Class** to **`AEC_PlayerState`** so GAS on PlayerState works.

## Optional verification

- [ ] Listen server + 2 clients (PIE): confirm health/UI and respawn after assigning the correct `PlayerStateClass` and modes.

---

*Add more editor-only TODOs here as needed.*
