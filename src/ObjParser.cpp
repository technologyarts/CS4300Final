#include "SDLGraphicsProgram.h"
#include <stdlib.h>
#include <vector>
using namespace std;
void SDLGraphicsProgram::getObjDataCounts(const std::string filename, int &numVerts, int &numFaces)
{
    ifstream in(filename);
    string line;
    numVerts = 0;
    numFaces = 0;
    string prefix;
    while(getline(in, line))
    {
        switch (line.at(0))
        {
        case 'v':
            if (line.at(1) != ' ') break;
            numVerts++;
            break;
        case 'f':
            numFaces++;
            break;
        default:
            continue;
        }
    }
}

void SDLGraphicsProgram::getObjData(const std::string filename, GLfloat verts[], GLuint faces[])
{
    ifstream in(filename);
    string line;
    int vertIndex = 0;
    int faceIndex = 0;
    while(getline(in, line))
    {
	    switch (line.at(0)) {
        case 'v':
            {
                if (line.at(1) != ' ') break;
                istringstream vert(line.substr(2));
                vert >> verts[vertIndex];
                cout << verts[vertIndex] << ",";
                vertIndex++;
                vert >> verts[vertIndex];
                cout << verts[vertIndex] << ",";
                vertIndex++;
                vert >> verts[vertIndex];
                cout << verts[vertIndex] << endl;
                vertIndex++;

            }
            break;
        case 'f':
            {

                string substring = line.substr(2);
                string token = substring;
                size_t len = 0;
                do
                {
                    len = token.find("/");

                    faces[faceIndex++] = (GLuint)atof(token.substr(0, len).c_str()) - 1;
                    cout << faces[faceIndex - 1] << ",";
                    if (token.find(" ") != token.npos)
                    {
                        token = token.substr(token.find(" ") + 1);
                    }
                    else
                    {
                        break;
                    }
                } while (len != token.npos);
                cout << endl;
            }
            break;
        default:
            continue;
        }
    }

}
