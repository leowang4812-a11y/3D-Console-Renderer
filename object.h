#pragma once
#include <iostream>
#include <vector>

#include "settings.h"

using namespace Math;

class Object {
public:
	Object(unsigned short colour)
		: m_spheres{}
		, m_centre_pos{} 
		, m_colour{ colour } {
	}

	const std::vector<Sphere>& getSpheres() const { return m_spheres; }
	virtual void constructObject() = 0;

	void calculateCentre() {
		Vector3d calc_centre{ 0, 0, 0 };

		int count{ 0 };
		for (const auto& sphere : m_spheres) {
			calc_centre += sphere.centre;
			++count;
		}
		m_centre_pos = calc_centre / count;
	}

	void init() {
		constructObject();
		calculateCentre();
	}

	void rotateAroundX(const Vector3d& centre_of_rotation, double rotation_speed_around_x) {
		static double prev_rotation_speed_around_x{ rotation_speed_around_x };

		static Matrix3d around_x{
			{ 1, 0, 0 },
			{ 0, std::cos(rotation_speed_around_x), std::sin(rotation_speed_around_x) },
			{ 0, -std::sin(rotation_speed_around_x), std::cos(rotation_speed_around_x) } };

		if (rotation_speed_around_x != prev_rotation_speed_around_x) {
			around_x = {
				{ 1, 0, 0 },
				{ 0, std::cos(rotation_speed_around_x), std::sin(rotation_speed_around_x) },
				{ 0, -std::sin(rotation_speed_around_x), std::cos(rotation_speed_around_x) } };

			prev_rotation_speed_around_x = rotation_speed_around_x;
		}

		for (auto& sphere : m_spheres) {
			sphere.centre -= centre_of_rotation;

			sphere.centre = matrixVectorProd(around_x,  sphere.centre);

			sphere.centre += centre_of_rotation;

		}
	} 

	void rotateAroundY(const Vector3d& centre_of_rotation, double rotation_speed_around_y) {
		static double prev_rotation_speed_around_y{ rotation_speed_around_y };

		static Matrix3d around_y{
			{ std::cos(rotation_speed_around_y), 0, -std::sin(rotation_speed_around_y) },
			{ 0, 1, 0 },
			{ std::sin(rotation_speed_around_y), 0, std::cos(rotation_speed_around_y) } };

		if (rotation_speed_around_y != prev_rotation_speed_around_y) {
			around_y = {
				{ std::cos(rotation_speed_around_y), 0, -std::sin(rotation_speed_around_y) },
				{ 0, 1, 0 },
				{ std::sin(rotation_speed_around_y), 0, std::cos(rotation_speed_around_y) } };

			prev_rotation_speed_around_y = rotation_speed_around_y;
		}

		for (auto& sphere : m_spheres) {
			sphere.centre -= centre_of_rotation;

			sphere.centre = matrixVectorProd(around_y, sphere.centre);

			sphere.centre += centre_of_rotation;

		}
	}

	void rotateAroundZ(const Vector3d& centre_of_rotation, double rotation_speed_around_z) {
		static double prev_rotation_speed_around_z{ rotation_speed_around_z };

		static Matrix3d around_z{
			{ std::cos(rotation_speed_around_z), std::sin(rotation_speed_around_z), 0 },
			{ -std::sin(rotation_speed_around_z), std::cos(rotation_speed_around_z), 0 },
			{ 0, 0, 1 } };

		if (rotation_speed_around_z != prev_rotation_speed_around_z) {
			around_z = {
				{ std::cos(rotation_speed_around_z), std::sin(rotation_speed_around_z), 0 },
				{ -std::sin(rotation_speed_around_z), std::cos(rotation_speed_around_z), 0 },
				{ 0, 0, 1 } };

			prev_rotation_speed_around_z = rotation_speed_around_z;
		}

		for (auto& sphere : m_spheres) {
			sphere.centre -= centre_of_rotation;

			sphere.centre = matrixVectorProd(around_z, sphere.centre);

			sphere.centre += centre_of_rotation;

		}
	}

	void rotateCenterX(double rotation_speed_around_x) {
		rotateAroundX(m_centre_pos, rotation_speed_around_x);
	}

	void rotateCenterY(double rotation_speed_around_y) {
		rotateAroundY(m_centre_pos, rotation_speed_around_y);
	}

	void rotateCenterZ(double rotation_speed_around_z) {
		rotateAroundZ(m_centre_pos, rotation_speed_around_z);
	}

	virtual ~Object() = default;

protected:
	std::vector<Sphere> m_spheres;
	Vector3d m_centre_pos;
	unsigned short m_colour;
};


class HollowCube : public Object {
public:
	HollowCube(Vector3d corner, double edge_length, double edge_radius, unsigned short colour = Colours::LIGHT_GRAY)
		: Object{ colour }
		, m_start_corner_pos{ corner }
		, m_edge_length{ edge_length }
		, m_edge_radius{ edge_radius } {

		init();
	}

private:
	void constructObject() {
		int num_circles_per_edge{ std::lround(2 * m_edge_length / m_edge_radius) };
		for (int i{ 0 }; i < num_circles_per_edge; ++i) {
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + i * m_edge_radius / 2, m_start_corner_pos.y, m_start_corner_pos.z }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + i * m_edge_radius / 2, m_start_corner_pos.y + m_edge_length, m_start_corner_pos.z }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + i * m_edge_radius / 2, m_start_corner_pos.y, m_start_corner_pos.z + m_edge_length }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + i * m_edge_radius / 2, m_start_corner_pos.y + m_edge_length, m_start_corner_pos.z + m_edge_length }, m_edge_radius });

			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x, m_start_corner_pos.y + i * m_edge_radius / 2, m_start_corner_pos.z }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + m_edge_length, m_start_corner_pos.y + i * m_edge_radius / 2, m_start_corner_pos.z }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x, m_start_corner_pos.y + i * m_edge_radius / 2, m_start_corner_pos.z + m_edge_length }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + m_edge_length, m_start_corner_pos.y + i * m_edge_radius / 2, m_start_corner_pos.z + m_edge_length }, m_edge_radius });

			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x, m_start_corner_pos.y, m_start_corner_pos.z + i * m_edge_radius / 2 }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + m_edge_length, m_start_corner_pos.y, m_start_corner_pos.z + i * m_edge_radius / 2 }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x, m_start_corner_pos.y + m_edge_length, m_start_corner_pos.z + i * m_edge_radius / 2 }, m_edge_radius });
			m_spheres.emplace_back(Sphere{ { m_start_corner_pos.x + m_edge_length, m_start_corner_pos.y + m_edge_length, m_start_corner_pos.z + i * m_edge_radius / 2 }, m_edge_radius });
		}
	}

	Vector3d m_start_corner_pos;
	double m_edge_length;
	double m_edge_radius;
};



