# Preview-local and screen coordinates

The Lab preview uses two different coordinate spaces.

The creature itself is rendered into an off-screen render target.

That render target has its own local coordinate system starting at `(0, 0)`.

The ImGui Preview window exists in screen coordinates.

These should not be mixed.

The basic transform chain is:

Creature / world
    -> Preview-local
    -> Screen

`CRenderPartItem` is kept in Preview-local coordinates.

The native renderer can therefore draw it directly into the preview render target without knowing anything about ImGui.

When editor UI needs the same geometry, Preview-local coordinates are transformed into screen coordinates.

This is used for:

- hit testing
- selection frames
- resize handles
- pivot handles

The preview texture can also be scaled when displayed by ImGui.

That scaling belongs to the Preview-local -> Screen transform and should not affect the creature renderer itself.

Keeping these spaces explicit avoids making the renderer depend on the position or size of an ImGui window.