uniform float offset;
uniform float globalTime;
uniform vec3 gravity;

varying vec2 vertextUV;
varying vec3 normal;

const float spacing = 12.0;

void main()
{
    
    vec3 forceDir = vec3(sin(globalTime+position.x*0.05)*0.2,
        cos(globalTime*0.7+position.y*0.04)*0.2,
        sin(globalTime*0.7+position.y*0.04)*0.2);

    vec3 disp = gravity*forceDir;


}