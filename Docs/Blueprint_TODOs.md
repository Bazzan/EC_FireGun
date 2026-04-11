# Blueprint TODOs

Tracks Blueprint assets and editor-side work required by C++ changes. Sorted by feature.

*Last updated: 2026-04-11 — weapon-grants-ability integration*

---

## GAS Shooting Abilities

- [ ] **Create `GE_Damage` GameplayEffect asset** — Instant effect, one modifier: `Health` attribute, `Additive` operation, magnitude from SetByCaller keyed by tag `Data.Damage`. Place under e.g. `Content/AbilitySystem/Effects/`.
- [ ] **Create `BP_Projectile` (child of `AECProjectile`)** — Add a mesh or Niagara trail for visuals. Assign to `ProjectileClass` on the shoot-projectile ability.
- [ ] **Create `BP_ShootHitscan` (child of `UGA_ShootHitscan`)** — Set `DamageEffect` to `GE_Damage`, configure `BaseDamage`, `MaxRange`, and `TraceChannel`. Optionally implement `On Hitscan Hit` / `On Hitscan Miss` events for tracer and impact VFX.
- [ ] **Create `BP_ShootProjectile` (child of `UGA_ShootProjectile`)** — Set `DamageEffect` to `GE_Damage`, configure `BaseDamage`, `MaxRange`, `ProjectileClass` to `BP_Projectile`, and `SpawnOffset`.
- [ ] **Assign `ShootAbilityClass` on weapon BPs** — Open each weapon Blueprint child of `AShooterWeapon` and set the new `Shoot Ability Class` property (under the Ability category) to the matching `BP_ShootHitscan` or `BP_ShootProjectile`. The weapon now automatically grants/revokes the ability when equipped/unequipped. Weapons without this property set continue using the legacy projectile path.

---

*Add new sections below as features are implemented in C++.*
