# C Audio Recorder (No Third-Party Libraries)

A minimal audio recording project written in pure C, built entirely from scratch without any third-party libraries.

The goal of this project is to understand the wav file format and practice kernel-level device interaction.

---

## Overview

This project currently:

- Generates a valid `.wav` file
- Writes white noise into the file as PCM audio data

## Next: Microphone Input via Linux Kernel

The next stage is real audio capture.

Planned:

- Configure data with `ioctl`
- Open audio device (`/dev/...`)
- Poll audio buffers
- Stream captured PCM data directly into a WAV container

The goal is to interact directly with the kernel interface and capture data directly

---

## So... What's the point?
Every problem has been solved. Every truly fundamental program written. Any code that hasn't been made
can be generated at the request of a layman. Why should anyone go out of their way to choose the more difficult path? 
For me, it is the pursuit of knowlege. I want to fundamently understand the same way great people have always had too.
Not for greatness, but for my own sake, I will suffer and write C99 until I see 99.
