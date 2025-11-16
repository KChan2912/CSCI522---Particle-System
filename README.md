## Implementing a Particle System within a Custom Engine for USC's CSCI-522

We create a particle emitter, which is responsible for spawning the particleSceneNodes, which in turn instantiate a particleMesh and bind that to themselves. 
Every Particle has a LiveTime, which when exceeded, causes the particle to despawn. 
The particle Emitter has an upper limit of the number of particles that can be spawned, as well as controls for how long to wait between particle spawns. 

The particle emitter is also responsible for calculating the next position of the particle each frame. 
There are also different forces such as turbulence and gravity that can be added. You can also bind the emitter to other SceneNodes. 

*(I bound it to the camera hoping to achieve something similar to snow, but the camera moves a bit too quickly for it to truly work without increasing the limit on particles, which tanks framerate; a better approach would include adding an offset to the already spawned particles whenever the emitter’s position is updated).*


https://github.com/user-attachments/assets/6c49f899-566d-4807-a90f-66e2bd5e181b
