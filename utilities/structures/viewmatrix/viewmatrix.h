#pragma once
#include "../vec3d/vec3d.h"
#include "includes.h"
#include "pch.h"

typedef float vec_t;

class ViewMatrix {
public:

	ViewMatrix();
	ViewMatrix(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	ViewMatrix(const Vec3D& forward, const Vec3D& left, const Vec3D& up);

	// Construct from a 3x4 matrix
	ViewMatrix(const Matrix& m3x4);

	// Set the values in the matrix.
	void		Init(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);

	// Initialize from a 3x4
	void		Init(const Matrix& matrix3x4);

	// array access
	inline float* operator[](int i) {
		return m[i];
	}

	inline const float* operator[](int i) const {
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float* Base() {
		return &m[0][0];
	}

	inline const float* Base() const {
		return &m[0][0];
	}

	void		SetLeft(const Vec3D& left);
	void		SetUp(const Vec3D& up);
	void		SetForward(const Vec3D& forward);

	void		GetBasisVector3D(Vec3D& forward, Vec3D& left, Vec3D& up) const;
	void		SetBasisVector3D(const Vec3D& forward, const Vec3D& left, const Vec3D& up);

	// Get/set the translation.
	Vec3D&		GetTranslation(Vec3D& trans) const;
	void		SetTranslation(const Vec3D& trans);

	void		PreTranslate(const Vec3D& tans);
	void		PostTranslate(const Vec3D& trans);

	Matrix&			AsMatrix();
	const Matrix&	AsMatrix() const;
	void			CopyFromMatrix(const Matrix& m3x4);
	void			SetMatrix(Matrix& m3x4) const;

	bool		operator==(const ViewMatrix& src) const;
	bool		operator!=(const ViewMatrix& src) const { return !(*this == src); }

	// Access the basis Vec3Ds.
	Vec3D		GetLeft() const;
	Vec3D		GetUp() const;
	Vec3D		GetForward() const;
	Vec3D		GetTranslation() const;

	// Matrix->Vec3D operations.
public:
	// Multiply by a 3D Vec3D (same as operator*).
	void		Vector3DMultiply(const Vec3D& in, Vec3D& out) const;

	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vec3D		ApplyRotation(const Vec3D& vec) const;

	// Multiply by a Vec3D (divides by w, assumes input w is 1).
	Vec3D		operator*(const Vec3D& vec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vec3D		Vector3DMultiply(const Vec3D& vec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vec3D		Vector3DTransposeRotation(const Vec3D& vec) const;

	// Multiply by the upper 3 rows.
	Vec3D		Vector3DMultiplyUpper(const Vec3D& vec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vec3D		Vector3DTranspose(const Vec3D& vec) const;

public:

	ViewMatrix& operator=(const ViewMatrix& other);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const ViewMatrix& vm, ViewMatrix& out) const;

	// Add two matrices.
	const ViewMatrix& operator+=(const ViewMatrix& other);

	// Just calls MatrixMul and returns the result.
	ViewMatrix		operator*(const ViewMatrix& other) const;

	// Add/Subtract two matrices.
	ViewMatrix		operator+(const ViewMatrix& other) const;
	ViewMatrix		operator-(const ViewMatrix& other) const;

	// Negation.
	ViewMatrix		operator-() const;

	// Matrix operations.
public:
	// Set to identity.
	void		Identity();

	bool		IsIdentity() const;

	// Setup a matrix for origin and angles.
	void		SetupMatrixOrginalAngles(const Vec3D& origin, const Vec3D& angles);

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		InverseTr(ViewMatrix& ret) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit Vec3Ds and they are all orthogonal.
	bool		IsRotationMatrix() const;

	// This calls the other InverseTR and returns the result.
	ViewMatrix		InverseTr() const;

	// Get the scale of the matrix's basis Vec3Ds.
	Vec3D			GetScale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	ViewMatrix		Scale(const Vec3D& scale);

	// Normalize the basis Vec3Ds.
	ViewMatrix		NormalizeBasisVector3D() const;

	// Transpose.
	ViewMatrix		Transpose() const;

	// Transpose upper-left 3x3.
	ViewMatrix		Transpose3x3() const;

public:
	// The matrix.
	vec_t		m[4][4];
};
