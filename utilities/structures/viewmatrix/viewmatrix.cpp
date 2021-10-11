#include "viewmatrix.h"

ViewMatrix::ViewMatrix() {}

ViewMatrix::ViewMatrix(
	vec_t m00, vec_t m01, vec_t m02, vec_t m03,
	vec_t m10, vec_t m11, vec_t m12, vec_t m13,
	vec_t m20, vec_t m21, vec_t m22, vec_t m23,
	vec_t m30, vec_t m31, vec_t m32, vec_t m33) {
	Init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}

ViewMatrix::ViewMatrix(const Matrix& matrix3x4) {
	Init(matrix3x4);
}

//-----------------------------------------------------------------------------
// Creates a matrix where the X axis = forward
// the Y axis = left, and the Z axis = up
//-----------------------------------------------------------------------------
ViewMatrix::ViewMatrix(const Vec3D& _x, const Vec3D& _y, const Vec3D& _z) {
	Init(
		_x.x, _y.x, _z.x, 0.0f,
		_x.y, _y.y, _z.y, 0.0f,
		_x.z, _y.z, _z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void ViewMatrix::Init(
	vec_t m00, vec_t m01, vec_t m02, vec_t m03,
	vec_t m10, vec_t m11, vec_t m12, vec_t m13,
	vec_t m20, vec_t m21, vec_t m22, vec_t m23,
	vec_t m30, vec_t m31, vec_t m32, vec_t m33
) {
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

//-----------------------------------------------------------------------------
// Initialize from a 3x4
//-----------------------------------------------------------------------------
void ViewMatrix::Init(const Matrix& m3x4) {
	memcpy(m, m3x4.Base(), sizeof(Matrix));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Vec3D3DMultiplyPosition treats src2 as if it's a point (adds the translation)
//-----------------------------------------------------------------------------
// NJS: src2 is passed in as a full Vec3D rather than a reference to prevent the need
// for 2 branches and a potential copy in the body.  (ie, handling the case when the src2
// reference is the same as the dst reference ).
void Vector3DMultiplyPosition(const ViewMatrix& src1, const Vec3D& src2, Vec3D& dst) {
	dst.x = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
	dst.y = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
	dst.z = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}

//-----------------------------------------------------------------------------
// Methods related to the basis Vec3Ds of the matrix
//-----------------------------------------------------------------------------

Vec3D ViewMatrix::GetForward() const {
	return Vec3D(m[0][0], m[1][0], m[2][0]);
}

Vec3D ViewMatrix::GetLeft() const {
	return Vec3D(m[0][1], m[1][1], m[2][1]);
}

Vec3D ViewMatrix::GetUp() const {
	return Vec3D(m[0][2], m[1][2], m[2][2]);
}

void ViewMatrix::SetForward(const Vec3D& forward) {
	m[0][0] = forward.x;
	m[1][0] = forward.y;
	m[2][0] = forward.z;
}

void ViewMatrix::SetLeft(const Vec3D& left) {
	m[0][1] = left.x;
	m[1][1] = left.y;
	m[2][1] = left.z;
}

void ViewMatrix::SetUp(const Vec3D& up) {
	m[0][2] = up.x;
	m[1][2] = up.y;
	m[2][2] = up.z;
}

void ViewMatrix::GetBasisVector3D(Vec3D& forward, Vec3D& left, Vec3D& up) const {
	forward.Init(m[0][0], m[1][0], m[2][0]);
	left.Init(m[0][1], m[1][1], m[2][1]);
	up.Init(m[0][2], m[1][2], m[2][2]);
}

void ViewMatrix::SetBasisVector3D(const Vec3D& forward, const Vec3D& left, const Vec3D& up) {
	SetForward(forward);
	SetLeft(left);
	SetUp(up);
}

//-----------------------------------------------------------------------------
// Methods related to the translation component of the matrix
//-----------------------------------------------------------------------------

Vec3D ViewMatrix::GetTranslation() const {
	return Vec3D(m[0][3], m[1][3], m[2][3]);
}

Vec3D& ViewMatrix::GetTranslation(Vec3D& trans) const {
	trans.x = m[0][3];
	trans.y = m[1][3];
	trans.z = m[2][3];
	return trans;
}

void ViewMatrix::SetTranslation(const Vec3D& trans) {
	m[0][3] = trans.x;
	m[1][3] = trans.y;
	m[2][3] = trans.z;
}

//-----------------------------------------------------------------------------
// appply translation to this matrix in the input space
//-----------------------------------------------------------------------------
void ViewMatrix::PreTranslate(const Vec3D& trans) {
	Vec3D tmp;
	Vector3DMultiplyPosition(*this, trans, tmp);
	m[0][3] = tmp.x;
	m[1][3] = tmp.y;
	m[2][3] = tmp.z;
}

//-----------------------------------------------------------------------------
// appply translation to this matrix in the output space
//-----------------------------------------------------------------------------
void ViewMatrix::PostTranslate(const Vec3D& trans) {
	m[0][3] += trans.x;
	m[1][3] += trans.y;
	m[2][3] += trans.z;
}

const Matrix& ViewMatrix::AsMatrix() const {
	return *((const Matrix*)this);
}

Matrix& ViewMatrix::AsMatrix() {
	return *((Matrix*)this);
}

void ViewMatrix::CopyFromMatrix(const Matrix& m3x4) {
	memcpy(m, m3x4.Base(), sizeof(Matrix));
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;
}

void ViewMatrix::SetMatrix(Matrix& m3x4) const {
	memcpy(m3x4.Base(), m, sizeof(Matrix));
}

//-----------------------------------------------------------------------------
// Matrix math operations
//-----------------------------------------------------------------------------
const ViewMatrix& ViewMatrix::operator+=(const ViewMatrix& other) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] += other.m[i][j];
		}
	}
	return *this;
}

ViewMatrix& ViewMatrix::operator=(const ViewMatrix& other) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = other.m[i][j];
		}
	}
	return *this;
}

ViewMatrix ViewMatrix::operator+(const ViewMatrix& other) const {
	ViewMatrix ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
	}
	return ret;
}

ViewMatrix ViewMatrix::operator-(const ViewMatrix& other) const {
	ViewMatrix ret;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret.m[i][j] = m[i][j] - other.m[i][j];
		}
	}

	return ret;
}

ViewMatrix ViewMatrix::operator-() const {
	ViewMatrix ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = -((float*)m)[i];
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Vec3D transformation
//-----------------------------------------------------------------------------

Vec3D ViewMatrix::operator*(const Vec3D& vec) const {
	Vec3D ret;
	ret.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
	ret.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
	ret.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
	return ret;
}

Vec3D ViewMatrix::Vector3DMultiply(const Vec3D& vec) const {
	Vec3D result;
	Vector3DMultiplyPosition(*this, vec, result);
	return result;
}

Vec3D ViewMatrix::Vector3DTranspose(const Vec3D& vec) const {
	Vec3D tmp = vec;
	tmp.x -= m[0][3];
	tmp.y -= m[1][3];
	tmp.z -= m[2][3];

	return Vec3D(
		m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
		m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
		m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
	);
}

Vec3D ViewMatrix::Vector3DMultiplyUpper(const Vec3D& vec) const {
	return Vec3D(
		m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z,
		m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z,
		m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z
	);
}

Vec3D ViewMatrix::Vector3DTransposeRotation(const Vec3D& vec) const {
	return Vec3D(
		m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z,
		m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z,
		m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z
	);
}

void ViewMatrix::Vector3DMultiply(const Vec3D& in, Vec3D& out) const {
	vec_t rw;

	rw = 1.0f / (m[3][0] * in.x + m[3][1] * in.y + m[3][2] * in.z + m[3][3]);
	out.x = (m[0][0] * in.x + m[0][1] * in.y + m[0][2] * in.z + m[0][3]) * rw;
	out.y = (m[1][0] * in.x + m[1][1] * in.y + m[1][2] * in.z + m[1][3]) * rw;
	out.z = (m[2][0] * in.x + m[2][1] * in.y + m[2][2] * in.z + m[2][3]) * rw;
}

//-----------------------------------------------------------------------------
// Other random stuff
//-----------------------------------------------------------------------------
void ViewMatrix::Identity() {
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

bool ViewMatrix::IsIdentity() const {
	return
		m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

Vec3D ViewMatrix::ApplyRotation(const Vec3D& vec) const {
	return Vector3DMultiply(vec);
}