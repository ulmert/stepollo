# Stepollo
A **MIDI based (step) sequencer** sketch for the **Midiboy** (Arduino) platform. Designed to run within 2 kB of RAM, number of tracks and steps are limited (`N_TRACKS` and `SEQUENCE_LENGTH` can be configured in pairs of 3/64, 6/32, 9/24, 12/16 at build time).
Dynamic step length, loops and condition based triggers are some of the properties available to apply varitation to sequences.  

## Input

### Playback

Key(s) | Action
------------ | -------------
A | **Playback** start/stop

### Edit modes

Key(s) | Action
------------ | -------------
A + B | Toggle between **grid/step** and **track/song** mode

### Grid/step mode

Key(s) | Action
------------ | -------------
U/D/L/R | Move between steps and tracks
B + R | Mute/unmute step
B + L | Set track end
B + U | Insert step
B + D | Delete step
A + B | Toggle between **grid/step** and **track/song** mode
B | Enter step edit

#### Step edit mode
Key(s) | Action
------------ | -------------
L/R | Switch between properties
U/D | Toggle value of current property
B | Exit edit mode

### Track mode
Key(s) | Action
------------ | -------------
L/R | Switch between tracks
U/D | Switch between **track** and **song** mode
A + B | Toggle between **grid/step** and **track/song** mode
B | Enter edit mode

#### Track edit mode
Key(s) | Action
------------ | -------------
L/R | Switch between properties
U/D | Toggle value of current property
B | Exit edit mode

## Properties

### Step properties
Property | Description
------------ | -------------
Note | Note value (notes can also be entered using a connected MIDI keyboard)
Length (*l*) | Step length (see `LENGTH` array)
Duration (*d*) | Duration of note (see `DURATION` array)
Velocity (*v*) | Note velocity (see `VELOCITY` array)
Flag (*f*) | Hold step until flag value is set (if track is set as **master**, flag will be assigned value)
Hold (*h*) | Enable/Disable (note is active, regardless of duration, until next note is triggered)
Loop start (*s*) | Loop start 
Loop end (*e*) | Loop end/count (nested loops are not supported)

### Track properties
Property | Description
------------ | -------------
Mute | Off/On (If track is set as **master** and muted, it can still be used as transpose source)
Solo | Off/On
Time factor | Track replay speed factor
MIDI | Channel
Shift | Semitones
Transpose | Off/Use Master Track (*Master*)/MIDI-in source (*MIDI*)
Accent filter | Pattern (applies modulation to step, pattern length is set under song properties)
Accent modulation | Note volume/Double step length/Volume and step length (accented steps are always played, regardless of step filter)
Step filter | Pattern (trigger steps to be played or muted)
Step filter length | Length of pattern (current filter step is advanced for every triggered note)
Step filter rotation | Off/On (enable rotation of pattern for increased variation)
Track filter | Pattern (trigger track loop to be played or muted)
Track filter rotation | Off/On (enable rotation of pattern for increased variation)
Master reset | Off/On (Reset track playback when **master** track reach its end)
Polyphony | Number of notes which can be active at the same time (depending on note and step length)
Scale | Chromatic/Major/Minor (filter notes)
Predelay | Number of bars (before playback starts)

### Song properties
Property | Description
------------ | -------------
Tempo | BPM 
Master track | Track number (source for **tranpose**, **master reset** and **flag**)
Transpose mode | Step/Loop (change in transposition on a step basis or upon track reset)
Accent pattern length | Length of accent pattern (also applied to bar length)
Sync mode | Off/Send MIDI clock (*clk*)/Send MIDI clock + Start/Continue/Stop (*all*)
Center note | Note used for transposition
Initial flag | Flag value at start of playback
