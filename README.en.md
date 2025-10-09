# Midi 2 Qwerty plus
(MIDI input to Qwerty output) for a piano connected via cable. It emulates key presses on the keyboard.
Supports 88 keys, velocity (key press strength), and playback of midi files.

## Translations
- [Русский](README.md)
- [English](README.en.md)

## Settings Description
1. `Enable QWERTY output`, if enabled, keyboard key presses will be emulated when playing notes.
2. `Enable velocity`, if enabled, key presses corresponding to the velocity (strength of the piano key press) will be emulated.
3. `MIDI input` - the device for reading piano key presses (usually this is the piano connected via cable).
4. `MIDI output` - the device for playing back the pressed keys (optional).
5. `Open MIDI file` - opens a file from the `songs` folder for playback.

## Usage
1. For playing the piano in real time:
	1. Connect the piano;
	2. Launch the program and select the piano under `MIDI input` button;
2. For playing MIDI files:
	1. Move the desired `.mid` file to the `songs` folder;
	2. In the menu, select `Open MIDI file` for that file and press `F5`.

## Used Libraries
- [**ImGUI**](https://github.com/ocornut/imgui)
- [**PortMidi**](https://github.com/PortMidi/portmidi)
- [**MidiFile**](https://github.com/craigsapp/midifile)
- [**GLFW**](https://github.com/glfw/glfw)

## Building
The source code is written for building on `Windows` using the `g++` compiler.
Other options are not provided (not tested) and are open for pull requests.