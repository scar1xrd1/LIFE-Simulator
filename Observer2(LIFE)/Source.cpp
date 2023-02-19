#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(int text, int backgrnd) // Изменение цвета текста/фона консоли
{
	SetConsoleTextAttribute(h, (backgrnd << 4) + text);
}
void set_color(int text) // Изменение цвета текста/фона консоли
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
	int health, hungry, point, mood, fatigue;
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
		days++;
		
		int healthMin, healthMax, hungryMin, hungryMax, sugarMin, sugarMax;
		healthMin = healthMax = 0;
		hungryMin = 5; hungryMax = 20;
		sugarMin = -1; sugarMax = 0;


		if(hungry >= 125) { healthMin += 2; healthMax += 5; }
		else if(hungry >= 150) { healthMin += 5; healthMax += 10; }
		else if (hungry <= 25) { healthMin += 5; healthMax += 10; }
		else if(hungry <= 50) { healthMin += 2; healthMax += 5; }

		if(thirst >= 50) { healthMin += 2; healthMax += 5; }
		else if (thirst >= 100) { healthMin += 5; healthMax += 10; }
		else if (thirst >= 125) { healthMin += 7; healthMax += 15; }
		else if (thirst >= 150) { healthMin += 10; healthMax += 20; }
		else if (thirst >= 200) { healthMin += 20; healthMax += 40; }

		if (healthMax > 0) { health -= healthMin + rand() % healthMax; }
		if (hungryMax > 0) { hungry -= hungryMin + rand() % hungryMax; }
		sugar += sugarMin + rand() % (sugarMax - sugarMin + 1);

		hungry -= 5 + rand() % 20;
		thirst += 5 + rand() % 15;
		mood -= 2 + rand() % 7;
		point += 2;

		if (health > 100) health = 100;
		else if (health < 0) health = 0;

		if (hungry < 0) hungry = 0;

		if (mood > 100) mood = 100;
		else if (mood < 0) mood = 0;

		if (sugar > 100) sugar = 100;
		else if (sugar < 0) sugar = 0;
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

	void kill() { delete this; }

	void removeThis() { subject.detach(this); }

	void setId(vector<IHuman*> arr) { id = arr.size(); }
	int getId() { return id; }
	int getPoint() { return point; }
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
			cout << "1. Еда\n2. Напитки";
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
			//delete[] humans;
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