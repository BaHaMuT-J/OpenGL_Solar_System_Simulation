#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <header/Sphere.h>

#include <iostream>
#include <iomanip>
#include <cmath>


// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;



///////////////////////////////////////////////////////////////////////////////
// ctor
///////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(float radius, int sectors, int stacks, bool smooth, int up, int textureInt, int textureGL, unsigned int texture) : interleavedStride(32) // only contain vertices & texture, so stride = sizeof(float)*5 = 20
{
    set(radius, sectors, stacks, smooth, up, textureInt, textureGL, texture);
}



///////////////////////////////////////////////////////////////////////////////
// setters
///////////////////////////////////////////////////////////////////////////////
void Sphere::set(float radius, int sectors, int stacks, bool smooth, int up, int textureInt, int textureGL, unsigned int texture)
{
    if (radius > 0)
        this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (stacks < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;
    this->smooth = smooth;
    this->upAxis = up;
    if (up < 1 || up > 3)
        this->upAxis = 3;
    this->textureInt = textureInt;
    this->textureGL = textureGL;
    this->texture = texture;

    buildVerticesSmooth();
}

void Sphere::setRadius(float radius)
{
    if (radius != this->radius)
        set(radius, sectorCount, stackCount, smooth, upAxis);
}

void Sphere::setSectorCount(int sectors)
{
    if (sectors != this->sectorCount)
        set(radius, sectors, stackCount, smooth, upAxis);
}

void Sphere::setStackCount(int stacks)
{
    if (stacks != this->stackCount)
        set(radius, sectorCount, stacks, smooth, upAxis);
}

void Sphere::setSmooth(bool smooth)
{
    if (this->smooth == smooth)
        return;

    this->smooth = smooth;
    buildVerticesSmooth();
}

void Sphere::setUpAxis(int up)
{
    if (this->upAxis == up || up < 1 || up > 3)
        return;

    this->upAxis = up;
}

void Sphere::setTextureInt(int textureInt)
{
    this->textureInt = textureInt;
}

void Sphere::setTextureGL(int textureGL)
{
    this->textureGL = textureGL;
}

void Sphere::setTexture(int texture)
{
    this->texture = texture;
}



///////////////////////////////////////////////////////////////////////////////
// print itself
///////////////////////////////////////////////////////////////////////////////
void Sphere::printSelf() const
{
    std::cout << "===== Sphere =====\n"
        << "        Radius: " << radius << "\n"
        << "  Sector Count: " << sectorCount << "\n"
        << "   Stack Count: " << stackCount << "\n"
        << "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
        << "       Up Axis: " << (upAxis == 1 ? "X" : (upAxis == 2 ? "Y" : "Z")) << "\n"
        << "Triangle Count: " << getTriangleCount() << "\n"
        << "   Index Count: " << getIndexCount() << "\n"
        << "  Vertex Count: " << getVertexCount() << "\n"
        << "  Normal Count: " << getNormalCount() << "\n"
        << "TexCoord Count: " << getTexCoordCount() << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void Sphere::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}



///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// I write this on my own
///////////////////////////////////////////////////////////////////////////////
void Sphere::buildVerticesSmooth()
{
    clearArrays();

    const float PI = acos(-1.0f);

    // vertex and texture for simplicity
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    // sector and stack
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // generate vertices
    for (int i = 0; i <= stackCount; ++i)
    {
        // range from pi/2 to -pi/2
        stackAngle = (PI / 2) - i * stackStep;

        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            // range from 0 to 2pi
            sectorAngle = j * sectorStep;

            // vertex position
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);
        }
    }

    unsigned int k1, k2;

    // generate indices
    for (int i = 0; i <= stackCount; ++i)
    {
        k1 = i * (sectorCount + 1); // start stack
        k2 = k1 + (sectorCount + 1); // end and start next stack

        for (int j = 0; j <= sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                addIndices(k1, k2, k1 + 1); // top triangle in case of rectangle
            }

            if (i != stackCount-1)
            {
                addIndices(k1 + 1, k2, k2 + 1); // bottom triangle in case of rectangle
            }

            // vertical lines
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);

            if (i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate vertex array
    buildInterleavedVertices();

    // change up axis from Z-axis to the given
    if (this->upAxis != 3)
        changeUpAxis(3, this->upAxis);
}



///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}


///////////////////////////////////////////////////////////////////////////////
// transform vertex/normal (x,y,z) coords
// assume from/to values are validated: 1~3 and from != to
///////////////////////////////////////////////////////////////////////////////
void Sphere::changeUpAxis(int from, int to)
{
    // initial transform matrix cols
    float tx[] = { 1.0f, 0.0f, 0.0f };    // x-axis (left)
    float ty[] = { 0.0f, 1.0f, 0.0f };    // y-axis (up)
    float tz[] = { 0.0f, 0.0f, 1.0f };    // z-axis (forward)

    // X -> Y
    if (from == 1 && to == 2)
    {
        tx[0] = 0.0f; tx[1] = 1.0f;
        ty[0] = -1.0f; ty[1] = 0.0f;
    }
    // X -> Z
    else if (from == 1 && to == 3)
    {
        tx[0] = 0.0f; tx[2] = 1.0f;
        tz[0] = -1.0f; tz[2] = 0.0f;
    }
    // Y -> X
    else if (from == 2 && to == 1)
    {
        tx[0] = 0.0f; tx[1] = -1.0f;
        ty[0] = 1.0f; ty[1] = 0.0f;
    }
    // Y -> Z
    else if (from == 2 && to == 3)
    {
        ty[1] = 0.0f; ty[2] = 1.0f;
        tz[1] = -1.0f; tz[2] = 0.0f;
    }
    //  Z -> X
    else if (from == 3 && to == 1)
    {
        tx[0] = 0.0f; tx[2] = -1.0f;
        tz[0] = 1.0f; tz[2] = 0.0f;
    }
    // Z -> Y
    else
    {
        ty[1] = 0.0f; ty[2] = -1.0f;
        tz[1] = 1.0f; tz[2] = 0.0f;
    }

    std::size_t i, j;
    std::size_t count = vertices.size();
    float vx, vy, vz;
    for (i = 0, j = 0; i < count; i += 3, j += 8)
    {
        // transform vertices
        vx = vertices[i];
        vy = vertices[i + 1];
        vz = vertices[i + 2];
        vertices[i] = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
        vertices[i + 1] = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
        vertices[i + 2] = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z

        // trnasform interleaved array
        interleavedVertices[j] = vertices[i];
        interleavedVertices[j + 1] = vertices[i + 1];
        interleavedVertices[j + 2] = vertices[i + 2];
    }
}



///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}



///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}



///////////////////////////////////////////////////////////////////////////////
// add single texture coord to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}



///////////////////////////////////////////////////////////////////////////////
// add 3 indices to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}
