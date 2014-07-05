#include "parser.h"
#include "raytracer.h"

Parser::Parser(Raytracer* r)
{
    raytracer = r;
}

//load a scene from the given file
bool Parser::loadScene(string file, Config& c)
{
    SceneParser parse(raytracer, c, this);
    return parse.parseScene(file);
}

void trimString( string & str ) {
    const char * whiteSpace = " \t\n\r";
    size_t location;
    location = str.find_first_not_of(whiteSpace);
    str.erase(0,location);
    location = str.find_last_not_of(whiteSpace);
    str.erase(location + 1);
}

bool Parser::loadObj(string fileName, vector<Triangle*>& faces, vector<Vector3>& points)
{
    cout<<"loading mesh: "<<fileName<<endl;
    vector<Vector3> normals;
    vector<Vector3> texCoords;
    vector<float> texU;
    vector<float> texV;

    int nFaces = 0;

    ifstream objStream(fileName.c_str(), std::ios::in);

    if( !objStream ) {
        cerr << "Unable to open OBJ file: " << fileName << endl;
        return false;
    }

    string line, token;
    vector<int> face;
    vector<int> faceNormal;
    vector<int> faceTex;

    getline( objStream, line );
    while( !objStream.eof() ) {
        trimString(line);
        if( line.length( ) > 0 && line.at(0) != '#' ) {
            std::stringstream lineStream( line );

            lineStream >> token;

            if (token == "v" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                points.push_back( Vector3(x,y,z) );
            } else if (token == "vt") {
                // Process texture coordinate
                float s,t;
                lineStream >> s >> t;
                texCoords.push_back( Vector3(s,t,0) );
            } else if (token == "vn" ) {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.push_back( Vector3(x,y,z) );
            } else if (token == "f" ) {
                nFaces++;

                // Process face
                face.clear();
                faceNormal.clear();
                faceTex.clear();
                size_t slash1, slash2;
                //int point, texCoord, normal;
                while( lineStream.good() ) {
                    string vertString;
                    lineStream >> vertString;
                    int pIndex = -1, nIndex = -1 , tcIndex = -1;

                    slash1 = vertString.find("/");
                    if( slash1 == string::npos ){
                        pIndex = atoi( vertString.c_str() ) - 1;
                    } else {
                        slash2 = vertString.find("/", slash1 + 1 );
                        pIndex = atoi( vertString.substr(0,slash1).c_str() )
                                        - 1;
                        if( slash2 > slash1 + 1 ) {
                                tcIndex = atoi( vertString.substr(slash1 + 1, slash2).c_str() ) - 1;
                                faceTex.push_back(tcIndex);
                        }
                        nIndex = atoi( vertString.substr(slash2 + 1,vertString.length()).c_str() ) - 1;
                        if(nIndex >= 0)
                            faceNormal.push_back(nIndex);
                    }
                    if( pIndex == -1 ) {
                        cout<<"Missing point index!!!";
                    } else {
                        face.push_back(pIndex);
                    }
                }
                // If number of edges in face is greater than 3,
                // decompose into triangles as a triangle fan.
                if(faceNormal.size() == face.size()){
                    if( face.size() > 3 ) {
                        int v0 = face[0];
                        int v1 = face[1];
                        int v2 = face[2];

                        int n0 = faceNormal[0];
                        int n1 = faceNormal[1];
                        int n2 = faceNormal[2];

                        int tc0 = faceTex[0];
                        int tc1 = faceTex[1];
                        int tc2 = faceTex[2];
                        // First face
                        Triangle* newTri = new Triangle(&points[v0], &points[v2], &points[v1], false);
                        newTri->setNormals(normals[n0], normals[n2], normals[n1]);
                        if(faceTex.size() > 3)
                                newTri->setUV(texCoords[tc0], texCoords[tc2], texCoords[tc1]);
                        faces.push_back(newTri);
                        for( int i = 3; i < face.size(); i++ ) {
                            v1 = v2;
                            v2 = face[i];
                            n1 = n2;
                            n2 = faceNormal[i];
                            tc1 = tc2;
                            tc2 = faceTex[i];
                            newTri = new Triangle(&points[v0], &points[v2], &points[v1], false);
                            newTri->setNormals(normals[n0], normals[n2], normals[n1]);
                            if(faceTex.size() > 3)
                                newTri->setUV(texCoords[tc0], texCoords[tc2], texCoords[tc1]);
                            faces.push_back(newTri);
                        }
                    } else {
                        Triangle* newTri = new Triangle(&points[face[0]], &points[face[2]], &points[face[1]], false);
                        newTri->setNormals(normals[faceNormal[0]], normals[faceNormal[2]], normals[faceNormal[1]]);
                        if(faceTex.size() == 3)
                            newTri->setUV(texCoords[faceTex[0]], texCoords[faceTex[2]], texCoords[faceTex[1]]);
                        faces.push_back(newTri);
                    }
                }
                else{
                    if( face.size() > 3 ) {
                        int v0 = face[0];
                        int v1 = face[1];
                        int v2 = face[2];
                        // First face
                        faces.push_back(new Triangle(&points[v0], &points[v2], &points[v1], false));
                        for( int i = 3; i < face.size(); i++ ) {
                            v1 = v2;
                            v2 = face[i];
                            faces.push_back(new Triangle(&points[v0], &points[v2], &points[v1], false));
                        }
                    } else {
                        faces.push_back(new Triangle(&points[face[0]], &points[face[2]], &points[face[1]], false));
                    }
                }
            }
        }
        getline( objStream, line );
    }

    objStream.close();

    cout << "Loaded mesh from: " << fileName << endl;
    cout << " " << points.size() << " points" << endl;
    cout << " " << nFaces << " faces" << endl;
    cout << " " << faces.size()<< " triangles." << endl;
    cout << " " << normals.size() << " normals" << endl;
    cout << " " << texCoords.size() << " texture coordinates." << endl;

    return true;
}
