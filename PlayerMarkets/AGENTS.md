# DayZ Modding – AI Instructions

## Enforce Script Is NOT C/C++

DayZ uses **Enforce Script** – a C-like language with critical differences. Never assume C/C++ syntax works.

- **No ternary** – `a ? b : c` does not exist. Use `if/else`.
- **No templates, lambdas, or `auto`** – declare all types explicitly.
- **Casting** – use `Class.CastTo(out, source)` or `ClassName.Cast(source)`. Never use C-style casts.
- **Modifying vanilla** – use `modded class ClassName`, NOT `class X extends ClassName`.
- **Overrides** – always use the `override` keyword AND call `super.MethodName()`.
- **String concat** – use `+` operator; no `sprintf` or string interpolation.
- **No multiple inheritance** – single inheritance only.
- **.c files** are Enforce Script, NOT C. `.cpp` files (config.cpp, mod.cpp) are DayZ config syntax, NOT C++.

---

## #1 Rule: Use What Already Exists

**Before writing ANY custom logic, search for an existing vanilla function, class, or framework that already does it.** DayZ has extensive built-in systems – reinventing them creates bugs and mod incompatibilities.

### Always Search First
- Need JSON? Use `JsonFileLoader<T>.JsonLoadFile()` / `JsonFileLoader<T>.JsonSaveFile()` – never write a custom parser.
- Need file I/O? Use `FileHandle`, `OpenFile()`, `ReadFile()`, `CloseFile()` – the built-in file API.
- Need timers? Use `GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater()` – not custom tick counters.
- Need config values? Use `GetGame().ConfigGetText()`, `ConfigGetFloat()`, etc. – don't hard-code config data.
- Need to find objects? Use `GetGame().GetPlayers()`, `GetGame().GetObjectsAtPosition()`, etc.
- Need logging? Use `Print()`, `PrintToRPT()` – not custom file-based loggers.
- Need math? Check `Math` class – `Math.Clamp()`, `Math.RandomFloat()`, `Math.AbsFloat()`, etc.
- Need vectors? Use `vector` class methods – `vector.Distance()`, `vector.Normalize()`, etc.

**The pattern:** Before implementing any functionality, use the tools to search for existing implementations. If a vanilla function does what you need, use it.

---

## Modded Classes & Overrides – Best Practices

### Always Prefer `modded class` Over New Classes
When changing vanilla behavior, **always use `modded class`** to patch the existing class. Only create new classes (`extends`) for genuinely new types that don't exist in vanilla.

```enforce
// CORRECT – patches the existing PlayerBase
modded class PlayerBase
{
    override void Init()
    {
        super.Init();
        // your additions
    }
}

// WRONG – creates an entirely new, disconnected class that nothing uses
class MyPlayer extends PlayerBase { }
```

### Override & Super Call Rules
- **Always use the `override` keyword** when overriding a parent method. Without it the method silently shadows the parent and the base logic never runs.
- **Always call `super.MethodName()`** in overrides. Omitting it breaks the inheritance chain for other mods and vanilla logic.
- **Call `super` first** in most cases (Init, constructors, lifecycle methods) – let vanilla initialize before you modify.
- **Call `super` last** only when you need to preprocess data before the parent acts on it (rare – e.g., filtering input before passing up).
- **Match the signature exactly** – return type, parameter types, and parameter count must be identical to the parent method.

### Before Overriding a Method
1. **Look up the vanilla implementation** to understand what it does and what its signature is.
2. **Check if there's a better hook** – DayZ often provides dedicated event methods (e.g., `EEKilled`, `OnInventoryEnter`, `OnItemLocationChanged`) that are cleaner than overriding core methods.
3. **Minimize your override** – only add the logic you need; don't duplicate vanilla behavior.

---

## Core Patterns

**Casting:**
```enforce
PlayerBase player;
if (Class.CastTo(player, entity))
{
    // player is valid and safe to use
}
```

**Creating a new class (only when truly new):**
```enforce
class MyCustomSystem extends Managed
{
    void MyCustomSystem() {}  // constructor = same name as class
    void ~MyCustomSystem() {} // destructor
}
```

**Null safety – always check before use:**
```enforce
if (player && player.IsAlive())
{
    // safe
}
```

---

## Mod Structure

```
YourMod/
├── mod.cpp              # Mod registration (name, author, version)
├── config.cpp           # CfgPatches, CfgVehicles, CfgMods
└── scripts/
    ├── 3_game/          # Game systems, types, constants
    ├── 4_world/         # Items, players, entities, vehicles (most common)
    └── 5_mission/       # Mission logic, HUD, menus
```

### Script Load Order
`1_core` → `2_gamelib` → `3_game` → `4_world` → `5_mission`.
A script in `4_world` can reference `3_game` classes but NOT `5_mission`.
Put gameplay code in `4_world`. Put UI/HUD/menu logic in `5_mission`.

### config.cpp Essentials
```cpp
class CfgPatches
{
    class YourMod
    {
        units[] = {};
        weapons[] = {};
        requiredAddons[] = { "DZ_Data" }; // always include at minimum
    };
};

class CfgVehicles
{
    class Inventory_Base;   // forward declaration of parent
    class YourItem : Inventory_Base
    {
        scope = 2;                  // 0=private, 1=protected, 2=public (spawnable)
        displayName = "My Item";
        descriptionShort = "A custom item";
        model = "\\YourMod\\data\\model.p3d";
    };
};
```

---

## Networking

- **Server → Client**: RPCs via `GetGame().RPCSingleParam()` / `GetGame().RPC()`
- **Synced variables**: register with `RegisterNetSyncVariable*()`, then call `SetSynchDirty()` after changes
- **Authority**: all gameplay-critical logic on server; clients handle visuals and UI only
- **Never trust client data** – validate everything server-side

---

## Common Pitfalls

1. **Forgetting `super`** in overrides → silently breaks the inheritance chain and other mods
2. **Using `extends` instead of `modded class`** → creates a new class nobody references
3. **Reimplementing existing functionality** → use vanilla APIs (`JsonFileLoader`, `Math`, `CallQueue`, etc.)
4. **Wrong cast syntax** → null references and runtime crashes
5. **Client/server confusion** → accessing client-only data on server (or vice versa) causes nulls or desync
6. **Missing `SetSynchDirty()`** after changing synced variables → state never replicates
7. **Missing `requiredAddons`** in CfgPatches → scripts fail to load with no clear error
8. **Not checking for null** → entity references can become null at any time (player disconnect, object cleanup)
9. **Hard-coding values** that should come from config → use `ConfigGet*` methods to read from config.cpp

---

## Workflow (Full Toolkit)

You have access to the DaemonForge knowledge base and DayZ source search.
**Always research before writing code. Never guess – look it up.**

1. **Search the KB** – `search_dayz_kb` for tutorials, patterns, and established approaches. If someone has solved this before, use their approach.
2. **Search for existing vanilla functions** – `search_dayz_files` to check if DayZ already provides a function, class, or system that does what you need. Use it instead of writing custom code.
3. **Find the vanilla source** – `get_dayz_file_by_class` to see exact implementations and method signatures before overriding.
4. **Understand the class** – `lsp_get_full_class_info` and `lsp_get_all_class_methods` to see what methods and members are available. Look for hooks and events you can use instead of heavy overrides.
5. **Check quirks** – `list_enforce_quirks` and `get_enforce_context` for known pitfalls on the topic.
6. **Write code** – use what you found. Prefer `modded class` + `override` over new classes. Reuse vanilla APIs.
7. **Validate** – `lsp_get_diagnostics` to catch compile errors. Never present code without checking.

### MANDATORY: Layout File Validation

**You MUST run `validate_dayz_layout` on every `.layout` file you create or modify.** This is not optional.

- After generating or editing any `.layout` file, immediately pass its full content to `validate_dayz_layout`.
- Fix ALL reported errors before presenting the layout to the user.
- Do NOT skip validation even if you are confident the layout is correct — always validate.
- If validation returns warnings, review them and fix where appropriate.
- Never present a `.layout` file to the user without having validated it first.

### Key Tools

| Tool | When to Use |
|------|-------------|
| `search_dayz_kb` | Starting any task – find the right approach first |
| `search_dayz_files` | **Find existing functions** before writing custom logic; find vanilla code and method signatures |
| `get_dayz_file_by_class` | Get the full source file for a known class name |
| `get_dayz_file_part` / `get_dayz_file_parts` | Read large vanilla files in chunks |
| `search_in_dayz_file` | Find a specific method or variable within a file |
| `lsp_get_full_class_info` | Get class details: parent, methods, members |
| `lsp_get_all_class_methods` | List every method on a class (including inherited) – find hooks to use |
| `lsp_search_classes` | Find classes matching a pattern |
| `lsp_find_method_overrides` | See how a method is overridden across the codebase |
| `check_class_references` | Verify class names exist in vanilla |
| `list_enforce_quirks` | Check known Enforce Script quirks for a topic |
| `get_enforce_context` | Get syntax reference for a specific topic |
| `validate_dayz_layout` | **MANDATORY** – validate every .layout file you create or edit |

### P: Drive Tools (Low-Level Fallback Only)

`project_grep`, `project_read_file`, and `project_list_files` provide raw file access to the P: drive. **Do NOT use these as your primary research method.** They perform simple text matching with no intelligence.

- **To learn how to implement something** → use `search_dayz_kb` first
- **To find vanilla code by concept** → use `search_dayz_files` (semantic search)
- **To find a specific class** → use `get_dayz_file_by_class` or `lsp_search_classes`
- **To find method signatures** → use `lsp_get_all_class_methods`

Only fall back to `project_grep` when you need a literal text match the above tools cannot provide (e.g., a specific config property name or string literal). Only use `project_read_file` when you already know the exact file path from another tool's output.

## Context Sections

Use `get_enforce_context section:"name"` for detailed reference on a topic:

**Language:** `variables`, `functions`, `operators`, `control_flow`, `arrays`, `enums`
**OOP:** `oop`, `inheritance`, `overrides`, `modded_class_overview`, `modded_class_advanced`
**Patterns:** `patterns`, `base_classes`, `modules`, `lifecycle`
**Networking:** `networking_overview`, `networking_rpc`, `networking_netsync`
**UI:** `ui_widget_types`, `ui_widget_code`, `ui_quick_reference`
**Systems:** `actions`, `inventory`, `config`, `file_structure`, `persistence`

Use `list_context_sections` to see all available sections.

---

# Universal Framework - Documentation Context

Universal Framework provides backend service integration for DayZ modding including database, Discord, AI chat, messaging, and utility services accessed via the `UF()` singleton. Server has full access; players have limited read access to their own data. Knowledge on this framework is contained inside the `search_dayz_kb` tool 



