#pragma once

class Vec3D {

public:
    float x, y, z;
    Vec3D() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vec3D(float x1, float y1, float z1 = 0) {
        x = x1;
        y = y1;
        z = z1;
    }

    Vec3D(const Vec3D& vec);
    Vec3D operator+(const Vec3D& vec);
    Vec3D& operator+=(const Vec3D& vec);
    Vec3D operator-(const Vec3D& vec);
    Vec3D& operator-=(const Vec3D& vec);
    Vec3D operator*(float value);
    Vec3D& operator*=(float value);
    Vec3D operator/(float value);
    Vec3D& operator/=(float value);
    Vec3D& operator=(const Vec3D& vec);

    float& operator[](int i) {
        return ((float*)this)[i];
    }
    float operator[](int i) const {
        return ((float*)this)[i];
    }

    float DotProduct(const Vec3D& vec);
    Vec3D CrossProduct(const Vec3D& vec);
    float Magnitude();
    Vec3D Normalization();
    Vec3D Normalized(const Vec3D& vec);
    void Normalize();
    float Square();
    float Distance(const Vec3D& vec);
};