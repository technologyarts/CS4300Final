import os

COMPILER="clang++"

SOURCE="./src/*.cpp"

# You can can add other arguments as you see fit.
# What does the "-D MAC" command do?
ARGUMENTS="-D MAC -std=c++14"

# Which directories do we want to include.
INCLUDE_DIR="-I./include/ -I./thirdparty/old/glm/"

# What libraries do we want to include
LIBRARIES="-F/Library/Frameworks -framework SDL2"

# The name of our executable
EXECUTABLE="FINAL"

# Build a string of our compile commands that we run in the terminal
compileString=COMPILER+" "+ARGUMENTS+" -o "+EXECUTABLE+" "+" "+INCLUDE_DIR+" "+SOURCE+" "+LIBRARIES

# Print out the compile string
print compileString

# Run our command
os.system(compileString)
