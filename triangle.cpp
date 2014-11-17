#include "triangle.h"
#include "raytracer.h"

Triangle::Triangle(Vector3* a, Vector3* b, Vector3* c, bool d)
{
    //set the points on the triangle
    p1 = a;
    p2 = b;
    p3 = c;

    //set the normal
    Vector3 v1 = *p2 - *p1;
    v1.normalize();

    Vector3 v2 = *p3 - *p1;
    v2.normalize();

    n = Vector3::CrossProduct(v2, v1);
    n.normalize();

    //compute the plane's distance vector
    dist = -Vector3::DotProduct(n, *p1);

    destroy = d;
    smooth = false;
}

void Triangle::setNormals(Vector3* n1, Vector3* n2, Vector3* n3)
{
    N1 = n1;
    N2 = n2;
    N3 = n3;
    smooth = true;
}

bool Triangle::Intersection(Ray& ray, Hitpoint& h)
{
    //compute the intersection time
    float newT = -(Vector3::DotProduct(ray.origin, n) + dist) / Vector3::DotProduct(ray.dir, n);

    //if t is not infinite
    if(!isinf(newT)){
        //intersection point
        Vector3 P = ray.origin + newT * ray.dir;

        //compute necessary edges
        Vector3 e1 = *p2 - *p1;
        Vector3 e2 = *p3 - *p1;
        Vector3 e3 = P - *p1;

        //compute the necessary dot products
        float d11, d12, d13, d22, d23;
        d11 = Vector3::DotProduct(e1, e1);
        d12 = Vector3::DotProduct(e1, e2);
        d13 = Vector3::DotProduct(e1, e3);
        d22 = Vector3::DotProduct(e2, e2);
        d23 = Vector3::DotProduct(e2, e3);

        //compute u and v
        float det = (d11 * d22 - d12 * d12);
        if(det == 0.0f)
            return false;

        float f = 1.0f / det;
        h.f1 = (d22 * d13 - d12 * d23) * f;

        //determine if u is invalid
        if(h.f1 < 0.0f)
            return false;

        h.f2 = (d11 * d23 - d12 * d13) * f;

        // determine if u and v are invalid
        if(h.f2 < 0.0f || h.f1 + h.f2 >= 1.0f)
            return false;

        h.t = newT;

        return true;
    }

    return false;
}

Vector3 Triangle::getPlanarNormal(void)
{
    return n;
}

//return the triangle's planar normal
Vector3 Triangle::getNormal(Ray& ray)
{
    if(smooth)
        return (1-ray.cacheFloat1-ray.cacheFloat2) * *N1 + ray.cacheFloat1 * *N2 + ray.cacheFloat2 * *N3;
    else
        return n;
}

//the shadow rays are fucking up u and v
void Triangle::getUV(Vector3& point, Ray& ray, float& U, float& V)
{
    U = (1-ray.cacheFloat1-ray.cacheFloat2) * tex1.x + ray.cacheFloat1 * tex2.x + ray.cacheFloat2 * tex3.x;
    V = (1-ray.cacheFloat1-ray.cacheFloat2) * tex1.y + ray.cacheFloat1 * tex2.y + ray.cacheFloat2 * tex3.y;
}

void Triangle::setUV(Vector3 t1, Vector3 t2, Vector3 t3)
{
    tex1 = t1;
    tex2 = t2;
    tex3 = t3;
}
