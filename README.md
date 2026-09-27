## Project Status

Desktop Creature is under active development.

The current implementation provides the foundation for defining, editing, animating, and previewing a Creature in Creature Lab.

Implemented areas include:

- **Creature model**
  - Skeleton-based part hierarchy.
  - Appearance data associated with individual parts.
  - Stable IDs for parts and animations.
  - Editor APIs that keep mutation paths explicit.

- **Animation**
  - Track- and keyframe-based animation.
  - Pose sampling and playback.
  - Transform animation for individual Creature parts.

- **Creature Lab**
  - Interactive part selection and transform editing.
  - Pivot, scale, rotation, and position editing.
  - Undo / Redo with continuous edits grouped into a single operation.
  - Dockable editor UI built with ImGui.

- **Rendering**
  - Native D3D11 sprite renderer.
  - Shared rendering path intended for both Creature Lab and the runtime.
  - Off-screen rendering for the Lab preview, with editor overlays handled separately by ImGui.

The project is currently moving from the editor and rendering foundation toward a usable desktop runtime and persistent Creature format.

APIs and file formats are still expected to evolve while these pieces are being connected.

## Planned Work

The next major areas are:

- **Creature Runtime**
  - Run a saved Creature as a lightweight desktop pet.
  - Keep runtime state separate from the Creature definition.

- **Creature File Format**
  - Save and load complete Creature definitions as `.creature` files.
  - Add validation for cross-component consistency.

- **Genome**
  - Define physical and behavioral characteristics independently from individual animation data.
  - Allow Creature Lab to experiment with genome-driven variations.

- **Animation**
  - Improve authoring workflows and validation.
  - Create more natural idle and movement behavior.

- **Desktop Interaction**
  - Add system tray integration.
  - Add simple interactions such as feeding or playing with the Creature.

- **Diagnostics**
  - Add structured logging and log rotation.
  - Improve error reporting.

The long-term goal is for Creature Lab to produce complete `.creature` definitions that can be loaded and rendered by a lightweight desktop runtime.

## Design Notes

Short records of design decisions and the reasoning behind them.

See [docs/design-notes](docs/design-notes/).