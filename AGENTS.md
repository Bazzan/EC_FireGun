# EC_FireGun Project Rules

## Status Tracking

- At the **start** of every task session, read `Docs/STATUS.md` to understand what is in progress, what was recently completed, and any known issues.
- At the **end** of any task that changes code, resolves a bug, or introduces new issues, update `Docs/STATUS.md` before finishing.
- When updating, follow these rules:
  - Update the *Last updated* line with the current date.
  - Move finished items from **Current Focus** to **Recently Completed**.
  - Add whatever you are actively working on to **Current Focus**.
  - Record any new bugs or regressions in **Known Issues and Bugs**.
  - Record tech-debt observations or fragile areas in **Potential Risks**.
  - Use **Notes** for anything else relevant to future sessions.
- Keep every entry concise (one line per item).
- **Timestamps:** Prefix each entry with `[YYYY-MM-DD]` when it is added or meaningfully changed. This applies to all sections except **Notes**.
- Keep at most ~10 entries in **Recently Completed**; trim the oldest when the list grows beyond that.
- Keep `Docs/STATUS.md` under ~150 lines total. If it grows beyond that, trim the oldest entries from **Recently Completed** and **Notes** first, then condense older entries in other sections.
- Never remove entries from **Known Issues and Bugs** unless the issue has been verified as fixed; when fixed, move the entry to **Recently Completed** with a brief note.

### Blueprint TODOs tracking

- When a code change requires new or updated Blueprint assets (GameplayEffects, ability BPs, widget BPs, data assets, etc.) or editor-side configuration, add a TODO to `Docs/Blueprint_TODOs.md`.
- Group entries under a feature heading (e.g. `## GAS Shooting Abilities`).
- Use `- [ ]` checkboxes so items can be ticked off in the editor.
- Update the *Last updated* date when adding or completing items.
- When a Blueprint TODO is completed (by the user in-editor), mark it `- [x]` and leave it for reference; remove only when the section is clearly stale.

## Design Document Enforcement

- Before any code/design change, read `Docs/Design.md`.
- Treat `Docs/Design.md` as the primary source of truth for gameplay, class identity, progression, and co-op systems.
- In implementation responses, briefly state which design section(s) were followed.
- If a request conflicts with the design doc, stop and ask for explicit approval before proceeding.
- If required details are missing or ambiguous, ask clarifying questions before making changes.
- Keep new systems consistent with the documented core loop, class roles, passive/ultimate philosophy, and roguelike progression.
- Do not introduce new gameplay pillars or class mechanics that are not aligned with the design doc unless explicitly approved.

## Unreal C++ Standards

- Prefer Unreal types/macros: `UCLASS/USTRUCT/UENUM`, `UPROPERTY`, `UFUNCTION`, `TArray`, `TMap`, `TSet`, `TObjectPtr`, `TSoftObjectPtr`.
- Use `UE_LOG` (not `printf/std::cout`) and keep logs meaningful (category + severity).
- **Log categories:** Introduce a dedicated log category per feature (coherent gameplay or subsystem area), not a single shared category for everything. Use `DECLARE_LOG_CATEGORY_EXTERN` in a header and `DEFINE_LOG_CATEGORY` in one `.cpp` for that feature/module; name categories clearly (e.g. `LogWeapon`, `LogRoguelikeRun`). Reuse an existing category only when the log truly belongs to that same feature.
- Never use raw `new`/`delete`. If you believe it's necessary, stop and ask before adding any code that uses them; propose an Unreal-friendly alternative first (e.g. `NewObject`, components, `TUniquePtr` where appropriate).
- Agent changes are limited to the `Source/` folder only (add/change/remove code only there). If a change is needed outside `Source/`, stop and ask first.
- Redirects in `Config/DefaultEngine.ini` may be added only after explicit user approval in the current conversation.
- All gameplay code changes must comply with `Docs/Design.md`; ask before deviating.
- Networking model is listen-server: one player hosts and acts as the server during gameplay, and that host is authoritative for all server-side game logic.
- All gameplay features must be implemented for listen-server authority first (host authoritative + client prediction/reconciliation only where needed), and must not assume dedicated-server-only behavior.
- Null safety: check pointers before use; prefer `IsValid()` for UObject references when relevant.
- Follow Unreal naming conventions (PascalCase types/functions, b-prefixed bool members, etc.).
- Keep headers lean: forward-declare where possible; include in `.cpp` when you can.
- Don't block the game thread with heavy work; if needed, use async/tasks appropriately.
- When editing gameplay code, consider replication (`Server/Client/NetMulticast`) and authority checks.
- **Gameplay Ability System (GAS):** Implement character and world gameplay through GAS where it applies: use `UAbilitySystemComponent`, `UGameplayAbility`, `UGameplayEffect`, and Gameplay Tags for authority, costs, cooldowns, and replication. Examples include shooting/weapons, class passives and ultimates, pickups, interactions, and other player-triggered or AI-triggered actions. Prefer granting abilities and applying effects through GAS rather than ad-hoc timers or direct RPC-only logic for new features. When extending legacy code that is not yet on GAS, migrate toward GAS in the same change when reasonable, or ask before leaving new ability-shaped behavior outside GAS.
- **GAS damage & effects rule:** All damage and status effects originating from a `UGameplayAbility` **must** be applied through `UGameplayEffect` (via `ApplyGameplayEffectSpecToTarget` or equivalent GAS API). Never use `UGameplayStatics::ApplyDamage`, `UGameplayStatics::ApplyPointDamage`, or direct `TakeDamage` calls from ability code. If a target lacks an ASC, log a warning and skip the effect rather than falling back to the non-GAS damage pipeline.
- **Asset Tags API:** Use `SetAssetTags` in ability constructors (UE 5.5+). Do NOT use deprecated `AbilityTags.AddTag` or mutable `AbilityTags` member.
- **Simplicity:** Do not over-engineer solutions. Prefer straightforward, readable code that matches Unreal and project patterns. Add abstraction, indirection, or generic frameworks only when there is a clear, immediate need; otherwise keep the smallest change that works.
- Keep changes minimal and consistent with surrounding code style.