#include <iostream>

// Декларация
class Duck;
class ColorChanger;
void funcChanger(std::string, Duck*);

// Базовый абстракный класс
class Animal {
protected:
    // Конструктор по умолчанию
    Animal() { name_ = "Unknown"; }
    // Конструктор копирования принимает такой же объект
    Animal(Animal const& other) { name_ = other.name_; }
    // Конструктор
    Animal(std::string name) {
        name_ = name;
    }

    // Деструктор
    // Ничего не делает, так как не выделяем ничего при конструировании
    ~Animal() {}

public:
    // Метод, возвращающий имя
    std::string name() const { return name_; }

    // Число виртуальная функция для переопределения в дочерних
    virtual void sound() = 0;

private:
    std::string name_;
};


// Класс "Плавающий"
class Swiming {
    class Abc {}; // Вложенный класс, уточнить про права доступа для вложенных классов

public:
    virtual void swim() = 0; // Чисто виртуальная функция, поэтому класс абстрактный
};


class Duck : public Animal, public Swiming {
public:
    // Конструктор по умолчанию
    Duck() : Animal() {
        color_ = "brown";
        area_ = std::string("plain");
        sounded_ = new int;
        std::cout << "default-ctor for Duck" << std::endl;
    }
    // Копирующий
    Duck(Duck const& other) : Animal(other) { // вызывается копирующий конструктор базового ( : Animal(....) )
        color_ = other.color_;
        area_ = std::string("plain");
        sounded_ = new int;
        std::cout << "copy-ctor for Duck" << std::endl;
    }
    Duck(std::string name, std::string color) : Animal(name) { // вызывается обычный базового (Animal(string) )
        color_ = color;
        area_ = std::string("plain");
        sounded_ = new int;
        std::cout << "name-color-ctor for Duck" << std::endl;
    }

    ~Duck() {
        std::cout << "dtor for Duck" << std::endl;
        delete sounded_;
    }

    // Получение цвета
    std::string color() const { return color_; }

    // Переопределение виртуальной функции
    // Переопределив ее, сделали класс конкретным
    void sound() {
        *sounded_ += 1;
        std::cout << color_ << " duck " << name() << " say 'Crew-Crew'" << std::endl;
    }

    int sounded() { return *sounded_; }

    virtual void swim() {
        std::cout << "Duck can swim" << std::endl;
    }

    static std::string area() { return area_; }
    static void setArea(std::string area) { area_ = area; }

    // Оператор реализует заиствование цвета
    Duck& operator+=(Duck& other) {
        this->color_ = other.color_;
        return *this;
    }

    // Оператор реализует присваивание цвета
    Duck& operator-=(Duck& other) {
        other.color_ = this->color_;
        return *this;
    }

private:
    std::string color_;
    int * sounded_;

    static std::string area_; // Декларация ссылки на статическое поле

    friend class ColorChanger;
    friend void funcChanger(std::string, Duck *);
};

std::string Duck::area_; // Само статическое поле ( Duck:: - принадлежность классу)

std::ostream& operator<<(std::ostream& os, Duck const& duck) {
    os << "Duck{" << duck.name() << "," << duck.color() << "}";
    return os;
}

// Дружественный класс
class ColorChanger {
public:
    ColorChanger(std::string color) {
        color_ = color;
    }

    void change(Duck& duck) {
        duck.color_ = color_;
    }

private:
    std::string color_;
};

// Дружественная функция
void funcChanger(std::string newColor, Duck *duck) {
    duck->color_ = newColor;
}


int main(int argc, char *argv[])
{
    Abc *a = new Swiming::Abc;

    Duck duck1;
    Duck* duck2 = new Duck("McDuck", "white");

    duck1.sound();
    duck1.sound();
    duck1.sound();
    std::cout << "duck1 sounded " << duck1.sounded() << std::endl;

    duck2->sound();

    Duck duck3(*duck2);

    duck3.sound();
    std::cout << "duck3 " << duck3 << std::endl;

    std::cout << "duck1 area is " << duck1.area() << std::endl;
    duck2->setArea("forest");
    std::cout << "duck1 area is " << duck1.area() << std::endl;

    std::cout << "OPERATORS" << std::endl;
    std::cout << "1 " << duck1 << std::endl;
    std::cout << "3 " << duck3 << std::endl;
    duck1 -= duck3;
    std::cout << "1 " << duck1 << std::endl;
    std::cout << "3 " << duck3 << std::endl;

    std::cout << "FRIENDS" << std::endl;
    ColorChanger changer("red");
    changer.change(duck1);
    std::cout << "1 " << duck1 << std::endl;
    funcChanger("black", &duck1);
    std::cout << "1 " << duck1 << std::endl;

    delete duck2;
}
