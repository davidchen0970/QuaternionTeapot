# Quaternion Rotation Visualization

This program demonstrates a 3D visualization of quaternion rotation using OpenGL. The rotation is applied to a wireframe teapot, and the user can interactively change the viewing angle and teapot position.

## Dependencies

Ensure that you have the following dependencies installed:

- OpenGL
- GLUT (OpenGL Utility Toolkit)

## Installation

To install the necessary OpenGL and GLUT libraries, follow these steps:

1. **Open Visual Studio**
   - Launch Visual Studio.

2. **Open NuGet Package Manager**
   - In the Visual Studio toolbar, click on `Tools`.

3. **Open Package Manager Console**
   - From the dropdown menu, select `NuGet Package Manager`.
   - Then choose `Package Manager Console` from the submenu.

4. **Install OpenGL and GLUT**
   - In the Package Manager Console, type the following command and press Enter:
     ```
     Install-Package nupengl.core
     ```

## Usage

Once the program is running, you can interact with the visualization using the following keyboard controls:

### Viewing Angle Adjustment
- 'W' or 'w': Move the eye position up
- 'S' or 's': Move the eye position down
- 'A' or 'a': Move the eye position left
- 'D' or 'd': Move the eye position right
- 'Q' or 'q': Move the eye position closer
- 'E' or 'e': Move the eye position farther

### Teapot Position Adjustment
- 'I' or 'i': Move the teapot upward
- 'K' or 'k': Move the teapot downward
- 'L' or 'l': Move the teapot right
- 'J' or 'j': Move the teapot left
- 'O' or 'o': Move the teapot forward
- 'U' or 'u': Move the teapot backward

### Rotation Axis Adjustment (Special Keys)
- Arrow keys: Adjust the rotation axis (up, down, left, right)
- Page Up: Increase rotation axis depth
- Page Down: Decrease rotation axis depth

## Quaternion Operations

Quaternion operations are tested and displayed in the console when the program is executed. The operations include quaternion length calculation, normalization, conjugation, and inversion.

## Notes

- The program uses a quaternion-based rotation to smoothly rotate the teapot.
- Information about the viewing angle, rotation axis, eye position, and teapot position is displayed on the screen.
