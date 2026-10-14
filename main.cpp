#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <conio.h>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>

#include "settings.h"
#include "player.h"
#include "timer.h"
#include "windowsiofuncs.h"
#include "object.h"

#define NOMIXMAX

using namespace Math;
using DisplayArray2d = std::array<std::array<char, Settings::display_width>, Settings::display_height>;

static constexpr DisplayArray2d initDefaultArr2d() {
	DisplayArray2d arr{};
	for (auto& row : arr) {
		for (auto& ele : row) {
			ele = Settings::default_screen_look;
		}
	}
	return arr;
}

class Screen {
public:
	struct indexPair {
		int width_index;
		int height_index;

		friend indexPair operator+(const indexPair& p1, const indexPair& p2) {
			return { p1.width_index + p2.width_index, p1.height_index + p2.height_index };
		}

		friend double distance(const indexPair& p1, const indexPair& p2) {
			return std::sqrt(std::pow(p1.width_index - p2.width_index, 2) + std::pow(p1.height_index - p2.height_index, 2));
		}
	};

	Screen(const Viewer& viewer)
		: m_viewer{ viewer }
		, m_arr2d{ default_arr2d } {
	}

	void clear() {
		m_arr2d = default_arr2d;
	}

	void print() const {
		static char buf[20000];
		std::size_t index{ 0 };
		std::ios_base::sync_with_stdio(false);
		
		for (std::size_t i{ 0 }; i < m_arr2d.size(); ++i)  {
			for (std::size_t j{ 0 }; j < m_arr2d[0].size(); ++j) {
				if (buf[index] != m_arr2d[i][j]) {
					buf[index] = m_arr2d[i][j];
					setCursorPosition(2 * j, i);
					std::cout << buf[index];
				}¡
				++index;
			}
			buf[index] = '\n';
			++index;
		}

		std::cout << std::flush;
	}

	char getLightLevelChar(const Sphere& sphere) {
		std::size_t index{ static_cast<std::size_t>(std::lround(std::floor(
			getDistance(sphere.centre, m_viewer.pos()) / Settings::distance_per_light_level))) };

		if (index < Settings::light_level_char_look.size()) {
			return Settings::light_level_char_look[index];
		}

		return 'f'; // f stands for fail
	}

	void renderNearby(const indexPair& centre_pair, const Sphere& sphere, double distance_of_each_char, char display_char, unsigned short colour) {
		int radius_in_index{ std::lround(sphere.radius / distance_of_each_char) };

		int max_display_dimension{ max(Settings::display_height, Settings::display_width) };
		if (radius_in_index > max_display_dimension) {
			radius_in_index = max_display_dimension / 2;
		}

		for (int i{ -radius_in_index }; i <= radius_in_index; ++i) {
			for (int j{ -radius_in_index }; j <= radius_in_index; ++j) {

				std::size_t width_index{ static_cast<std::size_t>(centre_pair.width_index + i) };
				std::size_t height_index{ static_cast<std::size_t>(centre_pair.height_index + j) };
				
				if ( width_index >= Settings::display_width ||
					height_index >= Settings::display_height)
					continue;

				m_arr2d[height_index][width_index] = display_char;
			}
		}
	}

	void renderObject(const Sphere& sphere, Vector3d transformed_vector) {
		char light_level_char{ getLightLevelChar(sphere) };

		if (light_level_char == 'f')
			return; // object is too far away and cannot be seen

		double proj_dist{ -(transformed_vector.z) };

		if (proj_dist <= 0)
			return; // the object is behind the viewer

		double proj_width{ 2 * Settings::vector_to_top_right.x * proj_dist };
		double proj_height{ 2 * Settings::vector_to_top_right.y * proj_dist };

		transformed_vector.x += proj_width / 2;
		transformed_vector.y += proj_height / 2;

		double width_index_double{ (transformed_vector.x / proj_width) * Settings::display_width - 0.5 };
		double height_index_double{ (transformed_vector.y / proj_height) * Settings::display_height + 0.5 };

		if (height_index_double < 0 || height_index_double >= Settings::display_height && width_index_double < 0 && width_index_double >= Settings::display_width)
			return;

		int width_index{ std::lround(width_index_double) };
		int height_index{ Settings::display_height - std::lround(height_index_double) };

		double distance_of_each_char{ proj_width / Settings::display_width };
		indexPair centre_pair{ width_index, height_index };
		renderNearby(centre_pair, sphere, distance_of_each_char, light_level_char, sphere.colour);
	}

	void renderSpheres(std::vector<const Sphere*> v) {
		std::sort(v.begin(), v.end(), 
			[&](const auto& s1, const auto& s2) {
				return getDistanceSquared(s1->centre, m_viewer.pos()) > getDistanceSquared(s2->centre, m_viewer.pos());
			});

		double ha{ m_viewer.hori_angle() };
		double va{ m_viewer.vert_angle() };

		Matrix3d around_z{
			{ std::cos(ha - pi / 2), std::sin(ha - pi / 2), 0 },
			{ -std::sin(ha - pi / 2), std::cos(ha - pi / 2), 0 },
			{ 0, 0, 1 } };

		Matrix3d around_x{
			{ 1, 0, 0 },
			{ 0, std::cos(va + pi / 2), std::sin(va + pi / 2) },
			{ 0, -std::sin(va + pi / 2), std::cos(va + pi / 2) } };

		for (const auto* point : v) {
			renderObject(*point, matrixVectorProd(around_x, matrixVectorProd(around_z, point->centre - m_viewer.pos())));
		}
	}

	bool inDisplayWindow(std::size_t width_index, std::size_t height_index) {
		return !(width_index < 0 || width_index >= Settings::display_width 
			|| height_index < 0 || height_index >= Settings::display_height);
	}

private:
	const Viewer& m_viewer;
	DisplayArray2d m_arr2d;

	static constexpr DisplayArray2d default_arr2d{ initDefaultArr2d() };
};

class Game {
public:
	Game()
		: m_player{ { 0, 0 ,0 }, 0, 0 }
		, m_screen{ m_player }
		, m_spheres{}
		, m_objects{}
		, m_delta_time{ 1.0 } {

		atStart();
	}

	void atStart() {
		fullSc();
		m_objects.emplace_back(std::make_unique<HollowCube>(HollowCube{ { 1, 0 ,0 }, 5, 0.25 }));
		m_objects.emplace_back(std::make_unique<HollowCube>(HollowCube{ { -8, 0 ,0 }, 4, 0.2 }));
		initSpheres();
	}

	void mainLoop() {
		Timer t;
		bool if_quit{ false };
		while (!if_quit) {
			ShowConsoleCursor(false);
			t.reset();

			update();
			renderAndPrint();
			printDebugInfo();

			if_quit = getAndUseInput();

			double remaining_time = Settings::frame_duration - t.elapsed();
			if (remaining_time > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(remaining_time * 1000)));
			}
			m_delta_time = t.elapsed();
		}
	}

	void renderAndPrint() {
		Timer t;
		t.reset();

		m_screen.clear();
		m_screen.renderSpheres(m_spheres);

		setCursorPosition(2, 1);
		std::cout << "Time it takes to render each frame: " << t.elapsed();

		m_screen.print();

		setCursorPosition(2, 2);
		std::cout << "Time it takes to render and draw each frame: " << t.elapsed();
	}

	bool getAndUseInput() {

		char input{};
		if (_kbhit()) {
			input = static_cast<char>(_getch());
		}

		switch (input) {
		case 'd':
			m_player.moveRight();
			break;
		case 'a':
			m_player.moveLeft();
			break;
		case 'w':
			m_player.moveForward();
			break;
		case 's':
			m_player.moveBackwards();
			break;
		case 'q':
			m_player.rotatePositiveXToPositiveY(true);
			break;
		case 'e':
			m_player.rotatePositiveXToPositiveY(false);
			break;
		case 'r':
			m_player.rotateXYtoPositiveZ(true);
			break;
		case 'f':
			m_player.rotateXYtoPositiveZ(false);
			break;
		case ' ':
			m_player.moveUp();
			break;
		case 'v':
			m_player.moveDown();
			break;
		case '1':
			return true;
		default:
			break;
		}

		return false;
	}

	void update() {
		m_objects[0]->rotateCenterX(0.1);
		m_objects[0]->rotateCenterY(0.05);
		m_objects[0]->rotateCenterZ(0.04);
	}

	void printDebugInfo() {
		m_player.printDebugInfo(2, 4);
	}

private:
	Player m_player;
	Screen m_screen;
	std::vector<const Sphere*> m_spheres;
	std::vector<std::unique_ptr<Object>> m_objects;
	double m_delta_time;

	void initSpheres() {
		for (const auto& obj : m_objects) {
			for (const auto& sphere : obj->getSpheres()) {
				m_spheres.emplace_back(&sphere);
			}
		}
	}
};

int main() {
	std::cin.tie(NULL);

	Game game;
	game.mainLoop();

	return 0;
}