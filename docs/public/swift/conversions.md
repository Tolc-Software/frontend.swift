# C++ to Swift conversions #

This page shows what is automatically translated and to what.
On the left is the `C++` and to the right what the corresponding interface in `Swift` will be.
Not converted means there will be no automatic translation to a corresponding language object.

Note that any restriction this poses only applies to the public interface of your code (e.g. your public headers).
Your internal code can use anything from C++.

`???` is a pending implementation.

## Conversion tables ##

| C++                                 | Swift translation                                       |
|:----------------------------------- |:------------------------------------------------------- |
| Namespace                           | ???                                                     |
| Nested namespace                    | ???                                                     |
| Class                               | Class                                                   |
| Public function                     | Class function                                          |
| Private function                    | Not converted                                           |
| Virtual function                    | ???                                                     |
| Operator                            | ???                                                     |
| Static member function              | Static class function                                   |
| Static member variable              | Static class variable                                   |
| Public const member variable        | Readonly property                                       |
| Public non const member variable    | Readwrite property                                      |
| Private member variable             | Not converted                                           |
| Global variable                     | ???                                                     |
| Global static variable              | ???                                                     |
| Free function                       | Static class function**                                 |
| Overloaded function                 | ???                                                     |
| Enum                                | ???                                                     |
| Scoped enum                         | ???                                                     |
| Templated class/function            | Not converted****                                       |
| Specialized class template          | ???                                                     |
| Specialized function template       | ???                                                     |
| Documentation                       | ???                                                     |

\*\* Wrapped in a class with a name that starts with the module name.

\*\*\*\* No direct translation to `Swift`. Will not emit warning.

| C++ operator      | Swift operator        |
|:----------------- |:--------------------- |
| operator+         | ???                   |
| operator-         | ???                   |
| operator*         | ???                   |
| operator/         | ???                   |
| operator%         | ???                   |
| operator+=        | ???                   |
| operator-=        | ???                   |
| operator*=        | ???                   |
| operator/=        | ???                   |
| operator%=        | ???                   |
| operator==        | ???                   |
| operator!=        | ???                   |
| operator>         | ???                   |
| operator>=        | ???                   |
| operator<         | ???                   |
| operator<=        | ???                   |
| operator[]        | ???                   |
| operator()        | ???                   |
| operator=         | Not converted         |
| operator<<        | Not converted         |
| operator>>        | Not converted         |
| operator++        | Not converted         |
| operator--        | Not converted         |


| C++ Standard library class      | Swift translation                                                                 |
|:------------------------------- |:--------------------------------------------------------------------------------- |
| std::array                      | ???                                                                               |
| std::complex                    | ???                                                                               |
| std::deque                      | ???                                                                               |
| std::filesystem::path           | ???                                                                               |
| std::forward\_list              | Not converted                                                                     |
| std::function                   | ???                                                                               |
| std::list                       | ???                                                                               |
| std::map                        | ???                                                                               |
| std::multimap                   | Not converted                                                                     |
| std::multiset                   | Not converted                                                                     |
| std::optional                   | ???                                                                               |
| std::pair                       | ???                                                                               |
| std::priority\_queue            | Not converted                                                                     |
| std::queue                      | Not converted                                                                     |
| std::set                        | ???                                                                               |
| std::shared\_ptr                | ???                                                                               |
| std::stack                      | Not converted                                                                     |
| std::string                     | ???                                                                               |
| std::string\_view               | ???                                                                               |
| std::tuple                      | ???                                                                               |
| std::unique\_ptr                | ???                                                                               |
| std::unordered\_map             | ???                                                                               |
| std::unordered\_multimap        | Not converted                                                                     |
| std::unordered\_multiset        | Not converted                                                                     |
| std::unordered\_set             | ???                                                                               |
| std::valarray                   | ???                                                                               |
| std::variant                    | ???                                                                               |
| std::vector                     | ???                                                                               |

\* After using a `shared_ptr`, the object is now always managed by a `shared_ptr`.

| C++ builtin type           | Swift translation        |
|:-------------------------- |:------------------------ |
| bool                       | ???                      |
| char                       | ???                      |
| char16\_t                  | ???                      |
| char32\_t                  | ???                      |
| double                     | ???                      |
| float                      | ???                      |
| int                        | ???                      |
| int8\_t                    | ???                      |
| int16\_t                   | ???                      |
| int32\_t                   | ???                      |
| int64\_t                   | ???                      |
| long double                | ???                      |
| long int                   | ???                      |
| long long int              | ???                      |
| short int                  | ???                      |
| signed char                | ???                      |
| uint8\_t                   | ???                      |
| uint16\_t                  | ???                      |
| uint32\_t                  | ???                      |
| uint64\_t                  | ???                      |
| unsigned char              | ???                      |
| unsigned int               | ???                      |
| unsigned long int          | ???                      |
| unsigned long long int     | ???                      |
| unsigned short int         | ???                      |
| wchar\_t                   | ???                      |
| void                       | ???                      |

