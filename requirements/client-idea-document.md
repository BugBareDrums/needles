# Needles - an image to sound synthesizer

Needles is a VST and desktop standalone synthesizer written in the JUCE framework in C++. The UI is minimal and inspired by ableton live.

## Workflow

1. Users load image files from their computer
2. The image is displayed in the UI
3. The software converts this image into sound in different ways, depending on the parameters set by the user

## Synthesis method

1. The software traces a path across the image
2. The red, green and blue values are scaled to audio range based on a formula
3. The default path traces lines alternately right to left then left to right from the top to the bottom of the image
4. An average is taken of the red green and blue values of an area around the current position, the size of the area is taken from UI parameters
5. The default pixel to audio formula takes an average of red, green and blue and outputs to both the left and right channels
6. The scan speed is taken from the UI parameters

## UI

- User parameters are in the left half of the UI
- The image is displayed in the right half of the UI
