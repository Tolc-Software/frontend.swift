# C++ to python conversions #

This page shows what is automatically translated and to what.
On the left is the `C++` and to the right what the corresponding interface in `Objective-C` and `Swift` will be.
Not converted means there will be no automatic translation to a corresponding language object.
Note that any restriction this poses only applies to the public interface of your code (e.g. your public headers).

## Conversion tables ##

| C++                                 | Objective-C translation        | Swift translation              |
|:----------------------------------- |:------------------------------ |:------------------------------ |
| Namespace                           | Prefix*                        | Class*                         |
| Nested namespace                    | Prefix                         | Class                          |
| Class                               | Class                          | Class                          |
| Public function                     | Class function                 | Class function                 |
| Private function                    | Not converted                  | Not converted                  |
| Virtual function                    | ???                            | ???                            |
| Operator                            | ???                            | ???                            |
| Static member function              | ???                            | ???                            |
| Static member variable              | ???                            | ???                            |
| Public const member variable        | ???                            | ???                            |
| Public non const member variable    | ???                            | ???                            |
| Private member variable             | Not converted                  | Not converted                  |
| Global variable                     | ???                            | ???                            |
| Global static variable              | ???                            | ???                            |
| Free function                       | ???                            | ???                            |
| Overloaded function                 | ???                            | ???                            |
| Enum                                | ???                            | ???                            |
| Scoped enum                         | ???                            | ???                            |
| Templated class/function            | Not converted**                | Not converted**                |
| Specialized class template          | ???                            | ???                            |
| Specialized function template       | ???                            | ???                            |
| Documentation*****                  | ???                            | ???                            |

\* Prefix on all classes and free functions in the namespace. The Swift class only has static functions (corresponding to free functions)

\*\* No direct translation to Objective-C or Swift. Will not emit warning.

| C++ operator      | Objective-C operator  | Swift operator                 |
|:----------------- |:--------------------- |:------------------------------ |
| operator+         | ???                   | ???                            |
| operator-         | ???                   | ???                            |
| operator*         | ???                   | ???                            |
| operator/         | ???                   | ???                            |
| operator%         | ???                   | ???                            |
| operator+=        | ???                   | ???                            |
| operator-=        | ???                   | ???                            |
| operator*=        | ???                   | ???                            |
| operator/=        | ???                   | ???                            |
| operator%=        | ???                   | ???                            |
| operator==        | ???                   | ???                            |
| operator!=        | ???                   | ???                            |
| operator>         | ???                   | ???                            |
| operator>=        | ???                   | ???                            |
| operator<         | ???                   | ???                            |
| operator<=        | ???                   | ???                            |
| operator[]        | ???                   | ???                            |
| operator()        | ???                   | ???                            |
| operator=         | Not converted         | Not converted                  |
| operator<<        | Not converted         | Not converted                  |
| operator>>        | Not converted         | Not converted                  |
| operator++        | Not converted         | Not converted                  |
| operator--        | Not converted         | Not converted                  |

| C++ Standard library class      | Objective-C translation                                           | Swift translation              |
|:------------------------------- |:----------------------------------------------------------------- |:------------------------------ |
| std::array                      | ???                                                               | ???                            |
| std::complex                    | ???                                                               | ???                            |
| std::deque                      | ???                                                               | ???                            |
| std::filesystem::path           | ???                                                               | ???                            |
| std::forward\_list              | Not converted                                                     | Not converted                  |
| std::function                   | ???                                                               | ???                            |
| std::list                       | ???                                                               | ???                            |
| std::map                        | ???                                                               | ???                            |
| std::multimap                   | ???                                                               | ???                            |
| std::multiset                   | ???                                                               | ???                            |
| std::optional                   | ???                                                               | ???                            |
| std::pair                       | ???                                                               | ???                            |
| std::priority\_queue            | ???                                                               | ???                            |
| std::queue                      | ???                                                               | ???                            |
| std::set                        | ???                                                               | ???                            |
| std::shared_ptr                 | ???                                                               | ???                            |
| std::stack                      | ???                                                               | ???                            |
| std::tuple                      | ???                                                               | ???                            |
| std::unique_ptr                 | ???                                                               | ???                            |
| std::shared_ptr                 | ???                                                               | ???                            |
| std::unordered\_map             | ???                                                               | ???                            |
| std::unordered\_multimap        | ???                                                               | ???                            |
| std::unordered\_multiset        | ???                                                               | ???                            |
| std::unordered\_set             | ???                                                               | ???                            |
| std::valarray                   | ???                                                               | ???                            |
| std::variant                    | ???                                                               | ???                            |
| std::vector                     | ???                                                               | ???                            |


| C++ builtin type           | Objective-C translation  | Swift translation              |
|:-------------------------- |:------------------------ |:------------------------------ |
| bool                       | bool                     | Bool                           |
| char                       | char                     | Character                      |
| char16\_t                  | ???                      | Character                      |
| char32\_t                  | ???                      | Character                      |
| double                     | double                   | Double                         |
| float                      | float                    | Float                          |
| int                        | int                      | Int32                          |
| int8_t                     | ???                      | Int8                           |
| int16_t                    | ???                      | Int16                          |
| int32_t                    | ???                      | Int32                          |
| int64_t                    | ???                      | Int64                          |
| long double                | ???                      | Double                         |
| long int                   | ???                      | Int64                          |
| long long int              | ???                      | Int64                          |
| short int                  | ???                      | Int8                           |
| signed char                | ???                      | Character                      |
| string                     | ???                      | String                         |
| string\_view               | ???                      | String                         |
| uint8_t                    | ???                      | UInt                           |
| uint16_t                   | ???                      | UInt                           |
| uint32_t                   | ???                      | UInt                           |
| uint64_t                   | ???                      | UInt                           |
| unsigned char              | ???                      | Character                      |
| unsigned int               | ???                      | UInt                           |
| unsigned long int          | ???                      | UInt                           |
| unsigned long long int     | ???                      | UInt                           |
| unsigned short int         | ???                      | UInt                           |
| wchar\_t                   | ???                      | Character                      |
| void                       | void                     | Void                           |

