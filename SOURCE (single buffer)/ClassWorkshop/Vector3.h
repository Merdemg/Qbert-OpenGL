#ifndef _VECTOR_H
#define _VECTOR_H

struct CVector3
{
public:
    
    // A default constructor
    CVector3() {}

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector3(float X, float Y, float Z) 
    { 
        x = X; y = Y; z = Z;
    }

    // Here we overload the + operator so we can add vectors together 
    CVector3 operator+(CVector3 vVector)
    {
        // Return the added vectors result.
        return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
    }

    // Here we overload the - operator so we can subtract vectors 
    CVector3 operator-(CVector3 vVector)
    {
        // Return the subtracted vectors result
        return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
    }
    
    // Here we overload the * operator so we can multiply by scalars
    CVector3 operator*(float num)
    {
        // Return the scaled vector
        return CVector3(x * num, y * num, z * num);
    }

    // Here we overload the / operator so we can divide by a scalar
    CVector3 operator/(float num)
    {
        // Return the scale vector
        return CVector3(x / num, y / num, z / num);
    }

    float x, y, z;                        
};

struct CVector2
{
public:
    
    // A default constructor
    CVector2() {}

    // This is our constructor that allows us to initialize our data upon creating an instance
    CVector2(float X, float Y) 
    { 
        x = X; y = Y;
    }

    // Here we overload the + operator so we can add vectors together 
    CVector2 operator+(CVector2 vVector)
    {
        // Return the added vectors result.
        return CVector2(vVector.x + x, vVector.y + y);
    }

    // Here we overload the - operator so we can subtract vectors 
    CVector2 operator-(CVector2 vVector)
    {
        // Return the subtracted vectors result
        return CVector2(x - vVector.x, y - vVector.y);
    }
    
    // Here we overload the * operator so we can multiply by scalars
    CVector2 operator*(float num)
    {
        // Return the scaled vector
        return CVector2(x * num, y * num);
    }

    // Here we overload the / operator so we can divide by a scalar
    CVector2 operator/(float num)
    {
        // Return the scale vector
        return CVector2(x / num, y / num);
    }

    float x, y;                        
};

// vector routinen von gametuts, funktionieren nur mit 3 dimensionen

//    This returns a perpendicular vector from 2 given vectors by taking the cross product.
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

//    This returns a vector between 2 points
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

//    This returns the magnitude of a normal (or any other vector)
float Magnitude(CVector3 vNormal);

//    This returns a normalize vector (A vector exactly of length 1)
CVector3 Normalize(CVector3 vNormal);

//    This returns the normal of a polygon (The direction the polygon is facing)
CVector3 Normal(CVector3 vTriangle[]);

#endif