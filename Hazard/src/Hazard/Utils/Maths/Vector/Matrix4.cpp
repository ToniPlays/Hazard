#pragma once
#include <hzrpch.h>
#include "Hazard/Utils/Timer.h"
#include "Matrix4.h"
#include "glm/gtc/matrix_transform.hpp"

#define SET(col, row) [row * 4 + col]

namespace Hazard {

    Matrix4::Matrix4()
    {
    
    }
    Matrix4::Matrix4(float init)
    {
        elements[0 * 4 + 0] = init;
        elements[1 * 4 + 1] = init;
        elements[2 * 4 + 2] = init;
        elements[3 * 4 + 3] = init;
    }
    void Matrix4::Fill(float value)
    {
        for (uint8_t i = 0; i < 16; i++) {
            elements[i] = value;
        }
    }
    void Matrix4::Set(int column, int row, float value) {
        elements[row * 4 + column] = value;
    }
    Matrix4 Matrix4::InitPosition(Vector3<float> pos)
    {
        Matrix4 mat(1.0f);
        mat.elements[3 * 4 + 0] = pos.x;
        mat.elements[3 * 4 + 1] = pos.y;
        mat.elements[3 * 4 + 2] = pos.z;
        return mat;
    }
    Matrix4 Matrix4::InitRotation(Vector3<float> rotation)
    {
        Matrix4 x(1.0f);
        if (rotation.x != 0) {       
            float xRad = Math::ToRadians(rotation.x);
            //Row 2
            x.elements[1 * 4 + 1] = Math::Cos(xRad);
            x.elements[2 * 4 + 1] = -Math::Sin(xRad);
            //Row 3
            x.elements[1 * 4 + 2] = Math::Sin(xRad);
            x.elements[2 * 4 + 2] = Math::Cos(xRad);
        }
        Matrix4 y(1.0f);
        if (rotation.y != 0) {
            float yRad = Math::ToRadians(rotation.y);
            //Row 1
            y.elements[0 * 4 + 0] = Math::Cos(yRad);
            y.elements[2 * 4 + 0] = Math::Sin(yRad);
            //Row 3
            y.elements[0 * 2 + 2] = -Math::Sin(yRad);
            y.elements[2 * 4 + 2] = Math::Cos(yRad);
            x = x * y;
        }
        Matrix4 z(1.0f);
        if (rotation.z != 0) {
            float zRad = Math::ToRadians(rotation.z);
            //Row 1
            z.elements[0 * 4 + 0] = Math::Cos(zRad);
            z.elements[1 * 4 + 0] = -Math::Sin(zRad);
            //Row 2
            z.elements[0 * 4 + 1] = Math::Sin(zRad);
            z.elements[1 * 4 + 1] = Math::Cos(zRad);
            x = x * z;
        }
        return x;
    }
    Matrix4 Matrix4::InitScale(Vector3<float> scale)
    {
        Matrix4 s(1.0f);
        s.elements[0 * 4 + 0] = scale.x;
        s.elements[1 * 4 + 1] = scale.y;
        s.elements[2 * 4  + 2] = scale.z;
        return s;
    }
    Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        Matrix4 matrix;
        //Row 1
        matrix.elements[0 * 4 + 0] = 2.0f / (right - left);
        matrix.elements[3 * 4 + 0] = -(right + left) / (right - left);
        //Row 2
        matrix.elements[1 * 4 + 1] = 2.0f / (top - bottom);
        matrix.elements[3 * 4 + 1] = -(top + bottom) / (top - bottom);
        //Row 3
        matrix.elements[2 * 4 + 2] = -2.0f / (zFar - zNear);
        matrix.elements[3 * 4 + 2] =-(zFar + zNear) / (zFar - zNear);
        //Row 4
        matrix.elements[3 * 4 + 3] = 1.0f;
        return matrix;
    }
    Matrix4 Matrix4::Perspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        Matrix4 mat(1.0f);
        float tanHalfFov = Math::Tan(fov / 2.0f);
        float zRange = zNear - zFar;
        //Row 1
        mat.elements[0 * 4 + 0] = 1.0f / (tanHalfFov * aspectRatio);
        //Row 2
        mat.elements[1 * 4 + 1] = 1.0f / tanHalfFov;
        //Row 3
        mat.elements[2 * 4 + 2] = -(-zNear - zFar) / zRange;
        mat.elements[2 * 4 + 3] = -1.0f;
        //Row 4
        mat.elements[3 * 4 + 2] = 2.0f * zFar * zNear / zRange;
        
        return mat;
    }
    Matrix4 Matrix4::Inverse(Matrix4 matrix)
    {
        float inv[16], det;

        inv[0] = matrix.elements[5] * matrix.elements[10] * matrix.elements[15] - matrix.elements[5] * matrix.elements[11] * matrix.elements[14] - matrix.elements[9] * matrix.elements[6] * matrix.elements[15] + matrix.elements[9] * matrix.elements[7] * matrix.elements[14] + matrix.elements[13] * matrix.elements[6] * matrix.elements[11] - matrix.elements[13] * matrix.elements[7] * matrix.elements[10];
        inv[4] = -matrix.elements[4] * matrix.elements[10] * matrix.elements[15] + matrix.elements[4] * matrix.elements[11] * matrix.elements[14] + matrix.elements[8] * matrix.elements[6] * matrix.elements[15] - matrix.elements[8] * matrix.elements[7] * matrix.elements[14] - matrix.elements[12] * matrix.elements[6] * matrix.elements[11] + matrix.elements[12] * matrix.elements[7] * matrix.elements[10];
        inv[8] = matrix.elements[4] * matrix.elements[9] * matrix.elements[15] - matrix.elements[4] * matrix.elements[11] * matrix.elements[13] - matrix.elements[8] * matrix.elements[5] * matrix.elements[15] + matrix.elements[8] * matrix.elements[7] * matrix.elements[13] + matrix.elements[12] * matrix.elements[5] * matrix.elements[11] - matrix.elements[12] * matrix.elements[7] * matrix.elements[9];
        inv[12] = -matrix.elements[4] * matrix.elements[9] * matrix.elements[14] + matrix.elements[4] * matrix.elements[10] * matrix.elements[13] + matrix.elements[8] * matrix.elements[5] * matrix.elements[14] - matrix.elements[8] * matrix.elements[6] * matrix.elements[13] - matrix.elements[12] * matrix.elements[5] * matrix.elements[10] + matrix.elements[12] * matrix.elements[6] * matrix.elements[9];
        inv[1] = -matrix.elements[1] * matrix.elements[10] * matrix.elements[15] + matrix.elements[1] * matrix.elements[11] * matrix.elements[14] + matrix.elements[9] * matrix.elements[2] * matrix.elements[15] - matrix.elements[9] * matrix.elements[3] * matrix.elements[14] - matrix.elements[13] * matrix.elements[2] * matrix.elements[11] + matrix.elements[13] * matrix.elements[3] * matrix.elements[10];
        inv[5] = matrix.elements[0] * matrix.elements[10] * matrix.elements[15] - matrix.elements[0] * matrix.elements[11] * matrix.elements[14] - matrix.elements[8] * matrix.elements[2] * matrix.elements[15] + matrix.elements[8] * matrix.elements[3] * matrix.elements[14] + matrix.elements[12] * matrix.elements[2] * matrix.elements[11] - matrix.elements[12] * matrix.elements[3] * matrix.elements[10];
        inv[9] = -matrix.elements[0] * matrix.elements[9] * matrix.elements[15] + matrix.elements[0] * matrix.elements[11] * matrix.elements[13] + matrix.elements[8] * matrix.elements[1] * matrix.elements[15] - matrix.elements[8] * matrix.elements[3] * matrix.elements[13] - matrix.elements[12] * matrix.elements[1] * matrix.elements[11] + matrix.elements[12] * matrix.elements[3] * matrix.elements[9];
        inv[13] = matrix.elements[0] * matrix.elements[9] * matrix.elements[14] - matrix.elements[0] * matrix.elements[10] * matrix.elements[13] - matrix.elements[8] * matrix.elements[1] * matrix.elements[14] + matrix.elements[8] * matrix.elements[2] * matrix.elements[13] + matrix.elements[12] * matrix.elements[1] * matrix.elements[10] - matrix.elements[12] * matrix.elements[2] * matrix.elements[9];
        inv[2] = matrix.elements[1] * matrix.elements[6] * matrix.elements[15] - matrix.elements[1] * matrix.elements[7] * matrix.elements[14] - matrix.elements[5] * matrix.elements[2] * matrix.elements[15] + matrix.elements[5] * matrix.elements[3] * matrix.elements[14] + matrix.elements[13] * matrix.elements[2] * matrix.elements[7] - matrix.elements[13] * matrix.elements[3] * matrix.elements[6];
        inv[6] = -matrix.elements[0] * matrix.elements[6] * matrix.elements[15] + matrix.elements[0] * matrix.elements[7] * matrix.elements[14] + matrix.elements[4] * matrix.elements[2] * matrix.elements[15] - matrix.elements[4] * matrix.elements[3] * matrix.elements[14] - matrix.elements[12] * matrix.elements[2] * matrix.elements[7] + matrix.elements[12] * matrix.elements[3] * matrix.elements[6];
        inv[10] = matrix.elements[0] * matrix.elements[5] * matrix.elements[15] - matrix.elements[0] * matrix.elements[7] * matrix.elements[13] - matrix.elements[4] * matrix.elements[1] * matrix.elements[15] + matrix.elements[4] * matrix.elements[3] * matrix.elements[13] + matrix.elements[12] * matrix.elements[1] * matrix.elements[7] - matrix.elements[12] * matrix.elements[3] * matrix.elements[5];
        inv[14] = -matrix.elements[0] * matrix.elements[5] * matrix.elements[14] + matrix.elements[0] * matrix.elements[6] * matrix.elements[13] + matrix.elements[4] * matrix.elements[1] * matrix.elements[14] - matrix.elements[4] * matrix.elements[2] * matrix.elements[13] - matrix.elements[12] * matrix.elements[1] * matrix.elements[6] + matrix.elements[12] * matrix.elements[2] * matrix.elements[5];
        inv[3] = -matrix.elements[1] * matrix.elements[6] * matrix.elements[11] + matrix.elements[1] * matrix.elements[7] * matrix.elements[10] + matrix.elements[5] * matrix.elements[2] * matrix.elements[11] - matrix.elements[5] * matrix.elements[3] * matrix.elements[10] - matrix.elements[9] * matrix.elements[2] * matrix.elements[7] + matrix.elements[9] * matrix.elements[3] * matrix.elements[6];
        inv[7] = matrix.elements[0] * matrix.elements[6] * matrix.elements[11] - matrix.elements[0] * matrix.elements[7] * matrix.elements[10] - matrix.elements[4] * matrix.elements[2] * matrix.elements[11] + matrix.elements[4] * matrix.elements[3] * matrix.elements[10] + matrix.elements[8] * matrix.elements[2] * matrix.elements[7] - matrix.elements[8] * matrix.elements[3] * matrix.elements[6];
        inv[11] = -matrix.elements[0] * matrix.elements[5] * matrix.elements[11] + matrix.elements[0] * matrix.elements[7] * matrix.elements[9] + matrix.elements[4] * matrix.elements[1] * matrix.elements[11] - matrix.elements[4] * matrix.elements[3] * matrix.elements[9] - matrix.elements[8] * matrix.elements[1] * matrix.elements[7] + matrix.elements[8] * matrix.elements[3] * matrix.elements[5];
        inv[15] = matrix.elements[0] * matrix.elements[5] * matrix.elements[10] - matrix.elements[0] * matrix.elements[6] * matrix.elements[9] - matrix.elements[4] * matrix.elements[1] * matrix.elements[10] + matrix.elements[4] * matrix.elements[2] * matrix.elements[9] + matrix.elements[8] * matrix.elements[1] * matrix.elements[6] - matrix.elements[8] * matrix.elements[2] * matrix.elements[5];

        det = matrix.elements[0] * inv[0] + matrix.elements[1] * inv[4] + matrix.elements[2] * inv[8] + matrix.elements[3] * inv[12];
        if (det == 0) return false;
        det = 1.0 / det;
        int i;
        Matrix4 mat;
        for (i = 0; i < 16; i++)
            mat.elements[i] = inv[i] * det;
        return mat;
    }
    Matrix4 Matrix4::Multiply(const Matrix4& f, const Matrix4& s)
    {
        Matrix4 mat;
        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                float sums[4];
                for (uint8_t k = 0; k < 4; k++) {
                   sums[k] = f.elements[k * 4 + i] * s.elements[j * 4 + k];
                }
                float res = 0;
                for (uint8_t x = 0; x < 4; x++) {
                    res += sums[i];
                }
                mat.elements[j * 4 + i] = res;
            }
        }
        
        return f;
    }
    Matrix4 Matrix4::Multiply(const Matrix4& f, const Vector3<float>& v)
    {
        Matrix4 result;
        for (uint8_t i = 0; i < 4; i++) {
            result.elements[i * 4 + 0] = f.elements[i * 4 + 0] * v.x;
            result.elements[i * 4 + 1] = f.elements[i * 4 + 1] * v.y;
            result.elements[i * 4 + 2] = f.elements[i * 4 + 2] * v.z;
            result.elements[i * 4 + 3] = f.elements[i * 4 + 3];
        }
        return result;
    }
    Vector3<float> Matrix4::MultiplyByVector(const Matrix4& f, const Vector3<float>& v)
    {
        return Vector3<float>(f.elements[0 * 4 + 0] * v.x, f.elements[1 * 4 + 1] * v.y, f.elements[1 * 4 + 1] * v.z);
    }
}