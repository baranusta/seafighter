# Sea-Fighter
It is created for a course project at TUDelft. OpenGL 4.3 is used.
You play as the boat and the goal is killing the mines before you die. 

Used techniques:
- Sea animation(shaders used)
- Dynamic Lighting & Shadow Mapping
- Procedurally generated islands

    For each run, islands are generated randomly(position on map and shape). Heightmaps are used to create islands topography then vertices and their normals are generated.
    
    ![alt text](https://github.com/baranusta/seafighter/blob/master/Images/Island-left-sun.jpg "Sun at left")

    ![alt text](https://github.com/baranusta/seafighter/blob/master/Images/Island-right-sun.jpg "Sun at right")

- Mesh simplification

    Mesh is simplified using grids in its bounding box. The bounding box divided into cells then new vertices created having the average position and normal of the real vertices in the corresponding cell.
    
    Boss normal:
    
    ![alt text](https://github.com/baranusta/seafighter/blob/master/Images/monster-normal.jpg "Non-simplified monster")

    Boss simplified: 

    ![alt text](https://github.com/baranusta/seafighter/blob/master/Images/monster-simplified.jpg "Simplified monster")