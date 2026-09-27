# Why undo/redo stores creature snapshots

Undo/redo is implemented by storing snapshots of the creature state instead of defining an inverse operation for every edit.

I considered operation-based undo, but that would require each editing operation to correctly describe both its forward and reverse behavior.

That becomes harder once one user action affects more than one part of the model.

For example, removing a part can also affect:

- skeleton data
- appearance data
- animation tracks

Using snapshots keeps undo independent from the implementation details of each operation.

The important unit is not every small value change, but one user-visible edit.

For continuous edits such as dragging a slider or moving a part, the entire interaction is stored as one undo step.

Editor-only state such as selection or playback position is not part of the creature snapshot.
