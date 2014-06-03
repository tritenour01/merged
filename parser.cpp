#include "parser.h"
#include "raytracer.h"

Parser::Parser(Raytracer* r)
{
    raytracer = r;
}

//load a scene from the given file
bool Parser::loadScene(string file, Config& c)
{
    ifstream scene(file.c_str());
    if(!scene.good()){
        cout<<"Error: unable to open file: "<<file<<endl;
        return false;
    }

    string option;
    bool retValue = true;
    Shape* current = NULL;
    for(;;)
    {
        scene>>option;
        if(scene.eof())
            break;

        //set the width
        if(option == "width"){
            int newWidth;
            scene>>newWidth;
            c.width = newWidth;
        }
        //set the height
        else if(option == "height"){
            int newHeight;
            scene>>newHeight;
            c.height = newHeight;
        }
        //set the ambient value
        else if(option == "ambient"){
            float newAmbient;
            scene>>newAmbient;
            c.ambient = newAmbient;
        }
        //set the recursive depth
        else if(option == "depth"){
            int newDepth;
            scene>>newDepth;
            c.reflectionDepth = newDepth;
        }
        else if(option == "uniform"){
            int sampling;
            scene>>sampling;

            c.sampler = new uniformSampler(raytracer, c, sampling);
        }
        else if(option == "adaptive"){
            int sampling;
            scene>>sampling;

            Vector3 threshold;
            scene>>threshold.x;
            scene>>threshold.y;
            scene>>threshold.z;

            c.sampler = new adaptiveSampler(raytracer, c, sampling, threshold);
        }
        //set the background color
        else if(option == "background"){
            Vector3 newBackground;
            scene>>newBackground.x;
            scene>>newBackground.y;
            scene>>newBackground.z;
            c.backColor = newBackground;
        }
        //set the camera
        else if(option == "camera"){
            Vector3 pos;
            scene>>pos.x;
            scene>>pos.y;
            scene>>pos.z;

            Vector3 lookat;
            scene>>lookat.x;
            scene>>lookat.y;
            scene>>lookat.z;

            Vector3 up;
            scene>>up.x;
            scene>>up.y;
            scene>>up.z;

            c.camera = new Camera(pos, lookat, up, c.width, c.height);
        }
        //add a sphere to the scene
        else if(option == "sphere"){
            Vector3 center;
            float radius;

            scene>>center.x;
            scene>>center.y;
            scene>>center.z;
            scene>>radius;

            current = new Sphere(center, radius);
            raytracer->addObject(current);
        }
        //add a plane to the scene
        else if(option == "plane"){
            Vector3 center;
            Vector3 right;
            Vector3 up;

            scene>>center.x;
            scene>>center.y;
            scene>>center.z;

            scene>>right.x;
            scene>>right.y;
            scene>>right.z;

            scene>>up.x;
            scene>>up.y;
            scene>>up.z;

            //current = new Plane(center, right, 2.0f, up, 2.0f);
            current = new Plane(center, right, up);
            raytracer->addObject(current);
        }
        //add a triangle to the scene
        else if(option == "triangle"){
            float x, y, z;
            scene>>x;
            scene>>y;
            scene>>z;
            Vector3* p1 = new Vector3(x, y, z);

            scene>>x;
            scene>>y;
            scene>>z;
            Vector3* p2 = new Vector3(x, y, z);

            scene>>x;
            scene>>y;
            scene>>z;
            Vector3* p3 = new Vector3(x, y, z);

            current = new Triangle(p1, p2, p3, true);
            raytracer->addObject(current);
        }
        //add a box to the scene
        else if(option == "box"){
            Vector3 minPoint;
            Vector3 maxPoint;

            scene>>minPoint.x;
            scene>>minPoint.y;
            scene>>minPoint.z;

            scene>>maxPoint.x;
            scene>>maxPoint.y;
            scene>>maxPoint.z;

            current = new Box(minPoint, maxPoint);
            raytracer->addObject(current);
        }
        else if(option == "cylinder"){
            Vector3 base;
            float height, radius;

            scene>>base.x;
            scene>>base.y;
            scene>>base.z;

            scene>>height;
            scene>>radius;

            current = new Cylinder(base, height, radius);
            raytracer->addObject(current);
        }
        else if(option == "cone"){
            Vector3 base;
            float height, radius;

            scene>>base.x;
            scene>>base.y;
            scene>>base.z;

            scene>>height;
            scene>>radius;

            current = new Cone(base, height, radius);
            raytracer->addObject(current);
        }
        else if(option == "mesh"){
            string fileName;
            scene>>fileName;
            vector<Triangle*>* newMesh = new vector<Triangle*>;
            vector<Vector3>* newPoints = new vector<Vector3>;
            loadObj(fileName, *newMesh, *newPoints);

            current = new Mesh(newMesh, newPoints);
            raytracer->addObject(current);
        }
        //setup a material
        else if(option == "material"){
            if(!current)
                break;
            for(;;){
                scene>>option;
                if(scene.eof())
                    break;

                //set the diffuse color
                if(option == "dif"){
                    Vector3 newColor;
                    scene>>newColor.x;
                    scene>>newColor.y;
                    scene>>newColor.z;
                    current->getMaterial().setDiffuse(newColor);

                    float factor;
                    scene>>factor;
                    current->getMaterial().setDiffuseFactor(factor);
                }
                //set the spec color
                else if(option == "spec"){
                    Vector3 newColor;
                    scene>>newColor.x;
                    scene>>newColor.y;
                    scene>>newColor.z;
                    float factor;
                    scene>>factor;
                    newColor *= factor;
                    current->getMaterial().setSpecular(newColor);
                }
                //set the shineness
                else if(option == "shine"){
                    float newShine;
                    scene>>newShine;
                    current->getMaterial().setShineness(newShine);
                }
                //set the reflective coefficent
                else if(option == "reflect"){
                    float newReflective;
                    scene>>newReflective;
                    current->getMaterial().setReflective(newReflective);
                }
                //set the refractive coefficent
                else if(option == "refract"){
                    float newRefraction;
                    scene>>newRefraction;
                    current->getMaterial().setRefraction(newRefraction);
                }
                //end the setup
                else if(option == "end"){
                    break;
                }
                else{
                    retValue = false;
                    cout<<"BAD: "<<option<<endl;
                }
            }
        }
        else if(option == "transform"){
            if(!current)
                break;
            for(;;){
                scene>>option;
                if(scene.eof())
                    break;

                if(option == "trans"){
                    Vector3 factor;
                    scene>>factor.x;
                    scene>>factor.y;
                    scene>>factor.z;
                    current->Translate(factor);
                }
                else if(option == "rot"){
                    Vector3 factor;
                    scene>>factor.x;
                    scene>>factor.y;
                    scene>>factor.z;
                    current->Rotate(factor);
                }
                else if(option == "scale"){
                    Vector3 factor;
                    scene>>factor.x;
                    scene>>factor.y;
                    scene>>factor.z;
                    current->Scale(factor);
                }
                //end the setup
                else if(option == "end"){
                    break;
                }
                else{
                    retValue = false;
                    cout<<"BAD: "<<option<<endl;
                }
            }
        }
        //add a light to the scene
        else if(option == "light"){
            Vector3 newPos;
            scene>>newPos.x;
            scene>>newPos.y;
            scene>>newPos.z;

            Vector3 newColor;
            scene>>newColor.x;
            scene>>newColor.y;
            scene>>newColor.z;

            float intensity;
            scene>>intensity;

            Light* l = new Light(newPos, newColor, intensity);
            raytracer->addLight(l);
        }
        else{
            cout<<"BAD "<<option<<"\n";
            retValue = false;
        }
    }

    scene.close();
    return retValue;
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
    vector<Vector3> normals;
    vector<Vector3> texCoords;

    int nFaces = 0;

    ifstream objStream(fileName.c_str(), std::ios::in);

    if( !objStream ) {
        cerr << "Unable to open OBJ file: " << fileName << endl;
        return false;
    }

    string line, token;
    vector<int> face;
    vector<int> faceNormal;

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
                        // First face
                        Triangle* newTri = new Triangle(&points[v0], &points[v2], &points[v1], false);
                        newTri->setNormals(normals[n0], normals[n2], normals[n1]);
                        faces.push_back(newTri);
                        for( int i = 3; i < face.size(); i++ ) {
                            v1 = v2;
                            v2 = face[i];
                            n1 = n2;
                            n2 = faceNormal[i];
                            newTri = new Triangle(&points[v0], &points[v2], &points[v1], false);
                            newTri->setNormals(normals[n0], normals[n2], normals[n1]);
                            faces.push_back(newTri);
                        }
                    } else {
                        Triangle* newTri = new Triangle(&points[face[0]], &points[face[2]], &points[face[1]], false);
                        newTri->setNormals(normals[faceNormal[0]], normals[faceNormal[2]], normals[faceNormal[1]]);
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
