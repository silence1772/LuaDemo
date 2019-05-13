#include <iostream>

class User
{
public:
    User() : name_("default"), age_(0)
    {
        std::cout << "Create User" << std::endl;
    }

    ~User()
    {
        std::cout << "Destory User" << std::endl;
    }

    std::string get_name()
    {
        return name_;
    }

    void set_name(std::string name)
    {
        name_ = name;
    }

    int get_age()
    {
        return age_;
    }

    void set_age(int age)
    {
        age_ = age;
    }

    void Print()
    {
        std::cout << "User name: " << name_ << ", age: " << age_ << std::endl;
    }

private:
    std::string name_;
    int age_;
};