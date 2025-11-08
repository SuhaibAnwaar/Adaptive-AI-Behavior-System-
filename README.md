# Adaptive-AI-Behavior-System

Video demonstration: https://www.youtube.com/watch?v=mA1mHz4oy3Y

Author: Suhaib Anwaar

---

## Summary

This project implements a simple AI patrol bot mechanic for Unreal Engine 4. The AI patrols a defined zone by moving from checkpoint to checkpoint. The player's objective is to avoid being seen by the bot. When the player enters the bot's perception range the bot chases; the player must use the environment (shadows, higher ground, obstructions) to lose the bot. When the bot loses sight of the player it returns to its patrol.

Key gameplay twist: when the bot is in shadow its perception range is reduced, making it easier for the player to hide.

---

## Purpose & Motivation

- Produce a clean, extensible patrol-bot C++ implementation that demonstrates good encapsulation and class-based design.
- Create a foundation that can be expanded with more complex behaviors (attacks, ranged enemies, cameras, drones).
- Explore how lighting and shadows can influence AI perception (inspired by Thief and Assassin's Creed).

---

## Features

- AI patrols a sequence of checkpoints (trigger boxes).
- AI perceives the player with UE4's AIPerception (sight).
- Perception range is affected by lighting:
  - In light: 500 units sight radius.
  - In shadow: 300 units sight radius.
- Player can briefly visualize AI perception using an in-editor toggle (GamePlayer Debugger key binding).
- AI increases movement speed when chasing.
- Multiple instances of the AI can be added to the scene.
- Checkpoint linking is done in-editor (each checkpoint points to its next checkpoint).

---

## System / Software Requirements

- Unreal Engine 4 (project built using the standard Third Person template)
- Visual Studio (for C++ code)
- A development machine capable of running UE4 editor at stable framerates

---

## Project Structure

- AI Character (C++ derived pawn / blueprint)
  - Rotates to face movement direction (prevents gliding)
  - Exposes eye location used for light checks
- AI Controller (C++ derived controller)
  - Possesses the AI Character
  - Configures UAISenseConfig_Sight (sight radius, lose sight radius, FOV, sight age)
  - Decides whether to patrol or chase
- Checkpoint Actor (trigger box)
  - Holds a reference to the next checkpoint
  - OnComponentBeginOverlap advances the bot's target to the next checkpoint
- Point Light (scene)
  - Used to determine whether the AI is in light or shadow via a line trace from the light to the AI eye location
- Navigation Mesh Bounds Volume
  - Provides pathfinding for AI movement

---

## Technical Overview

AI Perception:
- Uses UAISenseConfig_Sight set up in the AI controller.
- Parameters used:
  - Sight Radius (adjusted between light/dark)
  - Lose Sight Radius
  - Sight Age (how long the bot remembers last seen target)
  - Peripheral Vision Angle (FOV = 90°)

Light / Shadow Detection:
- A line trace is performed from the point light to the AI eye location.
- If the trace hits the AI, the AI is considered in light.
- If an object blocks the trace (returns false for hitting the AI), the AI is considered in shadow.
- When in shadow, the AI's sight radius is reduced (500 → 300 units).

Patrolling:
- The bot moves along checkpoints specified by trigger boxes linked in-editor.
- When the bot overlaps a checkpoint trigger, the checkpoint sets the bot's next target to the checkpoint's configured next checkpoint.
- The bot follows straightforward paths using UE4 navigation; if interrupted (chase) it will resume toward the checkpoint it was heading to before detection.

Chasing:
- When the perception component detects the player, a global flag (isPlayerDetected) is set.
- In Tick(), if isPlayerDetected is true the bot moves toward the player's last known location and increases speed.
- If the player is lost (out of perception or breaks line-of-sight and sight age expires), the bot returns to patrolling.

Rotation:
- Character rotation is applied so the bot faces its movement direction (avoids unnatural gliding).

Limitations:
- Bot cannot jump — higher player positions can be used to evade.
- When the bot catches the player, no damage or game-over behavior is implemented (left as a future extension).
- Light attenuation in this demo is large; shadowing is determined purely by blocking geometry between the light and AI eye.

---

## Extending the Project

- Add attack/damage behavior when the bot catches the player.
- Implement behavior trees for more complex state management (sensing, searching, investigating).
- Add additional senses (hearing) or memory systems (last heard location).
- Add vertical navigation (allow bot to climb/jump or use stairs) or pathfinding around multi-story geometry.
- Implement patrol variations (randomized paths, waypoints with wait times).
- Add different AI types (security cameras, drones) that reuse the perception system.

## Conclusion

This repository demonstrates a minimal, extendable AI patrol bot system that combines UE4 perception with a checkpoint patrol mechanic and light-based visibility modulation. It is intended as a learning foundation for game programmers and designers who wish to extend AI functionality in C++.
