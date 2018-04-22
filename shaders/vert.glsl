// ==================================================================
#version 330 core

layout(location=0)in vec3 position; // We explicitly state which is the vertex information (The first 3 floats are positional data, we are putting in our vector)
layout(location=1)in vec3 vertexColor; // Our second attribute which is the color.

// Do not forget this!
out vec3 theColor;

void main()
{
  // gl_Position is a special glsl variable that tells us what
  // postiion to put things in.
  // It takes in exactly 4 things.
  // Note that 'w' (the 4th dimension) should be 1.
  gl_Position = vec4(position.x, position.y - 0.75f, position.z, 1.0f);

  // Store the vertex color that we take in as what we will output
  // to the next stage in the graphics pipeline.
  theColor = vec3(1,1,1);//vertexColor;

}
// ==================================================================
