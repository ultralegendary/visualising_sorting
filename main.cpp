


#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<thread>


//enum slider_names{width=0,height,count};
class slider
{
	sf::VertexArray line;
	sf::RectangleShape box;
	sf::Font font;
	sf::Text text;
	float min, max;
public:
	slider(const char* name, sf::Vector2f(pos),float m1,float m2):line(sf::Lines,2)
	{
		font.loadFromFile("RobotoCondensed_Italic.ttf");
		text.setFont(font);
		text.setPosition(pos.x, pos.y - 50);
		text.setString(name);
		box.setSize(sf::Vector2f(10, 30));
		box.setOrigin(5, 15);
		box.setPosition(pos);
		line[0] = sf::Vertex(pos);
		line[1] = sf::Vector2f(pos.x + 100, pos.y);
		min = m1;
		max = m2;

	}
	void update(sf::Vector2f mos_pos)
	{

		box.setPosition(mos_pos.x, line[0].position.y);
		if (box.getPosition().x > line[1].position.x)
		{
			box.setPosition(line[1].position);

		}
		else if (box.getPosition().x < line[0].position.x)
		{
			box.setPosition(line[0].position);
		}
	}
	float retcount()
	{
		float x = box.getPosition().x - line[0].position.x,total=line[1].position.x-line[0].position.x;
		float percent = x / total ;
		float count = (max-min)  * percent+min;
		
		return count;
	}
	void render(sf::RenderTarget&t)
	{
		t.draw(text);
		t.draw(line);
		t.draw(box);
	}
	sf::FloatRect ret_box_bounds()
	{
		return box.getGlobalBounds();
	}

};


class bars:public sf::Drawable
{
	sf::RectangleShape bar;
	float bar_height,bar_width;
	int bar_number;
	virtual void draw(sf::RenderTarget& t,sf::RenderStates s) const{
		t.draw(bar);
	}
public:
	bars(){}
	bars(int num,sf::Vector2f pre_pos)
	{
		bar_height = static_cast<float>(num);
		bar_width = 3.f;
		bar.setFillColor(sf::Color::White);
		bar.setSize(sf::Vector2f(bar_width, 1.f * num));
		bar.setOrigin(sf::Vector2f(0.f, 1.f * num));
		bar.setPosition(pre_pos);
		bar_number = num;
		
	}
	bars(float height, float width, sf::Vector2f pos,int n)
	{
		bar_number = n;
		bar_height = height;
		bar_width = width;
		bar.setOutlineThickness(0);
		bar.setOutlineColor(sf::Color::Red);
		bar.setFillColor(sf::Color::White);
		bar.setSize(sf::Vector2f(bar_width, bar_height));
		bar.setOrigin(sf::Vector2f(0.f, bar_height));
		bar.setPosition(pos);

	}
	void update_bar_width(float w,sf::Vector2f p)
	{
		bar_width = w;
		bar.setSize(sf::Vector2f(bar_width, 1.f * bar_height));
		bar.setOrigin(sf::Vector2f(0.f, 1.f * bar_height));
		bar.setPosition(p);
	}
	void update_bar_height(float h)
	{
		bar_height = h;
		bar.setSize(sf::Vector2f(bar_width,bar_height));
		bar.setOrigin(sf::Vector2f(0.f, 1.f * bar_height));
	}
	sf::Vector2f ret_position()
	{
		return bar.getPosition();
	}
	void set_position(sf::Vector2f pos)
	{
		bar.setPosition(pos);
	}
	float retbar_height()
	{
		return bar_height;
	}
	int ret_bar_number()
	{
		return bar_number;
	}
	void set_color(sf::Color c)
	{
		bar.setFillColor(c);
	}
};


class sorting
{
	const unsigned sampels , sam_rate;
	sf::Int16 raw[44100];
	sf::SoundBuffer Buffer;
	sf::Sound sound;
	std::vector<bars> bar_object;
	int size;
	bool sort_status;
	float height,width;
	sf::Vector2f bar_pos,last_bar_pos;
	sf::Time sleep_time;
	sf::Text text;
	sf::Font font;
	
	void merge(int low, int mid, int high)//private;
	{
		int n1, n2;
		sound.play();
		n1 = mid - low + 1, n2 = high - mid;
		sf::Vector2f temp_pos;
		std::vector<bars> L, R;
		L.assign(bar_object.begin() + low, bar_object.begin() + low + n1);
		R.assign(bar_object.begin() + mid + 1, bar_object.begin() + mid + n2 + 1);

		int i = 0, j = 0, k = low;
		while ((i < n1) && (j < n2))
		{

			bar_object[k].set_color(sf::Color::Green);
			bar_object[mid + 1 + j].set_color(sf::Color::Green);
			
			sf::sleep(sleep_time);
			bar_object[mid + 1 + j].set_color(sf::Color::Red);

			if (L[i].ret_bar_number() <= R[j].ret_bar_number())
			{
				temp_pos = bar_object[k].ret_position();

				bar_object[k] = L[i++];
				sound.setPitch(width / bar_object[k].retbar_height());
				bar_object[k].set_position(temp_pos);
				bar_object[k].set_color(sf::Color::White);
				k++;

			}
			else
			{
				temp_pos = (bar_object[k].ret_position());

				bar_object[k] = R[j++];
				sound.setPitch(width / bar_object[k].retbar_height());
				bar_object[k].set_position(temp_pos);
				bar_object[k].set_color(sf::Color::White);
				k++;


			}



		}

		while (i < n1)
		{
			bar_object[k].set_color(sf::Color::Green);
			sf::sleep(sleep_time);
			temp_pos = (bar_object[k].ret_position());

			bar_object[k] = L[i++];
			sound.setPitch(width / bar_object[k].retbar_height());
			bar_object[k].set_color(sf::Color::White);
			bar_object[k++].set_position(temp_pos);

		}
		while (j < n2)
		{
			bar_object[k].set_color(sf::Color::Green);
			sf::sleep(sleep_time);
			temp_pos = (bar_object[k].ret_position());

			bar_object[k] = R[j++];
			sound.setPitch(width / bar_object[k].retbar_height());
			bar_object[k].set_color(sf::Color::White);
			bar_object[k++].set_position(temp_pos);

		}
		sound.stop();
	}

	int partition(int low, int high)
	{
		sound.play();
		int i = low - 1;
		bars poit = bar_object[high];
		bar_object[high].set_color(sf::Color::Red);
		sf::Vector2f temp;
		for (int j = low; j <= high - 1; j++)
		{
			
			bar_object[j].set_color(sf::Color::Green);
			if (i > -1)
				bar_object[i].set_color(sf::Color::Green);
			sound.setPitch(width / bar_object[j].retbar_height());
			sf::sleep(sleep_time);
			if (i > -1)
				bar_object[i].set_color(sf::Color::White);
			if (bar_object[j].ret_bar_number() <= poit.ret_bar_number())
			{

				i++;
				swap_ele(i, j);
				bar_object[i].set_color(sf::Color::White);

			}
			bar_object[j].set_color(sf::Color::White);
		}

		bar_object[high].set_color(sf::Color::White);
		bar_object[i + 1].set_color(sf::Color::White);
		std::swap(bar_object[i + 1], bar_object[high]);
		temp = bar_object[high].ret_position();
		bar_object[high].set_position(bar_object[i + 1].ret_position());
		bar_object[i + 1].set_position(temp);

		return i + 1;
	}
	void swap_ele(int a,int b)
	{

		std::swap(bar_object[a], bar_object[b]);
		sf::Vector2f temp;
		temp = bar_object[a].ret_position();
		bar_object[a].set_position(bar_object[b].ret_position());
		bar_object[b].set_position(temp);
	}
	int getmax(int n)
	{
		bars temp = bar_object[0];
		int t = bar_object[0].ret_bar_number();
		for (int i = 1; i < n; i++)
		{
			if (bar_object[i].ret_bar_number() > temp.ret_bar_number())
			{
				temp = bar_object[i];
				t = bar_object[i].ret_bar_number();
			}
		}
		
		return t;
	}
	bool count_sort(int exp)
	{
		sound.play();
		std::vector<bars> L;
		bool flag = false;
		L.assign(bar_object.begin(), bar_object.end());
		
		int i, count[10] = { 0,0,0,0,0,0,0,0,0,0 };
		for (i = 0; i < size; i++)
		{

			count[bar_object[i].ret_bar_number() / exp % 10]++;

		}
		
		for (i = 1; i < 10; i++)
		{
			count[i] += count[i - 1];
			if (bar_object[i].ret_bar_number() < bar_object[i - 1].ret_bar_number())
				flag = true;

		}
		
		for (i = size - 1; i >= 0; i--)
		{
			L[count[(bar_object[i].ret_bar_number() / exp) % 10] - 1]=bar_object[i];
			L[count[(bar_object[i].ret_bar_number() / exp) % 10]-1].set_position(sf::Vector2f(bar_pos.x+i*width,bar_pos.y));
			count[(bar_object[i].ret_bar_number() / exp) % 10]--;
		}
		
		for (i = 0; i < size; i++)
		{
			sound.setPitch(width / bar_object[i].retbar_height());
			bar_object[i].set_color(sf::Color::Green);
			sf::sleep(sleep_time);
			bar_object[i].set_color(sf::Color::White);
			bar_object[i] = L[i];
		}
		sound.stop();
		return flag;
	}

	void end_display()
	{
		sound.play();
		for (int i = 0; i < size; i++)
		{
			bar_object[i].set_color(sf::Color::Green);
			sound.setPitch(width / bar_object[i].retbar_height());
			sf::sleep(sf::seconds(0.005f));
			bar_object[i].set_color(sf::Color::Red);
		}
		sound.stop();
		for (int i = 0; i < size; i++)
		{
			bar_object[i].set_color(sf::Color::White);
		}
	}

public:
	
	sorting() :bar_pos(50, 800), size(10),width(1.f),height(1.f),sampels(44100),sam_rate(41000),sort_status(true)
	{
		sleep_time = sf::seconds(0.01f);
		sf::Vector2f temp = bar_pos;
		for (int i = 0; i < size; i++) {
			
			bar_object.push_back(bars(i+1, temp));
			temp.x += width;

		}
		last_bar_pos = temp;
		
		for (unsigned i = 0; i < sampels ; i+=2)
		{
			raw[i] = 10000;
		}
		Buffer.loadFromSamples(&raw[0], sampels, 1, 44100);
		sound.setBuffer(Buffer);
		sound.setLoop(true);
		
		
	}
	bool ret_sort_status()
	{
		return sort_status;
	}
	void update_width(float w)
	{
		width = w;
		sf::Vector2f temp = bar_pos;
		for (int i = 0; i < size; i++)
		{
			bar_object[i].update_bar_width(width,temp);
			temp.x += width;
		}
		last_bar_pos = temp;
	}
	void update_height(float h)
	{
		height = h;
		for (int i = 0; i < size; i++)
		{
			bar_object[i].update_bar_height((i+1)*height);
		}

	}
	void update_count(int n)
	{
		sf::Vector2f temp = last_bar_pos;
		for (int i = size; i < n; i++)
		{
			 
			bar_object.push_back(bars((i + 1) *height,width, temp,i+1));
			temp.x += width;
		}
		if(size>n)
		{
			temp = bar_pos;
			for (int i = 0; i < n; i++)
			{
				temp.x += width;
			}
			
			bar_object.erase(bar_object.begin() + n, bar_object.begin() + size);
		
		}
		last_bar_pos = temp;
		size = n;
	}
	void mix()
	{

		int rand_num1,rand_num2, check_loop = 0;
		sf::Vector2f temp; 

		sort_status = false;
		for (int i = 0; i < 5000; i++)
		{
			
			rand_num1 = rand() % size;
			rand_num2 = rand() % size;
			temp = bar_object[rand_num1].ret_position();
			bar_object[rand_num1].set_position(bar_object[rand_num2].ret_position());
			bar_object[rand_num2].set_position(temp);
			bars temp_bar(bar_object[rand_num1]);
			bar_object[rand_num1] = bar_object[rand_num2];
			bar_object[rand_num2] = temp_bar;
		}
		
	}
	void update_merge_sort(int low,int high)
	{
		int mid;
		
		if (low < high)
		{
				
			mid = (low + high-1) / 2;
			update_merge_sort(low, mid);
			update_merge_sort( mid + 1,high);
			merge(low, mid, high);
			
		}
		if (low == 0 && high == size - 1)
		{
			end_display();
			sort_status = true;
		}
	}
	
	void update_quick_sort(int low, int high)
	{
		if (low < high)
		{
			int ele = partition(low, high);
			update_quick_sort(low, ele - 1);
			update_quick_sort(ele + 1, high);
		}
		if (low == 0 && high == size - 1)
		{
			end_display();
			sort_status = true;
		}
	}
	void update_selection_sort()
	{
		sound.play();
		int min_index,i,j;
		for (i = 0; i < size - 1; i++)
		{

			min_index = i;
			
			for (j = i + 1; j < size; j++)
			{
				bar_object[min_index].set_color(sf::Color::Red);
				bar_object[j].set_color(sf::Color::Green);
				sound.setPitch(width / bar_object[j].retbar_height());
				sf::sleep(sleep_time);
				
				if (bar_object[j].retbar_height()<bar_object[min_index].retbar_height())
				{
					bar_object[min_index].set_color(sf::Color::White);
					min_index = j;
				}
				bar_object[j].set_color(sf::Color::White);
			}
			bar_object[min_index].set_color(sf::Color::White);
			swap_ele(min_index, i);

		}
		sound.stop();
		end_display();
		sort_status = true;
	}

	void update_radix_sort()
	{
		
		int m = getmax(size);
		bool flag=true;
		for (int exp = 1; int(m / exp) >=0 &&flag; exp *= 10)
			flag=count_sort(exp);
		end_display();
		sort_status = true;
	}

	void update_bub_sort()
	{
		sound.play();
		
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size - 1-i; j++)
			{
				bar_object[j].set_color(sf::Color::Green);
				bar_object[j+1].set_color(sf::Color::Green);
				sound.setPitch(width / bar_object[j +1 ].retbar_height());
				sf::sleep(sleep_time);
				bar_object[j].set_color(sf::Color::White);
				bar_object[j + 1].set_color(sf::Color::White);
				if (bar_object[j].retbar_height() > bar_object[j + 1].retbar_height())
				{
					swap_ele(j, j + 1);
				}
			}
		}
		sound.stop();
		end_display();
		sort_status = true;
	}

	void render(sf::RenderTarget&t)
	{
	
		t.draw(text);
		for (int i = 0; i < size; i++)
		{
			t.draw(bar_object[i]);
		}
	
	}
};

int main() {
	srand(static_cast<unsigned>(time(0)));
	sf::RenderWindow w(sf::VideoMode(1550, 850), "SORTING ALGORITHMS", sf::Style::Default);
	w.setFramerateLimit(60);
	float width, height, count=10;
	slider slide[3] = { slider("Width",sf::Vector2f(30,50),1.f,25.f),slider("Height",sf::Vector2f(150,50),1.f,25.f),slider("Count",sf::Vector2f(280,50),10.f,1000.f) };
	bool slider_selected = false, active_slider[4] = { false,false,false },started=false,thread_called=false,shuffeled=false;
	sf::Text mix; sf::Font f;
	f.loadFromFile("OpenSans_Bold.ttf");
	int sort_selected = -1;
	mix.setFont(f);
	mix.setString("SHUFFLE");
	mix.setPosition(900, 25);
	std::vector<sf::Text> sort(5);
	for (int i = 0; i < 5; i++)
	{
		sort[i].setFont(f);
		sort[i].setPosition(sf::Vector2f(i%3 * 150 + 100, (i>=3?1:2)*100));
		
		
	}
	sort[0].setString("BUBBLE");
	sort[1].setString("QUICK");
	sort[2].setString("RADIX");
	sort[3].setString("MERGE");
	sort[4].setString("SELECTION");
	sorting objects;
	sf::Vector2f mos_pos;
	std::thread thread_1;
	
	sf::Event e;
	while (w.isOpen()) {
		while (w.pollEvent(e))
		{
			mos_pos = w.mapPixelToCoords(sf::Mouse::getPosition(w));
			switch (e.type) {
			case 0:
				if (thread_called)
					thread_1.detach();
				w.close();

				break;
			case sf::Event::MouseButtonPressed:
				for (int i = 0; i < 3; i++)
					if (slide[i].ret_box_bounds().contains(mos_pos) && !started)
					{
						slide[i].update(mos_pos);
						slider_selected = true;
						active_slider[i] = true;

					}
				for (int i = 0; i < 5; i++)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sort[i].getGlobalBounds().contains(mos_pos))
					{
						started = true;
						sort_selected = i;
					}
				}
				break;
			case sf::Event::MouseButtonReleased:
				slider_selected = false;
				for (int i = 0; i < 3; i++)
					active_slider[i] = false;
				break;
			case sf::Event::MouseMoved:
				
				if (mix.getGlobalBounds().contains(mos_pos))
					mix.setFillColor(sf::Color::Red);
				else
					mix.setFillColor(sf::Color::White);
				for (int i = 0; i < 5; i++)
				{
					if (sort[i].getGlobalBounds().contains(mos_pos))
					{
						sort[i].setFillColor(sf::Color::Red);
					}
					else
						sort[i].setFillColor(sf::Color::White);
				}
				break;
			}
			if (e.type == sf::Event::MouseMoved && slider_selected&&!shuffeled)
			{

				if (active_slider[0])
				{
					slide[0].update(mos_pos);
					width = slide[0].retcount();
					objects.update_width(width);
				}
				else if (active_slider[1])
				{
					slide[1].update(mos_pos);
					height = slide[1].retcount();
					objects.update_height(height);
				}
				else if (active_slider[2])
				{
					slide[2].update(mos_pos);
					count = slide[2].retcount();
					objects.update_count(int(count));
				}

			}

		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mix.getGlobalBounds().contains(mos_pos))
		{

			objects.mix();
			thread_called = false;
			started = false;
			shuffeled = true;
		}
		if (started&&objects.ret_sort_status())
		{
			started = false;
			thread_1.detach();
			thread_called = false;
			shuffeled = false;
			sort_selected = -1;
		}
		if (started&&!thread_called)
		{
			thread_called = true;
			if (sort_selected == 0)
			{
				thread_1 = std::thread(&sorting::update_bub_sort, &objects);
			}
			else if(sort_selected==1)
			{
				thread_1 = std::thread(&sorting::update_quick_sort, &objects, 0, count - 1);
			}
			else if (sort_selected == 2)
			{
				thread_1 = std::thread(&sorting::update_radix_sort, &objects);
			}
			else if (sort_selected == 3)
			{
				thread_1 = std::thread(&sorting::update_merge_sort, &objects, 0, count - 1);
			}
			else if (sort_selected == 4)
			{
				thread_1 = std::thread(&sorting::update_selection_sort, &objects);
			}
			else
			{
				thread_called = false;
				std::cout << "Error in selecting sort"; return -1;
			}
			
			
		}
		w.clear(sf::Color::Black);
		if(!started)
			w.draw(mix);
		if (!shuffeled&&!started)
		{
			for (int i = 0; i < 3; i++)
			{
				slide[i].render(w);
			}
		}
		else if(shuffeled&&!started)
		{
			for (int i = 0; i < 5; i++)
			{
				w.draw(sort[i]);
			}
		}
		objects.render(w);
		w.display();
	}
	return 0;
}