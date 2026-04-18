# Blueprint TODOs

Tracks Blueprint assets and editor-side work required by C++ changes. Sorted by feature.

*Last updated: 2026-04-18 — added Gunslinger Ultimate (Focus) and Grenade (Frag) scaffolding*

---

## GAS Shooting Abilities

- [x] **Create `GE_Damage` GameplayEffect asset** — Instant effect, one modifier: `Health` attribute, `Additive` operation, magnitude from SetByCaller keyed by tag `Data.Damage`. Place under e.g. `Content/AbilitySystem/Effects/`.
- [x] **Create `BP_Projectile` (child of `AECProjectile`)** — Add a mesh or Niagara trail for visuals. Assign to `ProjectileClass` on the shoot-projectile ability.
- [ ] **Create `BP_ShootHitscan` (child of `UShootHitscanAbility`)** — Set `DamageEffect` to `GE_Damage`, configure `BaseDamage`, `MaxRange`, and `TraceChannel`. Optionally implement `On Hitscan Hit` / `On Hitscan Miss` events for tracer and impact VFX.
- [x] **Create `BP_ShootProjectile` (child of `UShootProjectileAbility`)** — Set `DamageEffect` to `GE_Damage`, configure `BaseDamage`, `MaxRange`, `ProjectileClass` to `BP_Projectile`, and `SpawnOffset`.
- [ ] **Assign `ShootAbilityClass` on weapon BPs** — Open each weapon Blueprint child of `AShooterWeapon` and set the new `Shoot Ability Class` property (under the Ability category) to the matching `BP_ShootHitscan` or `BP_ShootProjectile`. The weapon now automatically grants/revokes the ability when equipped/unequipped. Weapons without this property set continue using the legacy projectile path.

---

## Class Abilities — Gunslinger

### GameplayEffects

- [ ] **Create `GE_Cooldown_Ultimate`** — Duration GE, 90s, **Granted Tags = `Cooldown.Ultimate`**. No modifiers. Place under `Content/AbilitySystem/Effects/Cooldowns/`.
- [ ] **Create `GE_Cooldown_Grenade`** — Duration GE, 12s, **Granted Tags = `Cooldown.Grenade`**. No modifiers.
- [ ] **Create `GE_FocusMode`** — Duration GE, 5s, **Granted Tags = `State.Focus`**, one modifier: `OutgoingDamageMultiplier`, **Multiply** by 1.5 (tunable). Place under `Content/AbilitySystem/Effects/Buffs/`.

### Ultimate (Gunslinger Focus)

- [ ] **Create `BP_UltimateAbility_GunslingerFocus`** (child of `UGunslingerFocusAbility`) — Set `BuffEffect = GE_FocusMode`, `CooldownGameplayEffectClass = GE_Cooldown_Ultimate`. Optional: implement `On Focus Start` / `On Focus End` events for HUD/SFX. Place under `Content/Gameplay/Abilities/PlayerAbilites/Ultimate/`.

### Grenade (Gunslinger Frag)

- [ ] **Create `BP_FragGrenade`** (child of `AECGrenade`) — Add static mesh, set `ExplosionRadius = 350`, `MaxLifetime = 5`, `DestroyDelay = 2`, `bUseDamageFalloff = true`, `MinDamageScale = 0.25`. Implement `On Explode` event for VFX/SFX/decal. Place under `Content/Gameplay/Abilities/PlayerAbilites/Grenade/Frag/`.
- [ ] **Create `BP_GrenadeAbility_GunslingerFrag`** (child of `UECGrenadeAbility`) — Set `GrenadeClass = BP_FragGrenade`, `DamageEffect = GE_Damage`, `BaseDamage = 80`, `ThrowSpeed = 1800`, `CooldownGameplayEffectClass = GE_Cooldown_Grenade`. Place under `Content/Gameplay/Abilities/PlayerAbilites/Grenade/`.

### Input

- [ ] **Create `IA_Ultimate` Input Action** (Digital, bool) and map **`X` → `IA_Ultimate`** in the existing player IMC.
- [ ] **Create `IA_Grenade` Input Action** (Digital, bool) and map **`G` → `IA_Grenade`** in the existing player IMC.

### Wiring on the player pawn BP

- [ ] On `BP_PlayerCharacter` (or a class-specific child like `BP_PlayerCharacter_Gunslinger`):
  - Set **Ultimate Action** = `IA_Ultimate`, **Grenade Action** = `IA_Grenade`.
  - Set **Ultimate Ability Class** = `BP_UltimateAbility_GunslingerFocus`.
  - Set **Grenade Ability Class** = `BP_GrenadeAbility_GunslingerFrag`.

---

*Add new sections below as features are implemented in C++.*
