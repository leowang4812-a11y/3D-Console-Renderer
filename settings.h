#pragma once
#include <iostream>
#include <cmath>
#include <array>
#include <windows.h>

namespace Colours {
	constexpr unsigned short BLACK{ 0 };
	constexpr unsigned short BLUE{ 1 };
	constexpr unsigned short GREEN{ 2 };
	constexpr unsigned short CYAN{ 3 };
	constexpr unsigned short RED{ 4 };
	constexpr unsigned short MAGENTA{ 5 };
	constexpr unsigned short BROWN{ 6 };
	constexpr unsigned short LIGHT_GRAY{ 7 };
	constexpr unsigned short DARK_GRAY{ 8 };
	constexpr unsigned short LIGHT_BLUE{ 9 };
	constexpr unsigned short LIGHT_GREEN{ 10 };
	constexpr unsigned short LIGHT_CYAN{ 11 };
	constexpr unsigned short LIGHT_RED{ 12 };
	constexpr unsigned short LIGHT_MAGENTA{ 13 };
	constexpr unsigned short YELLOW{ 14 };
	constexpr unsigned short WHITE{ 15 };
}

namespace Math {
	constexpr int round(auto value) {
		return static_cast<int>(value + 0.5);
	}

	constexpr double pi{ 3.141592653589 };

	struct Vector3d {
		double x;
		double y;
		double z;

		Vector3d& operator+=(const Vector3d& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			
			return *this;
		}

		Vector3d& operator-=(const Vector3d& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}
	};

	inline Vector3d operator+(const Vector3d& v1, const Vector3d& v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	inline Vector3d operator-(const Vector3d& v1, const Vector3d& v2) {
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	inline Vector3d operator*(double scalar, const Vector3d& v1) {
		return { v1.x * scalar, v1.y * scalar, v1.z * scalar };
	}

	inline Vector3d operator*(const Vector3d& v1, double scalar) {
		return { v1.x * scalar, v1.y * scalar, v1.z * scalar };
	}

	inline Vector3d operator/(const Vector3d& v, double scalar) {
		return { v.x / scalar, v.y / scalar, v.z / scalar };
	}

	inline double dotProduct(const Vector3d& v1, const Vector3d& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline double getMagnitude(const Vector3d& v) {
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline double getDistance(const Vector3d& v1, const Vector3d& v2) {
		return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2) + std::pow(v1.z - v2.z, 2));
	}

	inline double getDistanceSquared(const Vector3d & v1, const Vector3d & v2) {
		return std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2) + std::pow(v1.z - v2.z, 2);
	}

	inline std::ostream& operator<<(std::ostream& out, const Vector3d& v) {
		out << "Vector(" << v.x << ", " << v.y << ", " << v.z << ')';
		return out;
	}

	struct Matrix3d {
		Vector3d first_column;
		Vector3d second_column;
		Vector3d third_column;
	};

	inline Vector3d matrixVectorProd(const Matrix3d& m, const Vector3d& v) {
		return {
		dotProduct(m.first_column, v),
		dotProduct(m.second_column, v),
		dotProduct(m.third_column, v) };
	}

	struct Sphere {
		Vector3d centre;
		double radius;
		unsigned short colour{ Colours::LIGHT_GRAY };
	};
}

namespace Settings {
	constexpr int screen_pixel_width{ 1920 };
	constexpr int screen_pixel_height{ 1080 };
	constexpr double screen_display_ratio{ 16.3 };
	constexpr int display_width{ Math::round(screen_pixel_width / screen_display_ratio) };
	constexpr int display_height{ Math::round(screen_pixel_height / screen_display_ratio) };
	constexpr int total_display_chars{ display_width * display_height };

	constexpr double screen_dist_divide_center_to_corner{ 2 };
	inline const Math::Vector3d vector_to_top_right{
		screen_pixel_width / (0.5 * std::sqrt(screen_pixel_width * screen_pixel_width + screen_pixel_height * screen_pixel_height)),
		screen_pixel_height / (0.5 * std::sqrt(screen_pixel_width * screen_pixel_width + screen_pixel_height * screen_pixel_height)),
		1 };

	constexpr char default_screen_look{ ' ' };
	constexpr std::array<char, 10> light_level_char_look{ '@', '$', '#', '?', '=', ';', ':', '-', ',', '.' };
	constexpr double distance_per_light_level{ 2 };

	constexpr double frame_rate{ 30.0 };
	constexpr double frame_duration{ 1 / frame_rate };

	constexpr double player_move_speed{ 0.2 };
	constexpr double player_hori_rotation_speed{ 0.17 };
	constexpr double player_vert_rotation_speed{ 0.17 };
}

