# PlayerMarkets — Comprehensive Player Guide

**Mod:** PlayerMarkets by DaemonForge  
**Version:** 0.1  
**Last Updated:** February 2026

---

## Table of Contents

1. [Overview](#overview)
2. [Currency System](#currency-system)
3. [Market Stand Types](#market-stand-types)
4. [Crafting Market Kits](#crafting-market-kits)
5. [Building Your Market Stand](#building-your-market-stand)
6. [Selling Items (Owner Guide)](#selling-items-owner-guide)
7. [Buying Items (Shopper Guide)](#buying-items-shopper-guide)
8. [Pricing, Taxes & Fees](#pricing-taxes--fees)
9. [Market Areas & Concierge NPCs](#market-areas--concierge-npcs)
10. [Stall Abandonment](#stall-abandonment)
11. [Managing Your Market Stand](#managing-your-market-stand)
12. [Item Display Categories](#item-display-categories)
13. [Destruction & Dismantling](#destruction--dismantling)
14. [Blacklisted Items](#blacklisted-items)
15. [Tips & Best Practices](#tips--best-practices)
16. [FAQ / Troubleshooting](#faq--troubleshooting)

---

## Overview

PlayerMarkets is a player-to-player trading mod for DayZ. It lets you **craft and build physical market stalls** in the game world, **list your items for sale** with custom prices, and allow other players to **browse and purchase** them using an in-game coin currency. Think of it as setting up your own shop in the apocalypse.

**Key concepts:**
- You craft a kit, place it, and build it into a market stand
- You, the owner, stock it with items and set prices through a GUI menu
- Other players walk up and can browse/buy through a separate GUI menu
- All currency is physical coins you carry in your inventory
- Items are visually displayed on the stand for everyone to see
- The stand persists through server restarts

---

## Currency System

PlayerMarkets uses a physical coin-based economy. Coins are real items that take up inventory space and have weight.

### Coin Types

| Coin | Class Name | Value | Max Stack | Weight | Description |
|------|-----------|-------|-----------|--------|-------------|
| **Copper Coin** | `PM_CopperCoin` | 1 | 100 | 4g | Base unit of currency |
| **Silver Coin** | `PM_SilverCoin` | 100 | 50 | 20g | Worth 100 Copper |
| **Gold Coin** | `PM_GoldCoin` | 1000 | 20 | 100g | Worth 1,000 Copper or 10 Silver |

### Conversion Rates

| From | To | Rate |
|------|----|------|
| 100 Copper | 1 Silver | 100:1 |
| 10 Silver | 1 Gold | 10:1 |
| 1,000 Copper | 1 Gold | 1000:1 |

**Key details:**
- Coins are **splittable** — you can split a stack into smaller amounts
- Coins are **physical items** — you need to carry them. They have weight and take inventory space
- Coins **cannot be sold** on market stands (they are blacklisted by default)
- The system automatically makes change when buying/selling — you don't need exact coins

> **Note:** Server administrators can configure custom currencies with different item types and values. The coin system described above is the default. Your server may use a different currency — check with your server admin.

---

## Market Stand Types

There are three types of market stands, each offering different capacity and build complexity:

### Market Table (Small)

| Property | Value |
|----------|-------|
| **Kit** | Market Table Kit (`PM_MarketTableKit`) |
| **Max Sale Slots** | 8 (default) |
| **Display Slots** | 4 small + 2 large |
| **Build Stages** | 1 (Table only) |
| **Materials Required** | 6 Planks, 16 Nails |
| **Crafting Recipe** | 1 Rope + 2 Wooden Sticks |

The simplest option. Just one building phase, no roof. Good for a quick roadside shop.

### Market Stall (Medium)

| Property | Value |
|----------|-------|
| **Kit** | Market Stall Kit (`PM_MarketKit`) |
| **Max Sale Slots** | 16 (default) |
| **Display Slots** | 5 small + 4 large |
| **Build Stages** | 3 (Table → Posts → Roof) |
| **Total Materials** | 22 Planks, 32 Nails |
| **Crafting Recipe** | 1 Rope + 3 Wooden Sticks |

The standard stall. Has a full roof for a proper shop feeling, with moderate display capacity.

### Large Market Stall (Large)

| Property | Value |
|----------|-------|
| **Kit** | Large Market Stall Kit (`PM_MarketV2Kit`) |
| **Max Sale Slots** | 32 (default) |
| **Display Slots** | 10 small + 7 large |
| **Build Stages** | 3 (Table → Posts → Roof) |
| **Total Materials** | 30 Planks, 44 Nails |
| **Crafting Recipe** | 1 Rope + 4 Wooden Sticks |

The biggest stall — ideal for established merchants with lots of stock.

---

## Crafting Market Kits

Before you can build a stand, you need to craft the kit.

### Required Items

| Kit Type | Ingredient 1 | Ingredient 2 |
|----------|-------------|-------------|
| **Market Table Kit** | 1x Rope | 2x Wooden Sticks (or Sharp Sticks) |
| **Market Stall Kit** | 1x Rope | 3x Wooden Sticks (or Sharp Sticks) |
| **Large Market Stall Kit** | 1x Rope | 4x Wooden Sticks (or Sharp Sticks) |

### How to Craft

1. Hold the **Rope** in your hands (or have it in inventory)
2. Combine with **Wooden Sticks** (or Sharp Sticks) — ensure you have the required quantity
3. The crafting recipe will appear — select it
4. The kit will be created on the ground near you

### Conditions

- Both ingredients must **not be ruined** (damage state < 3)
- Neither ingredient can be an attachment (must be loose items)
- The Rope is **consumed** (destroyed) in the process
- The sticks are **consumed** (quantity reduced by 2 for all recipes, even if more are required to start)
- The resulting kit's health is averaged from the ingredients' health
- **Note for servers with HeroesAndBanditsMod_V2:** Crafting the Market Stall requires an absolute HABLevel of at least 2, and the Large Market Stall requires an absolute HABLevel of at least 4.

### Disassembling a Kit

To disassemble a kit back to raw materials:
- Remove the **Rope** from the kit (it's attached inside)
- This automatically destroys the kit and spawns **4 Wooden Sticks** and the **Rope** on the ground

---

## Building Your Market Stand

### Placing the Kit

1. Put the kit in your hands
2. Look at the ground where you want your stall
3. Use the **Deploy** action (you'll see a hologram preview)
4. Position and rotate it to your liking
5. Confirm placement

The kit transforms into an unbuilt market stand frame on the ground.

### Construction Stages

#### Market Table (1 Stage)
| Stage | Name | Materials |
|-------|------|-----------|
| 1 | Trader Table | 6 Wooden Planks + 16 Nails |

#### Market Stall (3 Stages)
| Stage | Name | Materials | Requires Previous |
|-------|------|-----------|-------------------|
| 1 | Market Table | 12 Wooden Planks + 16 Nails | — |
| 2 | Roof Posts | 2 Wooden Planks + 8 Nails | Stage 1 |
| 3 | Trader Roof | 8 Wooden Planks + 8 Nails | Stage 2 |
| **Total** | | **22 Planks + 32 Nails** | |

#### Large Market Stall (3 Stages)
| Stage | Name | Materials | Requires Previous |
|-------|------|-----------|-------------------|
| 1 | Trader Table | 16 Wooden Planks + 24 Nails | — |
| 2 | Roof Posts | 2 Wooden Planks + 8 Nails | Stage 1 |
| 3 | Trader Roof | 12 Wooden Planks + 12 Nails | Stage 2 |
| **Total** | | **30 Planks + 44 Nails** | |

### How to Build

1. Attach materials to the stand via its inventory (planks/nails/logs for the current stage)
2. Equip a tool (hammer, hatchet, etc.) in your hands
3. Look at the stand and use the **Build** action
4. Repeat for each construction stage

### Ownership

- **The player who completes the final build stage becomes the owner**
- The stand is automatically named `"[YourName]'s Market"` upon completion
- Only the owner can sell, delist, withdraw money, dismantle, or destroy the stand
- Building materials (planks, nails) are **dropped on the ground** after the final stage completes, freeing up slots for merchandise

---

## Selling Items (Owner Guide)

### Opening the Seller Menu

1. Stand **behind/inside** your completed market stand (within ~0.5m of the center point)
2. Look at the stand
3. The **"Sell"** action will appear — use it
4. The **Seller Menu** opens

> **Requirements:** You must be the owner, the stand must be fully built, and you must be standing close to the center of the stand (behind the counter).

### Seller Menu Interface

The seller menu has two main panels:

**Left Panel — Your Inventory:**
- Shows items currently in your inventory
- Each item has a **"List"** button

**Right Panel — Stall Items:**
- Shows items you've already listed for sale
- Each item has **"Edit"** and **"Delist"** buttons
- Shows remaining available sale slots (e.g., "5/16 Slots")

**Bottom Controls:**
- **Currency Selector** — Choose which currency your stall accepts
- **Withdraw** button — Collect your earnings
- **Slots Counter** — Shows how many listing slots are used vs. available

### Listing an Item for Sale

1. Open the Seller Menu
2. Drop the item you want to sell into the stall's cargo (inventory)
3. In the left panel, find the item and click **"List"**
4. A **Set Price** dialog appears:
   - Enter your desired price (numbers only)
   - The tax amount will be displayed so you know what buyers actually pay
   - Click **"Set"** to confirm, or **"Cancel"** to abort
5. The item is now listed and will be visually displayed on the stand

### Editing a Listed Item's Price

1. Open the Seller Menu
2. In the right panel, find the item and click **"Edit"**
3. The Set Price dialog opens with the current price
4. Enter the new price and click **"Set"**

> **Important:** Increasing a price affects the delist fee. The delist fee is based on the **highest price the item ever had**, not the current price. Lowering the price does NOT reduce the delist fee.

### Delisting an Item

1. Open the Seller Menu
2. Find the item in the right panel and click **"Delist"**
3. The item is moved back into the stall's cargo (or dropped on the ground if cargo is full)

> **Delist Fee:** Delisting may incur a fee. See [Pricing, Taxes & Fees](#pricing-taxes--fees).

### Withdrawing Earnings

1. Open the Seller Menu
2. Click the **"Withdraw"** button
3. All accumulated earnings are converted to coins and placed in your inventory
4. The balance resets to zero

> **Warning:** Don't leave money sitting in your stall for too long! See [Daily Tax](#daily-tax-on-stored-earnings) below.

---

## Buying Items (Shopper Guide)

### Opening the Buyer Menu

1. Walk up to any **completed market stand** (any player's)
2. Look at the stand
3. The **"Buy"** action will appear — use it
4. The **Buyer Menu** opens

> **Note:** The stand must be fully built. You do not need to be the owner. You can buy from anyone's stall.

### Buyer Menu Interface

**Main View:**
- Shop name displayed at the top (e.g., "PlayerName's Market")
- Grid of items for sale, each showing:
  - Item preview image
  - Item name
  - Price
  - Quantity/amount (for stackable items)
  - Health state indicator (color-coded circle)
- Your current coin balance displayed

**Item Detail View (click an item):**
- Large 3D rotatable preview of the item
- Full item name and description
- Price with tax breakdown
- Item weight
- Liquid type (for containers)
- Health state
- **"Buy"** button and **"Cancel"** button

### How to Buy

1. Open the Buyer Menu at any stall
2. Browse items — click one to see details
3. In the detail view, click **"Buy"**
4. If you have enough money:
   - The total cost (price + tax) is deducted from your coins
   - The item goes into your inventory (or is placed at your feet if inventory is full)
   - You get a notification confirming the purchase
5. If you can't afford it, you'll get a "Not Enough Money" notification

### Health State Colors

Items display a health indicator showing their condition:

| Color | Condition |
|-------|-----------|
| Green | Pristine / Worn |
| Yellow | Damaged |
| Orange | Badly Damaged |
| Red | Ruined |

> **Note:** You **cannot buy your own items**. If you try, you'll see a "Can't buy your own items" message.

---

## Pricing, Taxes & Fees

### Sale Tax (Buyer Pays)

When a buyer purchases an item, they pay the listed price **plus** a sale tax.

| Setting | Default Value |
|---------|---------------|
| `SaleTaxAmount` | 5% (0.05) |

**Example:** An item listed at 100 coins costs the buyer **105 coins** (100 + 5% tax). The seller receives the full 100 into their stall balance.

### Delist Fee

If a seller delists an item (removes it from sale), a fee may apply based on the item's **highest-ever listed price**. However, there is a **grace period** after listing during which delisting is free.

| Setting | Default Value |
|---------|---------------|
| `DeListFee` | 5% (0.05) |
| `DeListFeeMin` | 100 |
| `DeListGraceHours` | 4 hours |

**Rules:**
- If the item was listed **less than 4 hours ago**, delisting is **free** (grace period)
- If the item's highest price was **below 100** (the minimum threshold), there is **no delist fee**
- If the highest price was 100+, the fee is 5% of that highest price
- The fee is calculated as: `highest_price_ever × DeListFee`
- The delist fee is **deducted from your wallet** when you confirm the delist

**Example:** You listed an item at 500, then changed it to 200, then delist it 6 hours later. The fee is based on 500 (the max), so the delist fee = 500 × 0.05 = **25 coins**. If you had delisted within 4 hours, it would have been free.

### Daily Tax on Stored Earnings

Money left in your stall balance is subject to a daily tax after a grace period.

| Setting | Default Value |
|---------|---------------|
| `FreeTaxDays` | 3 days |
| `DailyTaxAmount` | 3% (0.03) |

**How it works:**
1. Each sale records the date it happened
2. Money from sales within the last 3 days is **tax-free**
3. After 3 days, each day's balance is taxed at 3% per day
4. If a day's balance is reduced to 0 or below by taxation, it's removed entirely

**Example timeline:**
- Day 1: You sell 1,000 coins worth of goods
- Day 2-3: No tax (grace period)
- Day 4: 3% tax → 30 coins removed → 970 remaining
- Day 5: 3% tax → ~29 coins removed → ~941 remaining
- And so on...

> **Tip:** **Withdraw your earnings regularly** to avoid losing money to daily taxation!

---

## Market Areas & Concierge NPCs

Server administrators can define **Market Areas** — named zones where the trading experience is enhanced.

### What Are Market Areas?

A Market Area is a circular region of the map (defined by a centre point and radius) that can provide:

- **Tax adjustments** — Stalls inside a market area may have a lower (or higher) tax rate than stalls elsewhere. You'll see the effective rate when browsing items.
- **A Concierge NPC** — An invincible NPC stationed in the area that lets you browse and buy from **all stalls in the area at once**, without walking to each one individually.
- **Placement notification** — When you deploy a market kit inside a market area, you'll see a notification showing the area name and the effective tax rate.

### The Concierge NPC

The Concierge is a special NPC that acts as a one-stop shop for an entire market area.

**How to use the Concierge:**

1. Walk up to the Concierge NPC (they stand in a fixed spot within the market area)
2. Look at them and use the **interaction action**
3. The **Concierge Menu** opens, showing all items for sale across every stall in the area

**Concierge Menu Features:**

- **Category filtering** — Filter by: All, Weapons, Ammo, Clothing, Food, Medical, Containers, Attachments, Tools, Melee, or Other
- **Aggregated listing** — See items from all stalls grouped by stall name
- **Item details** — Click any item to see a full 3D rotatable preview, description, health, quantity, weight, and attachments
- **Buy confirmation** — Shows base price, tax amount and percentage, total cost, currency type, and your current balance (colour-coded green/red for affordability)
- **Multi-currency display** — The bottom bar shows your balance for every currency that stalls in the area use
- **Remote purchasing** — Buy from any stall in the area without walking to it. The purchase goes through the normal buy flow.

**Concierge NPC Properties:**
- Invincible — cannot be killed or damaged
- Ignored by zombies and animals
- Cannot be looted
- Spawns automatically on server start

> **Note:** Not all servers have market areas configured. If your server has no concierge NPC, this feature is not enabled. Ask your server admin.

### Tax Differences in Market Areas

Stalls inside a market area may have modified tax rates. For example, a server might set up a "Free Trade Zone" with reduced taxes to encourage players to set up shop there.

- The effective tax is shown in the buy confirmation dialog
- Stalls outside all market areas use the server's base tax rate
- The tax modifier is set by the server admin per area

---

## Stall Abandonment

To keep the game world tidy and prevent permanently occupied stalls, servers can enable a **stall abandonment** system.

### How It Works

1. The server tracks the last time the stall owner was within **100 meters** of their stall
2. A check runs every 60 seconds
3. If the owner hasn't been within 100 meters for a configured number of hours (default: 4 hours), the stall is considered **abandoned**
4. When abandoned, **all items** (both listed sale items and cargo storage) are **dumped on the ground** at the stall's location
5. The stall's listing data is cleared

### What This Means for You

- **Visit your stall regularly!** If you don't come within 100 meters for the configured time, you'll lose your listings
- Items are not destroyed — they're dropped on the ground, but anyone can pick them up
- The timer **resets on every server restart**, so a restart gives you a fresh window
- Your stall structure itself is not affected — only the items and listings are cleared. You can re-stock it.

### Checking the Time Limit

The abandonment window is set by the server administrator (`StallAbandonmentHours` in the config, default 4 hours). Some servers may disable it entirely (set to 0) or use a longer window. Ask your server admin for the specific setting.

> **Tip:** If you're going to be away for a while, withdraw your earnings and delist valuable items to keep them safe.

---

## Managing Your Market Stand

### Naming

Your stall is automatically named `"[YourName]'s Market"` when you complete construction. This name is visible to all buyers.

### Currency Selection

In the Seller Menu, you can choose which currency your stall accepts (if the server has multiple currencies configured). Use the currency selector dropdown at the bottom.

### "In Use" Lock

When you have the Seller Menu open, the stall's cargo is hidden from other players' inventory view. This prevents conflicts while you're managing items.

### Capacity Management

Each stand type has a maximum number of sale slots:
- **Market Table:** 8 slots (default)
- **Market Stall:** 16 slots (default)
- **Large Market Stall:** 32 slots (default)

The Seller Menu shows your used/available slots. Once you hit the max, you cannot list more items until you delist some.

> **Note:** These limits are server-configurable. Your server may have different values.

---

## Item Display Categories

When you list an item for sale, the mod automatically places it on a visual display proxy on your stand. Different item types go to different display positions:

| Item Category | Displayed As | Examples |
|---------------|------------|----------|
| **Rifles/Long Guns** | Weapon rack (large slot) | AK, M4, Mosin, etc. |
| **Pistols** | Pistol display (small slot) | FX-45, Glock, CR-75, etc. |
| **Melee Weapons** | Melee display | Baseball bat, pipe, etc. |
| **Knives** | Knife display | Combat knife, steak knife, etc. |
| **Headgear** | Headgear display | Helmets, hats, etc. |
| **Magazines** | Magazine display | All weapon magazines |
| **Food** | Food display | Canned goods, edibles |
| **Drinks/Soda** | Alternate food display | Soda cans, drink items |
| **Water Containers** | Water display | Water bottles, canteens |
| **Canteens/Belt Items** | Canteen display | Canteen, hip flask |
| **Grenades** | Grenade display | Frag grenades, flashbangs |
| **Other Items** | Hidden storage | Anything that doesn't fit a display category goes into the merchant storage (not visually displayed but still purchasable) |

---

## Destruction & Dismantling

### Dismantling (Peaceful Removal)

- Only the **owner** can dismantle a stand
- You must **delist ALL items first** — you cannot dismantle while items are listed for sale
- Use a tool (hacksaw, pliers, etc.) and the **Dismantle** action
- Construction stages are removed in reverse order
- Materials are returned

### Destroying (Sledgehammer)

- Only the **owner** can use the destroy action
- Requires a **Sledgehammer** in hand
- The **"Destroy [Part]"** action appears when looking at the stand
- Must be within ~2m of the stand
- Destroying removes construction parts (with the sledgehammer receiving damage per use)
- When fully destroyed/dismantled, ownership and name are reset

### Damage Resistance

Market stands are designed to be protected:

| Damage Source | Table (Base) | Posts/Roof |
|---------------|-------------|------------|
| **Bullets** | **Immune** (0 damage) | Normal damage |
| **Melee** | **Immune** (0 damage) | Reduced (0.65x) |
| **Grenades** | **Immune** (0 damage) | High (50x) |

> The table/base of all stands is invulnerable to all damage types. Posts and roof sections can be damaged by bullets, melee, and explosives. The Market Table (small) base is also fully invulnerable since it only has one stage.

---

## Blacklisted Items

Certain items **cannot be placed into market stand cargo** and therefore cannot be sold. By default:

- **Gold Coins** (`PM_GoldCoin`)
- **Silver Coins** (`PM_SilverCoin`)
- **Copper Coins** (`PM_CopperCoin`)

> Server administrators can add additional items to the blacklist. Check with your server for specific restrictions.

---

## Tips & Best Practices

1. **Location matters** — Set up your stall in a high-traffic area for more customers. Near spawn areas, military zones, or popular towns work well. If the server has **Market Areas**, building inside one can give you a tax discount and concierge visibility.

2. **Withdraw earnings often** — The daily tax erodes your balance after 3 days. Withdraw regularly!

3. **Visit your stall** — If stall abandonment is enabled, make sure to come within 100 meters of your stall regularly to prevent items from being dumped.

4. **Price competitively** — Buyers can see your prices before buying. If a nearby stall has lower prices, they'll shop there.

5. **Stock variety** — Having weapons, ammo, food, and medical supplies gives buyers a reason to visit.

6. **Start small** — A Market Table is cheap to build and great for testing the waters before upgrading.

7. **Be careful with delist fees** — Setting a high price then delisting results in a fee based on that high price, even if you lowered it later.

8. **Keep your stall built** — An incomplete stall cannot be used for buying or selling. Make sure you have all materials before starting construction.

9. **Guard your stall** — While the base is immune to damage, the upper sections (posts/roof) can be destroyed. Consider building in a safe zone or defended area.

10. **Items on the ground** — If a buyer's inventory is full, purchased items drop at their feet. Make sure to pick them up!

11. **Ruined items are auto-removed** — If an item on your stall becomes ruined, it's automatically delisted and removed from the listings.

12. **Use the Concierge** — If a market area has a Concierge NPC, browse from there to see everything available without running stall to stall.

---

## FAQ / Troubleshooting

### Q: I can't see the "Sell" action on my stall
**A:** You must be the owner, the stand must be fully built (all stages complete), and you must be standing behind/inside the stall (within ~0.5 meters of the center).

### Q: I can't see the "Buy" action on someone else's stall
**A:** The stall must be fully built with all construction stages complete. Walk closer and look directly at the stand.

### Q: Where did my purchased item go?
**A:** If your inventory was full, the item was placed on the ground at your feet. Look down! You'll also see a notification saying it was placed on the ground.

### Q: Why can't I dismantle my stall?
**A:** You must delist ALL items from sale before dismantling. Open the Seller Menu and delist everything first.

### Q: I'm being charged more than the listed price!
**A:** Buyers pay the listed price PLUS the sale tax (default 5%). This is normal and is shown in the item detail view before you confirm the purchase.

### Q: My stall balance is shrinking!
**A:** Money left in your stall for more than 3 days (default) is taxed at 3% per day. Withdraw your earnings promptly!

### Q: Can someone steal items from my stall?
**A:** Items on display cannot be taken directly — they can only be purchased through the buy menu. The base-building component of the stall is also invulnerable to most damage.

### Q: Can I have multiple stalls?
**A:** Yes! You can build as many stalls as you want (subject to server rules). Each stall is independent with its own inventory, prices, and balance.

### Q: Can I sell to my own stall / buy my own items?
**A:** No. The system prevents you from purchasing items from your own stall. You'll see a "Can't buy your own items" message.

### Q: What happens if my stall is destroyed?
**A:** If the stall is dismantled or destroyed to the point where it's no longer "built," ownership and the stand name are reset. You'd need to rebuild and relist items. Items that were physically on the stand should drop.

### Q: What is the Concierge NPC?
**A:** The Concierge is an invincible NPC found in server-defined Market Areas. Interact with it to browse and purchase items from **all stalls in that market area** at once — no need to visit each stall individually. It shows categories, item details, and lets you buy remotely.

### Q: Why is the tax different at some stalls?
**A:** Stalls inside a Market Area may have a modified tax rate (higher or lower) set by the server admin. The Concierge buy dialog and the normal buy dialog both show the actual tax rate before you confirm a purchase.

### Q: My stall's items disappeared! What happened?
**A:** If the server has stall abandonment enabled (default: 4 hours), your items are dumped on the ground if you haven't been within 100 meters of your stall for the configured time. Visit your stall regularly to prevent this. The timer also resets on server restarts.

### Q: Can other players help me sell?
**A:** The mod has an authorized sellers system, but currently only the owner can access the sell menu. Authorized sellers may be available in a future update.

---

## Quick Reference Card

| Action | How |
|--------|-----|
| **Craft Table Kit** | Rope + 2 Sticks |
| **Craft Stall Kit** | Rope + 3 Sticks |
| **Craft Large Stall Kit** | Rope + 4 Sticks |
| **Place Kit** | Hold kit → Deploy action |
| **Build** | Attach materials → Hammer → Build action |
| **Sell Menu** | Stand behind stall → "Sell" action (owner only) |
| **Buy Menu** | Approach stall → "Buy" action (anyone) |
| **List Item** | Seller Menu → Drop item in cargo → List → Set price |
| **Edit Price** | Seller Menu → Edit button on listed item |
| **Delist Item** | Seller Menu → Delist button on listed item |
| **Withdraw Cash** | Seller Menu → Withdraw button |
| **Concierge** | Approach Concierge NPC → Interact → Browse & buy from all area stalls |
| **Dismantle** | Delist all items → Tool in hand → Dismantle action |
| **Destroy Part** | Sledgehammer in hand → Destroy action (owner only) |

---

*PlayerMarkets by DaemonForge — Version 0.1*
