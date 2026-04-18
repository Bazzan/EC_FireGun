# EC_FireGun Design Document

## 1. Vision

`EC_FireGun` is a 3-player cooperative first-person shooter roguelike.  
Each player selects a class with:
- one Ultimate ability (high-impact active),
- one Passive ability (always-on playstyle driver).

The design goal is replayable runs with strong class identity, team synergy, and readable combat pacing.

## 2. Pillars

1. Team-first co-op combat for exactly 3 players.
2. Distinct class playstyles through Passive + Ultimate.
3. Roguelike run variance with controlled randomness.
4. Fast, readable first-person gunplay and movement.

## 3. Core Loop

1. Lobby and class select.
2. Start run (seeded generation).
3. Fight through encounter rooms.
4. Choose rewards/upgrades.
5. Repeat room + reward cadence.
6. Reach boss or fail run.
7. Apply meta progression and restart.

## 4. Session Structure

- Party size: 3 players.
- Run length target: 25-40 minutes.
- Room count target: 8-12 + boss.
- Room types:
  - Combat
  - Objective (hold, escort, hack)
  - Event (risk/reward)
  - Shop/Rest
  - Boss

## 5. Class Framework

## 5.1 Class Count

- Launch target: 3 classes.
- Expansion target: 5 classes.

## 5.2 Ability Rules

- Passive:
  - Always active.
  - Must alter decisions in most encounters.
  - Avoid flat stat-only design where possible.
- Ultimate:
  - High-impact team utility, damage, or control.
  - Clear activation telegraph and UI feedback.
  - Charge/cooldown tuned for one meaningful use every 2-3 encounters.
- Grenade:
  - Each class has its own grenade variant tied to class identity.
  - Short cooldown (multiple uses per encounter), readable telegraph, server-authoritative detonation.

## 5.3 Starter Class Concepts

### Vanguard
- Role: Frontline.
- Passive: gains temporary protection when committing to close-range engagements.
- Ultimate: directional barrier that enables team push.

### Field Tech
- Role: Support.
- Passive: healing or assist actions grant short defensive bonuses.
- Ultimate: emergency team stabilization pulse.

### Gunslinger
- Role: Precision DPS.
- Passive: sustained weakpoint performance rewards precision.
- Ultimate: short focus mode for burst precision output.

## 6. Combat and Movement

- Perspective: first person.
- Movement: responsive and readable with clear acceleration/deceleration.
- Weapon handling: fire, reload, switch, recoil, and clear hit feedback.
- Enemy pressure should challenge movement and teamwork, not only raw aim.

## 7. Co-op Systems

- Downed and revive flow with risk during revive windows.
- Shared objective progress.
- Team-aware rewards and assist credit.
- Avoid hard role lock; any class combination must be viable.

## 8. Roguelike Progression

## 8.1 In-run Progression

- Enter a new area/room, clear all required enemies/objectives, then claim exactly one clear reward.
- Clear rewards are:
  - a Boon (run upgrade), or
  - a new Gun.
- No room clear means no reward claim.
- Boons and Guns use weighted reward pools with safeguards against bad streaks.
- Boons can come from class-specific or universal pools.

## 8.2 Meta Progression

- Players earn Meta Currency from completed runs, milestones, and optional challenges.
- Meta Currency is spent in the Hub between runs.
- Hub spending is limited to Talent-style meta upgrades only (no armor or item purchases).
- Meta talents should improve strategy options and consistency, not replace in-run decision making.
- Unlock new classes and upgrade options over time.
- Prioritize sidegrades and build variety over raw permanent power creep.
- Cosmetics and quality-of-life unlocks are preferred for long-term retention.

## 8.3 Meta Talent Structure

- Talent categories:
  - Economy (run reward quality and reroll utility),
  - Class Mastery (class-specific progression paths),
  - Team Utility (co-op focused passive bonuses),
  - Run Stability (small safety nets for early run consistency).
- Talent rules:
  - Keep power gains modest and capped.
  - Prefer branching choices over linear mandatory upgrades.
  - Avoid talents that invalidate encounter difficulty or class identity.
- Exclusions:
  - No direct Hub purchases of armor.
  - No direct Hub purchases of in-run items or combat consumables.

## 9. Encounter Director

- Controls pacing and difficulty curve.
- Inputs:
  - team status (HP, downs, resources),
  - run stage,
  - recent encounter intensity.
- Output:
  - enemy budget,
  - elite chance,
  - event room chance.
- Must include anti-frustration rules (pity limits, spike smoothing).

## 10. Balancing Targets (Initial)

- Average room clear time: 2-4 minutes.
- Ultimate cadence: one meaningful use every 2-3 rooms.
- Team wipe causes should be readable (clear telegraphs, understandable failures).
- Time-to-kill for standard enemies should preserve weapon fantasy and mobility.

## 11. Content Scope Milestones

## 11.1 M1 Prototype

- 1 biome/test map.
- 3 classes (passive + ultimate each).
- 1 boss.
- Basic run flow, rewards, and revive system.

## 11.2 M2 Vertical Slice

- 2 biomes.
- Expanded enemy roster.
- Encounter director and reward weighting pass.
- UI polish for class/ultimate/team status.

## 11.3 M3 Content Expansion

- 5 total classes.
- Larger event pool and upgrade pool.
- Meta progression pass.

## 12. Gameplay Ability System (GAS)

- Character and interactable gameplay is implemented with Unreal’s **Gameplay Ability System** where it fits: abilities, effects, tags, costs, and cooldowns live in GAS—not one-off bespoke systems for each feature.
- **Scope:** shooting and weapons, class passives and ultimates, pickups, world interactions, and other actions triggered by players or AI that behave like abilities (activation, duration, stacking, replication).
- **Patterns:** `UAbilitySystemComponent` on pawns; `UGameplayAbility` for activatable behavior; `UGameplayEffect` for stat changes and buffs/debuffs; **Gameplay Tags** for states, prerequisites, and cancellation.
- **Damage & effects rule:** All damage dealing and status effects that originate from a `UGameplayAbility` **must** go through `UGameplayEffect`. Do not use `UGameplayStatics::ApplyDamage` or direct `TakeDamage` calls from ability code. This ensures consistent interaction with attribute clamping, replication, and tag-based immunity.
- **Networking:** align with the listen-server model: server-authoritative activation and effect application; use GAS replication features instead of ad-hoc RPC chains for new ability-shaped features.
- **Legacy:** existing non-GAS code may remain until refactored; new features and meaningful extensions should move toward GAS. Prefer the smallest GAS setup that satisfies the design (avoid unnecessary abstraction layers).

## 13. Implementation Boundaries

- Gameplay code lives under `Source/`.
- New gameplay changes must align with this document.
- If a requested change conflicts with this document, stop and confirm before implementation.
- If this document is unclear or missing details, ask clarifying questions first.

## 14. Open Design Questions

- Final class roster and names for launch.
- Revive rules (duration, invulnerability, penalties).
- Meta progression limits to prevent runaway power.
- Boss count and biome order for first release.

