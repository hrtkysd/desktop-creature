### Why creature editing goes through CreatureEditor

Creature data should not be freely mutable from UI code.

The Lab edits the creature through `CCreatureEditor` and the editors owned by it instead of modifying `CCreature`, `CSkeleton`, `CAppearance`, or animations directly.

The main reason is to keep the mutation paths limited and explicit.

If mutable access is exposed in many places, it becomes harder to know where state can change and which invariants need to be considered for each change.

By routing edits through editor APIs, mutations are easier to trace, validate, and extend without having to inspect arbitrary callers.

This also gives one place to handle edits that affect related data.

For example, removing a part may require cleanup in the skeleton, appearance data, and animation tracks.

Panels should mainly deal with UI and user input.

They can inspect the creature, but mutations should go through an editor API.

This also keeps editor-specific behavior out of the runtime side, which only needs to consume creature data.