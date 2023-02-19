#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(int text, int backgrnd) // Изменение цвета текста/фона консоли
{
	SetConsoleTextAttribute(h, (backgrnd << 4) + text);
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
	int day;

public:
	virtual ~Subject() 
	{
		cout << "kill subject\n"; 
	}

	void attach(IHuman* human) override
	{
		humans.push_back(human);
		human->setId(humans);
	}
	void detach(IHuman* human) override { humans.erase(remove(humans.begin(), humans.end(), human), humans.end()); }

	void killHumans()
	{
		auto it = humans.begin();
		while (it != humans.end())
		{
			(*it)->kill();
			it++;
		}
	}

	void notify() override
	{
		auto it = humans.begin();
		while (it != humans.end())
		{
			(*it)->update();
			it++;
		}
	}

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
	int getHumanSize() { return humans.size(); }
	int getHumanId(int id) { return humans.at(id)->getId(); }
};

class Human : public IHuman
{
	Subject& subject;
	int id, days;
	int health, hungry;
	int thirst = 0;
	bool death, critical;	

public:
	Human(Subject& sub) : subject(sub)
	{
		this->subject.attach(this);
		health = hungry = 100;
		death = critical = false;
		days = 1;
	}

	virtual ~Human()
	{ 
		cout << "kill human\n"; 
	}

	void update() override
	{
		days++;
		cout << "UPDATE!\n";
	}

	void showStatus()
	{
		cout << "Ваши статусы:\n";


		if (health >= 60) { cout << "Здоровье: "; set_color(2, 0); cout << health; set_color(7, 0); cout << endl; }
		else if (health >= 30) { cout << "Здоровье: "; set_color(6, 0); cout << health; set_color(7, 0); cout << endl; }
		else { cout << "Здоровье: "; set_color(4, 0); cout << health; set_color(7, 0); }

		if (hungry >= 60) { cout << "Сытость: "; set_color(2, 0); cout << hungry; set_color(7, 0); cout << endl; }
		else if (hungry >= 30) { cout << "Сытость: "; set_color(6, 0); cout << hungry; set_color(7, 0); cout << endl; }
		else { cout << "Сытость: "; set_color(4, 0); cout << hungry; set_color(7, 0); cout << endl; }
		
		if (thirst <= 30) { cout << "Жажда: "; set_color(2, 0); cout << thirst; set_color(7, 0); cout << endl; }
		else if (thirst <= 60) { cout << "Жажда: "; set_color(6, 0); cout << thirst; set_color(7, 0); cout << endl; }
		else { cout << "Жажда: "; set_color(4, 0); cout << thirst; set_color(7, 0); cout << endl; }
	}

	void kill() { delete this; }

	void removeThis() { subject.detach(this); }

	void setId(vector<IHuman*> arr) { id = arr.size(); }
	int getId() { return id; }
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
		cout << "\tГЕЙМПЛЕЙ\n\t--------\n";
		hum.showStatus();
		break;
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
				sub->nextDay();
			}
			else cout << "Больше создать нельзя!\n\n";
		}
		else if (user == "2")
		{
			if (sub->canChoose())
			{
				for (int i = 0; i < sub->getHumanSize(); i++)
				{
					cout << i + 1 << ". Человек (ID " << sub->getHumanId(i) << ")\n";
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
			sub->killHumans();
			delete[] humans;
			delete sub;
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