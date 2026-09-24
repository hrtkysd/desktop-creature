## Project Status

Desktop Creature is currently under active development and is not yet a complete end-user application.

The current implementation is mainly focused on building the core model, editor architecture, rendering pipeline, and animation system required to define and preview a Creature.

APIs, file formats, and internal architecture may change as the project evolves.

## Planned Work

Some major areas are still incomplete or have not yet been started:

- **Creature Runtime**
  - Run a saved Creature as an actual desktop pet.
  - Manage runtime-specific state separately from the Creature definition.

- **Genome**
  - Introduce a genome model that defines physical and behavioral characteristics of a Creature.
  - Allow Creature Lab to edit and preview genome-driven variations.

- **Creature File Format**
  - Save and load Creature definitions as `.creature` files.
  - Add stricter validation and consistency checks when loading or editing data.

- **Animation**
  - Improve animation authoring and validation.
  - Add more natural idle and movement animations.

- **Creature Lab**
  - Add Undo / Redo support.
  - Improve editing workflows and usability.

- **Desktop Interaction**
  - Add system tray integration.
  - Support interactions such as feeding or playing with the Creature.

- **Diagnostics**
  - Add structured logging with log rotation.
  - Improve error reporting and diagnostics.

The long-term goal is for Creature Lab to be able to create, edit, preview, and export complete Creature definitions that can then be loaded by the lightweight desktop runtime.
