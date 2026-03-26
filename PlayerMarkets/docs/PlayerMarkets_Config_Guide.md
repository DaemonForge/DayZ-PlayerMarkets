# PlayerMarkets — Config Reading Guide for AI Knowledge Base Generation

This document explains how to interpret the PlayerMarkets server configuration file (`Config.json`) so that an AI can accurately answer player questions based on a server's specific settings.

---

## Table of Contents

1. [Config File Location & Format](#config-file-location--format)
2. [Config Field Reference](#config-field-reference)
3. [How to Read Each Field](#how-to-read-each-field)
4. [Example Config with Annotations](#example-config-with-annotations)
5. [Currency Configuration Deep Dive](#currency-configuration-deep-dive)
6. [Calculating Player-Facing Values](#calculating-player-facing-values)
7. [Mapping Config Values to Player Behavior](#mapping-config-values-to-player-behavior)
8. [Edge Cases & Defaults](#edge-cases--defaults)

---

## Config File Location & Format

- **Path:** `[ServerProfile]\PlayerMarkets\Config.json`
- **Format:** Standard JSON
- **Created automatically** on first server start with default values
- **Hot-reloadable:** No — requires server restart after changes
- **Distributed to clients:** Yes — clients request the config on mission start

---

## Config Field Reference

Here is every field in the config, its type, default value, and what it controls:

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `ConfigVersion` | string | `"0"` | Internal config version tracker. Not player-facing. |
| `Currencies` | array of objects | *(see below)* | Defines all accepted currencies with their coin types and values |
| `BlackList` | array of strings | `["PM_GoldCoin", "PM_SilverCoin", "PM_CopperCoin"]` | Item class names that cannot be placed in stall cargo (cannot be sold) |
| `GUIOption` | int | `0` | Which GUI theme to use. `0` = Classic (parchment/rustic), `1` = Modern |
| `MarketTableSaleSlots` | int | `8` | Maximum items a Market Table can list for sale |
| `MarketStallLv1SaleSlots` | int | `16` | Maximum items a Market Stall can list for sale |
| `MarketStallLv2SaleSlots` | int | `32` | Maximum items a Large Market Stall can list for sale |
| `LoggingChannel` | string | `""` | Discord channel ID for sale logging via UFramework. Empty = disabled |
| `ManagedTypesFile` | bool | `true` | When enabled, automatically copies `PlayerMarkets_types.xml` into the mission's `ModTypes` folder and registers it in `cfgeconomycore.xml` on server start. This ensures the DayZ economy system recognizes PlayerMarkets items (coins, kits, signs) without manual file setup |
| `SaleTaxAmount` | float | `0.05` | Percentage tax added to the buyer's cost (0.05 = 5%) |
| `FreeTaxDays` | int | `3` | Number of days earnings sit in a stall tax-free before daily tax kicks in |
| `DailyTaxAmount` | float | `0.03` | Percentage of stored earnings taxed per day after the grace period (0.03 = 3%) |
| `DeListFee` | float | `0.05` | Percentage of an item's highest-ever price charged when delisting (0.05 = 5%) |
| `DeListFeeMin` | float | `100` | Minimum price threshold for delist fees. Items whose max price is below this have no delist fee |
| `DeListGraceHours` | float | `4` | Hours after listing during which delisting is free (grace period). 0 = no grace |
| `StallAbandonmentHours` | float | `4.0` | Hours an owner must be away (>100m) before the stall dumps all items to the ground. 0 = disabled |
| `MarketAreas` | array of objects | *(see below)* | Defines named market zones with optional tax modifiers and concierge NPCs |

---

## How to Read Each Field

### `Currencies` — The Currency System

This is the most complex field. It's an **array** where each entry defines one currency that stalls can accept.

**Structure of each currency entry:**

```json
{
    "CurrencyName": "Coins",
    "CurrencyType": "coin",
    "Values": [
        { "ClassName": "PM_GoldCoin", "Value": 1000 },
        { "ClassName": "PM_SilverCoin", "Value": 100 },
        { "ClassName": "PM_CopperCoin", "Value": 1 }
    ]
}
```

**How to interpret:**

| Subfield | Type | Meaning |
|----------|------|---------|
| `CurrencyName` | string | Display name for this currency (shown in seller menu dropdown) |
| `CurrencyType` | string | Internal type identifier. Usually `"coin"` |
| `Values` | array | List of item-to-value mappings for this currency |
| `Values[].ClassName` | string | The DayZ item class name (what spawns in-game) |
| `Values[].Value` | int | How much one unit of this item is worth in base currency units |

**Reading example:** 
> If `PM_GoldCoin` has `Value: 1000` and `PM_CopperCoin` has `Value: 1`, then 1 Gold Coin = 1000 Copper Coins.

**Multiple currencies:**
If the array has multiple entries, stall owners can choose which currency their stall accepts. Each stall only accepts ONE currency at a time, but different stalls can use different currencies.

### `BlackList` — Unsellable Items

An array of DayZ item class names. Any item whose class name appears in this list **cannot be placed into a stall's cargo**, effectively preventing it from being listed for sale.

**Reading example:**
```json
"BlackList": ["PM_GoldCoin", "PM_SilverCoin", "PM_CopperCoin", "LandMine", "ClaymoreExplosive"]
```
> This means players cannot sell coins, land mines, or claymore explosives.

### `GUIOption` — Visual Theme

| Value | Theme | Description |
|-------|-------|-------------|
| `0` | Classic | Rustic parchment/board aesthetic. Uses `BoardBG`, `parchmentH`, `parchmentV` textures |
| `1` | Modern | Cleaner, more polished UI design |

This only affects visuals, not functionality.

### Sale Slot Limits

These three fields control how many items each stand type can list:

| Field | Applies To | Player-Facing Name |
|-------|-----------|-------------------|
| `MarketTableSaleSlots` | `PM_TraderTable` | Market Table |
| `MarketStallLv1SaleSlots` | `PM_MarketStand` | Market Stall |
| `MarketStallLv2SaleSlots` | `PM_MarketStandV2` | Large Market Stall |

**Reading example:**
> If `MarketStallLv1SaleSlots` is `20`, then a Market Stall owner can list up to 20 items for sale simultaneously.

### `SaleTaxAmount` — Buyer Tax

This is a **decimal percentage** applied on top of the listed price, paid by the buyer.

| Config Value | Percentage | Player Impact |
|-------------|-----------|---------------|
| `0.0` | 0% | Buyer pays exactly the listed price |
| `0.05` | 5% | Buyer pays 105% of listed price |
| `0.10` | 10% | Buyer pays 110% of listed price |
| `0.25` | 25% | Buyer pays 125% of listed price |

**Formula:** `Total buyer cost = Listed Price + (Listed Price × SaleTaxAmount)`

**Important:** The seller receives the full listed price. The tax is "eaten" — it doesn't go to anyone. This is an economic money sink.

### `FreeTaxDays` — Grace Period

An **integer** number of days. After a sale is made, the earnings from that sale sit in the stall balance. For this many days, no daily tax is applied to those specific earnings.

**Reading example:**
> `FreeTaxDays: 5` means a seller has 5 real-time days to withdraw earnings from a sale before daily tax starts eroding them.

### `DailyTaxAmount` — Earnings Decay

A **decimal percentage** applied daily to stall earnings that have exceeded the `FreeTaxDays` grace period.

| Config Value | Percentage | Effect |
|-------------|-----------|--------|
| `0.0` | 0% | No daily tax (earnings never decay) |
| `0.03` | 3% | 3% of old earnings removed per day |
| `0.10` | 10% | 10% of old earnings removed per day |
| `1.0` | 100% | All old earnings removed after grace period |

**Important:** Tax is applied per-day-bucket. Each day's sales are tracked separately. Only earnings older than `FreeTaxDays` are taxed.

**Formula per old bucket:** `new_amount = old_amount - ceil(old_amount × DailyTaxAmount)`

### `DeListFee` and `DeListFeeMin` — Delist Cost

These two fields work together to determine the cost of removing an item from sale:

1. Look up the item's **highest-ever listed price** (tracked per-item, includes all price edits)
2. If the item is still within the `DeListGraceHours` window → **Fee = 0** (free delist)
3. If `highest_price < DeListFeeMin` → **Fee = 0** (no charge)
4. If `highest_price >= DeListFeeMin` → **Fee = highest_price × DeListFee**

### `DeListGraceHours` — Free Delist Window

A **float** representing how many hours after listing an item can be delisted for free. This gives sellers a window to fix mistakes (wrong price, wrong item) without penalty.

- Default: `4` (4 hours)
- Set to `0` to disable the grace period entirely

**Reading example:**
```json
"DeListFee": 0.05,
"DeListFeeMin": 100,
"DeListGraceHours": 4
```
> - Item listed 2 hours ago then delisted → No fee (within 4-hour grace period)
> - Item listed 5 hours ago at price 200 then delisted → Fee = 200 × 0.05 = 10
> - Item listed 5 hours ago at price 50 then delisted → No fee (50 < 100 minimum)
> - Item listed 5 hours ago at 500, edited to 100, then delisted → Fee = 500 × 0.05 = 25 (uses highest-ever price of 500)

### `StallAbandonmentHours` — AFK / Absence Protection

A **float** representing how many real-time hours a stall owner can be absent (more than 100 meters away) before the server considers the stall abandoned.

**How it works:**
1. A 60-second repeating timer checks whether the owner is within 100 meters of the stall
2. Each time the owner is detected nearby, the "last seen" timestamp resets
3. If the owner has not been within 100 meters for `StallAbandonmentHours` hours, **all items are dumped to the ground** at the stall's position
4. This includes both listed sale items and cargo storage items
5. The stall's listing data is cleared

| Config Value | Behaviour |
|-------------|----------|
| `0` (or negative) | Abandonment system is **disabled** — items stay forever |
| `4` | Owner must visit within 4 hours or items are dumped |
| `24` | Owner has a full day before items are dumped |
| `168` | Owner has a week (7 × 24) before items are dumped |

**Important notes:**
- The timer is **in-memory only** — it resets on every server restart. A restart effectively gives the owner a fresh window.
- Only online presence matters. If the owner is offline, the timer keeps running until the server restarts.
- Server log message on trigger: `"[PlayerMarkets] Stall '<name>' abandoned — all items dumped to ground."`

---

### `MarketAreas` — Named Market Zones

An **array** where each entry defines a geographical market area with optional tax adjustments and a concierge NPC.

**Structure of each entry:**

```json
{
    "Name": "Novy Market",
    "CenterPosition": "6961.96 297.99 7689.1",
    "Radius": 65,
    "TaxModifier": 0.0,
    "ConciergePosition": "6977.8 296.88 7703.3",
    "NPCClassName": "PM_ConciergeM_Mirek",
    "NPCGear": ["DressShirt_Beige", "SlacksPants_DarkGrey", "DressShoes_Black", "FlatCap_Grey"],
    "ConciergeDirection": 225.0
}
```

**Field reference:**

| Subfield | Type | Default | Meaning |
|----------|------|---------|---------|
| `Name` | string | `"Market District"` | Display name shown to players (e.g., in placement notifications) |
| `CenterPosition` | vector (string) | `"0 0 0"` | World coordinates of the area's centre |
| `Radius` | float | `50.0` | Radius in meters from the centre |
| `TaxModifier` | float | `0.0` | Adjustment to the base `SaleTaxAmount`. Negative = discount, positive = surcharge |
| `ConciergePosition` | vector (string) | `"0 0 0"` | Where to spawn the concierge NPC. `"0 0 0"` = no NPC spawned |
| `NPCClassName` | string | `"PM_ConciergeM_Mirek"` | NPC model class. Options: `PM_ConciergeM_Mirek`, `PM_ConciergeM_Denis`, `PM_ConciergeM_Boris`, `PM_ConciergeF_Eva`, `PM_ConciergeF_Linda`, `PM_ConciergeF_Frida` |
| `NPCGear` | array of strings | *(see above)* | Clothing class names to equip on the NPC |
| `ConciergeDirection` | float | `225.0` | Compass heading the NPC faces (0 = North, 90 = East, etc.) |

**Effects of being inside a Market Area:**

1. **Tax adjustment:** Stalls within the radius use an effective tax of `SaleTaxAmount + TaxModifier`, clamped to [0.0, 1.0]. For example, with a base tax of 0.05 and `TaxModifier: -0.02`, stalls in that area charge only 3% tax.
2. **Placement notification:** When a player deploys a market kit inside an area, they see a notification with the area name and the effective tax rate.
3. **Concierge NPC:** If `ConciergePosition` is not `"0 0 0"`, an invincible NPC is auto-spawned on mission start. Players can interact with it to browse and buy from **all stalls in the area** without walking to each one.

**Multiple areas:** You can define as many areas as you want. Each stall uses the first area it falls within. Stalls outside all areas use the base `SaleTaxAmount` with no modifier.

**Tax modifier examples:**

| `SaleTaxAmount` | `TaxModifier` | Effective Tax | Player Impact |
|----------------|--------------|---------------|---------------|
| 0.05 | 0.0 | 5% | Standard tax |
| 0.05 | -0.05 | 0% | Tax-free zone |
| 0.05 | -0.02 | 3% | Discount zone (incentivise placing stalls here) |
| 0.05 | +0.05 | 10% | Premium location surcharge |

---

### `LoggingChannel` — Discord Integration

A string containing a Discord channel ID. When set (not empty), every sale is logged to that channel via UFramework's Discord integration.

**Log message format:** `"[StallName]([OwnerID]) sold [ItemName] for $[Price] to [BuyerName] ([BuyerID])"`

If empty string `""`, logging is disabled.

---

## Example Config with Annotations

```json
{
    "ConfigVersion": "0",
    
    // --- CURRENCY DEFINITION ---
    // This server uses the default coin system
    // Gold=1000, Silver=100, Copper=1
    "Currencies": [
        {
            "CurrencyName": "Coins",        // Name shown in seller menu
            "CurrencyType": "coin",          // Internal type
            "Values": [
                {
                    "ClassName": "PM_GoldCoin",   // Rarest coin
                    "Value": 1000                  // Worth 1000 base units
                },
                {
                    "ClassName": "PM_SilverCoin",
                    "Value": 100                   // Worth 100 base units
                },
                {
                    "ClassName": "PM_CopperCoin",
                    "Value": 1                     // This IS the base unit
                }
            ]
        }
    ],
    
    // --- ITEM RESTRICTIONS ---
    // Players cannot sell these items on any stall
    "BlackList": [
        "PM_GoldCoin",     // Can't sell currency
        "PM_SilverCoin",
        "PM_CopperCoin"
    ],
    
    // --- UI SETTINGS ---
    "GUIOption": 0,        // 0=Classic (parchment), 1=Modern
    
    // --- SALE SLOT LIMITS ---
    // How many items each stand type can list
    "MarketTableSaleSlots": 8,       // Small stand: 8 listings max
    "MarketStallLv1SaleSlots": 16,   // Medium stand: 16 listings max
    "MarketStallLv2SaleSlots": 32,   // Large stand: 32 listings max
    
    // --- LOGGING ---
    "LoggingChannel": "",    // Empty = no Discord logging
    
    // --- ECONOMY FILE ---
    "ManagedTypesFile": true,  // Whether to create economy type file
    
    // --- TAX & FEE SETTINGS ---
    "SaleTaxAmount": 0.05,    // 5% tax on purchases (paid by buyer)
    "FreeTaxDays": 3,         // 3 days before daily tax starts
    "DailyTaxAmount": 0.03,   // 3% daily tax on old earnings
    "DeListFee": 0.05,        // 5% fee to remove item from sale
    "DeListFeeMin": 100,       // No delist fee if max price was under 100
    "DeListGraceHours": 4,     // 4 hours free delist window after listing
    
    // --- STALL PROTECTION ---
    "StallAbandonmentHours": 4.0,   // Dump items if owner absent 4+ hours
    
    // --- MARKET AREAS ---
    "MarketAreas": [
        {
            "Name": "Novy Market",
            "CenterPosition": "6961.96 297.99 7689.1",
            "Radius": 65,
            "TaxModifier": 0.0,
            "ConciergePosition": "6977.8 296.88 7703.3",
            "NPCClassName": "PM_ConciergeM_Mirek",
            "NPCGear": ["DressShirt_Beige", "SlacksPants_DarkGrey", "DressShoes_Black", "FlatCap_Grey"],
            "ConciergeDirection": 225.0
        }
    ]
}
```

---

## Currency Configuration Deep Dive

### Single Currency (Default)

```json
"Currencies": [
    {
        "CurrencyName": "Coins",
        "CurrencyType": "coin",
        "Values": [
            { "ClassName": "PM_GoldCoin", "Value": 1000 },
            { "ClassName": "PM_SilverCoin", "Value": 100 },
            { "ClassName": "PM_CopperCoin", "Value": 1 }
        ]
    }
]
```

**Player-facing meaning:** 
> All stalls use "Coins." Prices are in Copper units. A price of 150 means 1 Silver + 50 Copper (or 150 Copper). The system auto-calculates denominations.

### Multiple Currencies (Custom Server)

```json
"Currencies": [
    {
        "CurrencyName": "Coins",
        "CurrencyType": "coin",
        "Values": [
            { "ClassName": "PM_GoldCoin", "Value": 1000 },
            { "ClassName": "PM_SilverCoin", "Value": 100 },
            { "ClassName": "PM_CopperCoin", "Value": 1 }
        ]
    },
    {
        "CurrencyName": "Rubles",
        "CurrencyType": "ruble",
        "Values": [
            { "ClassName": "RP_Ruble100", "Value": 100 },
            { "ClassName": "RP_Ruble10", "Value": 10 },
            { "ClassName": "RP_Ruble1", "Value": 1 }
        ]
    }
]
```

**Player-facing meaning:**
> Stall owners can choose between "Coins" and "Rubles." A stall set to "Rubles" only accepts ruble items. Each currency is independent — you can't pay Rubles at a Coins stall.

### Custom Currency with Non-Coin Items

Servers can use any item as currency:

```json
{
    "CurrencyName": "Barter",
    "CurrencyType": "barter",
    "Values": [
        { "ClassName": "Canteen", "Value": 50 },
        { "ClassName": "WoodenPlank", "Value": 10 },
        { "ClassName": "Nail", "Value": 1 }
    ]
}
```

**Player-facing meaning:**
> This server uses planks, nails, and canteens as money. A canteen is worth 50 nails, a plank is worth 10 nails.

---

## Calculating Player-Facing Values

When generating player-facing KB articles from a config, use these formulas:

### "How much does it cost to buy an item listed at X?"

```
Total Cost = X + (X × SaleTaxAmount)
Total Cost = X × (1 + SaleTaxAmount)
```

Example with defaults: `100 × 1.05 = 105`

### "How much do I get if I sell an item for X?"

The seller receives exactly X. The buyer pays X + tax. The tax is a money sink.

### "What's the delist fee for an item I listed at X?"

```
If item was listed less than DeListGraceHours ago:
    Fee = 0
Else if X < DeListFeeMin:
    Fee = 0
Else:
    Fee = X × DeListFee
```

Note: X here is the **highest price the item was ever listed at**, not the current price.

### "How long before my earnings start getting taxed?"

```
FreeTaxDays days after each individual sale
```

### "How fast do my earnings decay?"

```
Per day after grace period:
    Lost = ceil(remaining_for_that_day × DailyTaxAmount)
    Remaining = remaining_for_that_day - Lost
```

After N days past grace:
```
Remaining ≈ original × (1 - DailyTaxAmount)^N
```

### "How long before my stall is abandoned?"

```
If StallAbandonmentHours <= 0:
    Never — abandonment is disabled
Else:
    Owner must be within 100 meters at least once every StallAbandonmentHours hours
    Timer resets on server restart
```

### "What's the tax rate at a market area?"

```
Effective Tax = SaleTaxAmount + MarketArea.TaxModifier
Clamped to [0.0, 1.0]

If stall is outside all MarketAreas:
    Effective Tax = SaleTaxAmount
```

### "How many items can I sell on my [stand type]?"

| Stand | Config Field | Default |
|-------|-------------|---------|
| Market Table | `MarketTableSaleSlots` | 8 |
| Market Stall | `MarketStallLv1SaleSlots` | 16 |
| Large Market Stall | `MarketStallLv2SaleSlots` | 32 |

---

## Mapping Config Values to Player Behavior

Use this table to translate config settings into player-facing advice:

| If Config Shows... | Tell Players... |
|--------------------|----------------|
| `SaleTaxAmount: 0` | "There's no tax — you pay exactly the listed price" |
| `SaleTaxAmount: 0.05` | "A 5% tax is added to all purchases. An item listed at 100 will cost you 105" |
| `SaleTaxAmount: 0.20` | "A 20% tax is added to all purchases. Budget accordingly — a 100-coin item actually costs 120" |
| `FreeTaxDays: 0` | "Your earnings are taxed immediately! Withdraw after every sale" |
| `FreeTaxDays: 7` | "You have a full week to withdraw earnings before the daily tax kicks in" |
| `DailyTaxAmount: 0` | "Good news — there's no daily tax on stored earnings. Take your time withdrawing" |
| `DailyTaxAmount: 0.10` | "Stored earnings decay fast — 10% per day! Withdraw quickly" |
| `DeListFee: 0` | "Delisting items is free — feel free to adjust your inventory" |
| `DeListFeeMin: 0` | "All items have a delist fee regardless of price" |
| `DeListFeeMin: 500` | "Only high-value items (max listed price 500+) incur a delist fee" |
| `DeListGraceHours: 0` | "Delist fees apply immediately — no grace period" |
| `DeListGraceHours: 4` | "You have 4 hours after listing to delist for free" |
| `DeListGraceHours: 24` | "You have a full day after listing to delist for free" |
| `MarketTableSaleSlots: 4` | "Market Tables can only hold 4 items — choose wisely" |
| `MarketStallLv2SaleSlots: 64` | "Large Market Stalls can hold up to 64 items — that's a serious shop!" |
| `GUIOption: 0` | "The market uses a classic parchment-style interface" |
| `GUIOption: 1` | "The market uses a modern, clean interface" |
| `StallAbandonmentHours: 0` | "Stall abandonment is disabled — your items stay on the stall forever" |
| `StallAbandonmentHours: 4` | "If you don't come within 100 meters of your stall for 4 hours, all items are dumped to the ground" |
| `StallAbandonmentHours: 168` | "You have up to a week before your unvisited stall drops its items" |
| `MarketAreas` with `TaxModifier: -0.02` | "Stalls in [area name] get a 2% tax discount — buyers pay less!" |
| `MarketAreas` with `ConciergePosition` set | "Visit the concierge NPC in [area name] to browse all nearby stalls at once" |

---

## Edge Cases & Defaults

### When Config Is Missing

If no `Config.json` exists, the server creates one with all defaults on first start. The default currency is:
- `Coins` with Gold (1000), Silver (100), Copper (1)

### When BlackList Is Null/Missing

If the BlackList field is null or missing after load, it resets to the default: `["PM_GoldCoin", "PM_SilverCoin", "PM_CopperCoin"]`.

### When SaleTaxAmount Is 0

No tax is applied. Buyers pay exactly the listed price. The seller receives exactly the listed price.

### When DailyTaxAmount Is 0

No earnings decay. Money in the stall balance stays forever.

### When FreeTaxDays Is 0

Daily tax applies immediately starting the same day as the sale. There is no grace period.

### When DeListFee Is 0

Delisting is always free regardless of item price.

### When DeListFeeMin Is Very High

Effectively disables delist fees for most items. Only extremely high-priced items would incur a fee.

### When DeListGraceHours Is 0

No grace period. Delist fees apply immediately based on price thresholds.

### When LoggingChannel Is Empty

No Discord logging occurs. Sales happen silently on the server.

### Multiple Currency Entries

Each stall stores which currency it uses. Buyers must pay in that specific currency. The stall owner can change currencies from the seller menu, but existing balance stays in the old currency's value system.

### When StallAbandonmentHours Is 0 or Negative

The abandonment system is completely disabled. Stalls hold items indefinitely (until the owner delists them or the stall is destroyed).

### When StallAbandonmentHours Is Positive

The timer is **in-memory only** and resets on every server restart. A server restart gives each stall owner a fresh abandonment window. Only online-time counts — if a stall owner is offline, the timer still runs until the server restarts.

### When MarketAreas Is Empty

No market zones exist. All stalls use the base `SaleTaxAmount` with no modifier. No concierge NPCs are spawned.

### When MarketArea TaxModifier Makes Tax Negative

The effective tax is clamped to a minimum of `0.0`. A `TaxModifier` of `-0.10` with a `SaleTaxAmount` of `0.05` results in 0% tax, not -5%.

### When MarketArea ConciergePosition Is "0 0 0"

No concierge NPC is spawned for that area. The area still applies its tax modifier to stalls within it.

---

## Config-to-Documentation Prompt Template

When you receive a server's `Config.json`, use this template to generate a player-facing KB article:

```
Given this PlayerMarkets config:
[PASTE CONFIG HERE]

Generate a player-facing guide that covers:
1. What currency is used and what coins/items are worth
2. What the tax rates are (sale tax, daily tax, delist fee)
3. How many items each stand type can hold
4. What items are blacklisted from sale
5. Whether stall abandonment is enabled and the time limit
6. What market areas exist, their tax modifiers, and whether they have concierge NPCs
7. Any special notes based on non-default values
8. Practical tips based on the economy settings (e.g., "withdraw quickly" if high daily tax, "visit your stall" if abandonment is short)
```

---

*PlayerMarkets Config Guide — For AI Knowledge Base Generation*
