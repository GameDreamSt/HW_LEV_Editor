
#include <Vectors.h>

const float Deg2Rad = (PI * 2.f) / 360.f;
const float Rad2Deg = 360.f / (PI * 2.f);

Vector2 Vector2::up(.0, 1.0),
Vector2::down(.0, -1.),
Vector2::right(1.0, .0),
Vector2::left(-1.0, .0),
Vector2::zero(0, 0),
Vector2::one(1, 1);

Vector2::Vector2() :x(0.), y(0.)
{
}

Vector2::Vector2(float X, float Y) : x(X), y(Y)
{
}

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(Vector2 foo)
{
	return Vector2(x + foo.x, y + foo.y);
}

Vector2 Vector2::operator-(Vector2 foo)
{
	return Vector2(x - foo.x, y - foo.y);
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}


Vector2 Vector2::operator*(float foo)
{
	return Vector2(x * foo, y * foo);
}


Vector2 Vector2::operator/(float foo)
{
	return Vector2(x / foo, y / foo);
}


float Vector2::Scalar(Vector2 foo)
{
	return x * foo.x + y * foo.y;
}

void Vector2::operator+=(Vector2 foo)
{
	x += foo.x;
	y += foo.y;
}

void Vector2::operator-=(Vector2 foo)
{
	x -= foo.x;
	y -= foo.y;
}

bool Vector2::operator==(Vector2 foo)
{
	return x == foo.x && y == foo.y;
}

bool Vector2::operator!=(Vector2 foo)
{
	return x != foo.x || y != foo.y;
}

Vector2 Vector2::Normalize()
{
	Vector2 normalizedVec = *this;
	float magnitude = normalizedVec.Magnitude();

	Vector2 newVec(normalizedVec.x / magnitude, normalizedVec.y / magnitude);
	return newVec;
}

Vector2 Vector2::Absolute()
{
	return Vector2(abs(x), abs(y));
}

float Vector2::VectorAngle(Vector2 foo) // IN RADIANS
{
	return acos(((*this).Scalar(foo)) / (Magnitude() * foo.Magnitude()));
}

float Vector2::Magnitude()
{
	return sqrt(x * x + y * y);
}

// ************************************
// VECTOR3

Vector3 Vector3::up(.0, 1.0, 0),
Vector3::down(.0, -1., 0),
Vector3::right(1.0, .0, 0),
Vector3::left(-1.0, .0, 0),
Vector3::forwards(0., .0, 1.),
Vector3::backwards(0., .0, -1.),
Vector3::zero(0, 0, 0),
Vector3::one(1, 1, 1);

Vector3::Vector3() : x(0.), y(0.), z(0.)
{
}

Vector3::Vector3(Vector2 vec) : x(vec.x), y(vec.y), z(0.)
{
}

Vector3::Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z)
{
}

Vector3::~Vector3()
{
}

Vector3 Vector3::operator+(Vector3 foo)
{
	return Vector3(x + foo.x, y + foo.y, z + foo.z);
}

Vector3 Vector3::operator-(Vector3 foo)
{
	return Vector3(x - foo.x, y - foo.y, z - foo.z);
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}


Vector3 Vector3::operator*(float foo)
{
	return Vector3(x * foo, y * foo, z * foo);
}


Vector3 Vector3::operator/(float foo)
{
	return Vector3(x / foo, y / foo, z / foo);
}

void Vector3::operator+=(Vector3 foo)
{
	x += foo.x;
	y += foo.y;
	z += foo.z;
}

void Vector3::operator-=(Vector3 foo)
{
	x -= foo.x;
	y -= foo.y;
	z -= foo.z;
}

bool Vector3::operator==(Vector3 foo)
{
	return x == foo.x && y == foo.y && z == foo.z;
}

bool Vector3::operator!=(Vector3 foo)
{
	return x != foo.x || y != foo.y || z != foo.z;
}

Vector3 Vector3::Normalize()
{
	Vector3 normalizedVec = *this;
	float magnitude = normalizedVec.Magnitude();

	Vector3 newVec(normalizedVec.x / magnitude, normalizedVec.y / magnitude, normalizedVec.z / magnitude);
	return newVec;
}

Vector3 Vector3::Absolute()
{
	return Vector3(abs(x), abs(y), abs(z));
}

float Vector3::VectorAngle(Vector3 foo) // IN RADIANS
{
	return acos(((*this).Scalar(foo)) / (Magnitude() * foo.Magnitude()));
}

float Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::Scalar(Vector3 foo)
{
	return x * foo.x + y * foo.y + z * foo.z;
}
