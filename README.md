# Cloth-Simulation
Physics Based Cloth Simulation
Technique
Steps involved in my implementation are listed below: 

1.Intialize the particles:
Each particle has a set of attribute: Initial position, velocity, mass, color.I have 400 particles in my mesh, loop over all particles and set initial values.
  
2.Setup a header file: SoftBodyStateData
I have a simple data structure softEdge, which stores the particles connected using the edge ie.,
Where inode, store one end of the edge, jnode has the other end, Lij stores the length of the edge. Then loop over all the particles to set the values for each of the connected pairs through the function “computedistance()”.

3.I use Leap frog integrator to update the position of the particle.
The main advantages of using leap frog integration is that, it is time reversibility, ie., you can integrate forward n steps and then reverse the direction of integration and integrate backwards n steps to arrive at the same position. Secondly, it conserves energy of the system.

4.Set of forces used on the particles are defined as structural force in the header file. Its includes:
• Spring force due to Stretch
• Spring force due to Shear
• Frictional force
• Gravitational force
All the forces are computed for every particle and each instance of time and the position of the particle is updated using the solved. Finally, the results are displayed on the output screen.

[![Watch the video](https://github.com/HameedaTaj/Cloth-Simulation/blob/master/Cloth.mp4)
