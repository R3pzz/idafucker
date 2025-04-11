#include <idafucker/base/Variant.hpp>

#include <iostream> // cout

class Intf
{
public:
  virtual void a() = 0;
};

class Impl1 : public Intf
{
public:
  void a() override
  {
    std::cout << "Impl1::a invoked" << std::endl;
  }
};

class Impl2 : public Intf
{
public:
  void a() override
  {
    std::cout << "Impl2::a invoked" << std::endl;
  }
};

int main()
{
  idafucker::Variant<Impl1, Impl2> v{new Impl2{}};
}