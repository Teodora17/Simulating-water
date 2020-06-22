# Simulating-water

Tasks
By using C++/Java and OpenGL (including glut / freeglut /glfw libraries), write a small game
engine which simulates water behavior. You can start from the GameEngine presented in
Lab6.
You should use a grid (3d object) with at least 150 x 150 vertices (example in the pic below),
in order to make it realistic. A larger number of vertices will give you better results, but pay
also attention to your hardware performance.
Because we suppose that we work in a 3D space where y represents the height, we can
deform this y coordinate based on a formula:
Where:
o Ai = amplitude of the wave
o Di = wave direction (depends on x and z)
o Wi = wave frequency (w = 2π/L), where L = wave length
o φi = phase (depends on speed, φ = S x 2π/L), where S = wave speed
o t = time
o We noted with • the dot product and with x the classical product (NOT the cross
product)
This is called a directional wave. The wave direction stays constant (sent from the
main program)
We can also have circular waves, where the direction should be calculated in each vertex:
Where:
o Ci = center of the wave
o Di = wave direction (depends on x and z)
(and introduced in the same formula as above)
More info here: https://developer.nvidia.com/gpugems/GPUGems/gpugems_ch01.html
Obs: normally, because we change the vertex positions, the normal coords are also changed.
They can be calculated either with the formulas from the above link OR by approximating
them (the normal in V1(i,j) is the average of the 4 normals obtained from the 4 triangles
present in the image below; the normal of a triangle can be calculated as the cross product of
two edges).
1) (1.5p) Upload a 3D object with the proper number of vertices for the water. Add a proper
water texture to it.
2) (1p) Upload a different object with a different texture (e.g. a boat, a crate, a person etc.)
3) (2.5p) Create a directional wave (the parameters for amplitude, direction, wave etc. are
created in the main function and sent to the shader) which changes over time in order to
simulate movement. Choose the appropriate parameters to obtain a realistic wave.
4) (1.5p) Create a circular wave which changes over time and apply it on your water.
5) (0.5p) Sum the waves and apply them on your water. Add two or more waves in order to
see some different effects.
6) (1.5p) Create a light animation to simulate the day-night cycle (move it in your scene
over time OR move it when the user presses some keys). The light change should be seen
on the water’s surface.
0.5p for recalculating the normals (using the cross product mentioned above) and thus
getting a correct illumination. Partial points (0.25p) if the mathematical part is
demonstrated correctly (how do we take the positions of the 4 vertices, how do we
calculate the vectors for the cross product etc.), even if it is not working in the program.
1p for the app rendering with no errors and no crashing
IMPORTANT: In order to see DIFFERENT effects on water and on the other objects (ships,
boxes floating, light source etc.), you should create different shaders for them, based on their
behavior.
