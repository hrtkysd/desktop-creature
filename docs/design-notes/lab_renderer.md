# Why Lab rendering no longer goes through ImGui

The Lab preview and the runtime should display the creature through the same rendering path.

If the Lab uses ImGui-specific texture rendering while the runtime uses the native D3D11 renderer, differences in rendering behavior can appear between the editor preview and the actual runtime.

To avoid that, the Lab renders the creature using the native D3D11 renderer into an off-screen render target.

The resulting texture is displayed in the Preview window with `ImGui::Image()`.

ImGui is still used for editor-specific elements such as:

- selection frames
- resize handles
- pivot handles
- other editor UI

The goal is for the Lab preview to represent what the runtime will actually render as closely as possible.

ImGui remains the editor UI layer, while creature rendering itself uses the same native rendering path as the runtime.