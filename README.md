CS 4300 Final Project:

Team Members:
 * Samuel Vogel
 * Amanda Fode

Instructions:
 - To view this project, start up a server to host the markup, images, and javascript. We used php (and wamp on Windows), so these instructions will reflect that.
 - Open a command prompt in the root project directory.
 - run this command: "php -S localhost:8080". This will start a php server rooted in that directory.
 - Open a WebGL compatible browser (e.g. Chrome).
 - Go to the following URL: localhost/CS4300Final/
 
This is WebGL program that renders a mesh with fur that blows around. It works as follows:
 - The program generates a texture representing the parameters of the fur.
    - R is whether there is any fur for a given UV coordinate. We set this to always be 255 (always present).
    - G is the length of the fur at a given coordinate. This is randomized.
    - B is the darkness of the fur at a given coordinate. This is randomized.
    - A is the opacity of the  fur at a given coordinate. We set this to always be 255 (opaque).
 - Next, the program takes the mesh (we used a torus), and duplicates is a given number of times (this simulates multipass rendering for the object). These passes essentially represent the "resolution" of the hairs (it uses layers or "shells" to create the effect).
 - Each pass is processed as follows:
    - The vertex shader applies an offset, transforming the vertex in the direction of its normal, farther away each pass is from the original mesh.
    - A faked "force" is applied to each vertex, with greater magnitude the farther the vertex is from the original mesh. The values were taken directly from the tutorial.
    - The fragment shader colors the each fragment based on a simple texture, plus the data from the randomized texture.
    - The final effect is acheived by selectively discarding fragments such that hairs appear narrower near the outer layer and wider near the base mesh.
 - Each frame, the object is rotated some, and the time is passed to the shaders as a uniform to allow the fur to be animated.

Process:
Our process worked like this:
 - First we spent some time deciding what technology to use. We ultimately decided on WebGL, as Three.js could abstract out a large portion of the setup, allowing us to focus purely on writing the shaders and supporting code needed to create the desired effect.
 - We then looked around for existing information on creating fur shaders. We used the following resources to understand how the effect works:
    - http://www.xbdev.net/directx3dx/specialX/Fur/index.php
    - http://www.catalinzima.com/xna/tutorials/fur-rendering/
    - https://hub.jmonkeyengine.org/t/shader-based-grass-or-fur/16984
 - We then finally decided to base our implementation on http://oos.moxiecode.com/js_webgl/fur/, taking advantage of chrome's debugging tools to examine the code and figure out how to actually use three.js. A significant portion of our code is based on this project.
 - After that, we used the following two videos to learn how to set up three.js and a localhost server from which to test our code:
    - https://www.youtube.com/watch?v=YKzyhcyAijo
    - https://www.youtube.com/watch?v=WRQey_qpJls

Challenges:
There were a couple of hang-ups that we did not anticipate beforehand. We started this project not realizing that fur is typically a multi-pass effect. We had intended to use a lot of our code from previous assignments, but because we needed multiple passes, we realized that much of the setup would have to be different. This contributed to the decision to rely on three.js to handle a lot of the setup.

We also had some trouble following the example we were building off of. While it is extremely straightforward, it used an older version of three.js and some functions were deprecated or changed. Additionally, there are some slightly hidden facts about three that, while helpful, threw us off initially. The most notable of these is how three handles shader based materials. Our material is an instance of the ShaderMaterial class in three. This class a substantial amount of utility code to the shaders of materials that inherit from it (such as position and UV coordinates in vertex shaders). This meant that line numbers given by errors were wrong. It also led to naming conflicts (position is one of the names taken), which explained why the example named things the way it did.
