# Gemstone

## Version 1.0 Requirements

To achieve release 1.0 we need to have everything in place to construct a scene using the following functionality:
1. Load scenes from files and be able to switch between scenes (saving the state of a scene is not required)
1. Multiple objects present in the scene
    * Each with different models from different obj files
    * Each with different textures
    * Each with different materials
    * Each with different shaders applied to them
        * A cel/toon shader with a border outline must be in place
1. Multiple lighting types present in the scene
    * Ambient lights
    * Point lights
        * Able to move and alter the lighting of the objects around it
    * Directional lights
        * Able to move and alter the lighting of the objects around it
    * Object lights
        * Objects must be able to give off point source lighting and directional lighting from their meshes
        * Able to move and alter the lighting of the objects around it
1. A fixed timestep scene update loop must be enforced
    * The scene must be consistently updated at a rate which is predetermined yet variable (60 or 120 ticks per second)
1. A fixed timestep render loop must be enforced
    * The scene must be consistently rendered at a rate which is predetermined yet variable (60 or 120 frames per second)
    * This must differ from the scene update timestep
1. UI elements must be present in the scene
    * Display the active frame rate and physics update rate to verify the desired timesteps