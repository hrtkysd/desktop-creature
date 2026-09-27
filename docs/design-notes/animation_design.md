# Why animation produces a pose

Animation data and the current pose of a creature are treated as different things.

An animation stores tracks and keyframes.

A track describes how one property of one part changes over time.

Evaluating those tracks at a specific time produces a `CCreaturePose`.

The pose represents the creature at one moment.

It does not know about:

- keyframes
- interpolation
- playback time
- animation duration

Interpolation belongs to the animation / track side.

Playback state belongs to the animation player.

The renderer only needs the resulting pose.

Keeping these separate makes it possible to change interpolation or playback behavior without changing the pose or renderer.