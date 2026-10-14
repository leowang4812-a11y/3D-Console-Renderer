#pragma once
#include "settings.h"
#include "windowsiofuncs.h"
using namespace Math;

class Viewer {
public:
	Viewer(const Vector3d& v, double start_hori_angle, double start_vert_angle)
		: m_pos{ v }
		, m_hori_angle{ start_hori_angle }
		, m_vert_angle{ start_vert_angle } {
	}

	const Vector3d& pos() const { return m_pos; }
	double hori_angle() const { return m_hori_angle; }
	double vert_angle() const { return m_vert_angle; }

protected:
	Vector3d m_pos;
	// in radians 
	double m_hori_angle; // start facing parallel to pos x axis, pi / 2 radians is facing parallel to pos y axis.
	double m_vert_angle; // start facing parallel to pos x axis, pi / 2 radians is facing parallel to pos z axis.
};

class Player : public Viewer {
public:
	Player(const Vector3d& v, double start_hori_angle, double start_vert_angle, double move_speed = Settings::player_move_speed,
		double hori_rotation_speed = Settings::player_hori_rotation_speed, double vert_rotation_speed = Settings::player_vert_rotation_speed)
		: Viewer{ v, start_hori_angle, start_vert_angle } 
		, m_move_speed{ move_speed } 
		, m_hori_rotation_speed{ hori_rotation_speed } 
		, m_vert_rotation_speed{ vert_rotation_speed } {
	}

	void rotatePositiveXToPositiveY(bool rotate_left) {
		if (rotate_left) {
			m_hori_angle += m_hori_rotation_speed;
		}
		else {
			m_hori_angle -= m_hori_rotation_speed;
		}

		if (m_hori_angle > 2 * pi) {
			m_hori_angle -= 2 * pi;
		} 
		else if (m_hori_angle < 0) {
			m_hori_angle += 2 * pi;
		}
	}

	void rotateXYtoPositiveZ(bool rotate_up) {
		if (rotate_up) {
			m_vert_angle += m_vert_rotation_speed;
		}
		else {
			m_vert_angle -= m_vert_rotation_speed;
		}

		if (m_vert_angle > pi / 2) {
			m_vert_angle = pi / 2;
		}
		else if (m_vert_angle < -pi / 2) {
			m_vert_angle = -pi / 2;
		}
	}

	void moveLeft() {
		m_pos.x -= m_move_speed * std::cos(pi / 2 - m_hori_angle);
		m_pos.y += m_move_speed * std::sin(pi / 2 - m_hori_angle);
	}

	void moveRight() {
		m_pos.x += m_move_speed * std::cos(pi / 2 - m_hori_angle);
		m_pos.y -= m_move_speed * std::sin(pi / 2 - m_hori_angle);
	}

	void moveForward() {
		m_pos.x += m_move_speed * std::cos(m_hori_angle);
		m_pos.y += m_move_speed * std::sin(m_hori_angle);
	}

	void moveBackwards() {
		m_pos.x -= m_move_speed * std::cos(m_hori_angle);
		m_pos.y -= m_move_speed * std::sin(m_hori_angle);

	}

	void moveUp() {
		m_pos.z += m_move_speed;
	}

	void moveDown() {
		m_pos.z -= m_move_speed;
	}

	void printDebugInfo(int x, int y) {
		setCursorPosition(x, y);
		std::cout << "Player x position: " << m_pos.x;
		setCursorPosition(x, y + 1);
		std::cout << "Player y position: " << m_pos.y;
		setCursorPosition(x, y + 2);
		std::cout << "Player z position: " << m_pos.z;
		setCursorPosition(x, y + 4);
		std::cout << "Player horizontal angle: " << m_hori_angle;
		setCursorPosition(x, y + 5);
		std::cout << "Player vertical angle: " << m_vert_angle;
	}

private:
	double m_move_speed;
	double m_hori_rotation_speed;
	double m_vert_rotation_speed;
};

