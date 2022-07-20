#include "../ynot/ynot/ynot.h"
#include <ctime>
#include <string>
#include <vector>

struct Pixel
{
public:
	int red = 0;
	int green = 0;
	int blue = 0;
	std::string str = " ";
};

std::vector<std::vector<Pixel>> init_pixels(const ynot::Coord& window_size);
void update_pixels(std::vector<std::vector<Pixel>>& pixels, const int& color_delta);
std::string get_rand_char();
ynot::Coord get_rand_coord(const ynot::Coord& window_size);
bool update_leader(ynot::Coord& leader, const std::string& direction, const ynot::Coord& window_size);

int main()
{
	ynot::save_cursor_location();
	ynot::alternate_screen_buffer();
	ynot::set_cursor_style(ynot::CursorStyle::hidden);
	srand((unsigned)time(0));
	const ynot::Coord window_size = ynot::get_window_size();
	std::vector<std::vector<Pixel>> pixels = init_pixels(window_size);
	const int color_delta = 10;
	const int update_interval_ms = 1;
	std::vector<ynot::Coord> leaders;
	const int max_leaders = 75;
	const int new_leader_interval_ms = 1;
	int new_leader_countdown_ms = new_leader_interval_ms;
	std::string direction = "down arrow";
	
	while (true)
	{
		std::string input = ynot::get_key(false);
		/*if (input == " ")
			input = ynot::get_key();*/
		if (input == "escape")
		{
			ynot::restore_screen_buffer();
			ynot::restore_cursor_location();
			ynot::set_cursor_style(ynot::CursorStyle::not_hidden);
			return 0;
		}
		/*if (input == "down arrow" || input == "up arrow" || input == "left arrow" || input == "right arrow")
			direction = input;*/
		//ynot::sleep_(update_interval_ms);
		//new_leader_countdown_ms -= update_interval_ms;
		update_pixels(pixels, color_delta);
		if (leaders.size() < max_leaders/* && new_leader_countdown_ms <= 0*/)
		{
			leaders.push_back(get_rand_coord(window_size));
			//new_leader_countdown_ms += new_leader_interval_ms;
		}
		for (int i = 0; i < leaders.size(); i++)
		{
			int x = leaders[i].x;
			int y = leaders[i].y;
			std::string r = get_rand_char();
			Pixel& pixel = pixels[y][x];
			pixel.green = 255;
			pixel.str = r;
			ynot::set_rgb(0, 255, 0);
			ynot::print_at(x, y, r);
			if (!update_leader(leaders[i], direction, window_size))
			{
				leaders.erase(leaders.begin() + i);
				i--;
			}
		}
	}
}

std::vector<std::vector<Pixel>> init_pixels(const ynot::Coord& window_size)
{
	std::vector<std::vector<Pixel>> pixels;
	pixels.resize(window_size.y + 1);
	for (int y = 0; y < pixels.size(); y++)
		pixels[y].resize(window_size.x + 1);
	return pixels;
}

void update_pixels(std::vector<std::vector<Pixel>>& pixels, const int& color_delta)
{
	for (int y = 0; y < pixels.size(); y++)
	{
		std::vector<Pixel>& row = pixels[y];
		for (int x = 0; x < row.size(); x++)
		{
			Pixel& pixel = row[x];
			if (pixel.green == 0)
				continue;
			pixel.green -= color_delta;
			if (pixel.green <= 0)
			{
				pixel.green = 0;
				pixel.str = " ";
			}
			else
				ynot::set_rgb(0, pixel.green, 0);
			ynot::print_at(x, y, pixel.str);
		}
	}
}

std::string get_rand_char()
{
	int r = rand() % 62;
	std::string s;
	if (r <= 9)
		s.push_back(char(r + '0'));
	else if (r < 36)
		s.push_back(char(r - 10 + 'a'));
	else
		s.push_back(char(r - 36 + 'A'));
	return s;
}

ynot::Coord get_rand_coord(const ynot::Coord& window_size)
{
	ynot::Coord r;
	r.x = 1 + rand() % window_size.x;
	r.y = 1 + rand() % window_size.y;
	return r;
}

bool update_leader(ynot::Coord& leader, const std::string& direction, const ynot::Coord& window_size)
{
	/*if (direction == "down arrow")
	{*/
		leader.y += 1;
		if (leader.y > window_size.y)
			return false;
	/*}
	else if (direction == "up arrow")
	{
		leader.y -= 1;
		if (leader.y < 1)
			return false;
	}
	else if (direction == "left arrow")
	{
		leader.x -= 1;
		if (leader.x < 1)
			return false;
	}
	else if (direction == "right arrow")
	{
		leader.x += 1;
		if (leader.x > window_size.x)
			return false;
	}*/
	return true;
}
