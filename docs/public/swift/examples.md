# Examples #

Each example is taken from the test suite for `Tolc` and, given that you use the latest version, you can expect them all to work.

Each `C++` library named `MyLib` exports their functions and objects with a prefix of `MyLib`. In every test the library name is simply `m` for brevity. The examples that follow contains a bit of `C++` code, and the respective `Swift` code using it. Each `Swift` example is wrapped in the following boilerplate that is removed to make the examples more readable:

```objc
import m

assert(m.sayHello() == "Hello")
```


## Classes ##

```cpp

class WithConstructor {
public:
  explicit WithConstructor() : m_v(10) {}
  explicit WithConstructor(int v) : m_v(v) {}

  int getV() { return m_v; }
private:
  int m_v;
};

class WithFunction {
public:
  int add(int i, int j) {
    return i + j;
  }
};

class WithStatic {
public:
  static double getPi() {
    return 3.14;
  }

  static int const answer = 42;
};

class WithMember {
public:
  explicit WithMember() : i(10), phi(1.618) {}

  int i;
  double const phi;
};

```

```swift

// Constructors in swift
// does not need different names
var ten: m.WithConstructor = m.WithConstructor()
assert(ten.getV() == 10)

var five: m.WithConstructor = m.WithConstructor(v: 5)
assert(five.getV() == 5)

// Member functions are available after construction
var withFunction: m.WithFunction = m.WithFunction()
assert(withFunction.add(i: 2, j: 5) == 7)

// Static functions can be called
// and static variables accessed
// without instantiating the class
assert(m.WithStatic.getPi() == 3.14)
assert(m.WithStatic.answer == 42);

// Member variables
var member: m.WithMember = m.WithMember()
assert(member.i == 10);
// i is not marked const
member.i = 5;
assert(member.i == 5);

// phi is marked const
// Cannot be assigned
assert(member.phi == 1.618);

```


## Enums ##

```cpp

enum Unscoped {
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	explicit EnumTest(Scoped _s) : s(_s) {};

	Unscoped f(Unscoped u) {
		return u;
	}

	Scoped s;
};

namespace NS {
	// Documentation describing the enum
	enum class Deep {
		Double,
		Down,
	};
}


```

```swift

// C++11 enums work
scoped = m.Scoped.Snail
enumTest = m.EnumTest(scoped)
assert(enumTest.s == scoped)

// Aswell as legacy enums
unscoped = m.Unscoped.Uboat
u = m.f(unscoped)
assert(u == unscoped)

// Enums under namespaces are available
// under the corresponding submodule
deep = m.NS.Deep.Down
assert(deep != m.NS.Deep.Double)

// Documentation carries over from C++
// self.assertIn("Documentation describing the enum", m.NS.Deep.__doc__)

```


## Functions ##

```cpp

#include <filesystem>
#include <string>

int meaningOfLife() {
	return 42;
}

std::string sayHello(std::string const& name) {
	return "Hello " + name;
}

std::filesystem::path getPath() {
	return std::filesystem::path("/path/to/stuff.hpp");
}

namespace Inner {
	double pi() {
		return 3.14;
	}
}

```

```swift

```


## Global Variables ##

```cpp

#include <string>
#include <string_view>

static int i = 0;
namespace Nested {
	int life = 42;
	std::string s = "Hello World";
	constexpr std::string_view constant = "A constant";
}

```

```swift

```


## Member Variables ##

```cpp

#include <string>

class SimpleMember {
public:
	explicit SimpleMember() : myString("Hello") {}

	std::string myString;
};

class ConstMember {
public:
	const int i = 42;
};

class PrivateMember {
public:
	explicit PrivateMember(std::string s) : myString(s) {}

private:
	std::string myString;
};

namespace MyLib {

	class Nested {
	public:
		double d = 4.3;
	};
}

```

```swift

```


## Namespaces ##

```cpp

#include <string>

/*
* MyLib contains a bunch of MyLib functions
*/
namespace MyLib {

int complexFunction() {
	return 5;
}

	namespace We {
		namespace Are {
			namespace Going {
				namespace Pretty {
					namespace Deep {
						std::string meaningOfLife() {
							return "42";
						}
					}
				}
			}
		}
	}
}


```

```swift

```


## Overloaded Functions ##

```cpp

#include <string>

// Overloaded free functions
std::string sayHello() {
	return "Hello!";
}

std::string sayHello(std::string to) {
	return std::string("Hello ") + to;
}

std::string sayHello(size_t times) {
	std::string greeting = "";
	for (size_t i = 0; i < times; ++i) {
		greeting += "Hello!";
	}
	return greeting;
}

class Overload {
public:
	// Overloaded constructor
	Overload() : m_s() {};
	Overload(std::string s) : m_s(s) {};

	// Overloaded class functions
	std::string getStuff() { return "Stuff"; }
	std::string getStuff(std::string customStuff) { return customStuff; }

private:
	std::string m_s;
};

```

```swift

```


## Passing classes between languages ##

```cpp

#include <string>

class MyClass {
public:
	explicit MyClass(std::string s) : m_s(s) {}

	std::string* getS() { return &m_s; }

private:
	std::string m_s;
};

MyClass buildMyClass(std::string const& s) {
	return MyClass(s);
}

class Owner {
public:
	explicit Owner(MyClass m) : m_myClass(m) {};

	MyClass getMyClass() const { return m_myClass; }

private:
	MyClass m_myClass;
};

struct Point2d {
	int x;
	int y;
};

Point2d getMiddle(std::pair<Point2d, Point2d> p) {
	return {(p.first.x + p.second.x) / 2, (p.first.y + p.second.y) / 2};
}

```

```swift

```


## Smart Pointers ##

```cpp

#include <memory>

struct Data {
  int i = 5;
};

struct SharedData {
  int i = 10;
};

std::unique_ptr<Data> createData() {
  return std::make_unique<Data>();
}

// This moves the data,
// destroying it at the end
// Same as C++
int consumeData(std::unique_ptr<Data> data) {
  return data->i + 20;
}

std::shared_ptr<SharedData> createSharedData() {
  return std::make_shared<SharedData>();
}

// Does not move the data
// The pointer is valid after the function call
int consumeSharedData(std::shared_ptr<SharedData> data) {
  return data->i + 20;
}

```

```swift

```


## Templates ##

```cpp

#include <array>
#include <map>
#include <string>
#include <vector>

template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
template int getSomething(int);
template std::vector<std::string> getSomething(std::vector<std::string>);

template <typename T>
class MyClass {
public:
T myFun(T type) {
	return type;
}
};

MyClass<char> getMyClass(MyClass<char> c) {
	return c;
}

template class MyClass<int>;
template class MyClass<std::array<int, 3>>;

```

```swift

```


## std::array ##

```cpp

#include <algorithm>
#include <array>

std::array<int, 3> const f() {
  return {0, 1, 2};
}

bool allOf(std::array<bool, 3> const& conditions) {
  return std::all_of(
      conditions.begin(), conditions.end(),
      [](auto c) { return c; });
}

double sum(std::array<double, 3> const& numbers) {
  double sum = 0;
  for (double number : numbers) {
    sum += number;
  }
  return sum;
}


```

```swift

```


## std::deque ##

```cpp

#include <string>
#include <deque>

std::deque<std::string>
surround(std::deque<std::string> d,
         std::string const& message) {
  d.push_front(message);
  d.push_back(message);
  return d;
}

```

```swift

```


## std::filesystem::path ##

```cpp

#include <filesystem>
#include <vector>

std::filesystem::path
takingPath(std::filesystem::path const& p) {
	return p;
}

std::filesystem::path
parent(std::filesystem::path const& p) {
	return p.parent_path();
}

std::filesystem::path
joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}

```

```swift

```


## std::list ##

```cpp

#include <string>
#include <list>

std::list<std::string> getList() {
  return {"Linked", "list", "fun"};
}

```

```swift

```


## std::map ##

```cpp

#include <map>
#include <string>
#include <vector>

std::map<std::string, int> getThings() {
  return {{"Greetings", 5}};
}

std::map<std::string, std::vector<double>> getCities() {
  return {
  {"Stockholm",
    {59.33, 18.06}},
  {"San Francisco",
    {37.77, -122.43}}
  };
}

```

```swift

```


## std::optional ##

```cpp

#include <optional>
#include <string>

std::string
answer(std::optional<std::string> const& question) {
  if (question) {
    return "Please be more specific.";
  }
  return "That's no question!";
}


```

```swift

```


## std::pair ##

```cpp

#include <string>

class Greeter {
public:
  explicit Greeter(std::pair<std::string, int> greetings)
    : m_greetings(greetings) {}

  std::pair<std::string, int> getGreetings() {
    return m_greetings;
  }

  std::string joinGreetings() {
    std::string joined;
    for (int i = 0; i < m_greetings.second; ++i) {
      joined += m_greetings.first;
    }
    return joined;
  }

private:
  std::pair<std::string, int> m_greetings;
};

```

```swift

```


## std::set ##

```cpp

#include <set>
#include <string>

std::set<std::string> getLanguages() {
	return {"English", "Spanish"};
}

```

```swift

```


## std::tuple ##

```cpp

#include <string>
#include <tuple>

std::tuple<int, std::string> sumInts(std::tuple<int, int, std::string> t) {
  // Sum the first two elements
  return {
    std::get<0>(t) + std::get<1>(t),
    std::get<2>(t)
  };
}

```

```swift

```


## std::unordered_map ##

```cpp

#include <string>
#include <unordered_map>

std::unordered_map<std::string, int>
getUnordered() {
  return {{"Unordered", 1}};
}

```

```swift

```


## std::unordered_set ##

```cpp

#include <string>
#include <unordered_set>

std::unordered_set<std::string> getLanguages() {
	return {"C++", "Objective-C"};
}

```

```swift

```


## std::valarray ##

```cpp

#include <valarray>

std::valarray<int> getIt() {
	return {0, 1, 2};
}

```

```swift

```


## std::vector ##

```cpp

#include <algorithm>
#include <vector>

std::vector<int> f() {
  return {0, 1, 2};
}

bool allOf(std::vector<bool> const& conditions) {
  return std::all_of(
      conditions.begin(), conditions.end(),
      [](auto c) { return c; });
}

double sum(std::vector<double> const& numbers) {
  double sum = 0;
  for (double number : numbers) {
    sum += number;
  }
  return sum;
}


```

```swift

```

