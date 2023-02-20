#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

// Симулятор жизни с помощью паттерна проектирования наблюдатель (Observer)
// by Anton

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(int text, int backgrnd) // Изменение цвета текста/фона консоли
{
	SetConsoleTextAttribute(h, (backgrnd << 4) + text);
}
void set_color(int text) // Изменение цвета текста консоли
{
	SetConsoleTextAttribute(h, (0 << 4) + text);
}

class IHuman
{
public:
	virtual ~IHuman() {};
	virtual void update() = 0;
	virtual void setId(vector <IHuman*> arr) = 0;
	virtual void kill() = 0;
	virtual int getId() = 0;
};

class ISubject
{
public:
	virtual ~ISubject() {};
	virtual void attach(IHuman* human) = 0;
	virtual void detach(IHuman* human) = 0;
	virtual void notify() = 0;
};

class Subject : public ISubject
{
	vector <IHuman*> humans;
	int day = 1;
	bool showPoinT = true;

public:
	virtual ~Subject() 
	{
		cout << "delete subject\n"; 
	}

	void attach(IHuman* human) override
	{
		humans.push_back(human);
		human->setId(humans);
	}
	void detach(IHuman* human) override { humans.erase(remove(humans.begin(), humans.end(), human), humans.end()); }

	void deleteHumans() { auto it = humans.begin(); while (it != humans.end()) (*it++)->kill(); {} }
		
	void notify() override { auto it = humans.begin(); while (it != humans.end()) { (*it++)->update(); } }

	void nextDay()
	{
		day++;
		notify();
	}

	bool canChoose()
	{
		if (!humans.empty()) return true;
		cout << "Вы ещё не создали ни одного человека!\n\n";
		return false;
	}
	bool allDeath()
	{
		auto it = humans.begin(); while (it != humans.end())
		{
			cout << "yes";
		}
	}

	int getHumanSize() { return humans.size(); }
	int getHumanId(int id) { return humans.at(id)->getId(); }
	int getDay() { return day; }
	bool showPoint() { return showPoinT; }
	void showPoint(bool value) { showPoinT = value; }
};

class Human : public IHuman
{
	Subject& subject;
	int id, days;
	int health, hungry, point, mood, fatigue, p; // ДОДЕЛАТЬ ДОБАВЛЕНО ФАТИГУЕ УСТАЛОРСТЬ
	int thirst = fatigue = 0;
	int sugar;
	bool death, critical;	

public:
	Human(Subject& sub) : subject(sub)
	{
		this->subject.attach(this);
		health = hungry = 100;
		mood = 80;
		sugar = 50;
		death = critical = false;
		days = 1;
		point = 2;
	}
	
	virtual ~Human()
	{ 
		cout << "delete human\n"; 
	}

	void checkDeath()
	{
		if (health <= 0) {
			if(death == false) cout << "Человек ID" << id << " умер!\n";
			death = true;
		}
	}

	void update() override
	{
		checkDeath();
		srand(time(NULL));

		if (!death)
		{
			days++;

			int healthMin, healthMax, hungryMin, hungryMax, sugarMin, sugarMax, moodMin, moodMax, thirstMin, thirstMax;
			int pointMin, pointMax;

			healthMin = healthMax = 0;
			hungryMin = 5; hungryMax = 20;
			sugarMin = -3; sugarMax = 0;
			moodMin = 1; moodMax = 5;
			thirstMin = 5; thirstMax = 15;
			pointMin = 2; pointMax = 2;

			if (hungry >= 125) { healthMin += 2; healthMax += 5; }
			else if (hungry >= 150) { healthMin += 5; healthMax += 10; }
			else if (hungry <= 25) { healthMin += 5; healthMax += 10; }
			else if (hungry <= 50) { healthMin += 2; healthMax += 5; }
			if (hungry <= 50) { sugarMin -= 2; }

			if (thirst >= 50) { healthMin += 2; healthMax += 5; }
			else if (thirst >= 100) { healthMin += 5; healthMax += 10; }
			else if (thirst >= 125) { healthMin += 7; healthMax += 15; }
			else if (thirst >= 150) { healthMin += 10; healthMax += 20; }
			else if (thirst >= 200) { healthMin += 20; healthMax += 40; }

			if (mood <= 20) { healthMin += 2; healthMax += 5; }

			if (sugar <= 20) { hungryMin += 5; hungryMax += 10; moodMin += 2; moodMax += 5; healthMin += 2; healthMax += 5; }
			else if (sugar >= 80) { healthMin += 10; healthMax += 20; }

			// ------------------------------------------------------------------- //

			if (healthMax > 0) { health -= healthMin + rand() % healthMax; }
			if (hungryMax > 0) { hungry -= hungryMin + rand() % hungryMax; }
			sugar += sugarMin + rand() % (sugarMax - sugarMin + 1);
			mood -= moodMin + rand() % (moodMax - moodMin + 1);
			thirst += thirstMin + rand() % (thirstMax - thirstMin + 1);

			p = pointMin + rand() % (pointMax - pointMin + 1);
			point += p;

			//hungry -= 5 + rand() % 20;
			//thirst += 5 + rand() % 15;
			//mood -= 2 + rand() % 6;


			// проверки ниже нужны чтобы значения не были меньше 0 / больше 100 (применяется не для всех)
			if (health > 100) health = 100;
			else if (health < 0) health = 0;

			if (hungry < 0) hungry = 0;

			if (mood > 100) mood = 100;
			else if (mood < 0) mood = 0;

			if (sugar > 100) sugar = 100;
			else if (sugar < 0) sugar = 0;

			if (point <= 0) point = 1;
		}		
	}

	void showStatus()
	{
		cout << "Ваши статусы:\n";

		if (health >= 60) { cout << "Здоровье: "; set_color(2, 0); cout << health; set_color(7, 0); cout << endl; }
		else if (health >= 30) { cout << "Здоровье: "; set_color(6, 0); cout << health; set_color(7, 0); cout << endl; }
		else { cout << "Здоровье: "; set_color(4, 0); cout << health; set_color(7, 0); cout << endl; }

		if (hungry >= 60) { cout << "Сытость: "; set_color(2, 0); cout << hungry; set_color(7, 0); cout << endl; }
		else if (hungry >= 30) { cout << "Сытость: "; set_color(6, 0); cout << hungry; set_color(7, 0); cout << endl; }
		else { cout << "Сытость: "; set_color(4, 0); cout << hungry; set_color(7, 0); cout << endl; }
		
		if (thirst <= 30) { cout << "Жажда: "; set_color(2, 0); cout << thirst; set_color(7, 0); cout << endl; }
		else if (thirst <= 60) { cout << "Жажда: "; set_color(6, 0); cout << thirst; set_color(7, 0); cout << endl; }
		else { cout << "Жажда: "; set_color(4, 0); cout << thirst; set_color(7, 0); cout << endl; }
	
		if (mood >= 60) { cout << "Настроение: "; set_color(2, 0); cout << mood; set_color(7, 0); cout << endl; }
		else if (mood >= 30) { cout << "Настроение: "; set_color(6, 0); cout << mood; set_color(7, 0); cout << endl; }
		else { cout << "Настроение: "; set_color(4, 0); cout << mood; set_color(7, 0); cout << endl; }
		
		if (sugar <= 40) { cout << "Сахар в крови: "; set_color(6, 0); cout << sugar; set_color(7, 0); cout << endl; }
		else if (sugar >= 60) { cout << "Сахар в крови: "; set_color(6, 0); cout << sugar; set_color(7, 0); cout << endl; }
		else if (sugar >= 75) { cout << "Сахар в крови: "; set_color(4, 0); cout << sugar; set_color(7, 0); cout << endl; }
		else if (sugar <= 25) { cout << "Сахар в крови: "; set_color(4, 0); cout << sugar; set_color(7, 0); cout << endl; }
		else { cout << "Сахар в крови: "; set_color(2, 0); cout << sugar; set_color(7, 0); cout << endl; }

		cout << "Очки взаимодействий: " << point << endl;
	}

	void eat(string value)
	{
		srand(time(NULL));

		if (value == "bread")
		{
			hungry += 2 + rand() % (7 - 2 + 1);
			thirst += 2 + rand() % (5 - 2 + 1);
			mood += 0 + rand() % (2 - 0 + 1);
			sugar += 0 + rand() % (2 - 0 + 1);
			point -= 2;
		}
		else if (value == "iceCream")
		{
			hungry += 1 + rand() % (3 - 1 + 1);
			thirst += 3 + rand() % (6 - 3 + 1);
			sugar += 1 + rand() % (2 - 1 + 1);
			mood += 1 + rand() % (2 - 1 + 1);
			point -= 2;
		}
		else if (value == "soup")
		{
			thirst -= 1 + rand() % (3 - 1 + 1);
			hungry += 6 + rand() % (10 - 6 + 1);
			mood += 0 + rand() % (1 - 0 + 1);
			point -= 3;
		}
		else if (value == "candy")
		{
			hungry += 1 + rand() % (2 - 1 + 1);
			sugar += 2 + rand() % (3 - 2 + 1);
			mood += 1 + rand() % (2 - 1 + 1);
			point -= 3;
		}
		else if (value == "heal")
		{
			health += 10 + rand() % (25 - 10 + 1);
			point -= 10;
		}
	}
	void fun(string value)
	{

	}

	void kill() { delete this; }

	void removeThis() { subject.detach(this); }

	void setId(vector<IHuman*> arr) { id = arr.size(); }
	int getId() { return id; }
	int getPoint() { return point; }
	int getP() { return p; }
	bool humanDeath() { return death; }
};

void input(string& txt) // Эти функции я создал с целью оптимизации кода. Вместо строчек cin и cls получается всего лишь одна input
{
	cin >> txt;
	system("cls");
}

void input(int& num) 
{ // А этот длинный цикл нужен чтобы проверить не ввёл ли игрок букву
	while (!(cin >> num) || (cin.peek() != '\n')) { cin.clear(); while (cin.get() != '\n'); cout << "\nТолько цифрами!\n--> "; }
	system("cls");
}

void gameplay(Subject &sub, Human &hum) // Это основная функция взаимодействия с человеком
{
	string user;
	int u;
	

	while (true)
	{
		if (hum.humanDeath())
		{
			cout << "Данный человек мёртв! Вы можете переключиться на другого если создали, либо завершить игровой сеанс.\n\n";
			break;
		}

		if(hum.getP() > 0) cout << "+" << hum.getP() << " ОВ\n\n";
		cout << "\tГЕЙМПЛЕЙ" <<  " ДЕНЬ " << sub.getDay() << " | Человек ID" << hum.getId() << "\n\t--------\n";
		hum.showStatus();

		cout << "1. Следующий день\n2. Питание\n3. Развлечения\n4. Вернуться в меню\n-> ";
		input(user);

		if (user == "1")
		{
			if (hum.getPoint() > 0)
			{
				if (sub.showPoint()) {
					cout << "Вы уверены? У вас ещё остались очки взаимодействий\n1. Нет\n2. Да\n3. Больше не спрашивать\n-> ";
					input(user);

					if (user == "2") sub.nextDay();
					else if (user == "3") { sub.showPoint(false); sub.nextDay(); }
				} 
				else sub.nextDay();				
			}
			else sub.nextDay();
			hum.checkDeath();
		}
		else if (user == "2")
		{
			cout << "1. Еда\n2. Напитки\n-> ";
			input(user);

			if (user == "1")
			{
				hum.showStatus();
				cout << "\n1. Хлеб (цена 2 ОВ; +сытость, +жажда)\n2. Мороженое (цена 2 ОВ; +сытость, +сахар, +жажда)\n3. Суп (цена 3 ОВ; +сытость)\n4. Конфеты (цена 3 ОВ; +сахар)\n5. Лекарство (цена 10 ОВ; +здоровье)\nВыберите что хотите съесть -> ";
				input(user);

				if (user == "1")
				{
					if (hum.getPoint() >= 2) { hum.eat("bread"); cout << "Вы съели хлеб! -2 ОВ\n\n"; }
					else cout << "Вам не хватает очков взаимодействия!\n\n";
				}
				else if (user == "2")
				{
					if (hum.getPoint() >= 2) { hum.eat("iceCream"); cout << "Вы съели мороженое! -2 ОВ\n\n"; }
					else cout << "Вам не хватает очков взаимодействия!\n\n";
				}
				else if (user == "3")
				{
					if (hum.getPoint() >= 3) { hum.eat("soup"); cout << "Вы съели суп! -3 ОВ\n\n"; }
					else cout << "Вам не хватает очков взаимодействия!\n\n";
				}
				else if (user == "4")
				{
					if (hum.getPoint() >= 3) { hum.eat("candy"); cout << "Вы съели конфеты! -3 ОВ\n\n"; }
					else cout << "Вам не хватает очков взаимодействия!\n\n";
				}
				else if (user == "5")
				{
					if (hum.getPoint() >= 10) { hum.eat("heal"); cout << "Вы съели лекарство! -10 ОВ\n\n"; }
					else cout << "Вам не хватает очков взаимодействия!\n\n";
				}
				else cout << "В меню нет такого пункта!\n\n";
			}
		}
		else if (user == "4") break;
	}
}

int main()
{
	setlocale(LC_ALL, "ru");
	string user;
	int u;

	cout << "Симулятор жизни\n\n";

	Subject* sub = new Subject;
	
	Human* humans[15];
	int amount = 0;

	//sub->nextDay();

	while (true)
	{
		cout << "\tГЛАВНОЕ МЕНЮ\n\t------------\n";
		cout << "1. Создать человека (осталось " << 15 - amount << ")\n2. Выбрать человека\n3. Выйти из игры\nВведите цифру -> ";
		input(user);

		if (user == "1")
		{
			if (amount <= 14)
			{
				humans[amount++] = new Human(*sub);
				cout << "Человек создан!\n\n";
			}
			else cout << "Больше создать нельзя!\n\n";
		}
		else if (user == "2")
		{
			if (sub->canChoose())
			{
				for (int i = 0; i < sub->getHumanSize(); i++)
				{
					cout << i + 1 << ". Человек (ID " << sub->getHumanId(i) << ") ";
					if (humans[i]->humanDeath()) { set_color(4); cout << "Мёртв\n"; set_color(7); }
					else { set_color(2); cout << "Жив\n"; set_color(7); }
				}
				cout << "Введите цифру -> ";
				input(u);

				if (u >= 1 && u <= sub->getHumanSize()) { gameplay(*sub, *humans[u-1]); }
				else cout << "Такого человека нет!\n\n";
			}
		}
		else if (user == "3")
		{
			// вывод статистики
			sub->deleteHumans();
			delete sub;
			//for (int i = 0; i < amount; i++) { delete humans[i]; }
			//delete []humans;

			
			cout << "\n\nСпасибо что сыграли! Вверху вы можете наблюдать работу деструкторов.\n";
			break;
		}
	}

	/*set_color(15, 2);
	cout << "Здоровье + 10 ^" << endl;
	set_color(15, 4);
	cout << "Сытость - 25 v" << endl;
	set_color(7, 0);*/ 
}